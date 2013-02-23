#ifndef sm401_video_h
#define sm401_video_h


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
void sm_set_palette(Bit8u *ptr, unsigned char first_color, unsigned short colors);
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

#endif