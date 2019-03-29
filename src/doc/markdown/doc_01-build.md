## Building the WIP comms demo

These instructions work for Ubuntu 16.04 (xenial) and Ubuntu 18.04 (bionic).

 - Install dependencies:

          sudo apt install software-properties-common
          sudo add-apt-repository ppa:tes/wip
          sudo apt update && sudo apt install git ssh cmake doxygen graphviz \
                     build-essential libprotobuf-dev protobuf-compiler libdccl3-dev dccl3-compiler \
                     libgoby3-dev clang texlive-latex-recommended texlive-latex-extra \
                     texlive-fonts-recommended libcgsn-mooring-dev libopenjp2-7-dev \
                     libgoby3-gui-dev libtiff-dev libpng-dev libmagick++-dev libpqxx-dev

           git clone https://github.com/GobySoft/progressive-imagery.git -b wipcomms
           cd progressive-imagery
           mkdir build
           cd build
           cmake ..
           make -j`nproc`

 - Install runtime tools

          sudo apt install socat goby3-apps

 - Clone wip-comms-benchtop-demo

          git clone git@github.com:WHOIGit/wip-comms-benchtop-demo.git

 - Edit CMakeLists

 - Run `CMake` / `make`:

          cd wip-comms-benchtop-demo
          mkdir build
          # change PROGRESSIVE_IMAGERY_PATH to where you cloned "progressive-imagery"
          cmake .. -DCMAKE_CXX_COMPILER=/usr/bin/clang++ -DPROGRESSIVE_IMAGERY_PATH=/opt/progressive-imagery 
          make -j`nproc`
