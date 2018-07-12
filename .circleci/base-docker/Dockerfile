FROM ubuntu:xenial

RUN apt-get update && apt-get -y upgrade && apt-get -y --no-install-recommends install devscripts equivs software-properties-common git ssh clang
RUN add-apt-repository ppa:tes/cgsn -y 
RUN dpkg --add-architecture armhf
# add armhf ubuntu repos
RUN sed -i 's/deb /deb [arch=amd64] /' /etc/apt/sources.list && egrep "^deb " /etc/apt/sources.list | sed 's/amd64/armhf/' | sed 's|http://.*.ubuntu.com/ubuntu/|http://ports.ubuntu.com/|' >> /etc/apt/sources.list

WORKDIR /root
RUN git clone https://bitbucket.org/ooicgsn/cgsn-mooring-debian.git -b master debian
RUN apt-get update && mk-build-deps -t "apt-get -y --no-install-recommends" -i "debian/control"
