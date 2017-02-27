
#include "../lingpio.h"
#include <stdio.h>
#include <string.h>

int main(int argc, const char *argv[]){
	if (argc!=2){
		printf("provide pin name or sysfs pin number to use\n");
		return -1;
	}
	if (strcmp(argv[1],"--list")==0){
		int av=0;
		pindescr_t*pind=gpio_get_board_info(&av);
		printf("lingpio reports api version %d our heder version is %d\nHere is a list of all currently known pins:\n",av,LINGPIO_API_VERSION);
		while (pind->pinname){
			printf("   %s on chip %s sysfspinnum:%d chipbased pinnum:%d\n",pind->pinname,pind->chipname,pind->sysfspinnum,pind->pinnum);
			pind++;
		}
		return 0;
	}
	printf ("using pin [%s]\n",argv[1]);
	pindescr_t*pind=gpio_get_pind(argv[1]);
	if (pind==NULL){
		printf("lingpio cant't find such pin use --list to list all supported by current board!\n");
		return 1;
	}
	printf("sysfs pin  number for this pin is: %d it is on gpiochip %s\n",pind->sysfspinnum,(pind->chipname==NULL?"???":pind->chipname));
	pinh_t pinh;
	if (!gpio_prepio(pind,GPIO_DIR_IN,&pinh)){
		printf("lingpio failed to prepare the pin for reading!\n");
		return 0;
	}
	int st=gpio_get(&pinh);
	if (st==GPIO_STATE_ERR) {
		printf("error getting current pin state\n");
	} else {
		if (st==GPIO_STATE_LO) printf("current pin state is LO\n");
		else if (st==GPIO_STATE_HI) printf("current pin state is HI\n");
		else printf("current pin state is strange: %d\n",st);
	}
	gpio_unprepio(&pinh);
	return 0;
}

