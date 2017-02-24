
#include "../lingpio.h"
#include <stdio.h>
int main(int argc, const char *argv[]){
	if (argc!=2){
		printf("provide pin name or sysfs pin number to use\n");
		return -1;
	}
	printf ("using pin [%s]\n",argv[1]);
	return 0;
}

