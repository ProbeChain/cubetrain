#!/bin/sh
cd /opt/cubetrain/bin

if [ -f '/opt/cubetrain/bin/data-dir/config.ini' ]; then
    echo
  else
    cp /config.ini /opt/cubetrain/bin/data-dir
fi

if [ -d '/opt/cubetrain/bin/data-dir/contracts' ]; then
    echo
  else
    cp -r /contracts /opt/cubetrain/bin/data-dir
fi

while :; do
    case $1 in
        --config-dir=?*)
            CONFIG_DIR=${1#*=}
            ;;
        *)
            break
    esac
    shift
done

if [ ! "$CONFIG_DIR" ]; then
    CONFIG_DIR="--config-dir=/opt/cubetrain/bin/data-dir"
else
    CONFIG_DIR=""
fi

exec /opt/cubetrain/bin/nodeseat $CONFIG_DIR "$@"
