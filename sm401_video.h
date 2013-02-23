#ifndef sm401_video_h
#define sm401_video_h

#include "sm401_GUI.h"

static const unsigned short PAL_301F0 = 1;
static const unsigned short PAL_30208 = 2;
static const unsigned short PAL_GGSTS = 3;
static const unsigned short PAL_30271 = 4;
static const unsigned short PAL_276E3 = 5;
static const unsigned short PAL_0_1F = 6;
static const unsigned short PAL_BL = 7 ;
static const unsigned short PAL_WH = 8 ;
static const unsigned short PAL_POPUP = 9 ;
static const unsigned short PAL_MISC = 10 ;
static const unsigned short PAL_GENBG = 11 ;
static const unsigned short PAL_HEADS = 12 ;
static const unsigned short PAL_27683 = 13 ;
static const unsigned short PAL_GRAY = 14 ;
static const unsigned short PAL_ = 0 ;


struct struct_color{
	unsigned char r;
	unsigned char g;
	unsigned char b;
};
struct struct_sm_mouse{
	Bit16u event,x,y;
};


void sm_init_video();
void sm_exit_video();
void sm_set_palette(unsigned short pal, unsigned char first_color, unsigned short colors);
void sm_set_palette(Bit8u *pal, unsigned char first_color, unsigned short colors);
unsigned short sm_KeyPressed();
void sm_GetMouse(struct_sm_mouse *sm_mouse);
void wait_for_tick();
unsigned short sm_delay_or_keypress(unsigned int delay);

void sm_images_init_popup(unsigned char* src);
unsigned short sm_popup_line(unsigned short n,char *text,unsigned short x1,unsigned short y1,unsigned short x2,unsigned short y2);
unsigned short sm_print_line(unsigned short n,char *text,unsigned short x1,unsigned short y1,unsigned short x2,unsigned short y2);
unsigned short sm_print_ral_line(unsigned short n,char *text,unsigned short x1,unsigned short y1);
void sm_activate_popup(GUI_class* GUI_);
void sm_images_close_popup();
void sm_images_init_heads(unsigned short n, unsigned char* src);
void sm_images_init_sex(unsigned char* src);
void sm_images_init_typus(unsigned short n, unsigned char* src);

void sm_images_draw_all(bool remain);


void sm_images_init(unsigned short, unsigned short, unsigned short, unsigned char* src);
void sm_images_do_page(unsigned short n);
void sm_images_do_typus(unsigned short);
void sm_images_do_heads(unsigned short n, unsigned short x, unsigned short y);
void sm_images_do_sex(unsigned short sex, unsigned short lev);


void sm_index_init(unsigned short n, unsigned short w, unsigned short h, unsigned char* src);
bool sm_index_inited(unsigned short n);
void sm_index_do(unsigned short n);
void sm_icons_init(unsigned char* src);
void sm_bicons_init(unsigned char* src);
void sm_do_icon(unsigned short n,signed short m);
void sm_images_init_compass(unsigned char* src);
void sm_images_do_compass(unsigned short direction);
void sm_images_init_anis(unsigned short n, unsigned char* src);
void sm_images_do_anis(unsigned short n,unsigned short x,unsigned short y);
void sm_images_init_tempicon(unsigned short n, unsigned char* src);
void sm_images_do_tempicon(unsigned short n);
unsigned short sm_print_loc_line(char *text);




#endif