FROM ubuntu:22.04

RUN apt-get update && apt-get install -y --no-install-recommends \
    g++ \
    make \
    valgrind \
    gdb \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /build
COPY . .

RUN make

CMD [ "./campusguard" ]