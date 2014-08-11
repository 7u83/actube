#!/bin/sh
echo clock set $(date "+%H:%M:%S %d %b %Y")
echo debug capwap console cli
echo configure terminal
echo crypto ca profile enrollment ACTube
echo authentication terminal
echo enrollment terminal 
echo exit
echo crypto ca trustpoint ACTube
echo enrollment profile ACTube
echo revocation-check none
echo exit
echo crypto ca authenticate ACTube
cat root-ca.crt
echo 
echo y
echo
echo exit
echo


