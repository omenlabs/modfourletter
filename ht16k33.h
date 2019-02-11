#ifndef _HT16K33_H
#define _HT16K33_H

#define HT16K33_BUS             1
#define HT16K33_ADDRESS         0x70
#define HT16K33_BLINK_CMD       0x80
#define HT16K33_BLINK_DISPLAYON 0x01
#define HT16K33_BLINK_OFF       0x00
#define HT16K33_BLINK_2HZ       0x02
#define HT16K33_BLINK_1HZ       0x04
#define HT16K33_BLINK_HALFHZ    0x06
#define HT16K33_SYSTEM_SETUP    0x20
#define HT16K33_OSCILLATOR      0x01
#define HT16K33_CMD_BRIGHTNESS  0xE0
#define HT16K33_DISPLAY_LENGTH  4

struct ht16k33 {
	int file;
	char buffer[HT16K33_DISPLAY_LENGTH];
	int brightness;
};

struct ht16k33* ht16k33_open();
void ht16k33_init(struct ht16k33* dev);
void ht16k33_set_brightness(struct ht16k33* dev, int brightness);
void ht16k33_clear(struct ht16k33* dev);
void ht16k33_outc(struct ht16k33* dev, char c);
void ht16k33_close(struct ht16k33* dev);
#endif
