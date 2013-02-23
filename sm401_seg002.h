#ifndef sm401_seg002_h
#define sm401_seg002_h

#include <stdio.h>

static const unsigned short KEY_ESC = 27;//0x01;
static const unsigned short KEY_1 = 49;//0x02;
static const unsigned short KEY_2 = 50;//0x03;
static const unsigned short KEY_3 = 51;//0x04;
static const unsigned short KEY_4 = 52;//0x05;
static const unsigned short KEY_5 = 53;//0x06;

static const unsigned short KEY_RET = 13;//0x1c;

static const unsigned short KEY_J = 74;//0x24;
static const unsigned short KEY_Y = 89;//0x2c;
static const unsigned short KEY_N = 78;//0x31;

static const unsigned short KEY_UP = 38;//0x48;
static const unsigned short KEY_LEFT = 37;//0x4b;
static const unsigned short KEY_RIGHT = 39;//0x4d;
static const unsigned short KEY_DOWN = 40;//0x50;

static const unsigned short KEY_PGUP = 33;//0x49;
static const unsigned short KEY_PGDOWN = 34;//0x51;

static const unsigned short KEY_CTRL_F3 = 114;//0x60;
static const unsigned short KEY_CTRL_F4 = 115;//0x61;

static const unsigned short MOUSE_LEFT_DOWN = 1;
static const unsigned short MOUSE_LEFT_UP = 2;
static const unsigned short MOUSE_RIGHT_DOWN = 3;
static const unsigned short MOUSE_RIGHT_UP =4;



struct sm_file{
	FILE* fd;
	Bit32u off,len,len_;
};
struct nvf_desc {
	Bit8u *dst;
	Bit8u *src;
	short nr;
	signed char type;
	Bit8u width;
	Bit8u height;
};


FILE* load_archive_file(unsigned short index, sm_file* smf);
Bit16u read_archive_file( sm_file* smf, Bit8u *buf, Bit16u readsize);
signed int process_nvf(struct nvf_desc *nvf);



#endif