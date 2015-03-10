#!/usr/bin/env python
import os
import os.path
import subprocess
from optparse import OptionParser

parser = OptionParser()
parser.add_option('--debug', dest='debug', action='store_true')
parser.add_option('--release', dest='debug', action='store_false', default=False)
(opts, args) = parser.parse_args()

cenv=os.environ
if cenv['PLATFORM'] == 'Linux-mipsel':
    cdir=cenv['CHROMIUM_BASE']
    scrd=cenv['WORKSPACE'].replace(cenv['CHROMIUM_CHROOT'],'',1)
    prefix=[os.path.join(cenv['DEPOT_TOOLS'], 'cros_sdk'), '--']
else:
    cdir=cenv['WORKSPACE']
    scrd=cenv['WORKSPACE']
    prefix=[]

cargs = ['--configure', '--clean', '--build', '--publish', '--platform', cenv['PLATFORM'], '--version', cenv['PUBLISH_VERSION']]
if opts.debug:
    cargs += ['--debug',]

subprocess.check_call(args=prefix + ['python', os.path.join(scrd, 'create_lib.py')] + cargs, cwd=cdir)

