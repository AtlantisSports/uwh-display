# uwh-display

http://navisjon.com/ultimate-uwh-camera/

https://www.gofundme.com/ultimateuwhcamera

Getting the source
------------------

    $ pushd
    $ git clone git@github.com:jroelofs/uwh-display.git
    $ cd uwh-display/matrix
    $ git submodule update
    $ popd

Building
--------

    $ mkdir build
    $ cd build
    $ cmake ../uwh-display
    $ make

Running the display daemon
--------------------------

    $ sudo ./bin/uwhdd
