#pragma once

// Constants for XYZ color conversions
#define REF_X  95.047F
#define REF_Y 100.000F
#define REF_Z 108.883F

// Values to automatically define min path length, best to tweak those in desmos
#define PATH_MIN_LIN_SCALE	0.00001F
#define PATH_MIN_EXP_SCALE	1.90F
#define PATH_MIN_INV_SCALE	1.65F

// Values needed by bitmap headers
#define BMP_HDR_SIZE			14
#define BITMAPINOFHEADER_SIZE	40