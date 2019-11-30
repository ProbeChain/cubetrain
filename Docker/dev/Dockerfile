FROM cubetrain/builder
ARG branch=master
ARG symbol=SEAT

RUN git clone -b $branch https://github.com/CUBETRAIN/seat.git --recursive \
    && cd seat && echo "$branch:$(git rev-parse HEAD)" > /etc/cubetrain-version \
    && cmake -H. -B"/opt/cubetrain" -GNinja -DCMAKE_BUILD_TYPE=Release -DWASM_ROOT=/opt/wasm -DCMAKE_CXX_COMPILER=clang++ \
       -DCMAKE_C_COMPILER=clang -DCMAKE_INSTALL_PREFIX=/opt/cubetrain  -DSecp256k1_ROOT_DIR=/usr/local -DBUILD_MONGO_DB_PLUGIN=true -DCORE_SYMBOL_NAME=$symbol \
    && cmake --build /opt/cubetrain --target install \
    && cp /seat/Docker/config.ini / && ln -s /opt/cubetrain/contracts /contracts && cp /seat/Docker/nodeseatd.sh /opt/cubetrain/bin/nodeseatd.sh && ln -s /seat/tutorials /tutorials

RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get -y install openssl ca-certificates vim psmisc python3-pip && rm -rf /var/lib/apt/lists/*
RUN pip3 install numpy
ENV CUBETRAIN_ROOT=/opt/cubetrain
RUN chmod +x /opt/cubetrain/bin/nodeseatd.sh
ENV LD_LIBRARY_PATH /usr/local/lib
VOLUME /opt/cubetrain/bin/data-dir
ENV PATH /opt/cubetrain/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin
