# Start master and two clones. One clone changes table and the change ends up in master + other clone.

# @TEST-PORT: BROKER_PORT

# @TEST-EXEC: btest-bg-run master "zeek -B broker -b ../master.zeek >../master.out"
# @TEST-EXEC: btest-bg-run cloneone "zeek -B broker -b ../cloneone.zeek >../cloneone.out"
# @TEST-EXEC: btest-bg-run clonetwo "zeek -B broker -b ../clonetwo.zeek >../clonetwo.out"
# @TEST-EXEC: btest-bg-wait 15
#
# @TEST-EXEC: btest-diff master.out
# @TEST-EXEC: btest-diff clonetwo.out

@TEST-START-FILE master.zeek
redef exit_only_after_terminate = T;

module TestModule;

global tablestore: opaque of Broker::Store;
global setstore: opaque of Broker::Store;
global recordstore: opaque of Broker::Store;

type testrec: record {
	a: count;
	b: string;
	c: set[string];
};

global t: table[string] of count &broker_store="table";
global s: set[string] &broker_store="set";
global r: table[string] of testrec &broker_allow_complex_type &broker_store="rec";

event zeek_init()
	{
	Broker::listen("127.0.0.1", to_port(getenv("BROKER_PORT")));
	tablestore = Broker::create_master("table");
	setstore = Broker::create_master("set");
	recordstore = Broker::create_master("rec");
	}

event dump_tables()
	{
	print t;
	print s;
	print r;
	}

event Broker::peer_added(endpoint: Broker::EndpointInfo, msg: string)
	{
	print "Peer added ";
  schedule 5secs { dump_tables() };
	}

event Broker::peer_lost(endpoint: Broker::EndpointInfo, msg: string)
	{
	terminate();
	}
@TEST-END-FILE

@TEST-START-FILE cloneone.zeek
redef exit_only_after_terminate = T;

module TestModule;

global tablestore: opaque of Broker::Store;
global setstore: opaque of Broker::Store;
global recordstore: opaque of Broker::Store;

type testrec: record {
	a: count;
	b: string;
	c: set[string];
};

global t: table[string] of count &broker_store="table";
global s: set[string] &broker_store="set";
global r: table[string] of testrec &broker_allow_complex_type &broker_store="rec";

event zeek_init()
	{
	Broker::peer("127.0.0.1", to_port(getenv("BROKER_PORT")));
	}

event send_stuff_over()
	{
	print "Inserting stuff";
	t["a"] = 5;
	delete t["a"];
	add s["hi"];
	t["a"] = 2;
	t["a"] = 3;
	t["b"] = 3;
	t["c"] = 4;
	t["whatever"] = 5;
	delete t["c"];
	r["a"] = testrec($a=1, $b="b", $c=set("elem1", "elem2"));
	r["a"] = testrec($a=1, $b="c", $c=set("elem1", "elem2"));
	r["b"] = testrec($a=2, $b="d", $c=set("elem1", "elem2"));
	print t;
	print s;
	print r;
	}

event killmeplease()
	{
	terminate();
	}

event Broker::peer_added(endpoint: Broker::EndpointInfo, msg: string)
	{
	print "Peer added", endpoint;
	tablestore = Broker::create_clone("table");
	setstore = Broker::create_clone("set");
	recordstore = Broker::create_clone("rec");
	schedule 2secs { send_stuff_over() };
	schedule 5secs { killmeplease() };
	}
@TEST-END-FILE

@TEST-START-FILE clonetwo.zeek
redef exit_only_after_terminate = T;

module TestModule;

global tablestore: opaque of Broker::Store;
global setstore: opaque of Broker::Store;
global recordstore: opaque of Broker::Store;

type testrec: record {
	a: count;
	b: string;
	c: set[string];
};

global t: table[string] of count &broker_store="table";
global s: set[string] &broker_store="set";
global r: table[string] of testrec &broker_allow_complex_type &broker_store="rec";

event zeek_init()
	{
	Broker::peer("127.0.0.1", to_port(getenv("BROKER_PORT")));
	}

event dump_tables()
	{
	print t;
	print s;
	print r;
	terminate();
	}

event Broker::peer_added(endpoint: Broker::EndpointInfo, msg: string)
	{
	print "Peer added";
	tablestore = Broker::create_clone("table");
	setstore = Broker::create_clone("set");
	recordstore = Broker::create_clone("rec");
	schedule 5secs { dump_tables() };
	}
@TEST-END-FILE
