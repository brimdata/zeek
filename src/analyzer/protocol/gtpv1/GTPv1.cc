// See the file "COPYING" in the main distribution directory for copyright.

#include "GTPv1.h"

#include "events.bif.h"

using namespace analyzer::gtpv1;

GTPv1_Analyzer::GTPv1_Analyzer(Connection* conn)
: Analyzer("GTPV1", conn)
	{
	interp = new binpac::GTPv1::GTPv1_Conn(this);
	}

GTPv1_Analyzer::~GTPv1_Analyzer()
	{
	delete interp;
	}

void GTPv1_Analyzer::Done()
	{
	Analyzer::Done();
	Event(udp_session_done);
	}

void GTPv1_Analyzer::DeliverPacket(int len, const u_char* data, bool orig, uint64_t seq, const IP_Hdr* ip, int caplen)
	{
	Analyzer::DeliverPacket(len, data, orig, seq, ip, caplen);
	try
		{
		interp->NewData(orig, data, data + len);
		}
	catch ( const binpac::Exception& e )
		{
		ProtocolViolation(fmt("Binpac exception: %s", e.c_msg()));
		}

	if ( inner_packet_offset <= 0 )
		return;

	auto odata = data;
	auto olen = len;
	data += inner_packet_offset;
	len -= inner_packet_offset;
	caplen -= inner_packet_offset;
	inner_packet_offset = -1;

	IP_Hdr* inner = nullptr;
	int result = sessions->ParseIPPacket(len, data, next_header, inner);

	if ( result == 0 )
		{
		interp->set_valid(orig, true);

		if ( (! zeek::BifConst::Tunnel::delay_gtp_confirmation) ||
		     (interp->valid(true) && interp->valid(false)) )
			ProtocolConfirmation();

		if ( gtp_hdr_val )
			zeek::BifEvent::enqueue_gtpv1_g_pdu_packet(this, Conn(),
			                                           std::move(gtp_hdr_val),
			                                           inner->ToPktHdrVal());

		EncapsulatingConn ec(Conn(), BifEnum::Tunnel::GTPv1);
		sessions->DoNextInnerPacket(network_time, nullptr,
		                            inner, Conn()->GetEncapsulation(), ec);
		}
	else if ( result == -2 )
		ProtocolViolation("Invalid IP version in wrapped packet",
		                  reinterpret_cast<const char*>(odata), olen);
	else if ( result < 0 )
		ProtocolViolation("Truncated GTPv1",
		                  reinterpret_cast<const char*>(odata), olen);
	else
		ProtocolViolation("GTPv1 payload length",
		                  reinterpret_cast<const char*>(odata), olen);

	if ( result != 0 )
		delete inner;
	}
