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

#define  _POSIX_C_SOURCE  200809L

#include "lingpio.h"

#include <stdbool.h>

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h> 
#include <sys/stat.h> 

#include <string.h>
#include <poll.h>

bool gpio_export(pindescr_t *pind) {
	char buf[255];
	int fd = open("/sys/class/gpio/export", O_WRONLY);
	if (fd==-1) return false;
	int sz=snprintf(buf,255, "%d", pind->sysfspinnum); 
	if (write(fd, buf, sz)!=sz) {
		close(fd);
		return false;
	};
	close(fd);
    snprintf(buf,255, "/sys/class/gpio/gpio%d", pind->sysfspinnum);
	struct stat st;
	if (stat(buf, &st)==-1) return false;
	if (!S_ISDIR(st.st_mode)) return false;
	return true;
}

bool gpio_unexport(pindescr_t *pind) {
	char buf[255];
	int fd = open("/sys/class/gpio/unexport", O_WRONLY);
	if (fd==-1) return false;
	int sz=snprintf(buf,255, "%d", pind->sysfspinnum); 
	if (write(fd, buf, sz)!=sz) {
		close(fd);
		return false;
	};
	close(fd);
    snprintf(buf,255, "/sys/class/gpio/gpio%d", pind->sysfspinnum);
	struct stat st;
	if (stat(buf, &st)==-1) return true;
	return false;
}

// DIR_IN/DIR_OUT
bool gpio_set_direction(pindescr_t *pind, int direction) {
	char buf[255];
    snprintf(buf,255, "/sys/class/gpio/gpio%d/direction", pind->sysfspinnum);
    int fd = open(buf, O_WRONLY);
	if (fd==-1) return false;
	
    if (direction==GPIO_DIR_OUT) {
        if (write(fd, "out", 3)!=3) goto fail;
		if (read(fd, buf,5)<3) goto fail;
		if (buf[0]!='o' || buf[1]!='u' || buf[2]!='t') goto fail;
    } else {
        if (write(fd, "in", 2)!=2) goto fail;
		if (read(fd, buf,5)<2) goto fail;
		if (buf[0]!='i' || buf[1]!='n' ) goto fail;
    }
    close(fd);
	return true;
fail:
    close(fd);
	return false;
}


bool gpio_prepio(pindescr_t *pind, int direction,pindh_t *pinh){
	if (!gpio_export(pind)) return false;
	if (!gpio_set_direction(pind,direction)) return false;
	char buf[255];
    snprintf(buf,255, "/sys/class/gpio/gpio%d/value", pind->sysfspinnum);
    pinh->value_fd = open(buf, O_RDWR|O_NONBLOCK);
	if (pinh->value_fd==-1) return false;
	return true;
}

int gpio_get(pindh_t *pinh){
	char value;

	if (pread(pinh->value_fd, &value, 1,0)!=1) return GPIO_STATE_ERR;

	if(value == '0') return GPIO_STATE_LO;
	return GPIO_STATE_HI;
}

bool gpio_set(pindh_t *pinh,int state){
	char value;
	if (state==GPIO_STATE_HI) value='1';
	else if (state==GPIO_STATE_LO) value='0';
	else return false;
	
	if (write(pinh->value_fd, &value, 1)!=1) return false;

	return true;
}

bool gpio_set_edgedetect(pindescr_t *pind,int edgedetect){
	if (!gpio_export(pind)) return false;
	if (!gpio_set_direction(pind,GPIO_DIR_IN)) return false;

	const char *ed;
	unsigned ed_len;
	
	if (edgedetect==GPIO_EDGEDET_NONE)        {ed="none"; ed_len=4;}
	else if (edgedetect==GPIO_EDGEDET_RISING) {ed="rising"; ed_len=6;}
                                                // 1234567890 
	else if (edgedetect==GPIO_EDGEDET_FALLING){ed="falling"; ed_len=7;}
	else if (edgedetect==GPIO_EDGEDET_BOTH)   {ed="both"; ed_len=4;}
	else return false;
	
	char buf[255];
    snprintf(buf,255, "/sys/class/gpio/gpio%d/edge", pind->sysfspinnum);
    int fd = open(buf, O_RDWR);
	if (fd==-1) return false;
	
	if (write(fd, ed, ed_len)!=ed_len) return false;
	return true;
}

int gpio_wait_edge(pindh_t *pinh,int tmo_milliseconds){
	struct pollfd fdset[1];
	
	memset((void*)fdset, 0, sizeof(fdset));

	
	fdset[0].fd = pinh->value_fd;
	fdset[0].events = POLLPRI;

	int rc = poll(fdset, 1, tmo_milliseconds);      

	if (rc < 0) return GPIO_WAIT_ERR;
	
	
	if (rc == 0) return GPIO_WAIT_TMO

	if (fdset[0].revents & POLLPRI) {
		lseek(fdset[1].fd, 0, SEEK_SET);
		len = read(fdset[1].fd, buf, MAX_BUF);
		printf("\npoll() GPIO %d interrupt occurred\n", gpio);
	}
}
