## Imaging Flow CytoBot (IFCB) Prototype Payload setup

This gives more technical details about the embedded payload for the prototype described in this [Innovation Card](https://github.com/WHOIGit/wip-comms-benchtop-demo/wiki/Innovation-Card).


### Filesystem Image

The filesystem image was created using this project: [wip_rootfs-build](https://github.com/GobySoft/wip_rootfs-build):

     cd wip_rootfs-build
     sudo lb clean
     lb config
     sudo lb build

This is a fork of the [cgsn-mooring_rootfs-build](https://bitbucket.org/ooicgsn/cgsn-mooring_rootfs-build/) project.

Please reference this [script](https://bitbucket.org/ooicgsn/cgsn-mooring_rootfs-build/src/f2959ac2e4e490b88f156b68e0187191019adbc5/scripts/master_raspi_base_image.sh?at=master&fileviewer=file-view-default) from `cgsn-mooring` to see the full process if you are not familiar with creating Raspberry Pi filesystem images.

### Installing the software

Follow the [Building](doc_01-build.md) instructions for installing the dependencies using APT and clone `wip-comms-benchtop-demo`, but instead of building `progressive-imagery` and `wip-comms-benchtop-demo` from source, use the Debian packages built using CircleCI:

  - [progressive-imagery](https://48-126860141-gh.circle-artifacts.com/0/root/deb/progressive-imagery_1.0.0%2B25%2Bg8c06160-0~ubuntu16.04.1_armhf.deb)
  - [wip-comms](https://56-137246482-gh.circle-artifacts.com/0/root/deb/wip-comms_0.1.0~alpha1%2B56%2Bg91d5578-0~ubuntu16.04.1_armhf.deb)

Use `dpkg -i *.deb` to install these packages.

The versions deployed for the fall 2018-winter 2019 test were:
```
dpkg --list | egrep "wip-comms|progressive-imagery"
ii  progressive-imagery                    1.0.0+25+g8c06160-0~ubuntu16.04.1          armhf        Progressive Image Transmission for low-speed links
ii  wip-comms                              0.1.0~alpha1+56+g91d5578-0~ubuntu16.04.1   armhf        Software for the WHOI Innovation Project (WIP) Communications Demonstration
```

You'll need to install the [pyifcb](https://github.com/joefutrelle/pyifcb) and [wip-comms-ifcb-imagedb](https://github.com/WHOIGit/wip-comms-ifcb-imagedb) projects as well. For Ubuntu 16.04, I had to use [this branch](https://github.com/GobySoft/pyifcb/tree/ubuntu16.04) of `pyifcb`.

### IFCB Windows Computer

Used another USB-Ethernet adapter on the IFCB Windows computer to directly connect to the payload.

Enable Internet Connection Sharing for the internet enabled adapter ("Local Area Connection 2", then set "Home networking connection: Local Area Connection 3"). Windows machine is 192.168.137.1/24 and other machines are DHCP (assigned to 192.168.137.x/24, but GobySoft payload is statically assigned to 192.168.137.10).

### Samba Mount

Set up read-only mount from the IFCB Windows machine to `/media/data/ifcb` in `/etc/fstab`:
```
//192.168.137.1/data /media/data/ifcb cifs username={user},password={password},vers=2.0,dir_mode=0555,file_mode=0444,x-systemd.automount,x-systemd.device-timeout=30 0 0
```

### PostgresSQL

The `wip-comms-ifcb-imagedb` and `wip-comms-benchtop-demo` communicate through a PostgresSQL server on the Payload. The database is `wipcomms` and the username should be `wipcomms`. The server needs to be installed and configured manually:

This is a good [tutorial on setting up PostgreSQL](https://www.howtoforge.com/tutorial/postgresql-on-ubuntu-16-04/).

In addition, I used the `phppgadmin` tool to administrate the database.

### Setting up systemd

Install all the systemd service files in `/home/gobysoft/wip-comms-benchtop-demo/config/systemd/veh1`:

      cd /home/gobysoft/wip-comms-benchtop-demo/config/systemd/veh1
      for i in *.service;
        do sudo systemctl enable `pwd`/$i;
      done

### Set up OpenVPN

Copy the client configuration for OpenVPN from the [server setup](doc_20-shore-server.md) instructions.

I had to patch the Ubuntu 16.04 openvpn service since it would sometimes quit on startup if the network wasn't available, and wouldn't restart without these additional commands:

```
--- /media/root-ro/lib/systemd/system/openvpn@.service  2016-02-02 12:33:30.000000000 +0000
+++ /media/root-rw/overlay/lib/systemd/system/openvpn@.service 2018-11-02 12:02:19.600687566 +0000
@@ -20,6 +20,8 @@
 LimitNPROC=10
 DeviceAllow=/dev/null rw
 DeviceAllow=/dev/net/tun rw
+Restart=always
+RestartSec=10
 
 [Install]
 WantedBy=multi-user.target
```