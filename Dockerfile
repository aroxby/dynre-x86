FROM ubuntu:bionic

RUN apt-get update && apt-get -y install build-essential cmake git

COPY . /src
WORKDIR src

RUN make
CMD ./dynre-x86.exe