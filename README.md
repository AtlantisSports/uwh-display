# uwh-display

![Timeshark](/docs/img/timeshark-logo0-cropped.jpg)

About
-----

TImeshark is a custom-built platform for display of timing & scoring
information for Underwater Hockey tournaments. `uwh-display` is the software
that runs on top of it. For details on previous versions of the hardware, see:
[docs/Hardware.md](docs/Hardware.md).

![Display Testing](/docs/img/display-testing.png)

http://navisjon.com/ultimate-uwh-camera/

https://www.gofundme.com/ultimateuwhcamera

Building
--------

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

Running the display daemon
--------------------------

```bash
$ sudo ./bin/uwhdd
```
