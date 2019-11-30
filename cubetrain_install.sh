#!/bin/bash
##########################################################################
# This is the CUBETRAIN automated install script for Linux and Mac OS.
# This file was downloaded from https://github.com/ShanChain/cubetrain/
#
# Copyright (c) 2017, Respective Authors all rights reserved.
#
# After June 1, 2018 this software is available under the following terms:
# 
# The MIT License
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
# https://github.com/ShanChain/cubetrain//blob/master/LICENSE.txt
##########################################################################

if [ "$(id -u)" -ne 0 ]; then
        printf "\n\tThis requires sudo. Please run with sudo.\n\n"
        exit -1
fi   

   CWD="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
   if [ "${CWD}" != "${PWD}" ]; then
      printf "\\n\\tPlease cd into directory %s to run this script.\\n \\tExiting now.\\n\\n" "${CWD}"
      exit 1
   fi

   BUILD_DIR="${PWD}/build"
   CMAKE_BUILD_TYPE=Release
   TIME_BEGIN=$( date -u +%s )
   INSTALL_PREFIX="/usr/local/cubetrain"
   VERSION=1.2

   txtbld=$(tput bold)
   bldred=${txtbld}$(tput setaf 1)
   txtrst=$(tput sgr0)

   create_symlink() {
      pushd /usr/local/bin &> /dev/null
      ln -sf ../cubetrain/bin/$1 $1
      popd &> /dev/null
   }

   install_symlinks() {
      printf "\\n\\tInstalling CUBETRAIN Binary Symlinks\\n\\n"
      create_symlink "cliseat"
      create_symlink "cubetrain-abigen"
      create_symlink "cubetrain-launcher"
      create_symlink "cubetrain-s2wasm"
      create_symlink "cubetrain-wast2wasm"
      create_symlink "cubetraincpp"
      create_symlink "keyseatd"
      create_symlink "nodeseat"
   }

   if [ ! -d "${BUILD_DIR}" ]; then
      printf "\\n\\tError, cubetrain_build.sh has not ran.  Please run ./cubetrain_build.sh first!\\n\\n"
      exit -1
   fi

   ${PWD}/scripts/clean_old_install.sh
   if [ $? -ne 0 ]; then
      printf "\\n\\tError occurred while trying to remove old installation!\\n\\n"
      exit -1
   fi

   if ! pushd "${BUILD_DIR}"
   then
      printf "Unable to enter build directory %s.\\n Exiting now.\\n" "${BUILD_DIR}"
      exit 1;
   fi
   
   if ! make install
   then
      printf "\\n\\t>>>>>>>>>>>>>>>>>>>> MAKE installing CUBETRAIN has exited with the above error.\\n\\n"
      exit -1
   fi
   popd &> /dev/null 

   install_symlinks   

   printf "\n\n${bldred}\t _______  _     _  _______  _______ __________  _____    _    ________  _     _\n"
   printf '\t(  ____ \| |   | ||  ___  )(  ____ \\\\___  ___/ ( ___ )/ / \ \ \__  __/ | (   | |\n'
   printf "\t| |    \/| |   | || |   ) || (    \/   ) (   | |    )| /   \ )   ) (   |  \  | |\n"
   printf "\t| |      | |   | || |___) || (__       | |   | | ___)| |___| |   | |   |   \ | |\n"
   printf "\t| |      | |   | || |___  )|  __)      | |   | |\ \  | |___| |   | |   | |\ \| |\n"
   printf "\t| |      | |   | || |   ) || (         | |   | | \ \ | |   | |   | |   | | \   |\n"
   printf "\t| |____/\| (___) ||_|___) || (____/\   | |   | |  \ \| |   | |___) (___| |  \  |\n"
   printf "\t(_______/(_______)\_______)\_______/   |_|   |_|   \_\_|   |_|\_______/|_|   )_|\n${txtrst}"

   printf "\\n\\tCUBETRAIN has been successfully built. \\n\\n"

   printf "\\tFor more information:\\n"
   printf "\\tCUBETRAIN website: https://cubetrain.io\\n"
   printf "\\tCUBETRAIN Telegram channel @ https://t.me/CubeTrainProject\\n"
   printf "\\tCUBETRAIN Twitter: https://twitter.com/CubetrainP\\n"
   printf "\\tCUBETRAIN Steemit: https://steemit.com/@cubetrain\\n"
   printf "\\tCUBETRAIN Reddit: https://www.reddit.com/r/CubeTrain\\n"
   printf "\\tCUBETRAIN Facebook: https://www.facebook.com/The-CubeTrain-Project-207059853499768\\n\\n\\n"
