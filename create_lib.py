#!/usr/bin/python

import glob
import os
import shutil
import subprocess
import sys
import tarfile
import optparse

openssl = 'openssl-1.0.0d'
freertos = 'FreertosLwip'

builddir = os.path.join(os.getcwd(), 'build')
workingdir = os.getcwd()
print 'Building to', builddir


def set_vsvars(architecture="x86"):
    """
    Creates a dictionary containing the environment variables set up by vcvarsall.bat
    and then attempts to update those environment variables.

    architecture - Architecture to pass to vcvarsall.bat. Normally "x86" or "amd64"

    win32-specific
    """

    try:
        vscomntools = os.environ['VS110COMNTOOLS']
    except:
        vscomntools = os.environ['VS100COMNTOOLS']
        if vscomntools is None:
            raise Exception("Neither VS110COMNTOOLS or VS100COMNTOOLS are set in environment.")
    vsvars32 = os.path.join(vscomntools, '..', '..', 'VC', 'vcvarsall.bat')
    python = sys.executable
    process = subprocess.Popen('("%s" %s>nul)&&"%s" -c "import os; print repr(os.environ)"' % (vsvars32, architecture, python), stdout=subprocess.PIPE, shell=True)
    stdout, _ = process.communicate()
    exitcode = process.wait()
    if exitcode != 0:
        raise Exception("Got error code %s from subprocess!" % exitcode)
    env_dict = eval(stdout.strip())
    for key in env_dict:
        os.environ[key] = env_dict[key]

def copy_files(src, dst_folder):
    if not os.path.exists(dst_folder):
        os.makedirs(dst_folder)
    for fname in glob.iglob(src):
        shutil.copy(fname, os.path.join(dst_folder, os.path.basename(fname)))

def set_env(aArch):
    if (aArch == 'Windows-x86'):
        set_vsvars('x86');
    elif (aArch == 'Windows-x64'):
        set_vsvars('x64');

def install_headers(aArch):
    # set up include directory
    includedir = os.path.join(workingdir, 'include')
    if not os.path.exists(includedir):
        os.mkdir(includedir)

    if (aArch in ['Core-armv5', 'Core-armv6', 'Core-ppc32']):
        # copy freertos headers
        freertosdir = os.path.join(workingdir, freertos)
        copy_files(os.path.join(freertosdir, 'arch', 'lwippools.h'), os.path.join(includedir, 'lwip'))
        copy_files(os.path.join(freertosdir, 'lwip', 'src', 'include', 'ipv4', 'lwip', '*.h'), os.path.join(includedir, 'lwip'))
        copy_files(os.path.join(freertosdir, 'lwip', 'src', 'include', 'lwip', '*.h'), os.path.join(includedir, 'lwip'))
        copy_files(os.path.join(freertosdir, 'lwip', 'src', 'include', 'netif', '*.h'), os.path.join(includedir, 'lwip', 'netif'))
        copy_files(os.path.join(freertosdir, 'lwip', 'src', 'include', 'posix', '*.h'), os.path.join(includedir, 'lwip', 'posix'))
        copy_files(os.path.join(freertosdir, 'lwip', 'src', 'include', 'posix', 'sys', '*.h'), os.path.join(includedir, 'lwip', 'posix', 'sys'))
        
        if (aArch in ['Core-armv5', 'Core-armv6']):
            copy_files(os.path.join(freertosdir, 'arch', 'IMX28', 'lwipopts.h'), os.path.join(includedir, 'lwip'))
            copy_files(os.path.join(freertosdir, 'arch', 'IMX28', 'cc.h'), os.path.join(includedir, 'lwip', 'arch'))
        elif (aArch == 'Core-ppc32'):
            copy_files(os.path.join(freertosdir, 'arch', 'PowerPC', 'lwipopts.h'), os.path.join(includedir, 'lwip'))
            copy_files(os.path.join(freertosdir, 'arch', 'PowerPC', 'cc.h'), os.path.join(includedir, 'lwip', 'arch'))

def configure(aArch, aRelease):
    print 'Configuring for', aArch, aRelease

    platform = ''
    #options = []
    options = ['no-err']
    # Does OpenSSL actually act on all these options?
    # options = ['no-idea', 'no-camellia', 'no-seed', 'no-bf', 'no-cast'
                # , 'no-des', 'no-rc2', 'no-rc4', 'no-rc5', 'no-md2', 'no-md4'
                # , 'no-md5', 'no-ripemd', 'no-mdc2', 'no-dsa', 'no-dh'
                # , 'no-ec', 'no-ecdsa', 'no-ecdh', 'no-sock', 'no-ssl2'
                # , 'no-ssl3', 'no-krb5', 'no-engine', 'no-hw', 'no-stdio'
                # , #'no-bio',
                # 'no-bf', 'no-gmp', 'no-gost', 'no-jpake', 'no-rfc3779', 'no-shared'
                # , 'no-store', 'no-tls1', 'no-tlsext', 'no-zlib', 'no-zlib-dynamic'
                # , 'no-asm', 'no-lhash', 'no-stack', 'no-rand'
                # , 'no-dso', 'no-asn1', 'no-pem', 'no-x509', 'no-x509-v3', 'no-err'
                # ]
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
    elif (aArch == 'Linux-ppc32'):
        platform = 'linux-ppc'
    elif (aArch in ['Core-armv5', 'Core-armv6']):
        platform = 'armv5-freertos'
        options = options + ['-msoft-float', '-fexceptions', '-pipe', '-g', '-Wno-psabi', '-mapcs', '-fno-omit-frame-pointer', '-I'+os.path.join(workingdir, 'include'), '-I'+os.path.join(workingdir, 'include', 'lwip'), '-I'+os.path.join(workingdir, 'include', 'lwip', 'posix')
        ]
        os.environ['PATH'] += os.pathsep + '/opt/rtems-4.11/bin'
    elif (aArch == 'Core-ppc32'):
        platform = 'powerpc-rtems'
        options = options + ['-mcpu=403', '-msoft-float', '-fexceptions', '-pipe', '-g', '-I'+os.path.join(workingdir, 'include'), '-I'+os.path.join(workingdir, 'include', 'lwip'), '-I'+os.path.join(workingdir, 'include', 'lwip', 'posix')
        ]
        os.environ['PATH'] += os.pathsep + '/opt/rtems-4.11/bin'
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
        make_cmd = ['nmake', '-f', os.path.join('ms', 'nt.mak'), 'install']
    elif (aArch in ['Linux-x86', 'Linux-x64', 'Linux-ARM', 'Linux-ppc32', 'Core-armv5', 'Core-armv6', 'Core-ppc32']):
        make_cmd = ['make', 'DIRS=\"crypto\"', 'all', 'install_sw']
        # The following command would be preferable.
        # However:
        #   Core-armv6 chokes when 'depend' is added
        #   ALL Linux-based builds fail during install_sw if 'all' is not included
        #
        #make_cmd = ['make', 'depend' 'buildcrypto', 'install_sw']
    else:
        print 'Error: Unknown arch:', aArch
        exit(1)
    print 'Building for', aArch, 'using cmd:', make_cmd
    subprocess.check_call(make_cmd)

def create_bundle(aArch, aVer, aRelease):
    print 'Packaging OpenSSL for', aArch
    
    cryptolib = ''
    windows_symbols = 'lib.pdb'
    if (aArch in ['Windows-x86', 'Windows-x64']):
        cryptolib = 'libeay32.lib'
    elif (aArch in ['Linux-x86', 'Linux-x64', 'Linux-ARM', 'Linux-ppc32', 'Core-armv5', 'Core-armv6', 'Core-ppc32']):
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
    tar.add(os.path.join(builddir, aArch, 'include'), arcname=os.path.join('openssl', 'include'))
    tar.add(os.path.join(builddir, aArch, 'lib', cryptolib), arcname=os.path.join('openssl', 'lib', cryptolib))
    if (aArch in ['Windows-x86', 'Windows-x64']):
        windows_tmp = 'tmp32'
        if (aRelease == 'debug'):
            windows_tmp = 'tmp32.dbg'
        tar.add(os.path.join(workingdir, openssl, windows_tmp, windows_symbols), arcname=os.path.join('openssl', 'lib', windows_symbols))
    tar.close()
    return tarname

def clean(aArch):
    make_cmd = []
    if (aArch in ['Windows-x86', 'Windows-x64']):
        make_cmd = ['nmake', '/f', os.path.join('ms', 'nt.mak'), 'clean']
    elif (aArch in ['Linux-x86', 'Linux-x64', 'Linux-ARM', 'Linux-ppc32', 'Core-armv5', 'Core-armv6', 'Core-ppc32']):
        make_cmd = ['make', 'clean']
    else:
        print 'Error: Unknown arch:', aArch
        exit(1)
    subprocess.check_call(make_cmd)

def create_package(aArch, aRelease, aVersion):
    clean(aArch)
    install_headers(aArch)
    configure(aArch, aRelease)
    build(aArch)
    archive_name = create_bundle(aArch, aVersion, aRelease)
    return archive_name

def publish(aPackageFile):
    bundle_dest = 'artifacts@core.linn.co.uk:/home/artifacts/public_html/artifacts/openssl/'
    rsync_cmd   = ['rsync', aPackageFile, bundle_dest]
    print(rsync_cmd)
    subprocess.check_call(rsync_cmd)

if __name__ == "__main__":
    avail_arch = ['Windows-x86', 'Windows-x64', 'Linux-x86', 'Linux-x64', 'Linux-ARM', 'Linux-ppc32', 'Core-armv5', 'Core-armv6', 'Core-ppc32']
    try:
        os.chdir(openssl)
    except OSError:
        print 'Error: Unable to change to dir:', openssl
        exit(1)

    # first arg is the 'command': either 'build' or 'clean'
    # second arg is the platform

    parser = optparse.OptionParser()
    parser.add_option('-p', '--publish', dest='publish', default=False, action="store_true", help='publish the bundle')
    parser.add_option(      '--debug',   dest='debug',   default=False, action="store_true", help='generate a debug build')
    parser.add_option('-v', '--version', dest='version', default='development', help='create the bundle with this version string')

    (options, args) = parser.parse_args()

    command  = args[0]
    platform = args[1]

    if not (platform in avail_arch):
        exit(1)

    set_env(platform) # set up req'd environment variables

    if command == 'clean':
        clean(platform)

    if command == 'build':
        if options.debug:
            variant = 'debug'
        else:
            variant = 'release'

        package_file = create_package(platform, variant, options.version)

        if options.publish:
            publish(package_file)

    try:
        os.chdir('..')
    except OSError:
        print 'Error: Unable to return to root dir'
        exit(1)
