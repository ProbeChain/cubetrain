#!/bin/bash

# Usage:
# Go into cmd loop: sudo ./cliseat.sh
# Run single cmd:  sudo ./cliseat.sh <cliseat paramers>

PREFIX="docker-compose exec nodeseatd cliseat"
if [ -z $1 ] ; then
  while :
  do
    read -e -p "cliseat " cmd
    history -s "$cmd"
    $PREFIX $cmd
  done
else
  $PREFIX "$@"
fi
