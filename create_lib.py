#!/usr/bin/python

import glob
import os
import platform
import shutil
import subprocess
import sys
import tarfile
import optparse

openssl = 'openssl-1.0.1j'
freertos = 'FreertosLwip'

builddir = os.path.join(os.getcwd(), 'build')
workingdir = os.getcwd()
print 'Building to', builddir

"""
FIXME - The below helper setup functions are taken from ohdevtools.
Should probably go fetch ohdevtools instead of duplicating functionality.
"""

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

def windows_program_exists(program):
    return subprocess.call(["where", "/q", program], shell=False)==0

def other_program_exists(program):
    nul = open(os.devnull, "w")
    return subprocess.call(["/bin/sh", "-c", "command -v "+program], shell=False, stdout=nul, stderr=nul)==0

program_exists = windows_program_exists if platform.platform().startswith("Windows") else other_program_exists

def scp(*args):
    program = None
    for p in ["scp", "pscp"]:
        if program_exists(p):
            program = p
            break
    if program is None:
        raise "Cannot find scp (or pscp) in the path."
    subprocess.check_call([program] + list(args))

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
    options = ['no-ec', 'no-ui']
    #options = ['no-err']
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
        platform = 'oh-windows-x86-vc'
        options = options + ['no-asm']
    elif (aArch == 'Windows-x64'):
        platform = 'oh-windows-x64-vc'
        options = ['no-asm']
    elif (aArch == 'Linux-x86'):
        platform = 'oh-linux-x86-gcc'
    elif (aArch == 'Linux-x64'):
        platform = 'oh-linux-x64-gcc'
    elif (aArch == 'Linux-ARM'):
        # find compiler option/location for this and add to OpenSSL Configure file
        print 'Error: Linux-ARM target not yet defined'
        exit(1)
    elif (aArch == 'Linux-ppc32'):
        platform = 'oh-linux-ppc32-gcc'
    elif (aArch == 'Mac-x64'):
        platform = 'oh-mac-x64-clang'
        options = options + ['-DPLATFORM_MACOSX_GNU', '-mmacosx-version-min=10.7']
    elif (aArch == 'Mac-x86'):
        platform = 'oh-mac-x86-clang'
        options = options + ['-DPLATFORM_MACOSX_GNU', '-mmacosx-version-min=10.7', '-m32']
    elif (aArch in ['Core-armv5', 'Core-armv6']):
        platform = 'oh-core-armv5-rtems'
        options = options + ['-msoft-float', '-fexceptions', '-pipe', '-g', '-Wno-psabi', '-mapcs', '-fno-omit-frame-pointer', '-I'+os.path.join(workingdir, 'include'), '-I'+os.path.join(workingdir, 'include', 'lwip'), '-I'+os.path.join(workingdir, 'include', 'lwip', 'posix')
        ]
    elif (aArch == 'Core-ppc32'):
        platform = 'oh-core-ppc32-rtems'
        options = options + ['-mcpu=403', '-msoft-float', '-fexceptions', '-pipe', '-g', '-I'+os.path.join(workingdir, 'include'), '-I'+os.path.join(workingdir, 'include', 'lwip'), '-I'+os.path.join(workingdir, 'include', 'lwip', 'posix')
        ]
    else:
        print 'Error: configure unknown arch:', aArch
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
    elif (aArch in ['Linux-x86', 'Linux-x64', 'Linux-ARM', 'Linux-ppc32', 'Mac-x86', 'Mac-x64', 'Core-armv5', 'Core-armv6', 'Core-ppc32']):
        if (aArch in ['Core-armv5', 'Core-armv6']):
            os.environ['PATH'] += os.pathsep + '/opt/rtems-4.11/bin'
        elif (aArch == 'Core-ppc32'):
            os.environ['PATH'] += os.pathsep + '/opt/rtems-4.11/bin'
        make_cmd = ['make', 'DIRS=crypto ssl', 'all', 'install_sw']
        # The following command would be preferable.
        # However:
        #   Core-armv6 chokes when 'depend' is added
        #   ALL Linux-based builds fail during install_sw if 'all' is not included
        #
        #make_cmd = ['make', 'depend' 'buildcrypto', 'install_sw']
    else:
        print 'Error: build unknown arch:', aArch
        exit(1)
    print 'Building for', aArch, 'using cmd:', make_cmd
    subprocess.check_call(make_cmd)

def bundle_name(aArch, aDebug, aVer):
    debugTag = '-Release'
    if (aDebug == True):
        debugTag = '-Debug'
    return os.path.join(builddir, 'openssl-'+aVer+'-'+aArch+debugTag+'.tar.bz2')

def create_bundle(aArch, aVer, aRelease):
    print 'Packaging OpenSSL for', aArch
    
    cryptolib = ''
    ssllib = ''
    windows_symbols = 'lib.pdb'
    if (aArch in ['Windows-x86', 'Windows-x64']):
        cryptolib = 'libeay32.lib'
        ssllib = 'ssleay32.lib'
    elif (aArch in ['Linux-x86', 'Linux-x64', 'Linux-ARM', 'Linux-ppc32', 'Mac-x64', 'Mac-x86', 'Core-armv5', 'Core-armv6', 'Core-ppc32']):
        cryptolib = 'libcrypto.a'
        ssllib = 'libssl.a'
    else:
        print 'Error: create_bundle unknown arch:', aArch
        exit(1)

    debug = False
    if (aRelease == 'debug'):
        debug = True

    tarname = bundle_name(aArch, debug, aVer)
    print 'Creating ', tarname
    tar = tarfile.open(tarname, 'w:bz2')
    tar.add(os.path.join(builddir, aArch, 'include'), arcname=os.path.join('openssl', 'include'))
    tar.add(os.path.join(builddir, aArch, 'lib', cryptolib), arcname=os.path.join('openssl', 'lib', cryptolib))
    tar.add(os.path.join(builddir, aArch, 'lib', ssllib), arcname=os.path.join('openssl', 'lib', ssllib))
    if (aArch in ['Windows-x86', 'Windows-x64']):
        windows_tmp = 'tmp32'
        if (aRelease == 'debug'):
            windows_tmp = 'tmp32.dbg'
        tar.add(os.path.join(workingdir, openssl, windows_tmp, windows_symbols), arcname=os.path.join('openssl', 'lib', windows_symbols))
    tar.close()
    return tarname

def clean(aArch):
    # NOTE: configure() must have been called before this

    make_cmd = []
    if (aArch in ['Windows-x86', 'Windows-x64']):
        make_cmd = ['nmake', '-f', os.path.join('ms', 'nt.mak'), 'clean']
    elif (aArch in ['Linux-x86', 'Linux-x64', 'Linux-ARM', 'Linux-ppc32', 'Mac-x86', 'Mac-x64', 'Core-armv5', 'Core-armv6', 'Core-ppc32']):
        make_cmd = ['make', 'clean']
    else:
        print 'Error: clean unknown arch:', aArch
        exit(1)
    subprocess.check_call(make_cmd)

    platform_build_dir = os.path.join(builddir, aArch)
    if os.path.isdir(platform_build_dir):
        print 'Deleting: ', platform_build_dir
        shutil.rmtree(platform_build_dir)

    file_list = os.listdir(builddir)
    for f in file_list:
        if aArch in f:
            filepath = os.path.join(builddir, f)
            if os.path.isfile(filepath):
                print 'Deleting:', f
                os.remove(filepath)

def create_package(aArch, aRelease, aVersion):
    install_headers(aArch)
    configure(aArch, aRelease)
    #clean(aArch)
    build(aArch)
    archive_name = create_bundle(aArch, aVersion, aRelease)
    return archive_name

def convert_to_cygwin_path(aPath):
    """
    Convert an absolute Windows path from the form C:\some\path to /c/some/path.
    Useful when using tools that require a Unix-style path, such as scp.
    """
    delimiter = '/'

    # change start of path from C:\some\path to c/some/path
    path = aPath.replace('\\', delimiter)
    path = path.split(':')
    path[0] = path[0].lower()
    path = ''.join(path)

    # prepend '/' to start of path
    pathlist = ['/', path]
    path = ''.join(pathlist)
    return path

def platform_specific_path(aArch, aPackageFile):
    if (aArch in ['Windows-x86', 'Windows-x64']):
        return convert_to_cygwin_path(aPackageFile)
    else:
        return aPackageFile

def publish(aArch, aPackageFile):
    bundle_dest = 'artifacts@core.linn.co.uk:/home/artifacts/public_html/artifacts/openssl/'
    print('scp(' + aPackageFile + ', ' + bundle_dest + ')')
    scp(aPackageFile, bundle_dest)

if __name__ == "__main__":
    avail_arch = ['Windows-x86', 'Windows-x64', 'Linux-x86', 'Linux-x64', 'Linux-ARM', 'Linux-ppc32', 'Mac-x86', 'Mac-x64', 'Core-armv5', 'Core-armv6', 'Core-ppc32']
    try:
        os.chdir(openssl)
    except OSError:
        print 'Error: Unable to change to dir:', openssl
        exit(1)

    # first arg is the 'command': either 'build' or 'clean'
    # second arg is the platform

    parser = optparse.OptionParser()
    parser.add_option('',   '--platform', dest='platform', default=None, help='target platform (required)')
    parser.add_option('',   '--configure', dest='configure', default=False, action="store_true", help='configure the project')
    parser.add_option('',   '--clean', dest='clean', default=False, action="store_true", help='clean the project')
    parser.add_option('',   '--build', dest='build', default = False, action="store_true", help='build the project')
    parser.add_option('',   '--publish', dest='publish', default=False, action="store_true", help='publish the bundle')
    parser.add_option('',   '--debug',   dest='debug',   default=False, action="store_true", help='generate a debug variant')
    parser.add_option('',   '--version', dest='version', default='development', help='create the bundle with this version string')

    (options, args) = parser.parse_args()

    if (options.platform == None):
        print 'Error: --platform must be set'
        exit(1)

    if not (options.platform in avail_arch):
        print 'Error: Target platform not available:', options.platform
        exit(1)

    set_env(options.platform) # set up req'd environment variables

    if options.configure:
        if not os.path.exists(builddir):
            os.makedirs(builddir)

        variant = 'release'
        if options.debug:
            variant = 'debug'
        install_headers(options.platform)
        configure(options.platform, variant)

    if options.clean:
        clean(options.platform)

    debug = False
    if options.debug:
        debug = True
    archive_name = bundle_name(options.platform, debug, options.version)

    if options.build:
        variant = 'release'
        if options.debug:
            variant = 'debug'
        build(options.platform)
        create_bundle(options.platform, options.version, variant)

    if options.publish:
        if not os.path.isfile(archive_name):
            print 'publish archive does not exist:', archive_name
            exit(1)
        publish(options.platform, platform_specific_path(options.platform, archive_name))

    try:
        os.chdir('..')
    except OSError:
        print 'Error: Unable to return to root dir'
        exit(1)
