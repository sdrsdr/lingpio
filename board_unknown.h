/*
 * Copyright 2017 Stoian Ivanov <sdr@mail.bg>
 * 
 * based on https://github.com/xtacocorex/CHIP_IO/blob/master/source/common.c
 */
/*
 * 
 *	This file is part of lingpio library.
 * 
 *	lingpio library is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU Lesser General Public License as published by
 *	the Free Software Foundation,version 3 of the License
 * 
 *	lingpio library is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 * 
 *	You should have received a copy of the GNU Lesser General Public License
 *	along with lingpio library.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */
#ifndef LINGPIO_BOARD_UNKNOWN_H
#define LINGPIO_BOARD_UNKNOWN_H

#include "board.h"

#if GPIO_BOARD() == BOARD_UNKNOWN
#define DECLARENAME gpio_pins_info
#else 
#define DECLARENAME unknown_pins_info
#endif

#include "lingpio.h"
#ifndef GPIO_BOARD_DEFINE_NOW
extern pindescr_t DECLARENAME[];
#else
#include <stdlib.h>
pindescr_t DECLARENAME[] = {
	{ NULL,      -1, NULL ,-1},
};
#endif
#endif

