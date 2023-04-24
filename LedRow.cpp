/*******************************************************************************
Project   RoomLight

  This is an OpenSource Project.
  You can use, share or improve this project. If you improve this source code
  please share with the comunity or at least with the author of the original 
  source code
  
  Created 24. August 2020 by MarkusNTrains
================================================================================
$HeadURL:  $
$Id:  $
*******************************************************************************/


//-----------------------------------------------------------------------------
// includes
#include "LedRow.h"


//-----------------------------------------------------------------------------
// typedef


//-----------------------------------------------------------------------------
// define


//-----------------------------------------------------------------------------
// const
#if (ROOM_LIGHT == ROOM_LIGHT_MarkusNTrains)
const uint16_t LED_MATRIX_LUT[LedRow::LED_ROW_NOF][LedRow::LED_ROW_LENGTH] PROGMEM = {  // PROGMEM -> store data in flash
    { 0,    1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 91, 96, 97, 98, 99, 100,    101,    102,    103,    104,    105,    106,    107,    108,    109,    110,    111,    112,    113,    114,    115,    116,    117,    118,    119,    120,    121,    122,    123,    124,    125,    126,    127,    128,    129,    130,    131,    132,    133,    134,    135,    136,    137,    138,    139,    140,    141,    142,    147,    151,    152,    153,    154,    155,    156,    157,    158,    159,    160,    161,    162,    163,    164,    165,    166,    167,    168,    169,    170,    171,    172,    173,    174,    175,    176,    177,    178,    179,    180,    181,    182,    183,    184,    185,    186,    187,    188,    189,    190,    191,    192,    193,    194,    195,    196,    197,    198,    199,    200,    201,    202,    203,    204,    205,    206,    207,    208,    209,    210,    211,    212,    213,    214,    215,    216,    217,    218,    219,    220,    221,    222,    223,    224,    225,    226,    227,    228,    229,    230,    231,    232,    233,    234,    235,    236,    237,    238,    239,    240,    241,    242,    243,    244,    245,    246,    247,    248,    249,    250,    251,    252,    253,    254,    255,    256,    257,    258,    259,    260,    261,    262,    263,    264,    265,    266,    267,    268,    269,    270,    271,    272,    273,    274,    275,    276,    277,    278,    279,    280,    281,    282,    283,    284,    285,    286,    287,    288,    289,    290,    291,    292,    293,    294,    295,    296,    297,    298,    299},
    { 0,    1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100,    101,    102,    103,    104,    105,    106,    107,    108,    109,    110,    111,    112,    113,    114,    115,    116,    117,    118,    119,    120,    121,    122,    123,    124,    125,    126,    127,    128,    129,    130,    131,    132,    133,    134,    135,    136,    137,    138,    139,    140,    141,    142,    143,    144,    145,    146,    147,    148,    149,    150,    151,    152,    153,    154,    155,    156,    157,    158,    159,    160,    161,    162,    163,    164,    165,    166,    167,    168,    169,    170,    171,    172,    173,    174,    175,    176,    177,    178,    179,    180,    181,    182,    183,    184,    185,    186,    187,    188,    189,    190,    191,    192,    193,    194,    195,    196,    197,    198,    199,    200,    201,    202,    203,    204,    205,    206,    207,    208,    209,    210,    211,    212,    213,    214,    215,    216,    217,    218,    219,    220,    221,    222,    223,    224,    225,    226,    227,    228,    229,    230,    231,    232,    233,    234,    235,    236,    237,    238,    239,    240,    241,    242,    243,    244,    245,    246,    247,    248,    249,    250,    251,    252,    253,    254,    255,    256,    257,    258,    259,    260,    261,    262,    263,    264,    265,    266,    267,    268,    269,    270,    271,    272,    273,    274,    275,    276,    277,    278,    279,    280,    281,    282,    283,    284},
    { 0,    1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100,    101,    102,    103,    104,    105,    106,    107,    108,    109,    110,    111,    112,    113,    114,    115,    116,    117,    118,    119,    120,    121,    122,    123,    124,    125,    126,    127,    128,    129,    130,    131,    132,    133,    134,    135,    136,    137,    138,    139,    140,    141,    142,    143,    144,    145,    146,    147,    148,    149,    150,    151,    152,    153,    154,    155,    156,    157,    158,    159,    160,    161,    162,    163,    164,    165,    166,    167,    168,    169,    170,    171,    172,    173,    174,    175,    176,    177,    178,    179,    180,    181,    182,    183,    184,    185,    186,    187,    188,    189,    190,    191,    192,    193,    194,    195,    196,    197,    198,    199,    200,    201,    202,    203,    204,    205,    206,    207,    208,    209,    210,    211,    212,    213,    214,    215,    216,    217,    218,    219,    220,    221,    222,    223,    224,    225,    226,    227,    228,    229,    230,    231,    232,    233,    234,    235,    236,    237,    238,    239,    240,    241,    242,    243,    244,    245,    246,    247,    248,    249,    250,    251,    252,    253,    254,    255,    256,    257,    258,    259,    260,    261,    262,    263,    264,    265,    266,    267,    268,    269,    270,    271,    272,    273,    274,    275,    276,    277,    278,    279,    280,    281,    282,    283,    284},
    { 0,    1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100,    101,    102,    103,    104,    105,    106,    107,    108,    109,    110,    111,    112,    113,    114,    115,    116,    117,    118,    119,    120,    121,    122,    123,    124,    125,    126,    127,    128,    129,    130,    131,    132,    133,    134,    135,    136,    137,    138,    139,    140,    141,    142,    143,    144,    145,    146,    147,    148,    149,    150,    151,    152,    153,    154,    155,    156,    157,    158,    159,    160,    161,    162,    163,    164,    165,    166,    167,    168,    169,    170,    171,    172,    173,    174,    175,    176,    177,    178,    179,    180,    181,    182,    183,    184,    185,    186,    187,    188,    189,    190,    191,    192,    193,    194,    195,    196,    197,    198,    199,    200,    201,    202,    203,    204,    205,    206,    207,    208,    209,    210,    211,    212,    213,    214,    215,    216,    217,    218,    219,    220,    221,    222,    223,    224,    225,    226,    227,    228,    229,    230,    231,    232,    233,    234,    235,    236,    237,    238,    239,    240,    241,    242,    243,    244,    245,    246,    247,    248,    249,    250,    251,    252,    253,    254,    255,    256,    257,    258,    259,    260,    261,    262,    263,    264,    265,    266,    267,    268,    269,    270,    271,    272,    273,    274,    275,    276,    277,    278,    279,    280,    281,    282,    283,    284},
};

#elif (ROOM_LIGHT == ROOM_LIGHT_Altenglienicke)
const uint16_t LED_MATRIX_LUT[LedRow::LED_ROW_NOF][LedRow::LED_ROW_LENGTH] PROGMEM = {  // PROGMEM -> store data in flash
    { 0,	1,	2,	3,	4,	5,	6,	7,	8,	9,	10,	11,	12,	13,	14,	15,	16,	17,	18,	19,	20,	21,	22,	23,	24,	25,	26,	27,	28,	29,	30,	31,	32,	33,	34,	35,	36,	37,	38,	39,	40,	41,	42,	43,	44,	45,	46,	47,	48,	49,	50,	51,	52,	53,	54,	55,	56,	57,	58,	59,	60,	61,	62,	63,	64,	65,	66,	67,	68,	69,	70,	71,	72,	73,	74,	75,	76,	77,	78,	79,	80,	81,	82,	83,	84,	85,	86,	87,	88,	89,	90,	91,	92,	93,	94,	95,	96,	97,	98,	99,	100,	101,	102,	103,	104,	105,	106,	107,	108,	109,	110,	111,	112,	113,	114,	115,	116,	117,	118,	119,	120,	121,	122,	123,	124,	125,	126,	127,	128,	129,	130,	131,	132,	133,	134,	135,	136,	137,	138,	139,	140,	141,	142,	143,	144,	145,	146,	147,	148,	149,	150,	151,	152,	153,	154,	155,	156,	157,	158,	159,	160,	161,	162,	163,	164,	165,	166,	167,	168,	169,	170,	171,	172,	173,	174,	175,	176,	177,	178,	179,	180,	181,	182,	183,	184,	185,	186,	187,	188,	189,	190,	191,	192,	193,	194,	195,	196,	197,	198,	199,	200,	201,	202,	203,	204,	205,	206,	207,	208,	209,	210,	211,	212,	213,	214,	215,	216,	217,	218,	219,	220,	221,	222,	223,	224,	225,	226,	227,	228,	229,	230,	231,	232,	233,	234,	235,	236,	237,	238,	239},
    { 0,	1,	2,	3,	4,	5,	6,	7,	8,	9,	10,	11,	12,	13,	14,	15,	16,	17,	18,	19,	20,	21,	22,	23,	24,	25,	26,	27,	28,	29,	30,	31,	32,	33,	34,	35,	36,	37,	38,	39,	40,	41,	42,	43,	44,	45,	46,	47,	48,	49,	50,	51,	52,	53,	54,	55,	56,	57,	58,	59,	60,	61,	62,	63,	64,	65,	66,	67,	68,	69,	70,	71,	72,	73,	74,	75,	76,	77,	78,	79,	80,	81,	82,	83,	84,	85,	86,	87,	88,	89,	90,	91,	92,	93,	94,	95,	96,	97,	98,	99,	100,	101,	102,	103,	104,	105,	106,	107,	108,	109,	110,	111,	112,	113,	114,	115,	116,	117,	118,	119,	120,	121,	122,	123,	124,	125,	126,	127,	128,	129,	130,	131,	132,	133,	134,	135,	136,	137,	138,	139,	140,	141,	142,	143,	144,	145,	146,	147,	148,	149,	150,	151,	152,	153,	154,	155,	156,	157,	158,	159,	160,	161,	162,	163,	164,	165,	166,	167,	168,	169,	170,	171,	172,	173,	174,	175,	176,	177,	178,	179,	180,	181,	182,	183,	184,	185,	186,	187,	188,	189,	190,	191,	192,	193,	194,	195,	196,	197,	198,	199,	200,	201,	202,	203,	204,	205,	206,	207,	208,	209,	210,	211,	212,	213,	214,	215,	216,	217,	218,	219,	220,	221,	222,	223,	224,	225,	226,	227,	228,	229,	230,	231,	232,	233,	234,	235,	236,	237,	238,	239},
	{ 0,	1,	2,	3,	4,	5,	6,	7,	8,	9,	10,	11,	12,	13,	14,	15,	16,	17,	18,	19,	20,	21,	22,	23,	24,	25,	26,	27,	28,	29,	30,	31,	32,	33,	34,	35,	36,	37,	38,	39,	40,	41,	42,	43,	44,	45,	46,	47,	48,	49,	50,	51,	52,	53,	54,	55,	56,	57,	58,	59,	60,	61,	62,	63,	64,	65,	66,	67,	68,	69,	70,	71,	72,	73,	74,	75,	76,	77,	78,	79,	80,	81,	82,	83,	84,	85,	86,	87,	88,	89,	90,	91,	92,	93,	94,	95,	96,	97,	98,	99,	100,	101,	102,	103,	104,	105,	106,	107,	108,	109,	110,	111,	112,	113,	114,	115,	116,	117,	118,	119,	120,	121,	122,	123,	124,	125,	126,	127,	128,	129,	130,	131,	132,	133,	134,	135,	136,	137,	138,	139,	140,	141,	142,	143,	144,	145,	146,	147,	148,	149,	150,	151,	152,	153,	154,	155,	156,	157,	158,	159,	160,	161,	162,	163,	164,	165,	166,	167,	168,	169,	170,	171,	172,	173,	174,	175,	176,	177,	178,	179,	180,	181,	182,	183,	184,	185,	186,	187,	188,	189,	190,	191,	192,	193,	194,	195,	196,	197,	198,	199,	200,	201,	202,	203,	204,	205,	206,	207,	208,	209,	210,	211,	212,	213,	214,	215,	216,	217,	218,	219,	220,	221,	222,	223,	224,	225,	226,	227,	228,	229,	230,	231,	232,	233,	234,	235,	236,	237,	238,	239},
	{ 0,	1,	2,	3,	4,	5,	6,	7,	8,	9,	10,	11,	12,	13,	14,	15,	16,	17,	18,	19,	20,	21,	22,	23,	24,	25,	26,	27,	28,	29,	30,	31,	32,	33,	34,	35,	36,	37,	38,	39,	40,	41,	42,	43,	44,	45,	46,	47,	48,	49,	50,	51,	52,	53,	54,	55,	56,	57,	58,	59,	60,	61,	62,	63,	64,	65,	66,	67,	68,	69,	70,	71,	72,	73,	74,	75,	76,	77,	78,	79,	80,	81,	82,	83,	84,	85,	86,	87,	88,	89,	90,	91,	92,	93,	94,	95,	96,	97,	98,	99,	100,	101,	102,	103,	104,	105,	106,	107,	108,	109,	110,	111,	112,	113,	114,	115,	116,	117,	118,	119,	120,	121,	122,	123,	124,	125,	126,	127,	128,	129,	130,	131,	132,	133,	134,	135,	136,	137,	138,	139,	140,	141,	142,	143,	144,	145,	146,	147,	148,	149,	150,	151,	152,	153,	154,	155,	156,	157,	158,	159,	160,	161,	162,	163,	164,	165,	166,	167,	168,	169,	170,	171,	172,	173,	174,	175,	176,	177,	178,	179,	180,	181,	182,	183,	184,	185,	186,	187,	188,	189,	190,	191,	192,	193,	194,	195,	196,	197,	198,	199,	200,	201,	202,	203,	204,	205,	206,	207,	208,	209,	210,	211,	212,	213,	214,	215,	216,	217,	218,	219,	220,	221,	222,	223,	224,	225,	226,	227,	228,	229,	230,	231,	232,	233,	234,	235,	236,	237,	238,	239},
	{ 0,	1,	2,	3,	4,	5,	6,	7,	8,	9,	10,	11,	12,	13,	14,	15,	16,	17,	18,	19,	20,	21,	22,	23,	24,	25,	26,	27,	28,	29,	30,	31,	32,	33,	34,	35,	36,	37,	38,	39,	40,	41,	42,	43,	44,	45,	46,	47,	48,	49,	50,	51,	52,	53,	54,	55,	56,	57,	58,	59,	60,	61,	62,	63,	64,	65,	66,	67,	68,	69,	70,	71,	72,	73,	74,	75,	76,	77,	78,	79,	80,	81,	82,	83,	84,	85,	86,	87,	88,	89,	90,	91,	92,	93,	94,	95,	96,	97,	98,	99,	100,	101,	102,	103,	104,	105,	106,	107,	108,	109,	110,	111,	112,	113,	114,	115,	116,	117,	118,	119,	120,	121,	122,	123,	124,	125,	126,	127,	128,	129,	130,	131,	132,	133,	134,	135,	136,	137,	138,	139,	140,	141,	142,	143,	144,	145,	146,	147,	148,	149,	150,	151,	152,	153,	154,	155,	156,	157,	158,	159,	160,	161,	162,	163,	164,	165,	166,	167,	168,	169,	170,	171,	172,	173,	174,	175,	176,	177,	178,	179,	180,	181,	182,	183,	184,	185,	186,	187,	188,	189,	190,	191,	192,	193,	194,	195,	196,	197,	198,	199,	200,	201,	202,	203,	204,	205,	206,	207,	208,	209,	210,	211,	212,	213,	214,	215,	216,	217,	218,	219,	220,	221,	222,	223,	224,	225,	226,	227,	228,	229,	230,	231,	232,	233,	234,	235,	236,	237,	238,	239},
	{ 0,	1,	2,	3,	4,	5,	6,	7,	8,	9,	10,	11,	12,	13,	14,	15,	16,	17,	18,	19,	20,	21,	22,	23,	24,	25,	26,	27,	28,	29,	30,	31,	32,	33,	34,	35,	36,	37,	38,	39,	40,	41,	42,	43,	44,	45,	46,	47,	48,	49,	50,	51,	52,	53,	54,	55,	56,	57,	58,	59,	60,	61,	62,	63,	64,	65,	66,	67,	68,	69,	70,	71,	72,	73,	74,	75,	76,	77,	78,	79,	80,	81,	82,	83,	84,	85,	86,	87,	88,	89,	90,	91,	92,	93,	94,	95,	96,	97,	98,	99,	100,	101,	102,	103,	104,	105,	106,	107,	108,	109,	110,	111,	112,	113,	114,	115,	116,	117,	118,	119,	120,	121,	122,	123,	124,	125,	126,	127,	128,	129,	130,	131,	132,	133,	134,	135,	136,	137,	138,	139,	140,	141,	142,	143,	144,	145,	146,	147,	148,	149,	150,	151,	152,	153,	154,	155,	156,	157,	158,	159,	160,	161,	162,	163,	164,	165,	166,	167,	168,	169,	170,	171,	172,	173,	174,	175,	176,	177,	178,	179,	180,	181,	182,	183,	184,	185,	186,	187,	188,	189,	190,	191,	192,	193,	194,	195,	196,	197,	198,	199,	200,	201,	202,	203,	204,	205,	206,	207,	208,	209,	210,	211,	212,	213,	214,	215,	216,	217,	218,	219,	220,	221,	222,	223,	224,	225,	226,	227,	228,	229,	230,	231,	232,	233,	234,	235,	236,	237,	238,	239}
};

#else // ROOM_LIGHT_TestBoard
const uint16_t LED_MATRIX_LUT[LedRow::LED_ROW_NOF][LedRow::LED_ROW_LENGTH] PROGMEM = {  // PROGMEM -> store data in flash
    {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13 }
};
#endif


//-----------------------------------------------------------------------------
// static module variable



//*****************************************************************************
// description:
//   constructor
//*****************************************************************************
LedRow::LedRow(Adafruit_NeoPixel* led_strip_p, uint8_t row_idx)
{
    this->m_led_strip_p = led_strip_p;
    this->m_row_idx = row_idx;
}


//*****************************************************************************
// description:
//   destructor
//*****************************************************************************
LedRow::~LedRow()
{
}


//*****************************************************************************
// description:
//   Returns led idx from loock up table
//*****************************************************************************
uint16_t LedRow::GetLedIdxOfLut(uint16_t idx)
{
    return pgm_read_word_near(&LED_MATRIX_LUT[this->m_row_idx][idx]);
}


//*****************************************************************************
// description:
//   Send the updated pixel colors to the hardware.
//*****************************************************************************
void LedRow::Show(void)
{
    this->m_led_strip_p->show();    
}


//*****************************************************************************
// description:
//   Get Pixel Color
//*****************************************************************************
uint32_t LedRow::GetPixelColor(uint16_t idx)
{
    return this->m_led_strip_p->getPixelColor(this->GetLedIdxOfLut(idx));
}


//*****************************************************************************
// description:
//   Set Pixel
//*****************************************************************************
void LedRow::SetPixel(uint16_t idx, uint32_t color)
{
    this->m_led_strip_p->setPixelColor(this->GetLedIdxOfLut(idx), color);
}


//*****************************************************************************
// description:
//   Show White Pixel
//*****************************************************************************
void LedRow::SetPixel(uint16_t start_idx, uint16_t width, uint16_t space, uint16_t nof_repeat, uint32_t color)
{
    uint16_t px = 0;
    uint16_t idx = 0;
    uint16_t cnt = 0;
    uint16_t end_idx = 0;
    uint16_t nof_px_to_fill = 0;
    uint16_t additional_px = 0;
    
    if (   (start_idx >= LED_ROW_LENGTH)
        || (width == 0))
    { return; }
    
    if ((start_idx + width) > LED_ROW_LENGTH)
    {
        width = LED_ROW_LENGTH - start_idx;
    }

    for (cnt = 0; cnt < nof_repeat; cnt++)
    {
        nof_px_to_fill = 0;
        /*for (px = 0; px < width; px++)
        {
            // check if position is further than the rowlength
            if ((offset + px) >= this->m_length) { break; }
            
            idx = offset + px;
        Serial.println(idx);
            this->m_led_strip_p->setPixelColor(this->m_lookup_table_p[idx], color);    
        }*/
        end_idx = start_idx + width;
        if (end_idx >= LED_ROW_LENGTH) {
            end_idx = LED_ROW_LENGTH - 1;
            additional_px = 1;
        }
        if (this->GetLedIdxOfLut(start_idx) <= this->GetLedIdxOfLut(end_idx))
        {
            idx = this->GetLedIdxOfLut(start_idx);
            nof_px_to_fill = (this->GetLedIdxOfLut(end_idx) - idx);
        }
        else
        {
            idx = this->GetLedIdxOfLut((end_idx));
            nof_px_to_fill = (this->GetLedIdxOfLut(start_idx) - idx);  
        }

        nof_px_to_fill += additional_px;
        this->m_led_strip_p->fill(color, idx, nof_px_to_fill);
      
        start_idx += space + width;      
        // check if offset is further than the rowlength
        if (start_idx >= LED_ROW_LENGTH) { break; }
    }
}
