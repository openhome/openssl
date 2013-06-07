#!/usr/bin/python

import os
import sys

from create_lib import openssl, clean, create_package

def build_linux(targets, ver):
    for arch in targets:
        clean(arch)
        create_package(['', arch, ver], targets)

if __name__ == "__main__":
    linux_arch = ['Linux-x86', 'Linux-x64', 'Linux-ARM', 'Core-armv6', 'Core-ppc32']
    if (len(sys.argv) < 2):
        print 'Usage: %s <version>' % (sys.argv[0])
        exit(1)

    try:
        os.chdir(openssl)
    except OSError:
        print 'Error: Unable to change to dir:', openssl
        exit(1)
    build_linux(linux_arch, sys.argv[1])
    try:
        os.chdir('..')
    except OSError:
        print 'Error: Unable to return to root dir'
        exit(1)