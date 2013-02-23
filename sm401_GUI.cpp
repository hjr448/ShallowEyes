/*
 *      Rewrite of DSA1 v3.02_de functions of seg097 (GUI)
 *      Functions rewritten 13/16
 *
 *      Functions called rewritten 12/13
 *      Functions uncalled rewritten 1/3
*/

#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <stdio.h>

#include "sm401_main.h"
#include "sm401_seg002.h"
#include "sm401_GUI.h"
#include "sm401_video.h"




//000
//129
//15e
/*
char GUI_lookup_char_height(char c, unsigned short *p)
{
	short i;
	Bit8u *tab = p_datseg + 0xab42;

	for (i = 0; i != 210; i+=3) {
		if (c != ds_readb(0xab42 + i))
			continue;

		*p = ds_readb(0xab42 + i + 2) & 0xff;
		return ds_readb(0xab42 + i + 1) & 0xff;
	}

	if (c == 0x7e || c == 0xf0 || c == 0xf1 || c == 0xf2 || c == 0xf3) {
		*p = 0;
		return 0;
	} else {
		*p = 8;
		return 0;
	}
}

//1c2
void GUI_1c2(unsigned short v1, unsigned short v2, RealPt v3) {

	GUI_blank_char();
	GUI_font_to_buf(Real2Host(ds_readd(0xd2bd + v1 * 8)));
	GUI_write_char_to_screen(v3, 8, v2);
}
*/
//1f8
//static
//                      enter_text(unsigned char* dst, unsigned short x, unsigned short y, unsigned short num, unsigned short num_bel);
signed short GUI_class::enter_text(char* dst, unsigned short x, unsigned short y, unsigned short num, unsigned short num_bel) {
	unsigned short pos;
	unsigned short c;
	unsigned short in_key_ext;
	
	dst[0] = 0x5f;
	for (pos=1;pos<num;pos++){
		if (num_bel == 0) 
			dst[pos] = 0x5f;
		else
			dst[pos] = 0;
	}
	dst[num] = 0;

	pos = 0;

	sm_popup_line(1,dst,x,y,x+num*8,y*8);
 
	c = 0;
	while (c != 0xd || pos == 0) { //solange bis (enter) an nicht-erster Positon
		do {                     //solange bis Taste oder Maus gedrückt
			in_key_ext = sm_KeyPressed();
			wait_for_tick();
		} while (in_key_ext == 0 );
		c = in_key_ext;
		if (c == 0xd)                 // (enter)
			continue;
		if (in_key_ext == KEY_ESC) {  //(Abbruch)
			*dst = 0;
			in_key_ext = 0;
			return 1;
		}
		if (c == 8) {                 //(backspace)
			if (num_bel == 0){
				if (pos != num) dst[pos] = 0x5f;
			}else{
				dst[pos] = 0;
			}
			if (pos > 0) pos--;
			dst[pos] = 0x5f;
			sm_popup_line(1,dst,x,y,x+num*8,y*8);
		} else {                      //(alle anderen Tasten)
			if ( ((c>=48) &&(c<=57)) || ((c>=65) &&(c<=90)) || (c==186) || (c==192) || (c==222)){
			/* ae */
				if (c == 222) c = 0xc4;
			/* oe */
				if (c == 192) c = 0xd6;
			/* ue */
				if (c == 186) c = 0xdc;
			/* are we at the end of the input field */
				if (pos == num) pos--;
			
				dst[pos] = c & 0xff;
				pos++;
				if (pos != num) {
					dst[pos] = 0x5f;
					if (num_bel){ dst[pos+1] = 0;}
				}
		}	}
		sm_popup_line(1,dst,x,y,x+num*8,y*8);
	}

	if (num_bel == 0){
		while (pos < num) {
			dst[pos] = 0x20;
			pos++;
		}
	}else{
		dst[pos] = 0;
	}
	sm_print_line(0,dst,x,y,x+num*8,y*8);
	wait_for_tick();
	return 0;
/*	Bit8u *dst_start;
	unsigned short si, di;
	signed short pos, c;
	unsigned short length;

	dst_start = dst;
	update_mouse_cursor();
	di = x;
	pos = 0;
	length = strlen((char*)dst);

	if (zero == 0) {
		for (si = 0; si < num; si++) {
			if (ds_readw(0x26b7) != 0 && length >= si) {
				GUI_print_char(0x20, di, y);
				GUI_print_char(*dst, di, y);
				dst++;
				pos++;
				di += 6;
				x = di;
			} else {
				GUI_print_char(0x20, di, y);
				GUI_print_char(0x5f, di, y);
				di += 6;
			}
		}
		di = x;
	} else {
		GUI_print_char(0x20, di, y);
		GUI_print_char(0x5f, di, y);
	}

	wait_for_keyboard2();
	ds_writew(0xc3d1, 0);

	c = 0;
	while (c != 0xd || pos == 0) {
		/* This loop is evil /
		do {} while (CD_bioskey(1) == 0 && ds_readw(0xc3d1) == 0);

		if (ds_readw(0xc3d1) != 0) {
			ds_writew(0xc3d7, 0x0d);
			ds_writew(0xc3d5, 0x00);
			ds_writew(0xc3d1, 0x00);
		} else {
			ds_writew(0xc3d7, bioskey(0));
			ds_writew(0xc3d9, (ds_readw(0xc3d7) >> 8));
		}

		ds_writew(0xc3d7, ds_readw(0xc3d7) & 0xff);

		if (ds_readw(0xc3d9) == 0 && ds_readw(0xc3d7) == 0)
			continue;

		c = (signed short)ds_readw(0xc3d7);

		if (c == 0x0d)
			continue;

		if (ds_readw(0xc3d9) == 1) {
			host_writeb(dst_start, 0);
			refresh_screen_size();
			ds_writew(0xc3d9, 0);
			return -1;
		}

		if (c == 8) {
			if (pos <= 0)
				continue;

			if (zero == 1 && pos != num)
				GUI_print_char(0x20, di, y);

			di -= 6;
			pos--;
			dst--;
			GUI_print_char(0x20, di, y);
			GUI_print_char(0x5f, di, y);
			continue;
		}

		/* check if character is valid /
		if (c < 0x20 && c > 0x7a &&
			c != 0x81 && c != 0x84 && c != 0x94)
				continue;
		/* is_alpha(c) /
		if (ds_readb(0xb4e9 + c) & 0xc)
			c = toupper(c);

		/* ae /
		if (c == 0x84)
			c = (signed char)0x8e;
		/* oe /
		if (c == 0x94)
			c = (signed char)0x99;
		/* ue /
		if (c == 0x81)
			c = (signed char)0x9a;

		/* are we at the end of the input field ? /
		if (pos == num) {
			dst--;
			di -= 6;
			pos--;
		}

		host_writeb(dst++, (signed char)c);
		GUI_print_char(0x20, di, y);
		GUI_print_char((unsigned char)c, di, y);
		di += 6;
		pos++;

		if (zero != 1)
			continue;
		if (pos == num)
			continue;
		GUI_print_char(0x20, di, y);
		GUI_print_char(0x5f, di, y);
	}

	if (zero == 0) {
		while (pos < num) {
			GUI_print_char(0x20, di, y);
			di += 6;
			pos++;
		}
	}

	host_writeb(dst, 0);
	refresh_screen_size();
	return 0;*/
}

void GUI_class::draw_radio_bg()//Bit16u header, Bit16u options, Bit16u width,Bit16u height)
{
	/* play FX3.VOC */
	//if (ds_readw(0xbd25) == 0)
	//	play_voc(291);
	sm_activate_popup(this);
	//wait_for_keyboard1();
}
/*
//564
void GUI_copy_smth(unsigned short width, unsigned short height) {
	ds_writew(0xc011, ds_readw(0xbfff));
	ds_writew(0xc013, ds_readw(0xc001));
	ds_writew(0xc015, ds_readw(0xbfff) + width - 1);
	ds_writew(0xc017, ds_readw(0xc001) + height - 1);
	ds_writed(0xc019, ds_readd(0xbff9));
	do_pic_copy(0);
}

void GUI_output(Bit8u *str)
{
	GUI_input(str, 0);
}
*/
signed short GUI_class::input(Bit8u *header, unsigned short num)
{
	unsigned short w;
	Bit16s retval;
	char str[15];

	options=0;
	retval = 0;

	if (!NOT_NULL(header) || *header == '\0' )//|| ds_readw(AUTOFIGHT) != 0)
		return -1;

	w = menue_tiles * 32 + 32;
	left_border = (320 - w) / 2 + shift_x;
	right_border = left_border + w;
	text_x = left_border + 5;
	text_w = w - 8; 

	if (header){
		header_lines = sm_popup_line(0,(char*)header,text_x,0,text_x+text_w,200);
	}else{
		header_lines = 0;
	}

	if (num != 0)
		header_lines += 2;

	upper_border = abs(200 - (header_lines + options + 2) * 8) / 2 + shift_y;
	text_y = upper_border + 7;

	draw_radio_bg();

	//get_textcolor(&fg_bak, &bg_bak);
	sm_popup_line(0,(char*)header,text_x,text_y,  text_x+text_w,text_y+header_lines*8);
	
	if (num != 0) {
		if (enter_text(str, (w - num * 6) / 2 + left_border,header_lines * 8 + upper_border - 2, num, 0) == -1) {
			retval = -1;
		} else {
			retval = (unsigned short)atol(str);
		}
	} else {
		//if (ds_readw(0xc3c5) != 0) {
		//	wait_for_keypress();
		//} else {
			sm_delay_or_keypress(header_lines * menue_tiles * 50);
		//}
	}
	//set_textcolor(fg_bak, bg_bak);
	return retval;
}

signed short GUI_class::radio_bool(Bit8u *text,Bit8u *ja,Bit8u *nein)
{
	signed short ret_radio;

	is_bool=1;
	ret_radio = radio(text, 2, ja, nein);
	is_bool=0;

	if (ret_radio == 1)
		return 1;
	else
		return 0;
}

/*

//0x893
signed short GUI_dialogbox(RealPt picture, Bit8u *name, Bit8u *text,
		signed char options, ...)
{
	va_list arguments;

	char *lp;
	unsigned short i;
	Bit16s l2, l3, l4, l5, l6;
	unsigned short fg_bak, bg_bak;
	Bit16s l7, l8, l9, l10;
	signed short retval;
	Bit16s l11, l12, l13;
	Bit16s l_si, l_di;

	l13 = ds_readw(0x29ae);
	l12 = ds_readw(0xc3cb);
	ds_writew(0xc3cb, 0);

	set_var_to_zero();

	l11 = ds_readw(0xe113);
	ds_writew(0xe113, 0);
	ds_writeb(0x2c98, 1);
	l7 = ds_readw(0xd2d9);
	l8 = ds_readw(0xd2d7);
	l9 = ds_readw(0xd2d5);
	l6 = ds_readw(0xbffd);
	ds_writew(0xbffd, 9);

	l_di = ds_readw(0xbffd) * 32 + 32;
	ds_writew(0xbfff, (320 - l_di) / 2 + ds_readw(0x2ca2));
	ds_writew(0xd2d9, ds_readw(0xbfff) + 5);
	ds_writew(0xd2d5, l_di - 8);
	l10 = ds_readw(0xd313);
	ds_writew(0xd313, ds_readw(0xd2d9) + ds_readw(0xd2d5) - 24);
	ds_writew(0xe4db, 40);
	ds_writew(0xe4d9, 5);

	l_si = GUI_count_lines(text) - 1;

	if (NOT_NULL(name))
		l_si += 2;

	if (l_si < ds_readw(0xe4d9))
		l_si = ds_readw(0xe4d9) - 1;

	l4 = options + l_si;
	l5 = (l4 + 2) * 8;
	ds_writew(0xc001, (200 - (l5 + 2)) / 2);
	ds_writew(0xd2d7, ds_readw(0xc001) + 5);

	update_mouse_cursor();
	get_textcolor(&fg_bak, &bg_bak);

	GUI_draw_radio_bg(l_si, options, l_di, l5);

	if (picture != 0) {
		/* draw a frame //
		do_border(Real2Phys(ds_readd(0xd2ff)),
			ds_readw(0xbfff) + 5, ds_readw(0xc001) + 6,
			ds_readw(0xbfff) + 38, ds_readw(0xc001) + 39,
				(signed char)0xff);

		/* set the coordinates //
		ds_writew(0xc011, ds_readw(0xbfff) + 6);
		ds_writew(0xc013, ds_readw(0xc001) + 7);
		ds_writew(0xc015, ds_readw(0xbfff) + 37);
		ds_writew(0xc017, ds_readw(0xc001) + 38);
		ds_writed(0xc019, picture);

		do_pic_copy(0);
	}

	if (NOT_NULL(name)) {
		/* set text color //
		ds_writew(0xd2c5, 1);

		GUI_print_string(name, ds_readw(0xd2d9), ds_readw(0xd2d7));

		/* set text color //
		ds_writew(0xd2c5, 0);

		ds_writew(0xd2d7, ds_readw(0xd2d7) + 14);
		ds_writew(0xe4d9, ds_readw(0xe4d9) - 2);
	}

	if (l_si != 0)
		GUI_print_header(text);

	ds_writew(0xe4d9, 0);
	ds_writew(0xe4db, 0);

	if (options != 0) {
		l2 = ds_readw(0xd2d9) + 8;
		l3 = (l_si + 1) * 8 + ds_readw(0xc001);

		va_start(arguments, options);
		for (i = 0; i < options; l3 += 8, i++) {
			lp = va_arg(arguments, char*);
			GUI_print_string((Bit8u*)lp, l2, l3);
		}
	}

	retval = GUI_menu_input(options, l_si + 1, l_di);

	GUI_copy_smth(l_di, l5);

	refresh_screen_size();
	set_textcolor(fg_bak, bg_bak);

	ds_writew(0xd2d9, l7);
	ds_writew(0xd2d7, l8);
	ds_writew(0xd2d5, l9);

	ds_writew(0xbffd, l6);

	ds_writew(0xd313, l10);

	ds_writew(0xe113, l11);

	ds_writeb(0x2c98, 0);
	/* reset action //
	ds_writeb(0xc3d9, 0);

	ds_writew(0xc3cb, l12);

	if (l13 != 0)
		init_ani(2);

	return retval;
}
*/
//0xb43
//static
signed short GUI_class::menu_input()//unsigned short positions, unsigned short h_lines,unsigned short width)
{                             //options, l_di + 1, l11);
	//Bit16s l1, l2, l3, l4, l5, l6;
	Bit16s retval;
	unsigned short key_pressed,i;
	struct struct_sm_mouse sm_mouse;
	bool done;

	//l5 = -1;
	done = false;

	//ds_writew(0xe5ae, 1);
	button=1;//ds_writew(0xe5b0, 1);
	//ds_writew(0xe5b0, ds_readw(0xe5b0) + ds_readw(0xe5ac));

	if (options != 0) {
		//l6 = h_lines * 8;
		//l3 = ds_readw(0x299c);
		//l4 = ds_readw(0x299e);
		//ds_writew(0x299c, ds_readw(0xbfff) + 90);
		//ds_writew(0x29a0, ds_readw(0xbfff) + 90);
		//l1 = ds_readw(0xc001) + l6;
		//l2 = ds_readw(0xe5ac) * 8 + l1;
		//ds_writew(0x299e, l2);
		//ds_writew(0x29a2, l2);

		//mouse_move_cursor(ds_readw(0x299c), ds_readw(0x299e));

		//ds_writew(0x298e, ds_readw(0xbfff) + width - 16);
		//ds_writew(0x298a, ds_readw(0xbfff));
		//ds_writew(0x2988, ds_readw(0xc001) + l6);
		//ds_writew(0x298c, positions * 8 + l6 + ds_readw(0xc001) - 1);
		//refresh_screen_size();

		//ds_writew(0xc3d3, 0);
		//ds_writew(0xc3d1, 0);
		//ds_writew(0xc3d5, 0);

		while (!done) {
			//ds_writed(0x29e4, RealMake(datseg, 0x29cc));
			//key_pressed=handle_input();
			key_pressed = sm_KeyPressed();
			sm_GetMouse(&sm_mouse);
			wait_for_tick();
			//ds_writed(0x29e4, 0);
			/* wird in video erledigt
			if (l5 != ds_readw(0xe5b0)) {
				GUI_fill_radio_button(l5, ds_readw(0xe5b0),
					h_lines - 1);
				l5 = ds_readw(0xe5b0);
			}
			*/
			// 0xc3d9 ist Key input
			if (sm_mouse.event==MOUSE_RIGHT_DOWN ||//ds_readw(0xc3d3) != 0 ||     rechte Maustaste?
				key_pressed == KEY_ESC ||
				key_pressed == KEY_PGDOWN) {

				retval = -1;
				done = true;
				sm_mouse.event=0;//ds_writew(0xc3d3, 0);
			}

			if (sm_mouse.event==MOUSE_LEFT_DOWN){
				if (sm_mouse.x>=left_border && sm_mouse.x<=right_border){
					for(i=0;i<options;i++){
						if ((sm_mouse.y>=upper_options+i*8) && sm_mouse.y<upper_options+i*8+8){
							if (button == i+1){
								retval = button;
								done = true;
							}
							button = i+1;
						}
					}
				}
			}

				
			if (key_pressed == KEY_RET) {
				retval = button;
				done = true;
			}

			/* Key UP */
			if (key_pressed == KEY_UP) {
				button -=1;//ds_writew(0xe5b0, ds_readw(0xe5b0) - 1);
				if (button == 0)
					button = options;
			}
			/* Key DOWN */
			if (key_pressed == KEY_DOWN) {
				button +=1;
				if (button  > options)
					button = 1;
			}
/*
			if (ds_readw(0x299e) != l2) {
				l2 = ds_readw(0x299e);
				ds_writew(0xe5b0, (l2 - l1) / 8 + 1);
			}
*/
			if (is_bool != 0) {
				if (key_pressed == KEY_J || key_pressed == KEY_Y) {
					retval = 1;
					done = true;
				} else if (key_pressed == KEY_N) {
					retval = 2;
					done = true;
				}
			}
		}

		//update_mouse_cursor();

		//ds_writew(0x299c, l3);
		//ds_writew(0x29a0, l3);
		//ds_writew(0x299e, l4);
		//ds_writew(0x29a2, l4);
		//ds_writew(0x298e, 319);
		//ds_writew(0x298a, 0);
		//ds_writew(0x2988, 0);
		//ds_writew(0x298c, 199);

		//mouse_move_cursor(ds_readw(0x299c), ds_readw(0x299e));

	} else {
		key_pressed = sm_delay_or_keypress(10000);
		if (key_pressed != 0)
			retval = -1;
	}

	//ds_writew(0xe5ae, 0);
	//ds_writew(0xe5ac, 0);
	sm_images_close_popup();
	return retval;
}

signed short GUI_class::radio(Bit8u *header, signed char opt, ...)
{
	unsigned short w;
	va_list arguments;
	char *str;
	signed short retval;
	Bit16s i;

	options=opt;
fprintf(stdout,"GUI: %s, %d\n",header,opt);	
	//if (options == 0) {
	//	GUI_output(text);
	//	return 0;
	//}

	w = menue_tiles * 32 + 32;
	left_border = (320 - w) / 2 + shift_x;
	right_border = left_border + w;
	text_x = left_border + 5;
	text_w = w - 8; 

	if (header){
		header_lines = sm_popup_line(0,(char*)header,text_x,0,text_x+text_w,200);
	}else{
		header_lines = 0;
	}
	
	upper_border = abs(200 - (header_lines + options + 2) * 8) / 2 + shift_y;
	text_y = upper_border + 7;

	draw_radio_bg();

	if (header_lines != 0)
		sm_popup_line(0,(char*)header,text_x,text_y,  text_x+text_w,text_y+header_lines*8);

	text_x +=8;
	text_w -=8;
	text_y = (header_lines + 1) * 8 + upper_border;
	upper_options = text_y;
		
	va_start(arguments, opt);
	for (i = 0; i < options; text_y += 8, i++){

		/* highlight special option */
		//if (ds_readw(0xc003) == 1 && ds_readw(0x2cdb) == i)
		//	set_textcolor(0xc9, 0xdf);

		str = va_arg(arguments, char*);
		sm_popup_line(i+1,(char*)str,text_x,text_y,  text_x+text_w,text_y+8);

		/* reset highlight special option */
		//if (ds_readw(0xc003) == 1 && ds_readw(0x2cdb) == i)
		//	set_textcolor(0xdf, 0xff);
	}
	retval = menu_input();//options, l_di + 1, l11);
	return retval;
	/* TODO: 0xe48 - 0xf36 */
}

/**
 *	GUI_dialog_na() - print a Dialog windows without answers
 *	@head_index:	the number of a head, if another should be loaded
 *	@text:		the text
 //
void GUI_dialog_na(unsigned short head_index, Bit8u *text)
{

	if (head_index != 0)
		load_in_head(head_index);

	GUI_dialogbox(ds_readd(0xd2f3), NULL, text, 0);

}
/*/
