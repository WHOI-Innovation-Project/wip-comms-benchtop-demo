## Building WIP comms demo

 - Install dependencies:

          sudo apt install software-properties-common
          sudo add-apt-repository ppa:tes/cgsn
          sudo apt update && sudo apt install git ssh cmake doxygen graphviz \
                     build-essential libprotobuf-dev protobuf-compiler libdccl3-dev dccl3-compiler \
                     libgoby3-dev clang texlive-latex-recommended texlive-latex-extra \
                     texlive-fonts-recommended

 - Install runtime tools

          sudo apt install socat goby3-apps

 - Clone cgsn-mooring

          git clone git@github.com:WHOIGit/wip-comms-benchtop-demo.git

 - Run build script or run `CMake` / `make` manually:

          cd wip-comms-benchtop-demo
          ./build.sh

    or equivalently

          cd wip-comms-benchtop-demo
          mkdir build
          cmake ..
          make
