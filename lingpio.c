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

#define _POSIX_C_SOURCE  200809L
#define _GNU_SOURCE 

#include "lingpio.h"

#define GPIO_BOARD_DEFINE_NOW
#include "board.h"
#undef GPIO_BOARD_DEFINE_NOW

#include <stdbool.h>

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h> 
#include <sys/stat.h> 

#include <string.h>
#include <poll.h>
#include <dirent.h> 
#include <time.h>

static pindescr_t *current_info=gpio_pins_info;
pindescr_t *gpio_get_board_info(int *apiversion){
	if (apiversion) *apiversion=LINGPIO_API_VERSION;
	return current_info;
}
void gpio_set_board_info(pindescr_t *info){
	current_info=info;
}

bool gpio_export(pindescr_t *pind) {
	char buf[255];
	
    snprintf(buf,255, "/sys/class/gpio/gpio%d", pind->sysfspinnum);
	struct stat st;
	if (stat(buf, &st)==0 && S_ISDIR(st.st_mode)) {
		return true;
	}

	
	
	
	int fd = open("/sys/class/gpio/export", O_WRONLY);
	if (fd==-1) {
		return false;
	}
	int sz=snprintf(buf,255, "%d", pind->sysfspinnum); 
	if (write(fd, buf, sz)!=sz) {
		close(fd);
		//check if priorly exported:
		snprintf(buf,255, "/sys/class/gpio/gpio%d", pind->sysfspinnum);
		if (stat(buf, &st)==0 && S_ISDIR(st.st_mode)) {
			return true;
		}
		return false;
	};
	close(fd);
    snprintf(buf,255, "/sys/class/gpio/gpio%d", pind->sysfspinnum);
	if (stat(buf, &st)==0 && S_ISDIR(st.st_mode)) {
		return true;
	}
	return false;
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
    int fd = open(buf, O_RDWR);
	if (fd==-1) {
		return false;
	}
    if (direction==GPIO_DIR_OUT) {
        if (write(fd, "out", 3)!=3) goto fail;
		if (pread(fd, buf,5,0)<3) goto fail;
		if (buf[0]!='o' || buf[1]!='u' || buf[2]!='t') goto fail;
    } else {
        if (write(fd, "in", 2)!=2) goto fail;
		if (pread(fd, buf,5,0)<2) goto fail;
		if (buf[0]!='i' || buf[1]!='n' ) goto fail;
    }
    close(fd);
	return true;
fail:
	close(fd);
	return false;
}


bool gpio_prepio(pindescr_t *pind, int direction,pinh_t *pinh){
	pinh->descr=pind;
	if (!gpio_export(pind)) return false;
	if (!gpio_set_direction(pind,direction)) {
		gpio_unexport(pind);
		return false;
	}
	char buf[255];
    snprintf(buf,255, "/sys/class/gpio/gpio%d/value", pind->sysfspinnum);
	if (direction==GPIO_DIR_IN) {
		pinh->value_fd = open(buf, O_RDONLY|O_NONBLOCK);
	} else {
		pinh->value_fd = open(buf, O_WRONLY|O_NONBLOCK);
	}
	if (pinh->value_fd==-1) {
		gpio_unexport(pind);
		return false;
	}
	return true;
}

int gpio_get(pinh_t *pinh){
	char value;

	if (pread(pinh->value_fd, &value, 1,0)!=1) return GPIO_STATE_ERR;

	if(value == '0') return GPIO_STATE_LO;
	return GPIO_STATE_HI;
}

bool gpio_set(pinh_t *pinh,int state){
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

int gpio_wait_edge(pinh_t *pinh,int tmo_milliseconds){
	struct pollfd fdset[1];
	
	memset((void*)fdset, 0, sizeof(fdset));

	
	fdset[0].fd = pinh->value_fd;
	fdset[0].events = POLLPRI;

	int rc = poll(fdset, 1, tmo_milliseconds);

	if (rc < 0) return GPIO_WAIT_ERR;
	
	
	if (rc == 0) return GPIO_WAIT_TMO;

	if (fdset[0].revents & POLLPRI) return GPIO_WAIT_EDGE;
	return GPIO_WAIT_ERR;
}

pindescr_t *gpio_get_pind(const char *pinname){
	pindescr_t *cpin=current_info;
	while (cpin->pinname!=NULL) {
		if (strcasecmp(pinname,cpin->pinname)==0){
			if (cpin->sysfspinnum>=0) {
				return cpin;
			}
			if (cpin->chipname==NULL) return false;
			DIR *dir=opendir("/sys/class/gpio/");
			if (dir==NULL) {
				return NULL;
			}
			struct dirent *de;
			while ((de=readdir(dir))!=NULL){
				int l=strlen(de->d_name);
				if (l<9) continue;
				if (memcmp("gpiochip",de->d_name,8)==0) {
					char buf[255];
					snprintf(buf,255, "/sys/class/gpio/%s/label", de->d_name);
					int fh=open(buf,O_RDONLY);
					if (fh==-1){
						continue;
					};
					int sz=read(fh,buf,254);
					if (sz<=0) {
						close(fh); 
						continue;
					}
					buf[sz]=0;
					if (sz>0 && (buf[sz-1]=='\r' || buf[sz-1]=='\n')) {
						buf[sz-1]=0; sz--;
					}
					if (sz>0 && (buf[sz-1]=='\r' || buf[sz-1]=='\n')) {
						buf[sz-1]=0; sz--;
					}
					close(fh); 
					if (strcmp(buf,cpin->chipname)==0) {
						snprintf(buf,255, "/sys/class/gpio/%s/base", de->d_name);
						fh=open(buf,O_RDONLY);
						if (fh==-1){
							break;
						};
						sz=read(fh,buf,254);
						if (sz<=0) {
							close(fh); 
							break;
						}
						buf[sz]=0;
						if (sz>0 && (buf[sz-1]=='\r' || buf[sz-1]=='\n')) {
							buf[sz-1]=0; sz--;
						}
						if (sz>0 && (buf[sz-1]=='\r' || buf[sz-1]=='\n')) {
							buf[sz-1]=0; sz--;
						}
						int basei=atoi(buf);
						cpin->sysfspinnum=basei+cpin->pinnum;
						close(fh); closedir(dir);
						pindescr_t *xpin=current_info;
						while (xpin->pinname!=NULL) {
							if (strcasecmp(xpin->chipname,cpin->chipname)==0 &&xpin->sysfspinnum<0) xpin->sysfspinnum=basei+xpin->pinnum;
							xpin++;
						}
						return cpin;
					}
				}
			}
			closedir(dir);
			return NULL;
		}
		cpin++;
		
	}
	return NULL;
}


int gpio_microsleep (unsigned micros) {
   struct timespec req, rem;

   if(micros > 999999)
   {   
        req.tv_sec = (int)(micros / 1000000);                            /* Must be Non-Negative */
        req.tv_nsec = (micros - ((long)req.tv_sec * 1000000)) * 1000; /* Must be in range of 0 to 999999999 */
   }   
   else
   {   
        req.tv_sec = 0;                         /* Must be Non-Negative */
        req.tv_nsec = micros * 1000;    /* Must be in range of 0 to 999999999 */
   }   

   if (nanosleep(&req , &rem)==0) return 0;
   else return rem.tv_sec*1000000+rem.tv_nsec/1000;
}

int gpio_millisleep (unsigned millis) {
   struct timespec req, rem;

   if(millis > 999)
   {   
        req.tv_sec = (int)(millis / 1000);                            /* Must be Non-Negative */
        req.tv_nsec = (millis - ((long)req.tv_sec * 1000)) * 1000000; /* Must be in range of 0 to 999999999 */
   }   
   else
   {   
        req.tv_sec = 0;                         /* Must be Non-Negative */
        req.tv_nsec = millis * 1000000;    /* Must be in range of 0 to 999999999 */
   }   

   if (nanosleep(&req , &rem)==0) return 0;
   else return rem.tv_sec*1000+rem.tv_nsec/1000000;
}
