# uwh-display

![Timeshark](https://cloud.githubusercontent.com/assets/3663689/22133237/56fb6d52-de7c-11e6-925c-f44e24a107d2.jpg)

## About

TImeshark is a custom-built platform for display of timing & scoring
information for Underwater Hockey tournaments. `uwh-display` is the software
that runs on top of it.

![Display Testing](https://cloud.githubusercontent.com/assets/3663689/22133234/56f992d4-de7c-11e6-9f25-92d360ce3acb.png)

http://navisjon.com/ultimate-uwh-camera/

https://www.gofundme.com/ultimateuwhcamera

## Building

Prerequisites:

* cmake
* make
* g++
* swig
* python-dev
* libxbee

On Linux:

```bash
$ git clone --recursive git@github.com:Navisjon/uwh-display.git
$ cd uwh-display
$ mkdir build
$ cd build
$ cmake ..
$ make -j
```

## Running the tests

```bash
$ make test
```

## Running the display daemon

```bash
$ sudo ./bin/uwhdd
```

## Hardware

### Version 1.0

![Display v1.0](https://cloud.githubusercontent.com/assets/3663689/22133232/56bb09d8-de7c-11e6-8b97-6d863e91652d.jpg)

#### Display

Timeshark started with a water-resistant CNC machined Delrin enclosure. The
platform was built on top of a Raspberry Pi2, with a perfboard-prototyped Pi
Hat logic board for driving the chain of 32x32 LED panels.

![Display v1.0](https://cloud.githubusercontent.com/assets/3663689/22133229/56b8428e-de7c-11e6-9119-3807a8f20501.jpg)

#### Controller

The first version of the controller was a simple iPhone app, connected to the
display over WiFi. It had basic functionality for updating scores, and
starting/stopping/setting the game clock.

![Controller v1.0](https://cloud.githubusercontent.com/assets/3663689/22133232/56bb09d8-de7c-11e6-8b97-6d863e91652d.jpg)

### Version 2.0

#### Display

The second version of Timeshark sported a custom made waterproof enclosure,
with wireless communication over the Zigbee protocol.

![Display v2.0](https://cloud.githubusercontent.com/assets/3663689/22133230/56b940a8-de7c-11e6-8a5a-4fc9a52629dc.jpg)

For this one we designed a custom logic board, and ordered prints of it from
OshPark. This version includes level-shifting buffer chips to convert from the
Pi's 3.3v signals to the 5.0v that the displays prefer, which seems to
stabilize the image and reduce glitches as compared to v1.0. Also included in
this version is a Series 2 Zigbee, with ip68 bulkhead connection and underwater
cabling to get the signal up to the surface.

![Display v2.0 Logic Board](https://cloud.githubusercontent.com/assets/3663689/22133233/56f917dc-de7c-11e6-9e7f-29189c21c2a2.jpg)

#### Controller

For the controller, we built a stand-alone water-resistant touchscreen
interface. Internally, it has a Raspberry Pi v2, and another custom PCB from
OshPark, along with a 7\" display, and wireless remote.

![Controller v2.0](https://cloud.githubusercontent.com/assets/3663689/22133228/56b6f73a-de7c-11e6-88e6-037cae95bba9.png)

### Version 2.1

Seeing the difficulty in updating the software on the waterproofed displays, we
opted to upgrade the Raspberry Pis to v3 in order to take advantage of their
onboard WiFi chip. Unfortunately, the power budget for the 3.3v rail on the
newer Pis is not enough to drive the power-hungry Zigbee radios. This meant a
bit of electrical re-work to add an external 3.3v supply to the logic boards on
both the display's PCB and on the Controller's.

![Controller v2.1](https://cloud.githubusercontent.com/assets/3663689/22133236/56faf9bc-de7c-11e6-8449-b1447cb0f611.jpg)
