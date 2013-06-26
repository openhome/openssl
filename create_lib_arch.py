#!/usr/bin/python

import os
import sys

from create_lib import openssl, arch_opt_str, clean, create_package

def build_arch(targets, ver):
    for arch in targets:
        clean(arch)
        create_package(['', arch, ver], targets)

if __name__ == "__main__":
    base_arch = ['Windows', 'Linux']
    windows_arch = ['Windows-x86', 'Windows-x64']
    linux_arch = ['Linux-x86', 'Linux-x64', 'Linux-ppc32', 'Core-armv6', 'Core-ppc32']#, 'Linux-ARM']
    if (len(sys.argv) < 3) or (sys.argv[1] not in base_arch):
        availArchStr = arch_opt_str(base_arch)
        print 'Usage: %s (%s) <version>' % (sys.argv[0], availArchStr)
        exit(1)

    argBaseArch = sys.argv[1]
    argVer      = sys.argv[2]
    targetArch  = ''
    try:
        os.chdir(openssl)
    except OSError:
        print 'Error: Unable to change to dir:', openssl
        exit(1)
    if (argBaseArch == 'Windows'):
        targetArch = windows_arch
    else:
        targetArch = linux_arch
    build_arch(targetArch, argVer)
    try:
        os.chdir('..')
    except OSError:
        print 'Error: Unable to return to root dir'
        exit(1)

