#!/bin/bash

# Make linkable packet capture libraries
cp /cygdrive/c/Windows/System32/wpcap.dll .
cp /cygdrive/c/Windows/System32/Packet.dll .
gendef wpcap.dll
dlltool --output-lib libpcap.a --input-def wpcap.def
gendef Packet.dll
dlltool --output-lib libpacket.a --input-def Packet.def
cp libpcap.a libpacket.a /lib
cp -R /cygdrive/c/install/WpdPack/Include/* /usr/include

# Install CAF
git clone -b zeek-3.0 https://github.com/henridf/actor-framework/
cd actor-framework
CXXFLAGS=-D_GNU_SOURCE ./configure --build-static --libs-only
cd build
make -j8 
make -j8 install
cd ..

# Add/modify headers
curl https://raw.githubusercontent.com/lattera/glibc/master/sysdeps/unix/sysv/linux/net/ethernet.h > /usr/include/net/ethernet.h
curl https://raw.githubusercontent.com/lattera/glibc/master/sysdeps/unix/sysv/linux/net/if_arp.h > /usr/include/net/if_arp.h
curl https://gist.githubusercontent.com/henridf/28ab1ad4d2582d7e771564bfdbece77c/raw/532ce70d97be18d3092afcec8b4621bf092f1866/if_ether.h > /usr/include/netinet/if_ether.h
mkdir /usr/include/linux && cp /usr/include/netinet/if_ether.h /usr/include/linux
curl https://raw.githubusercontent.com/msys2/Cygwin/master/newlib/libc/sys/linux/include/netinet/icmp6.h > /usr/include/netinet/icmp6.h
curl https://raw.githubusercontent.com/msys2/Cygwin/master/newlib/libc/sys/linux/include/netinet/ip_icmp.h > /usr/include/netinet/ip_icmp.h
perl -i -pe "s/IPPROTO_ESP = 50/IPPROTO_GRE = 47,\n  IPPROTO_ESP = 50/" /usr/include/cygwin/in.h
echo "typedef long long int quad_t;" >> /usr/include/sys/types.h
echo "typedef long long unsigned u_quad_t;" >> /usr/include/sys/types.h

# Build Zeek
git clone -b release/3.0 --recursive https://github.com/henridf/zeek
cd zeek
CXXFLAGS=-D_GNU_SOURCE ./configure --disable-zeekctl --disable-python --disable-broker-tests --disable-auxtools --enable-static-broker --enable-static-binpac --with-caf=/home/$USER/actor-framework/build/
make -j8
make -j8 install

# Create install package for Zeek
ZIPFILE="$HOME/zeek.zip"
cd /usr/local/zeek/bin
zip -r "$ZIPFILE" zeek.exe
cd /usr/local/zeek/share/zeek
zip -r "$ZIPFILE" base policy site
cd /bin
zip -r "$ZIPFILE" cygcrypto-1.1.dll cyggcc_s-1.dll cygssl-1.1.dll cygstdc++-6.dll cygz.dll cygwin1.dll
cd "$HOME/actor-framework/build/lib"
zip -r "$ZIPFILE" cygcaf_core-0.16.5.dll cygcaf_io-0.16.5.dll cygcaf_openssl-0.16.5.dll
cd /cygdrive/c/Windows/System32
zip -r "$ZIPFILE" -r wpcap.dll Packet.dll
