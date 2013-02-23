


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
*/