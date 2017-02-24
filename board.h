/*
Copyright 2017 Stoian Ivanov <sdr@mail.bg>
*/
/*

	This file is part of lingpio library.

	lingpio library is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation,version 3 of the License

	lingpio library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with lingpio library.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef LINGPIO_BOARD_H
#define LINGPIO_BOARD_H

#define BOARD_UNKNOWN 0
#define BOARD_CHIP 1

#ifndef GPIO_BOARD 
#define GPIO_BOARD() BOARD_UNKNOWN
#endif

#if GPIO_BOARD() == BOARD_CHIP
#include "board_chip.h"
#else 
#include "board_unknown.h"
#endif

#endif
