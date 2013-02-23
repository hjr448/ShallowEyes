#ifndef SM_401_GUI_h
#define SM_401_GUI_h


class GUI_class{
public:
	GUI_class(){menue_tiles=3;shift_x=0;shift_y=0;is_bool=0;};
	void set_menue_tiles(unsigned short n){menue_tiles=n;};
	signed short radio(Bit8u* text, signed char opt, ...);
	signed short radio_bool(Bit8u *text,Bit8u *ja,Bit8u *nein);
	signed short input(Bit8u*, unsigned short);
	signed short enter_text(char* dst, unsigned short x, unsigned short y, unsigned short num, unsigned short num_bel);
	unsigned short left_border ;//0xbfff
	unsigned short right_border ;
	unsigned short upper_border;//0xc001
	unsigned short upper_options;//0xc001
	unsigned short menue_tiles ;//0xbffd
	unsigned short header_lines;// l_di
	unsigned short options;
	unsigned short button;//0xe5b0
	signed short shift_x;    //0x2ca2
private:
	void draw_radio_bg();
	signed short menu_input();
	signed short shift_y;    //0x2ca4
	unsigned short text_x; //0xd2d9
	unsigned short text_w; //0xd2d5
	unsigned short text_y;//0xd2d7
	unsigned short is_bool;//0xac0b
};

/*
//25
char GUI_lookup_char_height(char c, unsigned short *p);
//2a
void GUI_1c2(unsigned short v1, unsigned short v2, unsigned short v3, unsigned short v4);
//2f
signed short GUI_enter_text(Bit8u *, unsigned short, unsigned short, unsigned short, unsigned short);
//0x34
signed short GUI_input(Bit8u*, unsigned short);
//0x39
void GUI_output(Bit8u*);
//0x3e
signed short GUI_bool(Bit8u*);
//0x43
signed short GUI_radio(Bit8u*, signed char, ...);
//4d
void GUI_draw_radio_bg(Bit16u, Bit16u, Bit16u, Bit16u);
// 52
//static
void GUI_copy_smth(unsigned short width, unsigned short height);
//0x57
void GUI_fill_radio_button(signed short, unsigned short, unsigned short);
//0x5c
signed short GUI_dialogbox(RealPt, Bit8u*, Bit8u*, signed char, ...);
//0x61
signed short GUI_menu_input(unsigned short, unsigned short, unsigned short);
//0x6b
void GUI_dialog_na(unsigned short, Bit8u*);

//seg096
//20
void GUI_write_char_to_screen(PhysPt dst, unsigned short v1, unsigned short v2);
//25
RealPt GUI_names_grammar(unsigned short v1, unsigned short v2, unsigned short v3);
//2a
RealPt GUI_name_plural(unsigned short v1, Bit8u *s);
//2f
RealPt GUI_name_singular(Bit8u *s);
//34
RealPt GUI_2f2(unsigned short v1, unsigned short v2, unsigned short v3);
//39
RealPt GUI_get_ptr(Bit16u, Bit16u);
//3e
RealPt GUI_get_ptr2(unsigned short v1, unsigned short v2);
//43
void GUI_print_string(Bit8u *str, unsigned short x, unsigned short y);
//48
unsigned short GUI_print_char(unsigned char c, unsigned short x, unsigned short y);
//4d
unsigned short GUI_get_first_pos_centered(Bit8u *p, unsigned short v1, unsigned short v2, unsigned short v3);
//52
unsigned short GUI_count_lines(Bit8u *str);
//57
unsigned short GUI_lookup_char_width(unsigned char c, unsigned short *p);
//5c
unsigned short GUI_unused(Bit8u *str);
//61
void GUI_write_fonti_to_screen(unsigned short fi, unsigned short char_width, unsigned short x, unsigned short y);
//66
void GUI_blank_char();
//6B
void GUI_font_to_buf(Bit8u *p);
//70
void GUI_write_char_to_screen_xy(unsigned short x, unsigned short y, unsigned short char_height, unsigned short char_width);
//75
void set_textcolor(unsigned short, unsigned short);
void get_textcolor(unsigned short*, unsigned short*);
//7f*/
void GUI_print_loc_line(char *str);
//84
unsigned short GUI_print_header(Bit8u *str);/*
//89
unsigned short GUI_get_space_for_string(Bit8u *p, unsigned short dir);
//8e
void GUI_draw_popup_line(unsigned short line, unsigned short type);

*/

#endif