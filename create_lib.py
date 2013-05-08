#!/usr/bin/python

import os
import subprocess
import sys
import tarfile

openssl = 'openssl-1.0.0d'

builddir = os.path.join(os.getcwd(), 'build')
print 'Building to', builddir

def parse_args(aArgs, aAvailArch):
    if ((len(aArgs) < 2) or (aArgs[1] not in aAvailArch)
            or ((len(aArgs)>=3) and (aArgs[2] != 'debug'))):
        availArchStr = ''
        for i in range(len(aAvailArch)):
            joinChar = ''
            if (i < len(aAvailArch)-1):
                joinChar = '|'
            availArchStr = ''.join([availArchStr, aAvailArch[i], joinChar])
        print 'Usage: %s (%s) ?debug' % (aArgs[0], availArchStr)
        exit(1)
    arch = aArgs[1]
    release = 'release'
    if ((len(aArgs) >= 3) and (aArgs[2] == 'debug')):
        release = 'debug'
    return (arch, release)

def configure(aArch, aRelease):
    print 'Configuring for', aArch, aRelease

    platform = ''
    options = []
    debug_prefix = ''
    if (aRelease == 'debug'):
        debug_prefix = 'debug-'
    if (aArch == 'Windows-x86'):
        platform = 'VC-WIN32'
        options = ['no-asm']
    elif (aArch == 'Windows-x64'):
        platform = 'VC-WIN64A'
        options = ['no-asm']
    elif (aArch == 'Linux-x86'):
        platform = 'linux-generic32'
    elif (aArch == 'Linux-x64'):
        platform = 'linux-generic64'
    elif (aArch == 'Linux-ARM'):
        pass    # find compiler option/location for this and add to Configure file
    elif (aArch == 'Core-armv6'):
        platform = 'armv5-freertos'
        options = ['-msoft-float', '-fexceptions', '-pipe', '-g3', '-Wno-psabi', '-mapcs', '-fno-omit-frame-pointer', '-I'+builddir+'/../include']
        os.environ['PATH'] += os.pathsep + '/opt/rtems-4.11/bin'
    elif (aArch == 'Core-ppc32'):
        platform = 'powerpc-rtems'
        options = ['-mcpu=403', '-msoft-float', '-fexceptions', '-pipe', '-g3', '-I'+os.path.join(builddir, '..', 'include')]
        os.environ['PATH'] += os.pathsep + '/opt/rtems-4.9/bin'
    else:
        print 'Error: Unknown arch:', aArch
        exit(1)
    builddir_prefix = '--prefix='+os.path.join(builddir, aArch)
    subprocess.check_call(['perl', 'Configure'] + [debug_prefix+platform] + options + [builddir_prefix])
    if (aArch == 'Windows-x86'):
        subprocess.check_call([os.path.join('ms', 'do_ms')], shell=True)
    elif (aArch == 'Windows-x64'):
        subprocess.check_call([os.path.join('ms', 'do_win64a')], shell=True)

def build(aArch):
    make_cmd = []
    if (aArch in ['Windows-x86', 'Windows-x64']):
        make_cmd = ['nmake', '-f', os.path.join('ms', 'ntdll.mak'), 'install']
    elif (aArch in ['Linux-x86', 'Linux-x64', 'Linux-ARM', 'Core-armv6', 'Core-ppc32']):
        make_cmd = ['make', 'DIRS=\"crypto\"', 'all', 'install_sw']
    else:
        print 'Error: Unknown arch:', aArch
        exit(1)
    subprocess.check_call(make_cmd)

def createtargz(aArch, aRelease):
    print 'Packaging OpenSSL for', aArch
    
    cryptolib = ''
    if (aArch in ['Windows-x86', 'Windows-x64']):
        cryptolib = 'libeay32.lib'
    elif (aArch in ['Linux-x86', 'Linux-x64', 'Linux-ARM', 'Core-armv6', 'Core-ppc32']):
        cryptolib = 'libcrypto.a'
    else:
        print 'Error: Unknown arch:', aArch
        exit(1)

    debug = ''
    if (aRelease == 'debug'):
        debug = 'debug'

    tarname = os.path.join(builddir, openssl+'-include-'+aArch+'-debug.tar.bz2')
    print 'Creating ', tarname
    tar = tarfile.open(tarname, 'w:bz2')
    tar.add(os.path.join(builddir, aArch, 'include', 'openssl'), arcname='openssl')
    tar.close()

    tarname = os.path.join(builddir,openssl+'-lib-'+aArch+'-debug.tar.bz2')
    print 'Creating ', tarname
    tar = tarfile.open(tarname, 'w:bz2')
    tar.add(os.path.join(builddir, aArch, 'lib', cryptolib), arcname=cryptolib)
    tar.close()

def create_package(aArgs, aAvailArch):
    (arch, release) = parse_args(aArgs, aAvailArch)
    configure(arch, release)
    build(arch)
    createtargz(arch, release)

if __name__ == "__main__":
    avail_arch = ['Windows-x86', 'Windows-x64', 'Linux-x86', 'Linux-x64', 'Linux-ARM', 'Core-armv6', 'Core-ppc32']
    try:
        os.chdir(openssl)
    except OSError:
        print 'Error: Unable to change to dir:', openssl
        exit(1)
    #print os.listdir('.')
    create_package(sys.argv, avail_arch)
    try:
        os.chdir('..')
    except OSError:
        print 'Error: Unable to return to root dir'
        exit(1)