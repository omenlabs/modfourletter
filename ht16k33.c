#include <linux/i2c-dev.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdint.h>

#include "ht16k33.h"

uint16_t char_to_led[] = {
  0b00000000000000,
  0b00000000000000,
  0b00000000000000,
  0b00000000000000,
  0b00000000000000,
  0b00000000000000,
  0b00000000000000,
  0b00000000000000,
  0b00000000000000,
  0b00000000000000,
  0b00000000000000,
  0b00000000000000,
  0b00000000000000,
  0b00000000000000,
  0b00000000000000,
  0b00000000000000,
  0b00000000000000,
  0b00000000000000,
  0b00000000000000,
  0b00000000000000,
  0b00000000000000,
  0b00000000000000,
  0b00000000000000,
  0b00000000000000,
  0b00000000000000,
  0b00000000000000,
  0b00000000000000,
  0b00000000000000,
  0b00000000000000,
  0b00000000000000,
  0b00000000000000,
  0b00000000000000,
  0b00000000000000,
  0b00000000000110,
  0b00001000100000,
  0b01001011001110,
  0b01001011101101,
  0b00110000100100,
  0b10001101011101,
  0b00010000000000,
  0b10010000000000,
  0b00100100000000,
  0b11111111000000,
  0b01001011000000,
  0b00100000000000,
  0b00000011000000,
  0b00000000000000,
  0b00110000000000,
  0b00110000111111,
  0b00000000000110,
  0b00000011011011,
  0b00000010001111,
  0b00000011100110,
  0b10000001101001,
  0b00000011111101,
  0b00000000000111,
  0b00000011111111,
  0b00000011101111,
  0b01001000000000,
  0b00101000000000,
  0b10010000000000,
  0b00000011001000,
  0b00100100000000,
  0b01000010000011,
  0b00001010111011,
  0b00000011110111,
  0b01001010001111,
  0b00000000111001,
  0b01001000001111,
  0b00000011111001,
  0b00000001110001,
  0b00000010111101,
  0b00000011110110,
  0b01001000000000,
  0b00000000011110,
  0b10010001110000,
  0b00000000111000,
  0b00010100110110,
  0b10000100110110,
  0b00000000111111,
  0b00000011110011,
  0b10000000111111,
  0b10000011110011,
  0b00000011101101,
  0b01001000000001,
  0b00000000111110,
  0b00110000110000,
  0b10100000110110,
  0b10110100000000,
  0b01010100000000,
  0b00110000001001,
  0b00000000111001,
  0b10000100000000,
  0b00000000001111,
  0b00110000000011,
  0b00000000001000,
  0b00000100000000,
  0b01000001011000,
  0b10000001111000,
  0b00000011011000,
  0b00100010001110,
  0b00100001011000,
  0b00000001110001,
  0b00010010001110,
  0b01000001110000,
  0b01000000000000,
  0b00000000001110,
  0b11011000000000,
  0b00000000110000,
  0b01000011010100,
  0b01000001010000,
  0b00000011011100,
  0b00000101110000,
  0b00010010000110,
  0b00000001010000,
  0b10000010001000,
  0b00000001111000,
  0b00000000011100,
  0b10000000000100,
  0b10100000010100,
  0b10100011000000,
  0b10000000001100,
  0b00100001001000,
  0b00100101001001,
  0b01001000000000,
  0b10010010001001,
  0b00010100100000,
  0b00000000000000
};

void ht16k33_write(struct ht16k33* dev, uint8_t* buffer, size_t len) {
	int res = 0;

	res = write(dev->file, buffer, len);

	if (res != (int)len) {
		printf("res: %d, len: %d\n", res, len);
		perror("Failed to write to device");
		exit(1);
	}
}

struct ht16k33* ht16k33_open() {
	struct ht16k33* dev = calloc(sizeof(struct ht16k33), 1);

	char filename[32];

	if(dev == NULL) {
		perror("Failed to allocate dev");
		exit(1);
	}

	snprintf(filename, 31, "/dev/i2c-%d", HT16K33_BUS);

	dev->file = open(filename, O_RDWR);

	if (dev->file < 0) {
		perror("Failed to open i2c device");
		exit(1);
	}

	if (ioctl(dev->file, I2C_SLAVE, HT16K33_ADDRESS) < 0) {
		perror("Failed to set device address");
		exit(1);
	}

	return dev;
}

void ht16k33_close(struct ht16k33* dev) {
	close(dev->file);
}

void ht16k33_init(struct ht16k33* dev) {

	uint8_t buffer[1];

	buffer[0] = HT16K33_SYSTEM_SETUP | HT16K33_OSCILLATOR;

	ht16k33_write(dev, buffer, sizeof(buffer));
}

void ht16k33_set_brightness(struct ht16k33* dev, int brightness) {

	uint8_t buffer[1];

	// brightness is 0-16, so just mask it.
	buffer[0] = HT16K33_CMD_BRIGHTNESS | (brightness & 0xf);

	ht16k33_write(dev, buffer, sizeof(buffer));
}


void ht16k33_flush_buffer(struct ht16k33* dev) {

	uint8_t buffer[9];
	buffer[0] = 0;

	uint8_t* pos = buffer;

	for(int i = 0 ; i < HT16K33_DISPLAY_LENGTH;  i++) {
		*(++pos) = char_to_led[dev->buffer[i]] & 0xff; 
		*(++pos) = char_to_led[dev->buffer[i]] >> 8; 
	}

	ht16k33_write(dev, buffer, sizeof(buffer));
}

void ht16k33_clear(struct ht16k33* dev) {
	uint8_t buffer[9];

	for(int i = 0 ; i < HT16K33_DISPLAY_LENGTH;  i++) {
		dev->buffer[i] = 0;
	}

	ht16k33_flush_buffer(dev);
}

void ht16k33_outc(struct ht16k33* dev, char c) {


	// shift digits forward
	for(int i = 0; i < HT16K33_DISPLAY_LENGTH;  i++) {
		dev->buffer[i] = dev->buffer[i+1];
	}

	dev->buffer[3] = c;

	ht16k33_flush_buffer(dev);
}
