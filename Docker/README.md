# Run in docker

Simple and fast setup of CUBETRAIN_SEAT on Docker is also available.

## Install Dependencies

- [Docker](https://docs.docker.com) Docker 17.05 or higher is required
- [docker-compose](https://docs.docker.com/compose/) version >= 1.10.0

## Docker Requirement

- At least 7GB RAM (Docker -> Preferences -> Advanced -> Memory -> 7GB or above)
- If the build below fails, make sure you've adjusted Docker Memory settings and try again.

## Build seat image

```bash
git clone https://github.com/CUBETRAIN/seat.git --recursive  --depth 1
cd seat/Docker
docker build . -t cubetrain/seat
```

The above will build off the most recent commit to the master branch by default. If you would like to target a specific branch/tag, you may use a build argument. For example, if you wished to generate a docker image based off of the v0.0.1 tag, you could do the following:

```bash
docker build -t cubetrain/seat:v0.0.1 --build-arg branch=v0.0.1 .
```

By default, the symbol in cubetrain.system is set to SEAT. You can override this using the symbol argument while building the docker image.

```bash
docker build -t cubetrain/seat --build-arg symbol=<symbol> .
```

## Start nodeseat docker container only

```bash
docker run --name nodeseat -p 8888:8888 -p 9876:9876 -t cubetrain/seat nodeseatd.sh -e --http-alias=nodeseat:8888 --http-alias=127.0.0.1:8888 --http-alias=localhost:8888 arg1 arg2
```

By default, all data is persisted in a docker volume. It can be deleted if the data is outdated or corrupted:

```bash
$ docker inspect --format '{{ range .Mounts }}{{ .Name }} {{ end }}' nodeseat
fdc265730a4f697346fa8b078c176e315b959e79365fc9cbd11f090ea0cb5cbc
$ docker volume rm fdc265730a4f697346fa8b078c176e315b959e79365fc9cbd11f090ea0cb5cbc
```

Alternately, you can directly mount host directory into the container

```bash
docker run --name nodeseat -v /path-to-data-dir:/opt/cubetrain/bin/data-dir -p 8888:8888 -p 9876:9876 -t cubetrain/seat nodeseatd.sh -e --http-alias=nodeseat:8888 --http-alias=127.0.0.1:8888 --http-alias=localhost:8888 arg1 arg2
```

## Get chain info

```bash
curl http://127.0.0.1:8888/v1/chain/get_info
```

## Start both nodeseat and keyseatd containers

```bash
docker volume create --name=nodeseat-data-volume
docker volume create --name=keyseatd-data-volume
docker-compose up -d
```

After `docker-compose up -d`, two services named `nodeseatd` and `keyseatd` will be started. nodeseat service would expose ports 8888 and 9876 to the host. keyseatd service does not expose any port to the host, it is only accessible to cliseat when running cliseat is running inside the keyseatd container as described in "Execute cliseat commands" section.

### Execute cliseat commands

You can run the `cliseat` commands via a bash alias.

```bash
alias cliseat='docker-compose exec keyseatd /opt/cubetrain/bin/cliseat -u http://nodeseatd:8888 --wallet-url http://localhost:8900'
cliseat get info
cliseat get account inita
```

Upload sample exchange contract

```bash
cliseat set contract exchange contracts/exchange/
```

If you don't need keyseatd afterwards, you can stop the keyseatd service using

```bash
docker-compose stop keyseatd
```

### Develop/Build custom contracts

Due to the fact that the cubetrain/seat image does not contain the required dependencies for contract development (this is by design, to keep the image size small), you will need to utilize the cubetrain/seat-dev image. This image contains both the required binaries and dependencies to build contracts using cubetraincpp.

You can either use the image available on [Docker Hub](https://hub.docker.com/r/cubetrain/seat-dev/) or navigate into the dev folder and build the image manually.

```bash
cd dev
docker build -t cubetrain/seat-dev .
```

### Change default configuration

You can use docker compose override file to change the default configurations. For example, create an alternate config file `config2.ini` and a `docker-compose.override.yml` with the following content.

```yaml
version: "2"

services:
  nodeseat:
    volumes:
      - nodeseat-data-volume:/opt/cubetrain/bin/data-dir
      - ./config2.ini:/opt/cubetrain/bin/data-dir/config.ini
```

Then restart your docker containers as follows:

```bash
docker-compose down
docker-compose up
```

### Clear data-dir

The data volume created by docker-compose can be deleted as follows:

```bash
docker volume rm nodeseat-data-volume
docker volume rm keyseatd-data-volume
```

### Docker Hub

Docker Hub image available from [docker hub](https://hub.docker.com/r/cubetrain/seat/).
Create a new `docker-compose.yaml` file with the content below

```bash
version: "3"

services:
  nodeseatd:
    image: cubetrain/seat:latest
    command: /opt/cubetrain/bin/nodeseatd.sh --data-dir /opt/cubetrain/bin/data-dir -e --http-alias=nodeseatd:8888 --http-alias=127.0.0.1:8888 --http-alias=localhost:8888
    hostname: nodeseatd
    ports:
      - 8888:8888
      - 9876:9876
    expose:
      - "8888"
    volumes:
      - nodeseat-data-volume:/opt/cubetrain/bin/data-dir

  keyseatd:
    image: cubetrain/seat:latest
    command: /opt/cubetrain/bin/keyseatd --wallet-dir /opt/cubetrain/bin/data-dir --http-server-address=127.0.0.1:8900 --http-alias=localhost:8900 --http-alias=keyseatd:8900
    hostname: keyseatd
    links:
      - nodeseatd
    volumes:
      - keyseatd-data-volume:/opt/cubetrain/bin/data-dir

volumes:
  nodeseat-data-volume:
  keyseatd-data-volume:

```

*NOTE:* the default version is the latest, you can change it to what you want

run `docker pull cubetrain/seat:latest`

run `docker-compose up`

### CUBETRAIN Testnet

We can easily set up a CUBETRAIN local testnet using docker images. Just run the following commands:

Note: if you want to use the mongo db plugin, you have to enable it in your `data-dir/config.ini` first.

```
# create volume
docker volume create --name=nodeseat-data-volume
docker volume create --name=keyseatd-data-volume
# pull images and start containers
docker-compose -f docker-compose-cubetrain-latest.yaml up -d
# get chain info
curl http://127.0.0.1:8888/v1/chain/get_info
# get logs
docker-compose logs -f nodeseatd
# stop containers
docker-compose -f docker-compose-cubetrain-latest.yaml down
```

The `blocks` data are stored under `--data-dir` by default, and the wallet files are stored under `--wallet-dir` by default, of course you can change these as you want.

### About MongoDB Plugin

Currently, the mongodb plugin is disabled in `config.ini` by default, you have to change it manually in `config.ini` or you can mount a `config.ini` file to `/opt/cubetrain/bin/data-dir/config.ini` in the docker-compose file.
