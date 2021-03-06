#!/bin/bash

# Note:
# This script is meant to be run from the root of the working copy.
# 
# This script sets the name of the .dmg and dSYM bundle as it uploads them and does some link magic.

# Config
rtag="${1}"
uurl="buildbot@buildbot.pc-dummy.net"
opth="${rtag}/mac/"
rpth="public_html/"
lpth="macosx/build/dmgout/out/"
revt="-$(git rev-parse -q --short --verify HEAD)"
dmg_bn="warzone2100"
dmg_nv="-novideo.dmg"
tar_dS="-dSYM.tar.gz"


# Set bran
if [ -z ${1} ]; then
	echo "Must supply the branch name being built."
	exit 1
fi
bran="-${rtag}"


# Upload the dSYM bundle
if ! scp -pql 320 ${lpth}${dmg_bn}${tar_dS} ${uurl}:${rpth}${opth}${dmg_bn}${bran}${revt}${tar_dS}; then
	exit ${?}
fi

# Upload the .dmg
if ! scp -pql 320 ${lpth}${dmg_bn}${dmg_nv} ${uurl}:${rpth}${opth}${dmg_bn}${bran}${revt}.dmg; then
	exit ${?}
fi


# Link up the current .dmg and dSYM bundle
ssh ${uurl} -C "cd ${rpth} && ln -fs ${opth}${dmg_bn}${bran}${revt}.dmg ${dmg_bn}${bran}-current.dmg && ln -fs ${opth}${dmg_bn}${bran}${revt}${tar_dS} ${dmg_bn}${bran}-current${tar_dS}"
