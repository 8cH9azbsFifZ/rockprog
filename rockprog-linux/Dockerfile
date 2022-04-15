FROM debian:buster
RUN apt-get update
RUN apt-get -y install libusb-1.0-0-dev libusb-1.0-0 libpopt0 libpopt-dev
RUN apt-get -y install build-essential

WORKDIR /rockprog
ADD . /rockprog
RUN make

ENTRYPOINT ["./rockprog"]
