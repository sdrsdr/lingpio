#include "../lingpio.h"
#include <stdio.h>
#include <string.h>

int main(int argc, const char *argv[]){
	if (argc!=2){
		printf("provide pin name or sysfs pin number to use\n");
		return -1;
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
		return 2;
	}
	int st=gpio_get(&pinh);
	if (st==GPIO_STATE_ERR) {
		printf("error getting current pin state\n");
	} else {
		if (st==GPIO_STATE_LO) printf("current pin state is LO\n");
		else if (st==GPIO_STATE_HI) printf("current pin state is HI\n");
		else printf("current pin state is strange: %d\n",st);
	}
	
	if (!gpio_set_edgedetect(pind,GPIO_EDGEDET_BOTH)) {
		printf("failed to setup edge detect!\n");
		gpio_unprepio(&pinh);
		return 3;
	};
	int cnt=20;
	while (cnt--) {
		int wer=gpio_wait_edge(&pinh,1000);
		if (wer==GPIO_WAIT_TMO){
			printf("%d tmo\n",cnt);
		} else if (wer==GPIO_WAIT_EDGE){
			printf("%d edge state %s\n",cnt,(gpio_get(&pinh)==GPIO_STATE_HI?"HI":"LO"));
		} else {
			printf("%d wait err state %s\n",cnt,(gpio_get(&pinh)==GPIO_STATE_HI?"HI":"LO")); //this is not complete enough
		}
	}
	gpio_unprepio(&pinh);
	return 0;
}


