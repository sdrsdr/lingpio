/*
Copyright 2017 Stoian Ivanov <sdr@mail.bg>

based on https://github.com/xtacocorex/CHIP_IO/blob/master/source/common.c
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
#ifndef LINGPIO_BOARD_CHIP_H
#define LINGPIO_BOARD_CHIP_H

#include "board.h"

#if GPIO_BOARD() == BOARD_CHIP
#define DECLARENAME gpio_pins_info
#else 
#define DECLARENAME chip_pins_info
#endif

#include "lingpio.h"
#ifndef GPIO_BOARD_DEFINE_NOW
extern pindescr_t DECLARENAME[];
#else
#include <stdlib.h>
pindescr_t DECLARENAME[] = {
//  pin         chip     chip     sysfs
//  nane       pin num   label    pin num
  { "TWI1-SDA",    48, "default" , 48},
  { "TWI1-SCK",    47, "default" , 47},
  { "LCD-D2",      98, "default" , 98},
  { "PWM0",        34, "default" , 34},
  { "LCD-D4",     100, "default" ,100},
  { "LCD-D3",      99, "default" , 99},
  { "LCD-D6",     102, "default" ,102},
  { "LCD-D5",     101, "default" ,101},
  { "LCD-D10",    106, "default" ,106},
  { "LCD-D7",     103, "default" ,103},
  { "LCD-D12",    108, "default" ,108},
  { "LCD-D11",    107, "default" ,107},
  { "LCD-D14",    110, "default" ,110},
  { "LCD-D13",    109, "default" ,109},
  { "LCD-D18",    114, "default" ,114},
  { "LCD-D15",    111, "default" ,111},
  { "LCD-D20",    116, "default" ,116},
  { "LCD-D19",    115, "default" ,115},
  { "LCD-D22",    118, "default" ,118},
  { "LCD-D21",    117, "default" ,117},
  { "LCD-CLK",    120, "default" ,120},
  { "LCD-D23",    119, "default" ,119},
  { "LCD-VSYNC",  123, "default" ,123},
  { "LCD-HSYNC",  122, "default" ,122},
  { "LCD-DE",     121, "default" ,121},
  { "UART1-TX",   195, "default" ,195},
  { "UART1-RX",   196, "default" ,196},
  { "XIO-P0",       0, "pcf8574a", -1},
  { "XIO-P1",       1, "pcf8574a", -1},
  { "XIO-P2",       2, "pcf8574a", -1},
  { "XIO-P3",       3, "pcf8574a", -1},
  { "XIO-P4",       4, "pcf8574a", -1},
  { "XIO-P5",       5, "pcf8574a", -1},
  { "XIO-P6",       6, "pcf8574a", -1},
  { "XIO-P7",       7, "pcf8574a", -1},
  { "AP-EINT1",   193, "default" ,193},
  { "AP-EINT3",    35, "default" , 35},
  { "TWI2-SDA",    50, "default" , 50},
  { "TWI2-SCK",    49, "default" , 49},
  { "CSIPCK",     128, "default" ,128},
  { "CSICK",      129, "default" ,129},
  { "CSIHSYNC",   130, "default" ,130},
  { "CSIVSYNC",   131, "default" ,131},
  { "CSID0",      132, "default" ,132},
  { "CSID1",      133, "default" ,133},
  { "CSID2",      134, "default" ,134},
  { "CSID3",      135, "default" ,135},
  { "CSID4",      136, "default" ,136},
  { "CSID5",      137, "default" ,137},
  { "CSID6",      138, "default" ,138},
  { "CSID7",      139, "default" ,139},
  { NULL,      -1, NULL ,-1},
};
#endif
#endif
