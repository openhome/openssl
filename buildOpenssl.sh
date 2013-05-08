#! /bin/bash -xe

arch=$1
if [ "$arch" != "Linux-x86" ] && [ "$arch" != "Linux-x64" ] && [ "$arch" != "Linux-ARM" ] && [ "$arch" != "Core-armv6" ] && [ "$arch" != "Core-ppc32" ]; then
    echo "Usage: $0 (Linux-x86|Linux-x64|Linux-ARM|Core-armv6|Core-ppc32) $1 debug"
    exit 1
fi

case "$arch" in
"arm")
    PATH=/opt/rtems-4.11/bin:${PATH}
    options="armv5-freertos -msoft-float -fexceptions -pipe -g3 -Wno-psabi -mapcs -fno-omit-frame-pointer -I`pwd`/../../../install/include/trace"
    ;;

"powerpc")
    PATH=/opt/rtems-4.9/bin:${PATH}
    options="powerpc-rtems -mcpu=403 -msoft-float -fexceptions -pipe -g3 -I`pwd`/../../../install/include/trace/rtems/include"
    ;;
esac

# -I`pwd`/../../rtems/rtems49-src/cpukit/libnetworking 
# -I`pwd`/../../rtems/rtems49-src/cpukit/include -I`pwd`/../../rtems/rtems49-src/cpukit/libcsupport/include

perl ./Configure --prefix=../../build/openssl/${arch} ${options}
make INSTALL_PREFIX=`pwd`/ DIRS="crypto" all install_sw
#make
#make test
#make install
