# uwh-display

![Timeshark](/docs/img/timeshark-logo0-cropped.jpg)

## About

Timeshark is a custom-built platform for display of timing & scoring
information for Underwater Hockey tournaments. `uwh-display` is the software
we built that runs on top of it.

![Display Testing](/docs/img/display-testing.png)

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
$ git clone --recursive git@github.com:jroelofs/uwh-display.git
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

![Display v1.0](/docs/img/display-controller-v1.jpg)

#### Display

Timeshark started with a water-resistant CNC machined Delrin enclosure. The
platform was built on top of a Raspberry Pi 2, with a perfboard-prototyped Pi
Hat logic board for driving the chain of off-the-shelf 32x32 LED panels.

![Display v1.0](/docs/img/display-logicboard-v1.jpg)

#### Controller

The first version of the controller was a simple iPhone app, connected to the
display over WiFi. It had basic functionality for updating scores, and
starting/stopping/setting the game clock.

![Controller v1.0](/docs/img/display-controller-v1.gif)

### Version 2.0

#### Display

The second version of Timeshark sported a custom made waterproof enclosure,
with wireless communication over the Zigbee protocol.

![Display v2.0](/docs/img/display-enclosure-v2.jpg)

For this one we designed a custom PCB, and ordered prints of it from OshPark.
This version includes level-shifting buffer chips to convert from the Pi's 3.3v
signals to the 5.0v that the displays prefer, which seems to stabilize the image
and reduce glitches as compared to v1.0. Also included in this version is a
Series 2 Zigbee, with ip68 bulkhead connection and underwater cabling to get the
signal up to the surface.

![Display v2.0 Logic Board](/docs/img/display-logicboard-v2.jpg)

#### Controller

For the controller, we built a stand-alone water-resistant touchscreen
console. Internally, it has a Raspberry Pi v2, and another custom PCB from
OshPark, along with a 7" display, and wireless remote.

![Controller v2.0](/docs/img/display-controller-v2.png)

### Version 2.1

Seeing the difficulty in updating the software on the waterproofed displays, we
opted to upgrade to Raspberry Pi 3's in order to take advantage of their
onboard WiFi chip. Unfortunately, the power budget for the 3.3v rail on the
newer Pis is not enough to drive the power-hungry Zigbee radios. This meant a
bit of electrical re-work to add an external 3.3v supply to the logic boards on
both the displays' PCBs and on the controllers'.

![Controller v2.1](/docs/img/power-hacking.jpg)
