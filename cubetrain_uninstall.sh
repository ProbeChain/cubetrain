#! /bin/bash

binaries=(cliseat
          cubetrain-abigen
          cubetrain-launcher
          cubetrain-s2wasm
          cubetrain-wast2wasm
          cubetraincpp
          keyseatd
          nodeseat
          cubetrain-applesdemo)

if [ -d "/usr/local/cubetrain" ]; then
   printf "\tDo you wish to remove this install? (requires sudo)\n"
   select yn in "Yes" "No"; do
      case $yn in
         [Yy]* )
            if [ "$(id -u)" -ne 0 ]; then
               printf "\n\tThis requires sudo, please run ./cubetrain_uninstall.sh with sudo\n\n"
               exit -1
            fi

            pushd /usr/local &> /dev/null
            rm -rf cubetrain
            pushd bin &> /dev/null
            for binary in ${binaries[@]}; do
               rm ${binary}
            done
            # Handle cleanup of directories created from installation
            if [ "$1" == "--full" ]; then
               if [ -d ~/Library/Application\ Support/cubetrain ]; then rm -rf ~/Library/Application\ Support/cubetrain; fi # Mac OS
               if [ -d ~/.local/share/cubetrain ]; then rm -rf ~/.local/share/cubetrain; fi # Linux
            fi
            popd &> /dev/null
            break;;
         [Nn]* )
            printf "\tAborting uninstall\n\n"
            exit -1;;
      esac
   done
fi
