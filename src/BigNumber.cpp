#include "BigNumber.h"

#include <canvas.h>
#include <led-matrix.h>
#include <graphics.h>

#include <cassert>

using namespace uwhtimer;
using namespace rgb_matrix;

#define _ 0

#define NW 0x1
#define SW 0x2
#define NE 0x4
#define SE 0x8

#define TL SW|NW|NE
#define BL NW|SW|SE
#define BR SW|SE|NE
#define TR SE|NE|NW

#define LT NW|SW
#define RT NE|SE
#define TP NW|NE
#define BT SW|SE

static char Number32x32_0[256] =
{   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,
    _,   _,   _,  SE, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF,  SW,   _,   _,   _,
    _,   _,  SE, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF,  SW,   _,   _,
    _,   _, 0xF, 0xF,  NW,   _,   _,   _,   _,   _,  SE, 0xF, 0xF, 0xF,   _,   _,
    _,   _, 0xF, 0xF,   _,   _,   _,   _,   _,  SE, 0xF, 0xF, 0xF, 0xF,   _,   _,
    _,   _, 0xF, 0xF,   _,   _,   _,   _,  SE, 0xF, 0xF, 0xF, 0xF, 0xF,   _,   _,
    _,   _, 0xF, 0xF,   _,   _,   _,  SE, 0xF, 0xF, 0xF,  NW, 0xF, 0xF,   _,   _,
    _,   _, 0xF, 0xF,   _,   _,  SE, 0xF, 0xF, 0xF,  NW,   _, 0xF, 0xF,   _,   _,
    _,   _, 0xF, 0xF,   _,  SE, 0xF, 0xF, 0xF,  NW,   _,   _, 0xF, 0xF,   _,   _,
    _,   _, 0xF, 0xF,  SE, 0xF, 0xF, 0xF,  NW,   _,   _,   _, 0xF, 0xF,   _,   _,
    _,   _, 0xF, 0xF, 0xF, 0xF, 0xF,  NW,   _,   _,   _,   _, 0xF, 0xF,   _,   _,
    _,   _, 0xF, 0xF, 0xF, 0xF,  NW,   _,   _,   _,   _,   _, 0xF, 0xF,   _,   _,
    _,   _, 0xF, 0xF, 0xF,  NW,   _,   _,   _,   _,   _,  SE, 0xF, 0xF,   _,   _,
    _,   _,  NE, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF,  NW,   _,   _,
    _,   _,   _,  NE, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF,  NW,   _,   _,   _,
    _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _ };

static char Number32x32_1[256] =
{   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,
    _,   _,   _,   _,   _,   _,  SE, 0xF, 0xF,   _,   _,   _,   _,   _,   _,   _,
    _,   _,   _,   _,   _,  SE, 0xF, 0xF, 0xF,   _,   _,   _,   _,   _,   _,   _,
    _,   _,   _,   _,  SE, 0xF, 0xF, 0xF, 0xF,   _,   _,   _,   _,   _,   _,   _,
    _,   _,   _,  SE, 0xF, 0xF,  TL, 0xF, 0xF,   _,   _,   _,   _,   _,   _,   _,
    _,   _,   _, 0xF, 0xF,  TL,   _, 0xF, 0xF,   _,   _,   _,   _,   _,   _,   _,
    _,   _,   _, 0xF,  TL,   _,   _, 0xF, 0xF,   _,   _,   _,   _,   _,   _,   _,
    _,   _,   _,   _,   _,   _,   _, 0xF, 0xF,   _,   _,   _,   _,   _,   _,   _,
    _,   _,   _,   _,   _,   _,   _, 0xF, 0xF,   _,   _,   _,   _,   _,   _,   _,
    _,   _,   _,   _,   _,   _,   _, 0xF, 0xF,   _,   _,   _,   _,   _,   _,   _,
    _,   _,   _,   _,   _,   _,   _, 0xF, 0xF,   _,   _,   _,   _,   _,   _,   _,
    _,   _,   _,   _,   _,   _,   _, 0xF, 0xF,   _,   _,   _,   _,   _,   _,   _,
    _,   _,   _,   _,   _,   _,   _, 0xF, 0xF,   _,   _,   _,   _,   _,   _,   _,
    _,   _,   _, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF,   _,   _,   _,
    _,   _,   _, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF,   _,   _,   _,
    _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _ };

static char Number32x32_2[256] =
{   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,
    _,   _,   _,   _,  SE, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF,  SW,   _,   _,   _,
    _,   _,   _,  SE, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF,   _,   _,   _,
    _,   _,   _, 0xF, 0xF, 0xF,  NW,   _,   _,   _,  RT, 0xF, 0xF,   _,   _,   _,
    _,   _,   _, 0xF, 0xF,  NW,   _,   _,   _,   _,  RT, 0xF, 0xF,   _,   _,   _,
    _,   _,   _,   _,   _,   _,   _,   _,   _,  SE, 0xF, 0xF, 0xF,   _,   _,   _,
    _,   _,   _,   _,   _,   _,   _,   _,  SE, 0xF, 0xF, 0xF,  NW,   _,   _,   _,
    _,   _,   _,   _,   _,   _,   _,  SE, 0xF, 0xF, 0xF,  NW,   _,   _,   _,   _,
    _,   _,   _,   _,   _,   _,  SE, 0xF, 0xF, 0xF,  NW,   _,   _,   _,   _,   _,
    _,   _,   _,   _,   _,  SE, 0xF, 0xF, 0xF,  NW,   _,   _,   _,   _,   _,   _,
    _,   _,   _,   _,  SE, 0xF, 0xF, 0xF,  NW,   _,   _,   _,   _,   _,   _,   _,
    _,   _,   _,  SE, 0xF, 0xF, 0xF,  NW,   _,   _,   _,   _,   _,   _,   _,   _,
    _,   _,   _, 0xF, 0xF, 0xF,  NW,   _,   _,   _,   _,   _,   _,   _,   _,   _,
    _,   _,   _, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF,   _,   _,   _,
    _,   _,   _, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF,   _,   _,   _,
    _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _ };

static char Number32x32_3[256] =
{   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,
    _,   _,   _,  SE, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF,  SW,   _,   _,   _,
    _,   _,   _, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF,   _,   _,   _,
    _,   _,   _, 0xF, 0xF,   _,   _,   _,   _,   _,  NE, 0xF, 0xF,   _,   _,   _,
    _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _, 0xF, 0xF,   _,   _,   _,
    _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _, 0xF, 0xF,   _,   _,   _,
    _,   _,   _,   _,   _,   _,   _,   _,   _,   _,  SE, 0xF, 0xF,   _,   _,   _,
    _,   _,   _,   _,   _,   _, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF,  NW,   _,   _,   _,
    _,   _,   _,   _,   _,   _, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF,  SW,   _,   _,   _,
    _,   _,   _,   _,   _,   _,   _,   _,   _,   _,  NE, 0xF, 0xF,   _,   _,   _,
    _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _, 0xF, 0xF,   _,   _,   _,
    _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _, 0xF, 0xF,   _,   _,   _,
    _,   _,   _, 0xF, 0xF,   _,   _,   _,   _,   _,  SE, 0xF, 0xF,   _,   _,   _,
    _,   _,   _, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF,   _,   _,   _,
    _,   _,   _,  NE, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF,  NW,   _,   _,   _,
    _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _ };

static char Number32x32_4[256] =
{   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,
    _,   _,   _,   _,   _,   _,   _,   _,   _,  SE, 0xF, 0xF,   _,   _,   _,   _,
    _,   _,   _,   _,   _,   _,   _,   _,  SE, 0xF, 0xF, 0xF,   _,   _,   _,   _,
    _,   _,   _,   _,   _,   _,   _,  SE, 0xF,  TL, 0xF, 0xF,   _,   _,   _,   _,
    _,   _,   _,   _,   _,   _,  SE, 0xF,  TL,   _, 0xF, 0xF,   _,   _,   _,   _,
    _,   _,   _,   _,   _,  SE, 0xF,  TL,   _,   _, 0xF, 0xF,   _,   _,   _,   _,
    _,   _,   _,   _,  SE, 0xF,  TL,   _,   _,   _, 0xF, 0xF,   _,   _,   _,   _,
    _,   _,   _,  SE, 0xF,  TL,   _,   _,   _,   _, 0xF, 0xF,   _,   _,   _,   _,
    _,   _,   _, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF,   _,   _,   _,
    _,   _,   _, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF,   _,   _,   _,
    _,   _,   _,   _,   _,   _,   _,   _,   _,   _, 0xF, 0xF,   _,   _,   _,   _,
    _,   _,   _,   _,   _,   _,   _,   _,   _,   _, 0xF, 0xF,   _,   _,   _,   _,
    _,   _,   _,   _,   _,   _,   _,   _,   _,   _, 0xF, 0xF,   _,   _,   _,   _,
    _,   _,   _,   _,   _,   _,   _,   _,   _,   _, 0xF, 0xF,   _,   _,   _,   _,
    _,   _,   _,   _,   _,   _,   _,   _,   _,   _, 0xF, 0xF,   _,   _,   _,   _,
    _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _ };

static char Number32x32_5[256] =
{   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,
    _,   _,   _, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF,   _,   _,   _,
    _,   _,   _, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF,   _,   _,   _,
    _,   _,   _, 0xF, 0xF,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,
    _,   _,   _, 0xF, 0xF,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,
    _,   _,   _, 0xF, 0xF,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,
    _,   _,   _, 0xF, 0xF,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,
    _,   _,   _, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF,  SW,   _,   _,   _,   _,
    _,   _,   _, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF,  SW,   _,   _,   _,
    _,   _,   _,   _,   _,   _,   _,   _,   _,   _,  TR, 0xF, 0xF,   _,   _,   _,
    _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _, 0xF, 0xF,   _,   _,   _,
    _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _, 0xF, 0xF,   _,   _,   _,
    _,   _,   _,   _,   _,   _,   _,   _,   _,   _,  BR, 0xF, 0xF,   _,   _,   _,
    _,   _,   _, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF,  NW,   _,   _,   _,
    _,   _,   _, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF,  NW,   _,   _,   _,   _,
    _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _ };

static char Number32x32_6[256] =
{   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,
    _,   _,   _,   _,   _,  SE, 0xF, 0xF,   _,   _,   _,   _,   _,   _,   _,   _,
    _,   _,   _,   _,  SE, 0xF, 0xF, 0xF,   _,   _,   _,   _,   _,   _,   _,   _,
    _,   _,   _,  SE, 0xF, 0xF,  NW,   _,   _,   _,   _,   _,   _,   _,   _,   _,
    _,   _,   _, 0xF, 0xF,  NW,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,
    _,   _,   _, 0xF, 0xF,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,
    _,   _,   _, 0xF, 0xF,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,
    _,   _,   _, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF,  SW,   _,   _,   _,   _,
    _,   _,   _, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF,  SW,   _,   _,   _,
    _,   _,   _, 0xF, 0xF,  TL,   _,   _,   _,   _,  TR, 0xF, 0xF,   _,   _,   _,
    _,   _,   _, 0xF, 0xF,   _,   _,   _,   _,   _,   _, 0xF, 0xF,   _,   _,   _,
    _,   _,   _, 0xF, 0xF,   _,   _,   _,   _,   _,   _, 0xF, 0xF,   _,   _,   _,
    _,   _,   _, 0xF, 0xF,  BL,   _,   _,   _,   _,  BR, 0xF, 0xF,   _,   _,   _,
    _,   _,   _,  NE, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF,  NW,   _,   _,   _,
    _,   _,   _,   _,  NE, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF,  NW,   _,   _,   _,   _,
    _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _ };

static char Number32x32_7[256] =
{   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,
    _,   _,   _,  SE, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF,  SW,   _,   _,   _,
    _,   _,   _, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF,   _,   _,   _,
    _,   _,   _, 0xF, 0xF,   _,   _,   _,   _,   _,   _, 0xF, 0xF,   _,   _,   _,
    _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _, 0xF, 0xF,   _,   _,   _,
    _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _, 0xF, 0xF,   _,   _,   _,
    _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _, 0xF, 0xF,   _,   _,   _,
    _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _, 0xF, 0xF,   _,   _,   _,
    _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _, 0xF, 0xF,   _,   _,   _,
    _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _, 0xF, 0xF,   _,   _,   _,
    _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _, 0xF, 0xF,   _,   _,   _,
    _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _, 0xF, 0xF,   _,   _,   _,
    _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _, 0xF, 0xF,   _,   _,   _,
    _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _, 0xF, 0xF,   _,   _,   _,
    _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _, 0xF, 0xF,   _,   _,   _,
    _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _ };

static char Number32x32_8[256] =
{   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,
    _,   _,   _,   _,  SE, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF,  SW,   _,   _,   _,   _,
    _,   _,   _,  SE, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF,  SW,   _,   _,   _,
    _,   _,   _, 0xF, 0xF,  TL,   _,   _,   _,   _,  TR, 0xF, 0xF,   _,   _,   _,
    _,   _,   _, 0xF, 0xF,   _,   _,   _,   _,   _,   _, 0xF, 0xF,   _,   _,   _,
    _,   _,   _, 0xF, 0xF,   _,   _,   _,   _,   _,   _, 0xF, 0xF,   _,   _,   _,
    _,   _,   _, 0xF, 0xF,  BL,   _,   _,   _,   _,  BR, 0xF, 0xF,   _,   _,   _,
    _,   _,   _,  NE, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF,  NW,   _,   _,   _,
    _,   _,   _,  SE, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF,  SW,   _,   _,   _,
    _,   _,   _, 0xF, 0xF,  TL,   _,   _,   _,   _,  TR, 0xF, 0xF,   _,   _,   _,
    _,   _,   _, 0xF, 0xF,   _,   _,   _,   _,   _,   _, 0xF, 0xF,   _,   _,   _,
    _,   _,   _, 0xF, 0xF,   _,   _,   _,   _,   _,   _, 0xF, 0xF,   _,   _,   _,
    _,   _,   _, 0xF, 0xF,  BL,   _,   _,   _,   _,  BR, 0xF, 0xF,   _,   _,   _,
    _,   _,   _,  NE, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF,  NW,   _,   _,   _,
    _,   _,   _,   _,  NE, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF,  NW,   _,   _,   _,   _,
    _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _ };

static char Number32x32_9[256] =
{   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,
    _,   _,   _,  SE, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF,  SW,   _,   _,   _,
    _,   _,   _, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF,   _,   _,   _,
    _,   _,   _, 0xF, 0xF,   _,   _,   _,   _,   _,   _, 0xF, 0xF,   _,   _,   _,
    _,   _,   _, 0xF, 0xF,   _,   _,   _,   _,   _,   _, 0xF, 0xF,   _,   _,   _,
    _,   _,   _, 0xF, 0xF,   _,   _,   _,   _,   _,   _, 0xF, 0xF,   _,   _,   _,
    _,   _,   _, 0xF, 0xF,   _,   _,   _,   _,   _,   _, 0xF, 0xF,   _,   _,   _,
    _,   _,   _, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF,   _,   _,   _,
    _,   _,   _,  NE, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF,   _,   _,   _,
    _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _, 0xF, 0xF,   _,   _,   _,
    _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _, 0xF, 0xF,   _,   _,   _,
    _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _, 0xF, 0xF,   _,   _,   _,
    _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _, 0xF, 0xF,   _,   _,   _,
    _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _, 0xF, 0xF,   _,   _,   _,
    _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _, 0xF, 0xF,   _,   _,   _,
    _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _ };

static char Number32x32_empty[256] =
{   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,
    _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,
    _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,
    _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,
    _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,
    _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,
    _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,
    _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,
    _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,
    _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,
    _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,
    _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,
    _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,
    _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,
    _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,
    _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _ };

static char *Numbers32x32[] = {
   Number32x32_0,
   Number32x32_1,
   Number32x32_2,
   Number32x32_3,
   Number32x32_4,
   Number32x32_5,
   Number32x32_6,
   Number32x32_7,
   Number32x32_8,
   Number32x32_9,
   Number32x32_empty,
};

static char Number16x32_0[128] =
{   _,   _,   _,   _,   _,   _,   _,   _,
    _,  SE, 0xF, 0xF, 0xF, 0xF,  SW,   _,
    _, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF,   _,
    _, 0xF, 0xF,   _,   _, 0xF, 0xF,   _,
    _, 0xF, 0xF,   _,   _, 0xF, 0xF,   _,
    _, 0xF, 0xF,   _,   _, 0xF, 0xF,   _,
    _, 0xF, 0xF,   _,  SE, 0xF, 0xF,   _,
    _, 0xF, 0xF,  SE, 0xF, 0xF, 0xF,   _,
    _, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF,   _,
    _, 0xF, 0xF, 0xF,  TL, 0xF, 0xF,   _,
    _, 0xF, 0xF,  TL,   _, 0xF, 0xF,   _,
    _, 0xF, 0xF,   _,   _, 0xF, 0xF,   _,
    _, 0xF, 0xF,   _,   _, 0xF, 0xF,   _,
    _, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF,   _,
    _,  NE, 0xF, 0xF, 0xF, 0xF,  NW,   _,
    _,   _,   _,   _,   _,   _,   _,   _};

static char Number16x32_1[128] =
{   _,   _,   _,   _,   _,   _,   _,   _,
    _,   _,  SE, 0xF, 0xF,   _,   _,   _,
    _,  SE, 0xF, 0xF, 0xF,   _,   _,   _,
    _, 0xF,  TL, 0xF, 0xF,   _,   _,   _,
    _,  TL,   _, 0xF, 0xF,   _,   _,   _,
    _,   _,   _, 0xF, 0xF,   _,   _,   _,
    _,   _,   _, 0xF, 0xF,   _,   _,   _,
    _,   _,   _, 0xF, 0xF,   _,   _,   _,
    _,   _,   _, 0xF, 0xF,   _,   _,   _,
    _,   _,   _, 0xF, 0xF,   _,   _,   _,
    _,   _,   _, 0xF, 0xF,   _,   _,   _,
    _,   _,   _, 0xF, 0xF,   _,   _,   _,
    _,   _,   _, 0xF, 0xF,   _,   _,   _,
    _, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF,   _,
    _, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF,   _,
    _,   _,   _,   _,   _,   _,   _,   _};

static char Number16x32_2[128] =
{   _,   _,   _,   _,   _,   _,   _,   _,
    _,  SE, 0xF, 0xF, 0xF, 0xF,  SW,   _,
    _, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF,   _,
    _, 0xF, 0xF,   _,   _, 0xF, 0xF,   _,
    _,   _,   _,   _,   _, 0xF, 0xF,   _,
    _,   _,   _,   _,   _, 0xF, 0xF,   _,
    _,   _,   _,   _,  SE, 0xF, 0xF,   _,
    _,   _,   _,  SE, 0xF, 0xF,  TL,   _,
    _,   _,  SE, 0xF, 0xF,  TL,   _,   _,
    _,  SE, 0xF, 0xF,  TL,   _,   _,   _,
    _, 0xF, 0xF,  TL,   _,   _,   _,   _,
    _, 0xF, 0xF,   _,   _,   _,   _,   _,
    _, 0xF, 0xF,   _,   _,   _,   _,   _,
    _, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF,   _,
    _, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF,   _,
    _,   _,   _,   _,   _,   _,   _,   _};

static char Number16x32_3[128] =
{   _,   _,   _,   _,   _,   _,   _,   _,
    _,   SE, 0xF, 0xF, 0xF, 0xF, SW,   _,
    _, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF,   _,
    _, 0xF, 0xF,   _,   _, 0xF, 0xF,   _,
    _,   _,   _,   _,   _, 0xF, 0xF,   _,
    _,   _,   _,   _,   _, 0xF, 0xF,   _,
    _,   _,   _,   _,  SE, 0xF, 0xF,   _,
    _,   _,   _, 0xF, 0xF, 0xF,  NW,   _,
    _,   _,   _, 0xF, 0xF, 0xF,  SW,   _,
    _,   _,   _,   _,   _, 0xF, 0xF,   _,
    _,   _,   _,   _,   _, 0xF, 0xF,   _,
    _,   _,   _,   _,   _, 0xF, 0xF,   _,
    _, 0xF, 0xF,   _,   _, 0xF, 0xF,   _,
    _, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF,   _,
    _,  NE, 0xF, 0xF, 0xF, 0xF,  NW,   _,
    _,   _,   _,   _,   _,   _,   _,   _};

static char Number16x32_4[128] =
{   _,   _,   _,   _,   _,   _,   _,   _,
    _,   _,   _,   _,  SE, 0xF, 0xF,   _,
    _,   _,   _,  SE, 0xF, 0xF, 0xF,   _,
    _,   _,  SE, 0xF, 0xF, 0xF, 0xF,   _,
    _,  SE, 0xF, 0xF,  TL, 0xF, 0xF,   _,
    _, 0xF, 0xF,  TL,   _, 0xF, 0xF,   _,
    _, 0xF, 0xF,   _,   _, 0xF, 0xF,   _,
    _, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF,   _,
    _, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF,   _,
    _,   _,   _,   _,   _, 0xF, 0xF,   _,
    _,   _,   _,   _,   _, 0xF, 0xF,   _,
    _,   _,   _,   _,   _, 0xF, 0xF,   _,
    _,   _,   _,   _,   _, 0xF, 0xF,   _,
    _,   _,   _,   _,   _, 0xF, 0xF,   _,
    _,   _,   _,   _,   _, 0xF, 0xF,   _,
    _,   _,   _,   _,   _,   _,   _,   _};

static char Number16x32_5[128] =
{   _,   _,   _,   _,   _,   _,   _,   _,
    _, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF,   _,
    _, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF,   _,
    _, 0xF, 0xF,   _,   _,   _,   _,   _,
    _, 0xF, 0xF,   _,   _,   _,   _,   _,
    _, 0xF, 0xF,   _,   _,   _,   _,   _,
    _, 0xF, 0xF,   _,   _,   _,   _,   _,
    _, 0xF, 0xF, 0xF, 0xF, 0xF,  SW,   _,
    _, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF,   _,
    _,   _,   _,   _,   _, 0xF, 0xF,   _,
    _,   _,   _,   _,   _, 0xF, 0xF,   _,
    _,   _,   _,   _,   _, 0xF, 0xF,   _,
    _,   _,   _,   _,   _, 0xF, 0xF,   _,
    _, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF,   _,
    _, 0xF, 0xF, 0xF, 0xF, 0xF,  NW,   _,
    _,   _,   _,   _,   _,   _,   _,   _};

static char Number16x32_6[128] =
{   _,   _,   _,   _,   _,   _,   _,   _,
    _,  SE, 0xF, 0xF, 0xF, 0xF,   _,   _,
    _, 0xF, 0xF, 0xF, 0xF, 0xF,   _,   _,
    _, 0xF, 0xF,   _,   _,   _,   _,   _,
    _, 0xF, 0xF,   _,   _,   _,   _,   _,
    _, 0xF, 0xF,   _,   _,   _,   _,   _,
    _, 0xF, 0xF,   _,   _,   _,   _,   _,
    _, 0xF, 0xF, 0xF, 0xF, 0xF,  SW,   _,
    _, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF,   _,
    _, 0xF, 0xF,   _,   _, 0xF, 0xF,   _,
    _, 0xF, 0xF,   _,   _, 0xF, 0xF,   _,
    _, 0xF, 0xF,   _,   _, 0xF, 0xF,   _,
    _, 0xF, 0xF,   _,   _, 0xF, 0xF,   _,
    _, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF,   _,
    _,  NE, 0xF, 0xF, 0xF, 0xF,  NW,   _,
    _,   _,   _,   _,   _,   _,   _,   _};

static char Number16x32_7[128] =
{   _,   _,   _,   _,   _,   _,   _,   _,
    _,  SE, 0xF, 0xF, 0xF, 0xF,  SW,   _,
    _, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF,   _,
    _, 0xF, 0xF,   _,   _, 0xF, 0xF,   _,
    _,   _,   _,   _,   _, 0xF, 0xF,   _,
    _,   _,   _,   _,   _, 0xF, 0xF,   _,
    _,   _,   _,   _,   _, 0xF, 0xF,   _,
    _,   _,   _,   _,   _, 0xF, 0xF,   _,
    _,   _,   _,   _,   _, 0xF, 0xF,   _,
    _,   _,   _,   _,   _, 0xF, 0xF,   _,
    _,   _,   _,   _,   _, 0xF, 0xF,   _,
    _,   _,   _,   _,   _, 0xF, 0xF,   _,
    _,   _,   _,   _,   _, 0xF, 0xF,   _,
    _,   _,   _,   _,   _, 0xF, 0xF,   _,
    _,   _,   _,   _,   _, 0xF, 0xF,   _,
    _,   _,   _,   _,   _,   _,   _,   _};

static char Number16x32_8[128] =
{   _,   _,   _,   _,   _,   _,   _,   _,
    _,  SE, 0xF, 0xF, 0xF, 0xF,  SW,   _,
    _, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF,   _,
    _, 0xF, 0xF,   _,   _, 0xF, 0xF,   _,
    _, 0xF, 0xF,   _,   _, 0xF, 0xF,   _,
    _, 0xF, 0xF,   _,   _, 0xF, 0xF,   _,
    _, 0xF, 0xF,   _,   _, 0xF, 0xF,   _,
    _,  NE, 0xF, 0xF, 0xF, 0xF,  NW,   _,
    _,  SE, 0xF, 0xF, 0xF, 0xF,  SW,   _,
    _, 0xF, 0xF,   _,   _, 0xF, 0xF,   _,
    _, 0xF, 0xF,   _,   _, 0xF, 0xF,   _,
    _, 0xF, 0xF,   _,   _, 0xF, 0xF,   _,
    _, 0xF, 0xF,   _,   _, 0xF, 0xF,   _,
    _, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF,   _,
    _,  NE, 0xF, 0xF, 0xF, 0xF,  NW,   _,
    _,   _,   _,   _,   _,   _,   _,   _};

static char Number16x32_9[128] =
{   _,   _,   _,   _,   _,   _,   _,   _,
    _,  SE, 0xF, 0xF, 0xF, 0xF,  SW,   _,
    _, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF,   _,
    _, 0xF, 0xF,   _,   _, 0xF, 0xF,   _,
    _, 0xF, 0xF,   _,   _, 0xF, 0xF,   _,
    _, 0xF, 0xF,   _,   _, 0xF, 0xF,   _,
    _, 0xF, 0xF,   _,   _, 0xF, 0xF,   _,
    _,  NE, 0xF, 0xF, 0xF, 0xF, 0xF,   _,
    _,   _,  TR, 0xF, 0xF, 0xF, 0xF,   _,
    _,   _,   _,   _,   _, 0xF, 0xF,   _,
    _,   _,   _,   _,   _, 0xF, 0xF,   _,
    _,   _,   _,   _,   _, 0xF, 0xF,   _,
    _,   _,   _,   _,   _, 0xF, 0xF,   _,
    _,   _,   _,   _,   _, 0xF, 0xF,   _,
    _,   _,   _,   _,   _, 0xF, 0xF,   _,
    _,   _,   _,   _,   _,   _,   _,   _};

static char Number16x32_empty[128] =
{   _,   _,   _,   _,   _,   _,   _,   _,
    _,   _,   _,   _,   _,   _,   _,   _,
    _,   _,   _,   _,   _,   _,   _,   _,
    _,   _,   _,   _,   _,   _,   _,   _,
    _,   _,   _,   _,   _,   _,   _,   _,
    _,   _,   _,   _,   _,   _,   _,   _,
    _,   _,   _,   _,   _,   _,   _,   _,
    _,   _,   _,   _,   _,   _,   _,   _,
    _,   _,   _,   _,   _,   _,   _,   _,
    _,   _,   _,   _,   _,   _,   _,   _,
    _,   _,   _,   _,   _,   _,   _,   _,
    _,   _,   _,   _,   _,   _,   _,   _,
    _,   _,   _,   _,   _,   _,   _,   _,
    _,   _,   _,   _,   _,   _,   _,   _,
    _,   _,   _,   _,   _,   _,   _,   _,
    _,   _,   _,   _,   _,   _,   _,   _ };

static char *Numbers16x32[] = {
  Number16x32_0,
  Number16x32_1,
  Number16x32_2,
  Number16x32_3,
  Number16x32_4,
  Number16x32_5,
  Number16x32_6,
  Number16x32_7,
  Number16x32_8,
  Number16x32_9,
  Number16x32_empty
};

#undef _

void BigNumber::Render(Canvas *Canvas,
                       unsigned Display,
                       unsigned Value,
                       const Color &FG,
                       const Color *BG) {
  assert(Value < 100 && "Value out of range");

  if (Value < 10)
    RenderSingle(Canvas, Display, Value, FG, BG);
  else if (Value < 100)
    RenderDouble(Canvas, Display, Value, FG, BG);
}

void BigNumber::RenderHalf(Canvas *Canvas,
                           unsigned Display,
                           unsigned Value,
                           unsigned X, unsigned Y,
                           const Color &FG,
                           const Color *BG) {
  assert(Value < 100 && "Value out of range");

  if (Value < 10)
    RenderHalfSingle(Canvas, Display, X, Y, Value, FG, BG);
  else if (Value < 100)
    RenderHalfDouble(Canvas, Display, X, Y, Value, FG, BG);
}

void BigNumber::RenderSingle(Canvas *Canvas,
                             unsigned Display,
                             unsigned Value,
                             const Color &FG,
                             const Color *BG) {
  assert(Value < 10 && "Value out of range");
  unsigned xoffs = Display * 32;
  for (unsigned y = 0; y < 16; y++) {
    for (unsigned x = 0; x < 16; x++) {
      if (Numbers32x32[Value][x + y * 16] & NW) {
        Canvas->SetPixel(xoffs + x * 2 + 0, y * 2 + 0, FG.r, FG.g, FG.b);
      } else if (BG) {
        Canvas->SetPixel(xoffs + x * 2 + 0, y * 2 + 0, BG->r, BG->g, BG->b);
      }

      if (Numbers32x32[Value][x + y * 16] & SW) {
        Canvas->SetPixel(xoffs + x * 2 + 0, y * 2 + 1, FG.r, FG.g, FG.b);
      } else if (BG) {
        Canvas->SetPixel(xoffs + x * 2 + 0, y * 2 + 1, BG->r, BG->g, BG->b);
      }

      if (Numbers32x32[Value][x + y * 16] & NE) {
        Canvas->SetPixel(xoffs + x * 2 + 1, y * 2 + 0, FG.r, FG.g, FG.b);
      } else if (BG) {
        Canvas->SetPixel(xoffs + x * 2 + 1, y * 2 + 0, BG->r, BG->g, BG->b);
      }

      if (Numbers32x32[Value][x + y * 16] & SE) {
        Canvas->SetPixel(xoffs + x * 2 + 1, y * 2 + 1, FG.r, FG.g, FG.b);
      } else if (BG) {
        Canvas->SetPixel(xoffs + x * 2 + 1, y * 2 + 1, BG->r, BG->g, BG->b);
      }
    }
  }
}

void BigNumber::RenderHalfSingle(Canvas *Canvas,
                                 unsigned Display,
                                 unsigned Value,
                                 unsigned X, unsigned Y,
                                 const Color &FG,
                                 const Color *BG) {
  assert(Value < 10 && "Value out of range");
  unsigned xoffs = Display * 32 + X;
  unsigned yoffs = Y;
  for (unsigned y = 0; y < 16; y++) {
    for (unsigned x = 0; x < 16; x++) {
      if (Numbers32x32[Value][x + y * 16]) {
        Canvas->SetPixel(xoffs + x, yoffs + y, FG.r, FG.g, FG.b);
      } else if (BG) {
        Canvas->SetPixel(xoffs + x, yoffs + y, BG->r, BG->g, BG->b);
      }
    }
  }
}

void BigNumber::RenderDouble(Canvas *Canvas,
                             unsigned Display,
                             unsigned Value,
                             const Color &FG,
                             const Color *BG) {
  assert(Value < 100 && "Value out of range");

  unsigned Tens = Value / 10;
  unsigned Ones = Value % 10;

  unsigned xoffs = Display * 32;

  for (unsigned y = 0; y < 16; y++) {
    for (unsigned x = 0; x < 8; x++) {
      if (Tens != 0 && Numbers16x32[Tens][x + y * 8] & NW) {
        Canvas->SetPixel(xoffs + x * 2 + 0, y * 2 + 0, FG.r, FG.g, FG.b);
      } else if (BG) {
        Canvas->SetPixel(xoffs + x * 2 + 0, y * 2 + 0, BG->r, BG->g, BG->b);
      }

      if (Tens != 0 && Numbers16x32[Tens][x + y * 8] & SW) {
        Canvas->SetPixel(xoffs + x * 2 + 0, y * 2 + 1, FG.r, FG.g, FG.b);
      } else if (BG) {
        Canvas->SetPixel(xoffs + x * 2 + 0, y * 2 + 1, BG->r, BG->g, BG->b);
      }

      if (Tens != 0 && Numbers16x32[Tens][x + y * 8] & NE) {
        Canvas->SetPixel(xoffs + x * 2 + 1, y * 2 + 0, FG.r, FG.g, FG.b);
      } else if (BG) {
        Canvas->SetPixel(xoffs + x * 2 + 1, y * 2 + 0, BG->r, BG->g, BG->b);
      }

      if (Tens != 0 && Numbers16x32[Tens][x + y * 8] & SE) {
        Canvas->SetPixel(xoffs + x * 2 + 1, y * 2 + 1, FG.r, FG.g, FG.b);
      } else if (BG) {
        Canvas->SetPixel(xoffs + x * 2 + 1, y * 2 + 1, BG->r, BG->g, BG->b);
      }
    }
  }

  xoffs += 16;
  for (unsigned y = 0; y < 16; y++) {
    for (unsigned x = 0; x < 8; x++) {
      if (Numbers16x32[Ones][x + y * 8] & NW) {
        Canvas->SetPixel(xoffs + x * 2 + 0, y * 2 + 0, FG.r, FG.g, FG.b);
      } else if (BG) {
        Canvas->SetPixel(xoffs + x * 2 + 0, y * 2 + 0, BG->r, BG->g, BG->b);
      }

      if (Numbers16x32[Ones][x + y * 8] & SW) {
        Canvas->SetPixel(xoffs + x * 2 + 0, y * 2 + 1, FG.r, FG.g, FG.b);
      } else if (BG) {
        Canvas->SetPixel(xoffs + x * 2 + 0, y * 2 + 1, BG->r, BG->g, BG->b);
      }

      if (Numbers16x32[Ones][x + y * 8] & NE) {
        Canvas->SetPixel(xoffs + x * 2 + 1, y * 2 + 0, FG.r, FG.g, FG.b);
      } else if (BG) {
        Canvas->SetPixel(xoffs + x * 2 + 1, y * 2 + 0, BG->r, BG->g, BG->b);
      }

      if (Numbers16x32[Ones][x + y * 8] & SE) {
        Canvas->SetPixel(xoffs + x * 2 + 1, y * 2 + 1, FG.r, FG.g, FG.b);
      } else if (BG) {
        Canvas->SetPixel(xoffs + x * 2 + 1, y * 2 + 1, BG->r, BG->g, BG->b);
      }
    }
  }
}

void BigNumber::RenderHalfDouble(Canvas *Canvas,
                                 unsigned Display,
                                 unsigned Value,
                                 unsigned X, unsigned Y,
                                 const Color &FG,
                                 const Color *BG) {
  assert(Value < 100 && "Value out of range");

  unsigned Tens = Value / 10;
  unsigned Ones = Value % 10;

  unsigned xoffs = Display * 32 + X;
  unsigned yoffs = Y;

  for (unsigned y = 0; y < 16; y++) {
    for (unsigned x = 0; x < 8; x++) {
      if (Tens != 0 && Numbers16x32[Tens][x + y * 8]) {
        Canvas->SetPixel(xoffs + x, yoffs + y, FG.r, FG.g, FG.b);
      } else if (BG) {
        Canvas->SetPixel(xoffs + x, yoffs + y, BG->r, BG->g, BG->b);
      }
    }
  }

  xoffs += 8;
  for (unsigned y = 0; y < 16; y++) {
    for (unsigned x = 0; x < 8; x++) {
      if (Numbers16x32[Ones][x + y * 8]) {
        Canvas->SetPixel(xoffs + x, yoffs + y, FG.r, FG.g, FG.b);
      } else if (BG) {
        Canvas->SetPixel(xoffs + x, yoffs + y, BG->r, BG->g, BG->b);
      }
    }
  }
}

