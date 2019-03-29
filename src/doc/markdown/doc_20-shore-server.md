## Imaging Flow CytoBot (IFCB) Prototype Shore server setup

This gives more technical details about the shore server for the prototype described in this [Innovation Card](https://github.com/WHOIGit/wip-comms-benchtop-demo/wiki/Innovation-Card).

### CityCloud

This virtual machine is running in [CityCloud](https://citycontrolpanel.com), an OpenStack provider. I used Ubuntu 18.04 with 1 CPU, 2G RAM, and 20G disk space. I would recommend more disk space for a new provisioning, as the images collected over several months would fill the disk.


### Installing the software

Follow the [Building](doc_01-build.md) instructions on building the code. Ensure that the following environmental variables are set (in `~/.bashrc`):

```
export PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin
export PATH=$PATH:$HOME/wip-comms-benchtop-demo/bin
export PATH=$PATH:$HOME/progressive-imagery/build/src/goby
export GOBY_LIAISON_PLUGINS=$HOME/wip-comms-benchtop-demo/build/lib/libwip_comms_liaison.so
```

### Setting up systemd

Install all the systemd service files in `/home/gobysoft/wip-comms-benchtop-demo/config/systemd/server`:

      cd /home/gobysoft/wip-comms-benchtop-demo/config/systemd/server
      for i in *.service;
        do sudo systemctl enable `pwd`/$i;
      done


### OpenVPN

Following instructions [here](https://www.digitalocean.com/community/tutorials/how-to-set-up-an-openvpn-server-on-ubuntu-18-04), set up an OpenVPN server:

 - Server: 172.19.22.1
 - Payload: 172.19.22.20

Create a client configuration (I called the client `pi0`), then copy the payload client configuration file, including all the keys (I called it `pi0.ovpn`) to the Payload as `/etc/openvpn/wipcomms.conf`.

When this is all set up correctly, the Payload should create a tunnel Ethernet interface with the above IP address. This allows the Payload to be reachable directly from the server, regardless of the NAT or Dynamic IP setup.