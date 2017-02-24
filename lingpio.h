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
#include <stdbool.h>
#include <unistd.h>

#ifndef LINGPIO_H
#define LINGPIO_H
typedef struct{
	//user part
	const char *pinname;
	unsigned pinnum;
	const char *cipname;
	//sysfs part
	unsigned sysfspinnum;
	const char *pindir;
} pindescr_t;

typedef struct {
	pindescr_t *descr;
	int value_fd;
} pindh_t;


#define GPIO_DIR_IN 1
#define GPIO_DIR_OUT 0

#define GPIO_STATE_LO  0
#define GPIO_STATE_HI  1 
#define GPIO_STATE_ERR -1

#define GPIO_EDGEDET_NONE 0
#define GPIO_EDGEDET_RISING 1
#define GPIO_EDGEDET_FALLING 2
#define GPIO_EDGEDET_BOTH 3

#define GPIO_WAIT_TMO 0
#define GPIO_WAIT_EDGE 1 
#define GPIO_WAIT_ERR -1 

bool gpio_export(pindescr_t *pind);
bool gpio_unexport(pindescr_t *pind);

//consider using gpio_prepio_by_sysfspinnum as it also sets direction 
static inline 
bool gpio_export_by_sysfspinnum(int sysfspinnum){
	pindescr_t pind; pind.sysfspinnum=sysfspinnum;
	return 	gpio_export(&pind);
}


bool gpio_set_direction(pindescr_t *pind, int direction);

//consider using gpio_prepio_by_sysfspinnum as it also exports the pin 
static inline 
bool gpio_set_direction_by_sysfspinnum(int sysfspinnum, int direction){
	pindescr_t pind; pind.sysfspinnum=sysfspinnum;
	return 	gpio_set_direction(&pind,direction);
}

bool gpio_prepio(pindescr_t *pind, int direction,pindh_t *pinh);

static inline
bool gpio_prepio_by_sysfspinnum(int sysfspinnum, int direction, pindh_t *pinh){
	pindescr_t pind; pind.sysfspinnum=sysfspinnum;
	return 	gpio_prepio(&pind,direction,pinh);
}

static inline
void gpio_unprepio(pindh_t *pinh){
	close(pinh->value_fd);
}

int gpio_get(pindh_t *pinh);

//this is VERY unefficient! better do gpio_prepio_by_sysfspinnum and keep  the pinh arouind for proper gpio_get
static inline
int gpio_get_by_sysfspinnum(int sysfspinnum){
	pindescr_t pind; pind.sysfspinnum=sysfspinnum;
	pindh_t pinh;
	gpio_prepio(&pind, GPIO_DIR_IN, &pinh);
	return 	gpio_get(&pinh);
	
}

bool gpio_set(pindh_t *pinh,int state);

//this is VERY unefficient! better do gpio_prepio_by_sysfspinnum and keep  the pinh arouind for proper gpio_set
static inline
bool gpio_set_by_sysfspinnum(int sysfspinnum,int state){
	pindescr_t pind; pind.sysfspinnum=sysfspinnum;
	pindh_t pinh;
	gpio_prepio(&pind, GPIO_DIR_OUT, &pinh);
	return 	gpio_set(&pinh,state);
}



#endif
