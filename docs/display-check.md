display-check
=============

This is a tool for verifying expected output of the display library.

Example
-------

```text
# RUN: display-check %s

# SET-STATE: FirstHalf
# SET-BLACK: 5
# SET-WHITE: 3
# SET-TIME: 42

P3
96 32
255

# Image data elided
```

The basic structure is a standard PPM file, with a number of 'SET' directives
embedded as comments. These directives control setup of the GameModel to be
rendered, which is then compared against the file's image data. Since the file
still conforms to the PPM format, the testcases themselves can be viewed in
standard image manipulation tools.


Supported Directives
--------------------
* SET-STATE
* SET-BLACK
* SET-WHITE
* SET-TIME
* SET-DUMP

