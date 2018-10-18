FROM tsaubergine/progressive-imagery-build-armhf:1.0.2

WORKDIR /root
RUN rm -rf debian
RUN git clone https://github.com/GobySoft/wip-comms-debian.git -b master debian

RUN apt-get update && apt-get install --no-install-recommends -y libpng12-dev:armhf libpq-dev:armhf libcgsn-mooring-dev:armhf  libi2c-dev:armhf