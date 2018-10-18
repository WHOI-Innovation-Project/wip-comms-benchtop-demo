FROM tsaubergine/progressive-imagery-build-base:1.0.2

WORKDIR /root
RUN rm -rf debian
RUN git clone https://github.com/GobySoft/wip-comms-debian.git -b master debian
RUN apt-get update && mk-build-deps -t "apt-get -y --no-install-recommends" -i "debian/control"
