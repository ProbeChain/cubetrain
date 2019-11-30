FROM cubetrain/builder as builder
ARG branch=master
ARG symbol=SEAT

RUN git clone -b $branch https://github.com/CUBETRAIN/seat.git --recursive \
    && cd seat && echo "$branch:$(git rev-parse HEAD)" > /etc/cubetrain-version \
    && cmake -H. -B"/tmp/build" -GNinja -DCMAKE_BUILD_TYPE=Release -DWASM_ROOT=/opt/wasm -DCMAKE_CXX_COMPILER=clang++ \
       -DCMAKE_C_COMPILER=clang -DCMAKE_INSTALL_PREFIX=/tmp/build  -DSecp256k1_ROOT_DIR=/usr/local -DBUILD_MONGO_DB_PLUGIN=true -DCORE_SYMBOL_NAME=$symbol \
    && cmake --build /tmp/build --target install && rm /tmp/build/bin/cubetraincpp


FROM ubuntu:18.04

RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get -y install openssl ca-certificates && rm -rf /var/lib/apt/lists/*
COPY --from=builder /usr/local/lib/* /usr/local/lib/
COPY --from=builder /tmp/build/bin /opt/cubetrain/bin
COPY --from=builder /tmp/build/contracts /contracts
COPY --from=builder /seat/Docker/config.ini /
COPY --from=builder /etc/cubetrain-version /etc
COPY --from=builder /seat/Docker/nodeseatd.sh /opt/cubetrain/bin/nodeseatd.sh
ENV CUBETRAIN_ROOT=/opt/cubetrain
RUN chmod +x /opt/cubetrain/bin/nodeseatd.sh
ENV LD_LIBRARY_PATH /usr/local/lib
VOLUME /opt/cubetrain/bin/data-dir
ENV PATH /opt/cubetrain/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin
