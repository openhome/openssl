#!/usr/bin/python

import glob
import os
import shutil
import subprocess
import sys
import tarfile

openssl = 'openssl-1.0.1e'
rtems = 'rtems49-virtex'
freertos = 'FreertosLwip'

builddir = os.path.join(os.getcwd(), 'build')
workingdir = os.getcwd()
print 'Building to', builddir

def parse_args(aArgs, aAvailArch):
    if ((len(aArgs) < 3) or (aArgs[1] not in aAvailArch)
            or ((len(aArgs)>=4) and (aArgs[3] != 'debug'))):
        availArchStr = ''
        for i in range(len(aAvailArch)):
            joinChar = ''
            if (i < len(aAvailArch)-1):
                joinChar = '|'
            availArchStr = ''.join([availArchStr, aAvailArch[i], joinChar])
        print 'Usage: %s (%s) <version> ?debug' % (aArgs[0], availArchStr)
        exit(1)
    arch = aArgs[1]
    ver = aArgs[2]
    release = 'release'
    if ((len(aArgs) >= 4) and (aArgs[3] == 'debug')):
        release = 'debug'
    return (arch, ver, release)

def copy_files(src, dst_folder):
    if not os.path.exists(dst_folder):
        os.makedirs(dst_folder)
    for fname in glob.iglob(src):
        shutil.copy(fname, os.path.join(dst_folder, os.path.basename(fname)))

def install_headers(aArch):
    # set up include directory
    includedir = os.path.join(workingdir, 'include')
    if not os.path.exists(includedir):
        os.mkdir(includedir)

    if (aArch in ['Core-armv6', 'Core-ppc32']):
        # copy freertos headers
        freertosdir = os.path.join(workingdir, freertos)
        copy_files(os.path.join(freertosdir, 'arch', 'lwippools.h'), os.path.join(includedir, 'lwip'))
        copy_files(os.path.join(freertosdir, 'lwip', 'src', 'include', 'ipv4', 'lwip', '*.h'), os.path.join(includedir, 'lwip'))
        copy_files(os.path.join(freertosdir, 'lwip', 'src', 'include', 'lwip', '*.h'), os.path.join(includedir, 'lwip'))
        copy_files(os.path.join(freertosdir, 'lwip', 'src', 'include', 'netif', '*.h'), os.path.join(includedir, 'lwip', 'netif'))
        copy_files(os.path.join(freertosdir, 'lwip', 'src', 'include', 'posix', '*.h'), os.path.join(includedir, 'lwip', 'posix'))
        copy_files(os.path.join(freertosdir, 'lwip', 'src', 'include', 'posix', 'sys', '*.h'), os.path.join(includedir, 'lwip', 'posix', 'sys'))
        
        if (aArch == 'Core-armv6'):
            copy_files(os.path.join(freertosdir, 'arch', 'IMX28', 'lwipopts.h'), os.path.join(includedir, 'lwip'))
            copy_files(os.path.join(freertosdir, 'arch', 'IMX28', 'cc.h'), os.path.join(includedir, 'lwip', 'arch'))
        elif (aArch == 'Core-ppc32'):
            copy_files(os.path.join(freertosdir, 'arch', 'PowerPC', 'lwipopts.h'), os.path.join(includedir, 'lwip'))
            copy_files(os.path.join(freertosdir, 'arch', 'PowerPC', 'cc.h'), os.path.join(includedir, 'lwip', 'arch'))

    if (aArch == 'Core-ppc32'):
        #copy rtems headers
        rtemsdir = os.path.join(workingdir, rtems)
        if not os.path.exists(os.path.join(includedir, rtems)):
            shutil.copytree(os.path.join(rtemsdir, 'include'), os.path.join(includedir, rtems))

def configure(aArch, aRelease):
    print 'Configuring for', aArch, aRelease

    platform = ''
    options = []
    # options = ['no-idea', 'no-camellia', 'no-seed', 'no-bf', 'no-cast'
                # , 'no-des', 'no-rc2', 'no-rc4', 'no-rc5', 'no-md2', 'no-md4'
                # , 'no-md5', 'no-ripemd', 'no-mdc2', 'no-rsa', 'no-dsa', 'no-dh'
                # , 'no-ec', 'no-ecdsa', 'no-ecdh', 'no-sock', 'no-ssl2'
                # , 'no-ssl3', 'no-krb5', 'no-engine', 'no-hw', 'no-bio', 'no-bf'
                # , 'no-gmp', 'no-gost', 'no-jpake', 'no-rfc3779', 'no-shared'
                # , 'no-store', 'no-tls1', 'no-tlsext', 'no-zlib', 'no-zlib-dynamic'
                #, 'no-asm', 'no-hmac', 'no-lhash', 'no-evp', 'no-stack', 'no-rand'
                #, 'no-dso', 'no-asn1', 'no-pem', 'no-x509', 'no-x509-v3', 'no-err']
    #options += ['no-err']
    debug_prefix = ''
    if (aRelease == 'debug'):
        debug_prefix = 'debug-'
    if (aArch == 'Windows-x86'):
        platform = 'VC-WIN32'
        options = options + ['no-asm']
    elif (aArch == 'Windows-x64'):
        platform = 'VC-WIN64A'
        options = ['no-asm']
    elif (aArch == 'Linux-x86'):
        platform = 'linux-generic32'
    elif (aArch == 'Linux-x64'):
        platform = 'linux-generic64'
    elif (aArch == 'Linux-ARM'):
        # find compiler option/location for this and add to OpenSSL Configure file
        print 'Error: Linux-ARM target not yet defined'
        exit(1)
    elif (aArch == 'Core-armv6'):
        platform = 'armv5-freertos'
        options = options + ['-msoft-float', '-fexceptions', '-pipe', '-g3', '-Wno-psabi', '-mapcs', '-fno-omit-frame-pointer', '-I'+os.path.join(workingdir, 'include'), '-I'+os.path.join(workingdir, 'include', 'lwip'), '-I'+os.path.join(workingdir, 'include', 'lwip', 'posix')]
        os.environ['PATH'] += os.pathsep + '/opt/rtems-4.11/bin'
    elif (aArch == 'Core-ppc32'):
        platform = 'powerpc-rtems'
        options = options + ['-mcpu=403', '-msoft-float', '-fexceptions', '-pipe', '-g3', '-I'+os.path.join(workingdir, 'include', 'rtems49-virtex')]
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

def createtargz(aArch, aVer, aRelease):
    print 'Packaging OpenSSL for', aArch
    
    cryptolib = ''
    if (aArch in ['Windows-x86', 'Windows-x64']):
        cryptolib = 'libeay32.lib'
    elif (aArch in ['Linux-x86', 'Linux-x64', 'Linux-ARM', 'Core-armv6', 'Core-ppc32']):
        cryptolib = 'libcrypto.a'
    else:
        print 'Error: Unknown arch:', aArch
        exit(1)

    debugtag = '-Release'
    if (aRelease == 'debug'):
        debugtag = '-Debug'

    tarname = os.path.join(builddir, openssl+'-'+aVer+'-'+aArch+debugtag+'.tar.bz2')
    print 'Creating ', tarname
    tar = tarfile.open(tarname, 'w:bz2')
    tar.add(os.path.join(builddir, aArch, 'include', 'openssl'), arcname=os.path.join('openssl', 'include'))
    tar.add(os.path.join(builddir, aArch, 'lib', cryptolib), arcname=os.path.join('openssl', 'lib', cryptolib))
    tar.close()

def create_package(aArgs, aAvailArch):
    (arch, ver, release) = parse_args(aArgs, aAvailArch)
    install_headers(arch)
    configure(arch, release)
    build(arch)
    createtargz(arch, ver, release)

if __name__ == "__main__":
    avail_arch = ['Windows-x86', 'Windows-x64', 'Linux-x86', 'Linux-x64', 'Linux-ARM', 'Core-armv6', 'Core-ppc32']
    try:
        os.chdir(openssl)
    except OSError:
        print 'Error: Unable to change to dir:', openssl
        exit(1)
    create_package(sys.argv, avail_arch)
    try:
        os.chdir('..')
    except OSError:
        print 'Error: Unable to return to root dir'
        exit(1)
