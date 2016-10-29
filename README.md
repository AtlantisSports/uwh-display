# uwh-display

http://navisjon.com/ultimate-uwh-camera/

https://www.gofundme.com/ultimateuwhcamera

Building
--------

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
