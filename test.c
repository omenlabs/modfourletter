#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "ht16k33.h"


int main() {
	struct ht16k33* dev;
	dev = ht16k33_open();
	ht16k33_init(dev);
	ht16k33_set_brightness(dev, 15);
	char *foo = "Hello World";
	
	for(int i ; i < strlen(foo); i++) {
		ht16k33_outc(dev, foo[i]);
		for(int i =0 ; i < 4; i++) {
			printf("%c", dev->buffer[i]);
		}
		printf("\n");
		sleep(1);
	}
	ht16k33_clear(dev);
	ht16k33_close(dev);
	return 0;
}
