#pragma once

#include "quantum.h"

#define LAYOUT(\
	L00, L01, L02, 			R00, R01, R02, \
	L10, L11, L12,			R10, R11, R12, \
	L20, L21, L22, 			R20, R21, R22  \
	) \
	{ \
	{ L00, L01, L02 }, \
	{ L10, L11, L12 }, \
	{ L20, L21, L22 }, \
	{ R00, R01, R02 }, \
	{ R10, R11, R12 }, \
	{ R20, R21, R22 }  \
}
