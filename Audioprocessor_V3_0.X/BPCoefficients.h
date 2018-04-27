#ifndef BPCOEFFICIENTS_H
#define	BPCOEFFICIENTS_H

const int LP50Hz[101] = {
      447,    446,    446,    445,    443,    441,    438,    436,
      432,    429,    424,    420,    415,    410,    404,    398,    392,
      385,    378,    371,    364,    356,    348,    340,    331,    323,
      314,    305,    296,    287,    278,    269,    260,    250,    241,
      232,    222,    213,    204,    195,    186,    177,    169,    160,
      152,    143,    135,    127,    120,    112,    105,     98,     91,
       85,     79,     73,     67,     61,     56,     51,     46,     42,
       38,     34,     30,     27,     23,     20,     18,     15,     13,
       11,      9,      7,      6,      4,      3,      2,      1,      1,
        0,      0,     -1,     -1,     -1,     -1,     -1,     -1,     -1,
       -1,     -1,     -1,     -1,     -1,     -1,      0,      0,      0,
        0,      0,      0
};
const int LP100Hz[101] = {
	  817,    816,    813,    808,    801,    791,    780,    767,
      752,    736,    718,    698,    677,    654,    631,    606,    580,
      554,    526,    499,    471,    442,    414,    385,    356,    328,
      300,    273,    246,    220,    195,    170,    147,    124,    103,
       83,     64,     46,     29,     14,      0,    -13,    -24,    -34,
      -43,    -51,    -58,    -63,    -68,    -71,    -74,    -75,    -76,
      -76,    -75,    -74,    -72,    -69,    -67,    -63,    -60,    -56,
      -52,    -48,    -44,    -40,    -36,    -33,    -29,    -25,    -22,
      -18,    -15,    -13,    -10,     -8,     -6,     -4,     -2,     -1,
        0,      1,      2,      2,      2,      3,      3,      3,      2,
        2,      2,      2,      1,      1,      1,      1,      0,      0,
        0,      0,      0
};

const int LP150Hz[101] = {
	 1228,   1224,   1215,   1200,   1178,   1151,   1118,   1080,
     1037,    990,    939,    885,    828,    768,    707,    644,    581,
      518,    455,    393,    333,    275,    219,    166,    116,     69,
       26,    -13,    -48,    -79,   -106,   -128,   -147,   -162,   -173,
     -180,   -184,   -185,   -182,   -178,   -170,   -161,   -151,   -138,
     -125,   -111,    -97,    -82,    -68,    -54,    -40,    -27,    -15,
       -3,      7,     16,     23,     30,     35,     39,     42,     44,
       45,     45,     44,     43,     41,     38,     35,     32,     28,
       25,     21,     18,     14,     11,      8,      6,      4,      2,
        0,     -1,     -2,     -3,     -3,     -4,     -4,     -4,     -3,
       -3,     -3,     -2,     -2,     -1,     -1,     -1,      0,      0,
        0,      0,      0
};

const int LP200Hz[101] = {
	 1638,   1631,   1610,   1574,   1526,   1465,   1392,   1310,
     1218,   1119,   1015,    906,    794,    682,    571,    462,    357,
      257,    164,     78,      0,    -69,   -128,   -178,   -218,   -248,
     -269,   -281,   -285,   -281,   -270,   -254,   -233,   -207,   -179,
     -149,   -118,    -87,    -56,    -27,      0,     25,     46,     65,
       80,     92,    100,    104,    106,    105,    101,     95,     86,
       77,     66,     55,     43,     32,     20,     10,      0,     -9,
      -16,    -22,    -27,    -31,    -33,    -35,    -35,    -34,    -32,
      -30,    -27,    -23,    -20,    -16,    -12,     -9,     -6,     -3,
        0,      2,      4,      6,      7,      7,      7,      7,      7,
        7,      6,      5,      5,      4,      3,      2,      2,      1,
        1,      0,      0
};
const int LP250Hz[101] = {
	 2049,   2035,   1994,   1928,   1837,   1723,   1591,   1443,
     1282,   1112,    938,    763,    591,    426,    270,    128,      0,
     -111,   -203,   -276,   -330,   -366,   -383,   -384,   -371,   -344,
     -307,   -262,   -212,   -158,   -104,    -50,      0,     45,     85,
      117,    142,    159,    169,    170,    166,    155,    139,    119,
       96,     72,     47,     23,      0,    -21,    -39,    -53,    -64,
      -72,    -76,    -76,    -74,    -68,    -61,    -52,    -42,    -31,
      -20,    -10,      0,      9,     16,     22,     26,     28,     30,
       29,     28,     26,     23,     19,     15,     11,      7,      3,
        0,     -3,     -5,     -7,     -8,     -8,     -9,     -8,     -8,
       -7,     -6,     -5,     -4,     -2,     -1,     -1,      0,      1,
        1,      1,      1
};
const int LP300Hz[101] = {
	 2745,   2713,   2619,   2466,   2260,   2011,   1729,   1425,
     1112,    800,    502,    228,    -14,   -216,   -374,   -485,   -551,
     -572,   -553,   -500,   -420,   -321,   -210,    -97,     12,    110,
      192,    254,    293,    310,    305,    280,    238,    184,    122,
       56,     -8,    -68,   -118,   -157,   -182,   -194,   -192,   -177,
     -151,   -117,    -78,    -36,      6,     44,     77,    103,    120,
      128,    127,    118,    102,     80,     55,     28,      1,    -24,
      -46,    -62,    -74,    -79,    -78,    -72,    -61,    -45,    -27,
       -8,     12,     31,     49,     63,     75,     82,     86,     86,
       83,     77,     69,     59,     48,     36,     25,     15,      6,
       -2,     -8,    -12,    -15,    -16,    -16,    -15,    -14,    -12,
       -9,     -7,     -9
};
#endif	/* BPCOEFFICIENTS_H */