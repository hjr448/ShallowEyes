#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>


#include "sm401_main.h"
#include "sm401_GUI.h"
#include "sm401_video.h"
#include "gen201.h"

#include "sm401_seg002.h"
#include "sm401_seg009.h"
#include "hero.h"
#include "gen201_vars.h"



#define INC (1)
#define DEC (2)




static unsigned short rand_seed = 0x327b;



static inline
unsigned short my_rol16(unsigned short op, unsigned char count) {
	return (op << count) | (op >> (16 - count));
}
/**
	random_gen - generates a u16 random number
*/
unsigned short random_gen(short val) {

	unsigned short si, ax;

	if (val == 0)
		return 0;

	si = rand_seed ^ rand();
	ax = my_rol16(si, 2) + rand();
	ax = ax ^ rand_seed;
	si = my_rol16(ax, 3);

	rand_seed = si;		/* update rand_seed */
	si = abs(si) % val;

	return si + 1;
}

/**
	random_interval_gen - generates a u16 random number between lo and hi
*/
unsigned short random_interval_gen(unsigned short lo, unsigned short hi) {

	return lo + random_gen(hi - lo + 1) - 1;
}


void gen_make_valuta_str(char *dst, unsigned int money)
{
	unsigned short d = 0;
	unsigned short s = 0;

	while (money / 100) {
		d++;
		money -= 100;
	}
	while (money / 10) {
		s++;
		money -= 10;
	}
	sprintf(dst, texts[0x114 / 4], d, s, money);
}

void gen_print_attribs()
{
	char buf[10];
	unsigned short i;
	for (i = 0; i < 14; i++) {
		/* don't print 0s */
		if (hero.attribs[i].normal == 0)
			continue;
		sprintf(buf, "%d", hero.attribs[i].normal);
		sm_print_line(16+i,buf,attrib_xy[i].x,attrib_xy[i].y,attrib_xy[i].x+16,attrib_xy[i].y+8);
	}
}

void gen_print_values()
{
	char tmp[4];
	const unsigned short align_left = 222;
	const unsigned short align_right = 302;
	//unsigned short width;
	signed short i, pos;
	signed short j=1;


	switch (gen_page) {

		case 0: {
			sm_print_line(0,hero.name,180,12,280,20);
			gen_print_attribs();
			/* break if no typus */
			if (hero.typus == 0)
				break;
			sprintf(gen_ptr2, texts[0x118 / 4], hero.height);
			sm_print_line(1,gen_ptr2, 205, 25, 250, 33);
			sprintf(gen_ptr2, texts[0x11c / 4], hero.weight);
			sm_print_line(2,gen_ptr2, 205, 37, 250, 45);
			sm_print_line(3,texts[0xe0 / 4 + hero.god], 205, 49, 250, 57);
			gen_make_valuta_str(gen_ptr2, hero.money);
			sm_print_line(4,gen_ptr2, 205, 61, 250, 69);
			sprintf(tmp, "%d", hero.le_max);
			sm_print_line(5,tmp, 172, 164, 220, 172);
			sprintf(tmp, "%d", hero.ae_max);
			sm_print_line(6,tmp, 221, 164, 270, 172);
			sprintf(tmp, "%d",hero.le_max + hero.attribs[6].current);
			sm_print_line(7,tmp, 296, 164, 320, 172);
			sprintf(tmp, "%d", hero.mr);
			sm_print_line(8,tmp, 232, 184, 280, 192);
			break;
		}
		case 1: {
			/* SKILLS Page 1/3 */
			/* print fight skills */
			for (i = 0; i < 9; i++) {
				sprintf(tmp, "%d", hero.skills[i]);
				if (i & 1)		sm_print_ral_line(j++,tmp, align_right, (abs(i) / 2) * 12 + 42);
				else			sm_print_ral_line(j++,tmp, align_left , (abs(i) / 2) * 12 + 42);
			}
			/* print body skills */
			for (i = 9; i < 19; i++) {
				pos = i - 9;
				sprintf(tmp, "%d", hero.skills[i]);
				if (pos & 1) 	sm_print_ral_line(j++,tmp, align_right, (abs(pos) / 2) * 12 + 119);
				else 			sm_print_ral_line(j++,tmp, align_left , (abs(pos) / 2) * 12 + 119);
			}
			/* remaining attempts for skills */
			sprintf(tmp, "%d", hero.skill_incs);
			sm_print_line(j++,tmp, 271, 184, 287, 192);
			break;
		}
		case 2: {
			/* SKILLS Page 2/3 -/
			/* print social skills */
			for (i = 19; i < 26; i++) {
				pos = i - 19;
				sprintf(tmp, "%d", hero.skills[i]);
				if (pos & 1) 	sm_print_ral_line(j++,tmp, align_right, (abs(pos) / 2) * 12 + 42);
				else 			sm_print_ral_line(j++,tmp, align_left , (abs(pos) / 2) * 12 + 42);
			}
			/* print lore skills */
			for (i = 32; i < 41; i++) {
				pos = i - 32;
				/* originally it was itoa() */
				sprintf(tmp, "%d", hero.skills[i]);
				if (pos & 1) 	sm_print_ral_line(j++,tmp, align_right, (abs(pos) / 2) * 12 + 113);
				else 			sm_print_ral_line(j++,tmp, align_left , (abs(pos) / 2) * 12 + 113);
			}
			/* remaining attempts for skills */
			sprintf(tmp, "%d", hero.skill_incs);
			sm_print_line(j++,tmp, 271, 184, 287, 192);
			break;
		}
		case 3: {
			/* SKILLS Page 3/3 -/
			/* print craftmansship skills */
			for (i = 41; i < 50; i++) {
				pos = i - 41;
				sprintf(tmp, "%d", hero.skills[i]);
				if (pos & 1) 	sm_print_ral_line(j++,tmp, align_right, (abs(pos) / 2) * 12 + 42);
				else 			sm_print_ral_line(j++,tmp, align_left , (abs(pos) / 2) * 12 + 42);
			}
			/* print nature skills */
			for (i = 26; i < 32; i++) {
				pos = i - 26;
				sprintf(tmp, "%d", hero.skills[i]);
				if (pos & 1) 	sm_print_ral_line(j++,tmp, align_right, (abs(pos) / 2) * 12 + 119);
				else 			sm_print_ral_line(j++,tmp, align_left , (abs(pos) / 2) * 12 + 119);
			}
			/* print intuition skills */
			for (i = 50; i < 52; i++) {
				pos = i - 50;
				sprintf(tmp, "%d", hero.skills[i]);
				if (pos & 1) 	sm_print_ral_line(j++,tmp, align_right, (abs(pos) / 2) * 12 + 170);
				else 			sm_print_ral_line(j++,tmp, align_left , (abs(pos) / 2) * 12 + 170);
			}
			/* remaining attempts for skills */
			sprintf(tmp, "%d", hero.skill_incs);
			sm_print_line(j++,tmp, 271, 184, 287, 192);
			break;
		}
		case 4: {
			/* ATPA Page */
			/* Print base value  2x the same */
			sprintf(tmp, "%d", hero.atpa);
			sm_print_line(j++,tmp, 231, 30, 247, 38);
			sprintf(tmp, "%d", hero.atpa);
			sm_print_line(j++,tmp, 268, 30, 284, 38);
			for (i = 0; i < 7; i++) {
				/* print AT value */
				sprintf(tmp, "%d", hero.at[i]);
				sm_print_line(j++,tmp, 237 - 8, i * 12 + 48, 237 + 8, i * 12 + 56);
				/* print PA value */
				sprintf(tmp, "%d", hero.pa[i]);
				sm_print_line(j++,tmp, 274 - 8, i * 12 + 48, 274 + 8, i * 12 + 56);
				/* print skill value */
				sprintf(tmp, "%d", hero.skills[i]);
				sm_print_line(j++,tmp, 315 - 8, i * 12 + 48, 315 + 8, i * 12 + 56);
			}
			/* calc range base value (KL+GE+KK)/4 */
			pos = (hero.attribs[1].normal + hero.attribs[4].normal +
				hero.attribs[6].normal) / 4;
			/* print missle weapon value */
			sprintf(tmp, "%d", hero.skills[7] + pos);
			sm_print_line(j++,tmp, 231, 144, 247, 152);
			/* print thrown weapon value */
			sprintf(tmp, "%d", hero.skills[8] + pos);
			sm_print_line(j++,tmp, 231, 156, 247, 164);
			break;
		}
		case 5: {
			/* Spells Page 1/6 */
			for (i = 1; i < 6; i++) {
				pos = i - 1;
				sprintf(tmp, "%d", hero.spells[i]);
				if (pos & 1) 	sm_print_ral_line(j++,tmp, 302, (abs(pos) / 2) * 12 + 42);
				else 			sm_print_ral_line(j++,tmp, 157 , (abs(pos) / 2) * 12 + 42);
			}
			for (i = 33; i < 38; i++) {
				pos = i - 33;
				sprintf(tmp, "%d", hero.spells[i]);
				if (pos & 1) 	sm_print_ral_line(j++,tmp, 302, (abs(pos) / 2) * 12 + 95);
				else 			sm_print_ral_line(j++,tmp, 157 , (abs(pos) / 2) * 12 + 95);
			}
			for (i = 6; i <= 11; i++) {
				pos = i - 6;
				sprintf(tmp, "%d", hero.spells[i]);
				if (pos & 1) 	sm_print_ral_line(j++,tmp, 302, (abs(pos) / 2) * 12 + 146);
				else 			sm_print_ral_line(j++,tmp, 157 , (abs(pos) / 2) * 12 + 146);
			}
			/* print spell attempts */
			sprintf(tmp, "%d", hero.spell_incs);
			sm_print_line(j++,tmp, 217, 184, 225, 194);
			break;
		}
		case 6: {
			/* Spells Page 2/6 */
			for (i = 12; i <= 17; i++) {
				pos = i - 12;
				/* originally it was itoa() */
				sprintf(tmp, "%d", hero.spells[i]);
				if (pos & 1) 	sm_print_ral_line(j++,tmp, 302, (abs(pos) / 2) * 12 + 42);
				else 			sm_print_ral_line(j++,tmp, 157 , (abs(pos) / 2) * 12 + 42);
			}
			for (i = 18; i < 24; i++) {
				pos = i - 18;
				sprintf(tmp, "%d", hero.spells[i]);
				if (pos & 1) 	sm_print_ral_line(j++,tmp, 302, (abs(pos) / 2) * 12 + 95);
				else 			sm_print_ral_line(j++,tmp, 157 , (abs(pos) / 2) * 12 + 95);
			}
			for (i = 24; i < 27; i++) {
				pos = i - 24;
				sprintf(tmp, "%d", hero.spells[i]);
				if (pos & 1) 	sm_print_ral_line(j++,tmp, 302, (abs(pos) / 2) * 12 + 146);
				else 			sm_print_ral_line(j++,tmp, 157 , (abs(pos) / 2) * 12 + 146);
			}
			/* print spell attempts */
			sprintf(tmp, "%d", hero.spell_incs);
			sm_print_line(j++,tmp, 217, 184, 225, 194);
			break;
		}
		case 7: {
			/* Spells Page 3/6 */
			for (i = 27; i < 33; i++) {
				pos = i - 27;
				sprintf(tmp, "%d", hero.spells[i]);
				if (pos & 1) 	sm_print_ral_line(j++,tmp, 302, (abs(pos) / 2) * 12 + 42);
				else 			sm_print_ral_line(j++,tmp, 157 , (abs(pos) / 2) * 12 + 42);
			}
			for (i = 38; i < 45; i++) {
				pos = i - 38;
				sprintf(tmp, "%d", hero.spells[i]);
				if (pos & 1) 	sm_print_ral_line(j++,tmp, 303, (abs(pos) / 2) * 12 + 95);
				else 			sm_print_ral_line(j++,tmp, 157 , (abs(pos) / 2) * 12 + 95);
			}
			for (i = 45; i <= 46; i++) {
				pos = i - 45;
				sprintf(tmp, "%d", hero.spells[i]);
				if (pos & 1) 	sm_print_ral_line(j++,tmp, 302, (abs(pos) / 2) * 12 + 160);
				else 			sm_print_ral_line(j++,tmp, 157 , (abs(pos) / 2) * 12 + 160);
			}
			/* print spell attempts */
			sprintf(tmp, "%d", hero.spell_incs);
			sm_print_line(j++,tmp, 217, 184, 225, 194);
			break;
		}
		case 8: {
			/* Spells Page 4/6 */
			for (i = 47; i <= 48; i++) {
				pos = i - 47;
				sprintf(tmp, "%d", hero.spells[i]);
				if (pos & 1) 	sm_print_ral_line(j++,tmp, 302, (abs(pos) / 2) * 12 + 42);
				else 			sm_print_ral_line(j++,tmp, 157 , (abs(pos) / 2) * 12 + 42);
			}
			for (i = 49; i < 58; i++) {
				pos = i - 49;
				sprintf(tmp, "%d", hero.spells[i]);
				if (pos & 1) 	sm_print_ral_line(j++,tmp, 302, (abs(pos) / 2) * 12 + 71);
				else 			sm_print_ral_line(j++,tmp, 157 , (abs(pos) / 2) * 12 + 71);
			}
			for (i = 58; i < 60; i++) {
				pos = i - 58;
				sprintf(tmp, "%d", hero.spells[i]);
				if (pos & 1) 	sm_print_ral_line(j++,tmp, 302, (abs(pos) / 2) * 12 + 148);
				else 			sm_print_ral_line(j++,tmp, 157 , (abs(pos) / 2) * 12 + 148);
			}
			/* print spell attempts */
			sprintf(tmp, "%d", hero.spell_incs);
			sm_print_line(j++,tmp, 217, 184, 225, 194);
			break;
		}
		case 9: {
			/* Spells Page 5/6 */
			for (i = 60; i < 76; i++) {
				pos = i - 60;
				sprintf(tmp, "%d", hero.spells[i]);
				if (pos & 1) 	sm_print_ral_line(j++,tmp, 302, (abs(pos) / 2) * 12 + 42);
				else 			sm_print_ral_line(j++,tmp, 157 , (abs(pos) / 2) * 12 + 42);
			}
			/* print spell attempts */
			sprintf(tmp, "%d", hero.spell_incs);
			sm_print_line(j++,tmp, 217, 184, 225, 194);
			break;
		}
		case 10: {
			/* Spells Page 6/6 */
			for (i = 76; i < 86; i++) {
				pos = i - 76;
				sprintf(tmp, "%d", hero.spells[i]);
				if (pos & 1) 	sm_print_ral_line(j++,tmp, 302, (abs(pos) / 2) * 12 + 42);
				else 			sm_print_ral_line(j++,tmp, 157 , (abs(pos) / 2) * 12 + 42);
			}
			/* print spell attempts */
			sprintf(tmp, "%d", hero.spell_incs);
			sm_print_line(j++,tmp, 217, 184, 225, 194);
			break;
		}
	}
}

void gen_load_typus(unsigned short typus)
{
	unsigned char *ptr;
	FILE *fd;
	unsigned short index;

	index = typus + 19;

	/* check if this image is in the buffer */
	if (typus_buffer[typus]) {
		sm_images_do_typus(typus);
		return;
	}

	fd = gen_open_datfile(index);

		/* load the file direct */
		typus_buffer[typus] = true;
		gen_read_datfile(fd, buffer_dat, 25000);
		decomp_pp20(buffer_dat, gataosao, NULL, flen);
		sm_images_init_typus(typus, gataosao);
		sm_images_do_typus(typus);
	
	fclose(fd);
}
void gen_load_page(unsigned short page)
{
	unsigned char *ptr;
	FILE *fd;

	if (page <= 10) {
		/* check if this image is in the buffer */
		if (page_loaded[page]){//bg_buffer[page]) {
			sm_images_do_page(page);
			return;
		}else{

			fd = gen_open_datfile(page);
			ptr = (unsigned char*)malloc(flen);

			if (ptr) {
				gen_read_datfile(fd, ptr, flen);
				//decomp_rle(gataosao, ptr, 0, 0, 320, 200, 0);
				decomp_rle(320, 200, gataosao, ptr, buffer_dat,  0);
			
				sm_images_init(page, 320, 200, gataosao);
				page_loaded[page]=true;
				sm_images_do_page(page);
			}
			fclose(fd);
			free(ptr);
		}
	} 
}
void gen_refresh_screen()
{
	if (screen_f) {
		sm_images_draw_all(false);
		gen_load_page(gen_page);
		/* page with base values and hero is not male */
		if (gen_page == 0) {
			sm_images_do_sex(hero.sex,level);
		}

		/* if the page is lower than 5 */
		if (gen_page < 5) {
			/* draw DMENGE.DAT or the typus name */
			if (hero.typus != 0) {
				//need_refresh = true;
				gen_load_typus(hero.typus);

				if (hero.sex != 0) {
					char *p;
					p = texts[0x43c / 4 + hero.typus];
					sm_print_line(45,p,16,184,144,192);
				} else {
					char *p;
					p = texts[0x44 / 4 + hero.typus];
					sm_print_line(45,p,16,184,144,192);
				}
			} else {
				//wait_for_vsync();
				sm_images_do_typus(0);
			}
		}
		/* if hero has a typus */
		if (hero.typus != 0) {

			/* draw the head */
			if (gen_page == 0) {
				/* on the base page */
				sm_images_do_heads(head_current,272,8);
			} else if (gen_page > 4) {
				/* on the spell pages */
				sm_images_do_heads(head_current,272,4);
			}
		}
		gen_print_values();
	} else {
		gen_print_values();
	}
	sm_images_draw_all(true);
}
void do_gen()
{
	signed short si, di;
	struct struct_sm_mouse sm_mouse;
	di = 0;

	screen_f = true;

	/* main loop */
	while (!di) {		

		if (screen_f) {
			gen_refresh_screen();
			screen_f = false;
		}

		//action_table = (unsigned char*)action_page[gen_page];
		//action_table = NULL;
		in_key_ext = sm_KeyPressed();
		sm_GetMouse(&sm_mouse);//mouse anstelle pfeiltasten fehlt
		wait_for_tick();
	
		if (sm_mouse.event==MOUSE_LEFT_DOWN){
			if (gen_page == 0){
				if ((sm_mouse.x>272) && (sm_mouse.x<287) && (sm_mouse.y>40) && (sm_mouse.y<55)) 
					in_key_ext = KEY_UP;
				if ((sm_mouse.x>288) && (sm_mouse.x<304) && (sm_mouse.y>40) && (sm_mouse.y<55)) 
					in_key_ext = KEY_DOWN;
				}
			if ((sm_mouse.x>284) && (sm_mouse.x<304) && (sm_mouse.y>178) && (sm_mouse.y<193)) 
				in_key_ext = KEY_RIGHT;
			if (gen_page  <= 4)
				if ((sm_mouse.x>145) && (sm_mouse.x<165) && (sm_mouse.y>178) && (sm_mouse.y<193)) 
					in_key_ext = KEY_LEFT;
			if (gen_page  > 4)
				if ((sm_mouse.x> 16) && (sm_mouse.x< 36) && (sm_mouse.y>178) && (sm_mouse.y<193)) 
					in_key_ext = KEY_LEFT;
			}
			
		if (sm_mouse.event==MOUSE_RIGHT_DOWN || in_key_ext == KEY_PGUP) {
			/* print the menu for each page */
			switch(gen_page) {
				case 0: {
					si = GUI_gen.radio((unsigned char*)texts[0x1c/4], 9,
						texts[0x28 / 4],
						texts[0x2c / 4],
						texts[0x3c / 4],
						texts[0x20 / 4],
						texts[0x38 / 4],
						texts[0x30 / 4],
						texts[0x418 / 4],
						texts[0x24 / 4],
						texts[0x408 / 4]);

					if (si != -1) {
						if (si >= 4 && si < 6 &&
							hero.attribs[0].normal &&
							!GUI_gen.radio_bool((unsigned char*)texts[0x34 /4], (Bit8u*)texts[4], (Bit8u*)texts[5])) {
							si = 0;
						}
						in_key_ext = 0;
						switch (si) {
							case 1: {
								gen_enter_name();
								break;
							}
							case 2: {
								gen_change_sex();
								break;
							}
							case 3: {
								gen_change_attribs();
								break;
							}
							case 4: {
								memset(&hero, 0, sizeof(hero));
								gen_clear_hero();
								//ds_writew(0x4599,1);
								screen_f = 1;
								break;
							}
							case 5: {
								gen_new_values();
								break;
							}
							case 6: {
								gen_select_typus();
								break;
							}
							case 7: {
								gen_choose_typus();//frei wählen
								break;
							}
							case 8: {
								gen_save_chr();
								break;
							}
							case 9: {
								if (GUI_gen.radio_bool((unsigned char*)texts[0x40c / 4], (Bit8u*)texts[4], (Bit8u*)texts[5]))
									di = 1;
								break;
							}
						}
					}
					break;
				}
				case 1:
				case 2:
				case 3: {
					gen_select_skill();
					break;
				}
				case 4: {
					gen_choose_atpa();
					break;
				}
				case 5:
				case 6:
				case 7:
				case 8:
				case 9:
				case 10: {
					gen_select_spell();
					break;
				}
			}
		}

		if (in_key_ext == KEY_CTRL_F3)
			gen_change_sex();

		if (in_key_ext == KEY_CTRL_F4)
			gen_enter_name();

		if (in_key_ext == KEY_UP && gen_page == 0) {
			if (hero.typus == 0) {
				GUI_gen.radio((Bit8u*)texts[0x44 / 4], 0);
			} else {
				if (head_current < head_last) {
					head_current++;
				} else {
					head_current = head_first;
				}
				gen_change_head();
			}
		}

		if (in_key_ext == KEY_DOWN && gen_page == 0) {
			if (hero.typus == 0) {
				GUI_gen.radio((Bit8u*)texts[0x44 / 4], 0);
			} else {
				if (head_current > head_first) {
					head_current--;
				} else {
					head_current = head_last;
				}
				gen_change_head();
			}
		}

		if (in_key_ext == KEY_RIGHT && level != 1) {
			if (hero.typus == 0) {
				GUI_gen.radio((Bit8u*)texts[0x120 / 4], 0);
			} else {
				screen_f = true;

				if (((hero.typus >= 7) ? 10 : 4) > gen_page)
					gen_page++;
				else
					gen_page = 0;
			}
		}

		if (in_key_ext == KEY_LEFT) {
			if ((signed short)gen_page > 0) {
				screen_f = true;
				gen_page--;
			} else {
				if (level != 1) {
					if (hero.typus == 0) {
						GUI_gen.radio((Bit8u*)texts[0x120 / 4], 0);
					} else {
						screen_f = true;
						gen_page = hero.typus < 7 ? 4 : 10;
					}
				}
			}
		}

		if (in_key_ext >= KEY_1 && in_key_ext <= KEY_5 &&
			level == 2 && hero.typus) {
			switch (in_key_ext) {
				case KEY_1: {
					si = 0;
					break;
				}
				case KEY_2: {
					si = 1;
					break;
				}
				case KEY_3: {
					si = 4;
					break;
				}
				case KEY_4: {
					si = 5;
					break;
				}
				default: {
					si = 10;
				}
			}
			if (si != gen_page && (si < 5 || hero.typus >= 7)) {
				gen_page = si;
				screen_f = true;
			}
		}
	}
}
static unsigned short gen_read_datfile(FILE * fd, unsigned char *buf, unsigned short len)
{
	if (len > flen_left)
		len = (unsigned short)flen_left;

	len = fread(buf, 1, len, fd);
	flen_left -= len;
	return len;
}

signed int gen_archive_offset(const char *name, unsigned char *table) {
	signed int offset, length;
	unsigned short i;

	for (i = 0; i < 50; i++) {

		/* check the filename */
		if (strncmp((char*)name, (char*)table + i * 16, 12))
			continue;

		/* calculate offset and length */
		offset = readd(table + i * 16 + 0x0c);
		length = readd(table + (i + 1) * 16 + 0x0c) - offset;

		/* save length in 2 variables */
		flen = length;
		flen_left = length;

		return offset;
	}
	return -1;
}
static FILE * gen_open_datfile(unsigned short index)
{
	FILE *fd;
	char fname[] = "DSAGEN.DAT";;
	signed int offset;
	unsigned char buf[800];

	fd = fopen(fname, "rb");

	if (fd == NULL) {
		fprintf(stdout,"%s(): failed to open datafile at %s\n",
			__func__, fname);
		return NULL;
	}

	if (fread(buf, 1, 800, fd) != 800) {
		fprintf(stdout,"%s(): failed to read datafile\n", __func__);
		fclose(fd);
		return NULL;
	}

	offset = gen_archive_offset(fnames_g105de[index], buf);
	if (offset == -1) {
		fprintf(stdout,"FILE %s IS MISSING!", fnames_g105de[index]);
		fclose(fd);
		return NULL;
	}

	fseek(fd, offset, SEEK_SET);

	return fd;
}
void gen_read_common_files()
{
	FILE *fd;
	long len;
	unsigned short i;
	struct nvf_desc nvf;
	
	/* load HEADS.DAT */
	fd = gen_open_datfile(11);
	gen_read_datfile(fd, buffer_dat, 64000);
	fclose(fd);
	
	for (i=0;i<62;i++){

		nvf.dst = gataosao;
		nvf.src = buffer_dat;
		nvf.nr = i;
		nvf.type = 0;
		nvf.width = 0;
		nvf.height = 0;
		process_nvf(&nvf);
		sm_images_init_heads(i,gataosao);
	}
	
	/* load SEX.DAT */
	fd = gen_open_datfile(12);
	gen_read_datfile(fd, buffer_dat, 900);
	fclose(fd);
	sm_images_init_sex(buffer_dat);

	/* load DMENGE.DAT */
	fd = gen_open_datfile(32);
	len = gen_read_datfile(fd, buffer_dat, 25000);
	fclose(fd);
	decomp_pp20(buffer_dat,gataosao, NULL, len);
	sm_images_init_typus(0,gataosao);

}
void gen_init_colors()
{
	sm_set_palette((unsigned char*)&col_black, 0x00, 1);
	sm_set_palette((unsigned char*)&col_white, 0xff, 1);
	sm_set_palette((unsigned char*)col_popup, 0xd8, 8);
	sm_set_palette((unsigned char*)col_misc, 0xc8, 3);
	sm_set_palette((unsigned char*)pal_genbg, 0x40, 0x20);
	sm_set_palette((unsigned char*)pal_heads, 0x20, 0x20);
	//sm_set_textcolor(0xff, 0x0);
}
void gen_split_textbuffer(char **dst, char *src, unsigned int len)
{
	unsigned int i = 0;

	for (i = 0, *dst++ = src; i != len; src++, i++) {
		/* continue if not the end of the string */
		if (*src)
			continue;
		/* return if "\0\0" (never happens) */
		if (*(src + 1) == 0)
			return;
		/* write the adress of the next string */
		*dst++ = src + 1;
	}
}
void gen_load_font_and_text()
{
	FILE *fd;
	unsigned int len;

	fd = gen_open_datfile(0x0f);
	len = gen_read_datfile(fd, buffer_text, 64000);
	fclose(fd);

	gen_split_textbuffer(texts, (char*)buffer_text, len);

}
void gen_alloc_buffers()
{
	gen_ptr2 = (char*)malloc(1524);
	buffer_text = (unsigned char*)malloc(6000);

	gen_load_font_and_text();

	buffer_dat = (unsigned char*)malloc(64001);
	gataosao = (unsigned char*)malloc(64001);
}
void gen_cleanup(){
	free(buffer_dat);
	free(gataosao);
	free(buffer_text);
	free(gen_ptr2);
};
void do_call_gen(unsigned short _level){
	/* so far no sound implemented */
	unsigned short sound_off = 1;
	level=_level;
	
	//if (sound_off == 0)
	//	init_music(13000);
	
	gen_alloc_buffers();
	gen_init_colors();
	
	//if (sound_off == 0)
	//	read_soundcfg();
	//start_music(33);

	gen_read_common_files();
	gen_init_colors();
	do_gen();

	gen_cleanup();
};
void gen_change_sex(){
	signed char tmp;
	/* change sex of the hero */
	hero.sex = hero.sex ^ 1;
	/* hero has a typus */
	if (hero.typus) {
		if (hero.sex != 0) {
			/* To female */
			head_current = head_first_female[head_typus];
			head_first = head_first_female[head_typus];
			head_last = head_first_male[head_typus + 1] - 1;
		} else {
			/* To male */
			tmp = head_first_male[head_typus];
			head_current = tmp;
			head_first = tmp;
			head_last = head_first_female[head_typus] - 1;
		}
		screen_f = true;
		return;
	} 
	screen_f = true;
}
unsigned short gen_enter_string(char *dst, unsigned short x, unsigned short y, unsigned short num, unsigned short num_bel)
{
	unsigned short pos;
	unsigned short c;

	dst[0] = 0x5f;
	for (pos=1;pos<num;pos++){
		if (num_bel == 0) 
			dst[pos] = 0x5f;
		else
			dst[pos] = 0;
	}
	dst[num] = 0;

	pos = 0;

	sm_print_line(0,dst,x,y,x+num*8,y*8);
 
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
			sm_print_line(0,dst,x,y,x+num*8,y*8);
		} else {                      //(alle anderen Tasten)
			if ( ((c>=65) &&(c<=90)) || (c==186) || (c==192) || (c==222)){
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
		sm_print_line(0,dst,x,y,x+num*8,y*8);
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
}
void gen_enter_name()
{
	gen_enter_string(hero.name, 180, 12, 15, 1);
}
unsigned short gen_can_change_attribs()
{
	unsigned short i;
	signed short pa_inc, pa_dec, na_inc, na_dec;

	na_dec = na_inc = pa_dec = pa_inc = 0;

	for (i = 0; i < 7; i++) {
		if (attrib_changed[i] != INC && hero.attribs[i].normal > 8)
			pa_dec += 8 - hero.attribs[i].normal;
		if (attrib_changed[i] != DEC && hero.attribs[i].normal < 13)
			pa_inc += 13 - hero.attribs[i].normal;
	}

	for (i = 7; i < 14; i++) {
		if (attrib_changed[i] != INC && hero.attribs[i].normal > 2)
			na_dec += 2 - hero.attribs[i].normal;
		if (attrib_changed[i] != DEC && hero.attribs[i].normal < 8)
			na_inc += 8 - hero.attribs[i].normal;
	}

	/* no values from positive attributes left */
	if (pa_inc == 0 && pa_dec == 0)
		return 0;

	if (pa_inc == 0 && na_dec < 2)
		return 0;

	if (na_inc < 2 && pa_dec == 0)
		return 0;

	if (na_dec >= 2)
		return 1;

	if (na_inc >= 2)
		return 1;

	return 0;
}

void gen_change_attribs()
{
	signed short tmp1, tmp2, tmp3, si, di;
	unsigned char c;
	/* check if attributes have been set */
	if (hero.attribs[0].normal == 0) {
		GUI_gen.radio((Bit8u*)texts[16], 0);
		return;
	}
	/* check if changing is possible */
	if (gen_can_change_attribs() == 0) {
		GUI_gen.radio((Bit8u*)texts[266], 0);
		return;
	}
	/* if typus != 0 */
	if (hero.typus) {
		if (!GUI_gen.radio_bool((unsigned char*)texts[73], (Bit8u*)texts[4], (Bit8u*)texts[5]))
			return;
		/* set typus to 0 */
		hero.typus = 0;
		/* remove MU boni */
		if (got_mu_bonus) {
			hero.attribs[0].normal--;
			hero.attribs[0].current--;
			got_mu_bonus = false;
		}
		/* remove CH boni */
		if (got_ch_bonus) {
			hero.attribs[2].normal--;
			hero.attribs[2].current--;
			got_ch_bonus = false;
		}
		screen_f = true;
		gen_refresh_screen();
		screen_f = false;
	}
	/* check again if changing is possible */
	if (gen_can_change_attribs() == 0) {
		GUI_gen.radio((Bit8u*)texts[266], 0);
		return;
	}
	/* select a positive attribute to change */
	GUI_gen.shift_x = -80;
	tmp2 = GUI_gen.radio((unsigned char*)texts[78], 7,
			texts[32], texts[33], texts[34], texts[35],
			texts[36], texts[37], texts[38]);
	GUI_gen.shift_x = 0;

	if (tmp2 == -1)
		return;
	tmp2--;
	/* get the modification type */
	if (attrib_changed[tmp2] == 0) {
		/* ask user if inc or dec */
		GUI_gen.shift_x = 0;
		tmp3 = GUI_gen.radio((unsigned char*)NULL, 2, (Bit8u*)texts[75], (Bit8u*)texts[76]);
		GUI_gen.shift_x = 0;

		if (tmp3 == -1)
			return;
	} else {
		tmp3 = attrib_changed[tmp2];
	}

	if (tmp3 == INC) {
		/* increment */
		if (hero.attribs[tmp2].normal == 13) {
			GUI_gen.radio( (Bit8u*)texts[77], 0);
			return;
		}
		c = 0;
		for (di = 7; di < 14; di++) {
			if (attrib_changed[di] == DEC)
				continue;
			if (hero.attribs[di].normal >= 8)
				continue;
			c += 8 - hero.attribs[di].normal;
		}
		if (c < 2) {
			GUI_gen.radio((Bit8u*)texts[85], 0);
			return;
		}
		/* increment positive attribute */
		hero.attribs[tmp2].current++;
		hero.attribs[tmp2].normal++;

		attrib_changed[tmp2] = INC;

		gen_refresh_screen();

		tmp1 = 0;
		while (tmp1 != 2) {
			/* ask which negative attribute to increment */
			GUI_gen.shift_x = -80;
			si = GUI_gen.radio((unsigned char*)texts[80], 7,
					texts[39], texts[40], texts[41],
					texts[42], texts[43], texts[44],
					texts[45]);
			GUI_gen.shift_x = 0;

			if (si == -1)
				continue;

			si--;
			/* check if this attribute has been decremented */
			if (attrib_changed[si + 7] == DEC) {
				GUI_gen.radio((Bit8u*)texts[83], 0);
				continue;
			}
			/* check if attribute can be incremented */
			if (hero.attribs[si + 7].normal == 8) {
				GUI_gen.radio((Bit8u*)texts[77], 0);
				continue;
			}
			/* increment the negative attribute */
			tmp1++;
			attrib_changed[si + 7] = INC;
			hero.attribs[si + 7].normal++;
			hero.attribs[si + 7].current++;

			gen_refresh_screen();
		}
	} else {
		/* decrement */
		/* check if the positive attribute can be decremented */
		if (hero.attribs[tmp2].normal == 8) {
			GUI_gen.radio((Bit8u*)texts[81], 0);
			return;
		}
		c = 0;
		for (di = 7; di < 14; di++) {
			if (attrib_changed[di] == INC)
				continue;
			if (hero.attribs[di].normal <= 2)
				continue;
			c += hero.attribs[di].normal - 2;
		}
		if (c < 2) {
			GUI_gen.radio((Bit8u*)texts[84], 0);
			return;
		}
		/* decrement positive attribute */
		hero.attribs[tmp2].normal--;
		hero.attribs[tmp2].current--;
		/* mark this attribute as decremented */
		attrib_changed[tmp2] = DEC;

		gen_refresh_screen();

		tmp1 = 0;
		while (tmp1 != 2) {
			/* ask which negative attribute to increment */
			GUI_gen.shift_x = -80;
			si = GUI_gen.radio((unsigned char*)texts[79], 7,
					texts[39], texts[40], texts[41],
					texts[42], texts[43], texts[44],
					texts[45]);
			GUI_gen.shift_x = 0;

			if (si == -1)
				continue;

			si--;
			/* check if this attribute has been incremented */
			if (attrib_changed[si + 7] == INC) {
				GUI_gen.radio((Bit8u*)texts[82], 0);
				continue;
			}
			/* check if attribute can be decremented */
			if (hero.attribs[si + 7].normal == 2) {
				GUI_gen.radio((Bit8u*)texts[81], 0);
				continue;
			}
			/* deccrement the negative attribute */
			tmp1++;
			hero.attribs[si + 7].normal--;
			hero.attribs[si + 7].current--;
			attrib_changed[si + 7] = DEC;

			gen_refresh_screen();
		}
	}
}
void gen_clear_hero() {

	unsigned short i;

	got_ch_bonus = false;
	got_mu_bonus = false;

	head_current = 0;
	head_last = 0;
	head_first = 0;
	head_typus = 0;

	for (i = 0; i < 14; i++)
		attrib_changed[i] = 0;

	for (i = 0; i < 86; i++) {
		spell_incs[i].incs = 0;
		spell_incs[i].tries = 0;
	}
	for (i = 0; i < 52; i++) {
		skill_incs[i].tries = 0;
		skill_incs[i].incs = 0;
	}

	hero.level = 1;
}
void gen_new_values()
{
	/* Original-Bugfix:	there once was a char[11],
				which could not hold a char[16] */

	char name_bak[17];
	signed char values[8];
	unsigned short di, i, j;
	signed char bv1, bv2, bv3;

	/* set variable if hero has a typus -*/
	if (hero.typus)
		screen_f = true;

	/* save the name of the hero */
	strcpy(name_bak, hero.name);

	/* save the sex of the hero */
	bv3 = hero.sex;

	/* clear the hero */
	memset(&hero, 0 , sizeof(hero));
	gen_clear_hero();

	/* restore the sex of the hero */
	hero.sex = bv3;

	/* restore the name of the hero */
	strcpy(hero.name, name_bak);

	gen_refresh_screen();

	screen_f = false;

	for (j = 0; j < 7; j++) {
		bv1 = (unsigned char)random_interval_gen(8, 13);
		bv2 = 0;

		for (i = 0; i < 7; i++) {
			if (hero.attribs[i].normal)
				continue;

			values[bv2] = (signed char)i;
			type_names[bv2] = texts[0x80 / 4 + i];
			bv2++;
		}

		sprintf(gen_ptr2, texts[46], bv1);

		do {
			GUI_gen.shift_x = -80;

			di = GUI_gen.radio((unsigned char*)gen_ptr2,
				bv2,
				type_names[0],
				type_names[1],
				type_names[2],
				type_names[3],
				type_names[4],
				type_names[5],
				type_names[6]);

			GUI_gen.shift_x = 0;
		} while (di == -1);
		di = values[di - 1];
		hero.attribs[di].current = bv1;
		hero.attribs[di].normal = bv1;
		gen_refresh_screen();
	}

	for (j = 0; j < 7; j++) {
		bv1 = (signed char)random_interval_gen(2, 7);
		bv2 = 0;

		for (i = 0; i < 7; i++) {
			if (hero.attribs[i + 7].normal)
				continue;

			values[bv2] = (signed char)i;
			type_names[bv2] = texts[0x9c / 4 + i];
			bv2++;
		}

		sprintf(gen_ptr2, texts[46], bv1);

		do {
			GUI_gen.shift_x = -80;

			di = GUI_gen.radio((unsigned char*)gen_ptr2,
				bv2,
				type_names[0],
				type_names[1],
				type_names[2],
				type_names[3],
				type_names[4],
				type_names[5],
				type_names[6]);

			GUI_gen.shift_x = 0;
		} while (di == 0xffff);
		di = values[di - 1];
		hero.attribs[di + 7].current = bv1;
		hero.attribs[di + 7].normal = bv1;
		gen_refresh_screen();
	}
}
void gen_calc_at_pa() {
	unsigned short i;
	signed short tmp, base;

	/* base = (GE + IN + KK) / 5 */
	tmp = hero.attribs[5].normal + hero.attribs[6].normal +
		hero.attribs[4].normal ;
	base = tmp / 5;

	/* round up if neccessary */
	if ((tmp % 5) >= 3)
		base++;

	/* save AT/PA base value */
	hero.atpa = (signed char)base;

	for (i = 0; i < 7; i++) {
		/* set the weapon values to base */
		hero.pa[i] = (signed char)base;
		hero.at[i] = (signed char)base;

		if (hero.skills[i] < 0) {
			/* calculate ATPA for negative weapon skill */
			tmp = abs(hero.skills[i]) / 2;

			/* sub skill / 2 from AT */
			hero.at[i] -= tmp;

			/* sub skill / 2 from PA */
			hero.pa[i] -= tmp;

			/* if skill % 2, then decrement PA */
			if (abs(hero.skills[i]) != tmp * 2)
				hero.pa[i]--;
		} else {
			/* calculate ATPA for positive weapon skill */
			tmp = abs(hero.skills[i]) / 2;

			/* add skill / 2 to AT */
			hero.at[i] += tmp;

			/* add skill / 2 to PA */
			hero.pa[i] += tmp;

			/* if skill % 2, then increment AT */
			if (hero.skills[i] != tmp * 2)
				hero.at[i]++;
		}

	}
}
void gen_skill_inc_novice(unsigned short skill)
{
	unsigned short done = 0;

	while (!done) {
		/* leave the loop if 3 tries have been done */
		if (skill_incs[skill].tries == 3) {
			/* set the flag to leave this loop */
			done++;
			continue;
		}

		/* Original-Bugfix: add check if skill_attempts are left */
		if (hero.skill_incs == 0) {
			done++;
			continue;
		}
		/* decrement counter for skill increments */
		hero.skill_incs--;

		/* check if the test is passed */
		if (random_interval_gen(2, 12) > hero.skills[skill]) {
			/* increment skill */
			hero.skills[skill]++;

			/* set inc tries for this skill to zero */
			skill_incs[skill].tries = 0;

			/* set the flag to leave this loop */
			done++;

			if (skill > 6)
				continue;

			/* set increment the lower AT/PA value */
			if (hero.at[skill] > hero.pa[skill])
				hero.pa[skill]++;
			else
				hero.at[skill]++;
		} else
			/* inc tries for that skill */
			skill_incs[skill].tries++;
	}
}

void gen_spell_inc_novice(unsigned short spell)
{
	unsigned short done = 0;

	while (!done) {
		/* leave the loop if 3 tries have been done */
		if (spell_incs[spell].tries == 3) {
			/* set the flag to leave this loop */
			done++;
			continue;
		}

		/* Original-Bugfix: add check if skill_attempts are left */
		if (hero.spell_incs == 0) {
			done++;
			continue;
		}
		/* decrement counter for spell increments */
		hero.spell_incs--;

		/* check if the test is passed */
		if (random_interval_gen(2, 12) > hero.spells[spell]) {
			/* increment spell */
			hero.spells[spell]++;

			/* set inc tries for this spell to zero */
			spell_incs[spell].tries = 0;

			/* set the flag to leave this loop */
			done++;
		} else
			spell_incs[spell].tries++;
	}
}

void gen_fill_values()
{
	unsigned char *ptr;
	unsigned short i;
	unsigned short si, di;

	/* fill skill values */
	for (i = 0; i < 52; i++) {

		hero.skills[i] = skills[hero.typus][i];

		/* set skill_incs and skill_tries to zero */
		skill_incs[i].incs = 0;
		skill_incs[i].tries = 0;
	}

	/* set skill_attempts */
	hero.skill_incs = initial_skill_incs[hero.typus - 1];

	/* do magic user init */
	if (hero.typus >= 7) {
		/* fill initial spell values */
		for (i = 0; i < 86; i++) {
			hero.spells[i] = spells[hero.typus - 7][i];

			/* set spell_incs and spell_tries to zero */
			spell_incs[i].incs = 0;
			spell_incs[i].tries = 0;
		}
		/* special mage values */
		if (hero.typus == 9) {
			/* set staff spell to level 1 */
			hero.staff_level = 1;
			/* select mage school */
			do {
				hero.school = GUI_gen.radio((unsigned char*)texts[47], 9,
							texts[48], texts[49],
							texts[50], texts[51],
							texts[52], texts[53],
							texts[54], texts[55],
							texts[56]) - 1;
			} while (hero.school == -2);

			/* add magic school modifications */
			for (i = 0; house_mod[hero.school].nr > i; i++) {
				signed short spell, mod;

				spell = house_mod[hero.school].spells[i];
				mod = house_mod[hero.school].mod[i];
				hero.spells[spell] += mod;
			}
		}

		/* set spell attempts */
		hero.spell_incs = initial_spell_incs[hero.typus - 7];

		/* get convertable increase attempts */
		di = initial_conv_incs[hero.typus - 7];

		if (di && level == 2 && GUI_gen.radio_bool((unsigned char*)texts[269], (Bit8u*)texts[4], (Bit8u*)texts[5])) {
			/* create string */
			sprintf(gen_ptr2, texts[270], di);

			i = GUI_gen.input((Bit8u*)gen_ptr2, 1);

			if (i > 0) {
				/* spell attempts to skill attempts */
				if (i > di)
					i = di;
				di -= i;
				/* change spell attempts */
				hero.spell_incs -= i;
				/* change skill attempts */
				hero.skill_incs += i;
			} else {

				/* create string */
				sprintf(gen_ptr2, texts[271], di);

				i = GUI_gen.input((Bit8u*)gen_ptr2, 1);
				if (i > 0) {
					if (i > di)
						i = di;
					/* change spell attempts */
					hero.spell_incs += i;
					/* change skill attempts */
					hero.skill_incs -= i;
				}
			}
		}


	}

	/* set LE */
	hero.le = hero.le_max = init_le[hero.typus];

	/* set AE */
	hero.ae = hero.ae_max = init_ae[hero.typus];

	/* wanna change 10 spell_attempts against 1W6+2 AE ? */
	if (hero.typus == 9 && level == 2 && GUI_gen.radio_bool((unsigned char*)texts[268], (Bit8u*)texts[4], (Bit8u*)texts[5])) {
		/* change spell_attempts */
		hero.spell_incs -= 10;
		hero.ae_max = random_interval_gen(3, 8) + hero.ae_max;
		hero.ae = hero.ae_max;
	}

	/* roll out size */
	hero.height = (unsigned char)random_interval_gen(height_range[hero.typus].min,
				height_range[hero.typus].max);

	/* calculate weight i = (height - weight_mod) * 40 */
	hero.weight = (hero.height - weight_mod[hero.typus]) * 40;

	/* roll out the money */
	i = random_gen(20);
	//ptr = Real2Host(ds_readd(0xa51 + hero.typus * 4));
	for (si = ptr_money[hero.typus]; v_money[si].i < i; si++);
	//for (si = 0; host_readw(ptr + si * 6) < i; si++);

	hero.money = random_interval_gen(v_money[si].min,v_money[si].max) * 10;

	/* calculate MR  = (KL + SI + Level) / 3 - 2 * AG */
	hero.mr = (hero.attribs[1].normal + hero.attribs[0].normal + hero.level) / 3 -
		hero.attribs[7].normal * 2;
	/* add typus MR Modificator */
	hero.mr += mr_mod[hero.typus];

	/* roll out god */
	hero.god = (unsigned char)random_gen(12);

	/* add gods boni */
	switch (hero.god) {
		case 1 : {
			/* Praios: MU + 1 */
			hero.attribs[0].normal++;
			hero.attribs[0].current++;
			got_mu_bonus = true;
			break;
		}
		case 2 : {
			/* Rondra: skill swords + 1 */
			hero.skills[3]++;
			break;
		}
		case 3 : {
			/* Efferd: skill swim + 1 */
			hero.skills[14]++;
			break;
		}
		case 4 : {
			/* Travia: skill treat poison + 1 */
			hero.skills[44]++;
			break;
		}
		case 5 : {
			/* Boron: skill human nature + 1 */
			hero.skills[24]++;
			break;
		}
		case 6 : {
			/* Hesinde: skill alchemy + 1 */
			hero.skills[32]++;
			break;
		}
		case 7 : {
			/* Firun: skills track and missle weapons + 1  */
			hero.skills[26]++;
			hero.skills[7]++;
			break;
		}
		case 8 : {
			/* Tsa: CH + 1 */
			hero.attribs[2].normal++;
			hero.attribs[2].current++;
			got_ch_bonus = true;
			break;
		}
		case 9 : {
			/* Phex: skills hide and pickpocket + 1 */
			hero.skills[49]++;
			hero.skills[13]++;
			break;
		}
		case 10 : {
			/* Peraine: skills treat disease and wounds + 1 */
			hero.skills[45]++;
			hero.skills[46]++;
			break;
		}
		case 11 : {
			/* Ingerimm: skill tactics + 1*/
			hero.skills[37]++;
			break;
		}
		case 12 : {
			/* Rhaja: skills dance, seduce and instrument + 1*/
			hero.skills[20]++;
			hero.skills[16]++;
			hero.skills[47]++;
			break;
		}
	}
	/* calclate AT and PA values */
	gen_calc_at_pa();

	/* if mode == novice */
	if (level == 1) {
		/* automatic increase skills */
		i = 0;
		while (hero.skill_incs > 0) {
			gen_skill_inc_novice(autoskills[hero.typus][i++]);
		}

		si = 0;
		/* prepare mage automatic spell list */
		if (hero.typus == 9) {
			/* 1. house spells */
			for (i = 0; house_mod[hero.school].nr > i; si++, i++) {
				autospells[hero.typus - 7][si] =
					house_mod[hero.school].spells[i];
			}
			/* 2. all schools spells */
			for (i = 0; school_tab[hero.school].spells > i; si++, i++) {
				autospells[hero.typus - 7][si] =
					school_tab[hero.school].first_spell + i;
			}
			/* 3. five domination spells */
				/* Herr der Tiere */
			autospells[hero.typus - 7][si++] = 0x52;
				/* Horriphobus */
			autospells[hero.typus - 7][si++] = 0x31;
				/* Mag. Raub */
			autospells[hero.typus - 7][si++] = 0x35;
				/* Respondami */
			autospells[hero.typus - 7][si++] = 0x21;
				/* Sanftmut */
			autospells[hero.typus - 7][si++] = 0x4f;

			/* 4. all house spells */
			for (i = 0; house_mod[hero.school].nr > i; si++, i++) {
				autospells[hero.typus - 7][si] =
					house_mod[hero.school].spells[i];
			}
			/* 5. all house spells */
			for (i = 0; house_mod[hero.school].nr > i; si++, i++) {
				autospells[hero.typus - 7][si] =
					house_mod[hero.school].spells[i];
			}
			/* 6. random spells */
			while (si < 45) {
				autospells[hero.typus - 7][si++] = random_gen(85);
			}
		}
		/* automatic increase spells */
		i = 0;
		while (hero.spell_incs > 0) {
			gen_spell_inc_novice(autospells[hero.typus - 7][i++]);
		}
	}
}
void gen_select_typus()
{
	signed char old_typus, possible_types, ltmp2;
	bool impossible;
	signed short i, si, di;
	struct type_bitmap t;

	old_typus = -1;
	t = empty_bitmap;

	/* check if attribs have bee set */
	if (hero.attribs[0].normal == 0) {
		GUI_gen.radio((Bit8u*)texts[265], 0);
		return;
	}
	/* save the old typus */
	old_typus = hero.typus;
	/* disable MU bonus */
	if (got_mu_bonus) {
		hero.attribs[0].normal--;
		hero.attribs[0].current--;
	}
	/* disable CH bonus */
	if (got_ch_bonus) {
		hero.attribs[2].normal--;
		hero.attribs[2].current--;
	}
	possible_types = 0;

	for (i = 1; i <= 12; i++) {
		impossible = false;
		for (si = 0; si < 4; si++) {
			unsigned char req;

			ltmp2 = hero.attribs[reqs[i][si].attrib].normal;

			req = reqs[i][si].requirement;

			if (req & 0x80) {
				if (ltmp2 <= (req & 0x7f))
					continue;
				impossible = true;
			} else {
				if (req <= ltmp2)
					continue;
				impossible = true;
			}
		}

		if (impossible)
			continue;

		if (hero.sex)
			type_names[possible_types] = texts[271 + i];
		else
			type_names[possible_types] = texts[17 + i];

		t.t[possible_types] = (char)i;
		possible_types++;

	}

	if (possible_types == 0) {
		if (gen_can_change_attribs() == 0) {
			/* totally messed up values */
			GUI_gen.radio((Bit8u*)texts[284], 0);
			return;
		} else {
			GUI_gen.radio((Bit8u*)texts[31], 0);
			return;
		}
	}

	di = GUI_gen.radio((unsigned char*)texts[30], possible_types,
			type_names[0],
			type_names[1],
			type_names[2],
			type_names[3],
			type_names[4],
			type_names[5],
			type_names[6],
			type_names[7],
			type_names[8],
			type_names[9],
			type_names[10],
			type_names[11]);

	/*	restore attibute boni when selection is canceled
	 *	or the same typus is selected.
	 */
	if (di == -1 || t.t[di - 1] == old_typus) {
		if (got_mu_bonus) {
			hero.attribs[0].normal++;
			hero.attribs[0].current++;
		}
		if (got_ch_bonus) {
			hero.attribs[2].normal++;
			hero.attribs[2].current++;
		}
		return;
	}

	/* set new typus */
	hero.typus = t.t[di - 1];
	screen_f = true;

	gen_load_typus(hero.typus);
	//set_palette(Real2Host(ds_readd(0x47b3)) + 0x5c02, 0, 32);


	if (hero.typus > 10)
		head_typus = 10;
	else
		head_typus = hero.typus;

	if (hero.sex) {
		head_current = head_first_female[head_typus];
		head_first = head_first_female[head_typus];
		head_last = head_first_male[head_typus + 1] - 1;
	} else {
		head_current = head_first_male[head_typus];
		head_first = head_first_male[head_typus];
		head_last = head_first_female[head_typus] - 1;
	}

	/* reset boni flags */
	got_ch_bonus = false;
	got_mu_bonus = false;
	gen_fill_values();
	return;
}
void gen_choose_typus()
{
	char name_bak[20];
	unsigned short i, typus_names;
	signed short choosen_typus;
	unsigned char randval;
	char sex_bak;

	if (!GUI_gen.radio_bool((unsigned char*)texts[264], (Bit8u*)texts[4], (Bit8u*)texts[5]))
		return;

	if (hero.sex)
		/* famale typus names */
		typus_names = 271;
	else
		/* male tyuse names */
		typus_names = 17;
		choosen_typus = GUI_gen.radio((unsigned char*)texts[30], 12,
				texts[typus_names + 1], texts[typus_names + 2],
				texts[typus_names + 3], texts[typus_names + 4],
				texts[typus_names + 5], texts[typus_names + 6],
				texts[typus_names + 7], texts[typus_names + 8],
				texts[typus_names + 9], texts[typus_names + 10],
				texts[typus_names + 11], texts[typus_names + 12]);

	if (choosen_typus == -1)
		return;

	/* clear the hero area with saved name and sex */
	strcpy(name_bak, hero.name);
	sex_bak = hero.sex;
	memset(&hero, 0, sizeof(hero));
	gen_clear_hero();
	hero.sex = sex_bak;
	strcpy(hero.name, name_bak);

	/* set typus */
	hero.typus = (unsigned char)choosen_typus;

	/* roll out good attribute values */
	for (i = 0; i < 7; i ++) {

		randval = (unsigned char)random_interval_gen(8, 13);

		if (randval > 8)
			randval--;

		hero.attribs[i].normal = randval;
		hero.attribs[i].current = randval;
	}

	/* roll out bad attribute values */
	for (i = 0; i < 7; i ++) {

		randval = (unsigned char)random_interval_gen(2, 7);

		if (randval < 7)
			randval++;

		hero.attribs[i + 7].normal = randval;
		hero.attribs[i + 7].current = randval;
	}

	/* adjust typus attribute requirements */
	for (i = 0; i < 4; i++) {
		unsigned char ta;
		/* calc pointer to attribute */
		ta = reqs[choosen_typus][i].attrib;

		/* get the required value */
		randval = reqs[choosen_typus][i].requirement;

		if (randval == 1)
			continue;

		if (randval & 0x80) {
			/* attribute upper bound */
			if (hero.attribs[ta].normal <= (randval & 0x7f))
				continue;

			hero.attribs[ta].current = randval & 0x7f;
			hero.attribs[ta].normal = randval & 0x7f;
		} else {
			/* attribute lower bound */
			if (hero.attribs[ta].normal >= randval)
				continue;

			hero.attribs[ta].current = randval;
			hero.attribs[ta].normal = randval;
		}
	}

	gen_load_typus(hero.typus);
	//set_palette(Real2Host(ds_readd(0x47b3)) + 0x5c02, 0, 32);

	if (hero.typus > 10)
		head_typus = 10;
	else
		head_typus = hero.typus;

	if (hero.sex) {
		head_current = head_first_female[head_typus];
		head_first = head_first_female[head_typus];
		head_last = head_first_male[head_typus + 1] - 1;
	} else {
		head_current = head_first_male[head_typus];
		head_first = head_first_male[head_typus];
		head_last = head_first_female[head_typus] - 1;
	}
	gen_fill_values();
	screen_f=true;
}
static char hero_out[1754];

static inline void hero_writeb(unsigned off, char v)
{
	writeb((unsigned char*)hero_out + off - 0x132c, v);
}

static inline void hero_writew(unsigned off, short v)
{
	writew((unsigned char*)hero_out + off - 0x132c, v);
}

static inline void hero_writed(unsigned off, int v)
{
	writed((unsigned char*)hero_out + off - 0x132c, v);
}

static void update_hero_out()
{
	unsigned long i;

	memset(hero_out, 0, 1754);

	strncpy(hero_out, hero.name, 16);
	strncpy(hero_out + 0x10, hero.alias, 16);

	hero_writeb(0x134d, hero.typus);
	hero_writeb(0x134e, hero.sex);
	hero_writeb(0x134f, hero.height);
	hero_writew(0x1350, hero.weight);
	hero_writeb(0x1352, hero.god);
	hero_writeb(0x1353, hero.level);
	hero_writed(0x1358, hero.money);

	for (i = 0; i < 14; i++) {
		hero_writeb(0x1360 + i * 3 + 0, hero.attribs[i].normal);
		hero_writeb(0x1360 + i * 3 + 1, hero.attribs[i].current);
		hero_writeb(0x1360 + i * 3 + 2, hero.attribs[i].mod);
	}

	hero_writew(0x138a, hero.le);
	hero_writew(0x138c, hero.le_max);
	hero_writew(0x138e, hero.ae);
	hero_writew(0x1390, hero.ae_max);
	hero_writeb(0x1392, hero.mr);
	hero_writeb(0x1393, hero.atpa);

	for (i = 0; i < 7; i++)
		hero_writeb(0x1394 + i, hero.at[i]);
	for (i = 0; i < 7; i++)
		hero_writeb(0x139b + i, hero.pa[i]);

	hero_writeb(0x13b4, hero.group);

	for (i = 0; i < 52; i++)
		hero_writeb(0x1434 + i, hero.skills[i]);
	hero_writeb(0x1468, hero.skill_incs);
	for (i = 0; i < 86; i++)
		hero_writeb(0x1469 + i, hero.spells[i]);
	hero_writeb(0x14bf, hero.spell_incs);
	hero_writeb(0x14c0, hero.school);
	hero_writeb(0x14c1, hero.staff_level);

	memcpy(hero_out + 0x2da, &hero.pic, 1024);
}
void gen_save_chr()
{
	struct nvf_desc nvf;
	FILE *fd;
	signed short tmp;
	char *pwd;
	char path[80];
	char filename[20];

	unsigned short i;

	/* check for typus */
	if (hero.typus == 0) {
		GUI_gen.radio((Bit8u*)texts[0x120 / 4], 0);
		return;
	}
	/* check for name */
	if (hero.name[0] == 0) {
		GUI_gen.radio((Bit8u*)texts[0x120 / 4], 0);
		return;
	}
	/* Load picture from nvf */
	/* TODO: why not just copy? because format changed...*/
	/* load HEADS.DAT */
	fd = gen_open_datfile(11);
	gen_read_datfile(fd, buffer_dat, 64000);
	fclose(fd);

	nvf.dst = gataosao;
	nvf.src = buffer_dat;
	nvf.nr = head_current;
	nvf.type = 0;
	nvf.width = 0;
	nvf.height = 0;

	process_nvf(&nvf);

	/* copy picture to the character struct */
	memcpy(&hero.pic, gataosao, 1024);
	/* put the hero in the first group */
	hero.group = 1;

	/* wanna save ? */
	if (!GUI_gen.radio_bool((unsigned char*)texts[3], (Bit8u*)texts[4], (Bit8u*)texts[5]))
		return;

	/* copy name to alias */
	strcpy(hero.alias, hero.name);
	/* copy name to buffer */
	strcpy(gen_ptr2, hero.name);
	/* prepare filename */
	for (i = 0; i < 8; i++) {
		char c = gen_ptr2[i];
		/* leave the loop if the string ends */
		if (c == 0)
			break;
		if (isalnum(c))
			continue;
		/* replace non alphanumerical characters with underscore */
		gen_ptr2[i] = '_';
	}

	strncpy(filename, gen_ptr2, 8);
	filename[8] = 0;
	strcat(filename, ".CHR");

	//pwd = get_pwd();
	//strncat(pwd, filename, 12);
	//prepare_path(pwd);

	/* try to open the filename */
	//fd = fopen(pwd, "rb");
	fd = fopen(filename, "rb");
	//free(pwd);

	/* if the file exists ask if should overwrite */
	if (fd) {
		/* Original-Bugfix: the file should be closed */
		fclose(fd);

		if (!GUI_gen.radio_bool((unsigned char*)texts[0x414 / 4], (Bit8u*)texts[4], (Bit8u*)texts[5]))
			return;
	}

	/* here originally creat() was used */
	fd = fopen(filename, "wb");

	if (fd) {
		/* write the CHR file to the current directory */
		update_hero_out();
		i=fwrite(hero_out, 1, 1754, fd);
		fclose(fd);

		/* save it to the TEMP dir if called from with arguments /
		if (ds_readw(0x3f60) != 0) {
			strcpy(path, "TEMP\\");
			strcat(path, filename);

			pwd = get_pwd();
			strncat(pwd, path, 80);
			prepare_path(pwd);

			fd = fopen(pwd, "w+");
			free(pwd);

			if (fd) {
				fwrite(hero_out, 1, 1754, fd);
				fclose(fd);
			}
		}/*/
	} else {
		/* should be replaced with GUI_gen.radio((Bit8u*)) */
		GUI_gen.radio((Bit8u*)"could not save", 0);
	}
}
void gen_inc_skill(unsigned short skill, unsigned short max, char *msg)
{
	/* no more increments than the maximum */
	if (skill_incs[skill].incs >= max) {
		GUI_gen.radio((Bit8u*)(char*)msg, 0);
		return;
	}
	/* we just have 3 tries to increment */
	if (skill_incs[skill].tries == 3) {
		GUI_gen.radio((Bit8u*)texts[151], 0);
		return;
	}

	/* decrement total number of skill inc tries */
	hero.skill_incs--;
	if (random_interval_gen(2, 12) > hero.skills[skill]) {
		/* increment skill */
		hero.skills[skill]++;
		/* reset tries */
		skill_incs[skill].tries = 0;
		/* increment skill increments */
		skill_incs[skill].incs++;
		/* print sucess message */
		gen_refresh_screen();
		GUI_gen.radio((Bit8u*)texts[152], 0);

		/* check if we have a melee attack skill */
		if (skill <= 6) {
			/* check if AT > PA */
			if (hero.at[skill] > hero.pa[skill]) {
				/* inc PA */
				hero.pa[skill]++;
			} else {
				/* inc AT */
				hero.at[skill]++;
			}
		}
	} else {
		/* print failure message */
		gen_refresh_screen();
		GUI_gen.radio((Bit8u*)texts[153], 0);
		/* increment try */
		skill_incs[skill].tries++;
	}

}

void gen_select_skill()
{
	signed short skill, group;

	do {

		/* check skill attempts */
		if (hero.skill_incs == 0) {
			GUI_gen.radio((Bit8u*)texts[94], 0);
			return;
		}

		GUI_gen.shift_x = -80;

		switch (gen_page) {
			case 1: {
				group = GUI_gen.radio((unsigned char*)texts[93], 2,
						texts[86], texts[87]);
				if (group == -1)
					break;

				switch (group) {
					case 1: {
						/* Fight */
						skill = GUI_gen.radio((unsigned char*)texts[147],
								9,
								texts[95],
								texts[96],
								texts[97],
								texts[98],
								texts[99],
								texts[100],
								texts[101],
								texts[102],
								texts[103]);
						skill--;

						if (skill == -2)
							break;

						gen_inc_skill(skill, 1, texts[148]);

						break;
					}
					case 2: {
						/* body */
						skill = GUI_gen.radio((unsigned char*)texts[147],
							10,
							texts[104], texts[105],
							texts[106], texts[107],
							texts[108], texts[109],
							texts[110], texts[111],
							texts[112], texts[113]);
						skill--;

						if (skill == -2)
							break;

						skill += 9;
						gen_inc_skill(skill, 2, texts[149]);
						break;
					}
				}
				break;
			}
			case 2: {
				group = GUI_gen.radio((unsigned char*)texts[93], 2,
						texts[88], texts[89]);
				if (group == -1)
					break;
				switch (group) {
					case 1: {
						skill = GUI_gen.radio((unsigned char*)texts[147],
								7,
								texts[114],
								texts[115],
								texts[116],
								texts[117],
								texts[118],
								texts[119],
								texts[120]);
						skill--;

						if (skill == -2)
							break;

						skill += 19;
						gen_inc_skill(skill, 2, texts[149]);
						break;
					}
					case 2: {
						skill = GUI_gen.radio((unsigned char*)texts[147],
								9,
								texts[127],
								texts[128],
								texts[129],
								texts[130],
								texts[131],
								texts[132],
								texts[133],
								texts[134],
								texts[135]);
						skill--;

						if (skill == -2)
							break;

						skill += 32;
						gen_inc_skill(skill, 3, texts[150]);
						break;
					}
				}
				break;
			}
			case 3: {
				group = GUI_gen.radio((unsigned char*)texts[93], 3,
						texts[90], texts[91], texts[92]);
				if (group == -1)
					break;

				switch (group) {
					case 1: {
						skill = GUI_gen.radio((unsigned char*)texts[147],
								9,
								texts[136],
								texts[137],
								texts[138],
								texts[139],
								texts[140],
								texts[141],
								texts[142],
								texts[143],
								texts[144]);

						skill--;

						if (skill == -2)
							break;

						skill += 41;
						gen_inc_skill(skill, 2, texts[149]);
						break;
					}
					case 2: {
						skill = GUI_gen.radio((unsigned char*)texts[147],
								6,
								texts[121],
								texts[122],
								texts[123],
								texts[124],
								texts[125],
								texts[126]);

						skill--;

						if (skill == -2)
							break;

						skill += 26;
						gen_inc_skill(skill, 2, texts[149]);
						break;
					}
					case 3: {
						skill = GUI_gen.radio((unsigned char*)texts[147],
								2,
								texts[145],
								texts[146]);

						skill--;

						if (skill == -2)
							break;

						skill += 50;
						gen_inc_skill(skill, 1, texts[148]);
						break;
					}
				}
				break;
			}
		}
		GUI_gen.shift_x = 0;

	} while (group != -1);
}
unsigned short is_in_word_array(unsigned short val, unsigned char *p)
{
	while (readw(p) != 0) {
		p += 2;
		if (readw(p) == val)
			return 1;
	}
	return 0;
}
void gen_inc_spell(unsigned short spell)
{
	unsigned short max_incs = 1;

	/* if typus == warlock and the origin of the spell is warlock */
	if (hero.typus == 7 && spelltab[spell].origin == 3)
		max_incs = 2;
	/* if typus == elf and the origin of the spell is elven */
	if (hero.typus >= 10 && spelltab[spell].origin == 2)
		max_incs = 2;
	/* if typus == druid and the origin of the spell is druid */
	if (hero.typus == 8 && spelltab[spell].origin == 0)
		max_incs = 2;
	/* if typus == mage */
	if (hero.typus == 9) {
		/* and the origin of the spell is mage */
		if (spelltab[spell].origin == 1)
			max_incs = 2;

		unsigned char *array = (unsigned char*)house_spells[hero.school];
		/* and is a school spell */
		if (is_in_word_array(spell, array))
			max_incs = 3;
	}

	/* all spell increments used for that spell */
	if (spell_incs[spell].incs >= max_incs) {
		GUI_gen.radio((Bit8u*)texts[0x404 / 4], 0);
		return;
	}
	/* all tries used for that spell */
	if (spell_incs[spell].tries == 3) {
		GUI_gen.radio((Bit8u*)texts[0x25c / 4], 0);
		return;
	}

	/* decrement spell attempts */
	hero.spell_incs--;

	if (random_interval_gen(2, 12) > hero.spells[spell]) {
		/* increment spell value */
		hero.spells[spell]++;
		/* reset tries */
		spell_incs[spell].tries = 0;
		/* increment incs */
		spell_incs[spell].incs++;
		/* show success */
		gen_refresh_screen();
		GUI_gen.radio((Bit8u*)texts[0x260 / 4], 0);
	} else {
		/* show failure */
		gen_refresh_screen();
		GUI_gen.radio((Bit8u*)texts[0x264 / 4], 0);
		/* increment tries */
		spell_incs[spell].tries++;
	}

	gen_refresh_screen();
}

void gen_select_spell()
{
	signed short group, spell;

	do {

		/* check if we have spell attempts */
		if (hero.spell_incs == 0) {
			GUI_gen.radio((Bit8u*)texts[94], 0);
			GUI_gen.shift_x = 0;
			return;
		}

		GUI_gen.shift_x = -90;

		switch (gen_page) {
			case 5: {
				group = GUI_gen.radio((unsigned char*)texts[155], 3,
						texts[157], texts[162],
						texts[158]);
				if (group == -1)
					break;

				switch (group) {
					case 1: {
						spell = GUI_gen.radio((unsigned char*)texts[156], 5,
								texts[169],
								texts[170],
								texts[171],
								texts[172],
								texts[173]);
						spell--;
						if (spell == -2)
							break;
						spell++;
						gen_inc_spell(spell);
						break;
					}
					case 2: {
						spell = GUI_gen.radio((unsigned char*)texts[156], 5,
								texts[201],
								texts[202],
								texts[203],
								texts[204],
								texts[205]);
						spell--;
						if (spell == -2)
							break;
						spell += 33;
						gen_inc_spell(spell);
						break;
					}
					case 3: {
						spell = GUI_gen.radio((unsigned char*)texts[156], 6,
								texts[174],
								texts[175],
								texts[176],
								texts[177],
								texts[178],
								texts[179]);
						spell--;
						if (spell == -2)
							break;
						spell += 6;
						gen_inc_spell(spell);
						break;
					}
				}
				break;
			}
			case 6: {
				group = GUI_gen.radio((unsigned char*)texts[155], 3,
						texts[158], texts[159],
						texts[160]);
				if (group == -1)
					break;

				switch (group) {
					case 1: {
						spell = GUI_gen.radio((unsigned char*)texts[156], 6,
								texts[180],
								texts[181],
								texts[182],
								texts[183],
								texts[184],
								texts[185]);

						spell--;

						if (spell == -2)
							break;

						spell += 12;
						gen_inc_spell(spell);
						break;
					}
					case 2: {
						spell = GUI_gen.radio((unsigned char*)texts[156], 6,
								texts[186],
								texts[187],
								texts[188],
								texts[189],
								texts[190],
								texts[191]);

						spell--;

						if (spell == -2)
							break;

						spell += 18;
						gen_inc_spell(spell);
						break;
					}
					case 3: {
						spell = GUI_gen.radio((unsigned char*)texts[156], 3,
								texts[192],
								texts[193],
								texts[194]);

						spell--;

						if (spell == -2)
							break;

						spell += 24;
						gen_inc_spell(spell);
						break;
					}
				}

				break;
			}
			case 7: {
				group = GUI_gen.radio((unsigned char*)texts[155], 3,
						texts[161], texts[163],
						texts[164]);
				if (group == -1)
					break;

				switch (group) {
					case 1: {
						spell = GUI_gen.radio((unsigned char*)texts[156], 6,
								texts[195],
								texts[196],
								texts[197],
								texts[198],
								texts[199],
								texts[200]);

						spell--;

						if (spell == -2)
							break;

						spell += 27;
						gen_inc_spell(spell);
						break;
					}
					case 2: {
						spell = GUI_gen.radio((unsigned char*)texts[156], 7,
								texts[206],
								texts[207],
								texts[208],
								texts[209],
								texts[210],
								texts[211],
								texts[212]);

						spell--;

						if (spell == -2)
							break;

						spell += 38;
						gen_inc_spell(spell);
						break;
					}
					case 3: {
						spell = GUI_gen.radio((unsigned char*)texts[156], 2,
								texts[213],
								texts[214]);

						spell--;

						if (spell == -2)
							break;

						spell += 45;
						gen_inc_spell(spell);
						break;
					}
				}

				break;
			}
			case 8: {
				group = GUI_gen.radio((unsigned char*)texts[155], 3,
						texts[164], texts[86],
						texts[166]);
				if (group == -1)
					break;


				switch (group) {
					case 1: {
						spell = GUI_gen.radio((unsigned char*)texts[156], 2,
								texts[215],
								texts[216]);

						spell--;

						if (spell == -2)
							break;

						spell += 47;
						gen_inc_spell(spell);
						break;
					}
					case 2: {
						spell = GUI_gen.radio((unsigned char*)texts[156], 9,
								texts[217],
								texts[218],
								texts[219],
								texts[220],
								texts[221],
								texts[222],
								texts[223],
								texts[224],
								texts[225]);

						spell--;

						if (spell == -2)
							break;

						spell += 49;
						gen_inc_spell(spell);
						break;
					}
					case 3: {
						spell = GUI_gen.radio((unsigned char*)texts[156], 2,
								texts[226],
								texts[227]);

						spell--;

						if (spell == -2)
							break;

						spell += 58;
						gen_inc_spell(spell);
						break;
					}
				}
				break;
				/* TODO */
			}
			case 9: {
				spell = GUI_gen.radio((unsigned char*)texts[156], 16,
						texts[228], texts[229],
						texts[230], texts[231],
						texts[232], texts[233],
						texts[234], texts[235],
						texts[236], texts[237],
						texts[238], texts[239],
						texts[240], texts[241],
						texts[242], texts[243]);
				spell--;

				if (spell == -2) {
					group = -1;
					break;
				}

				spell += 60;
				gen_inc_spell(spell);
				break;
			}
			case 10: {
				spell = GUI_gen.radio((unsigned char*)texts[156], 10,
						texts[244], texts[245],
						texts[246], texts[247],
						texts[248], texts[249],
						texts[250], texts[251],
						texts[252], texts[253]);
				spell--;

				if (spell == -2) {
					group = -1;
					break;
				}

				spell += 76;
				gen_inc_spell(spell);
				break;
			}
		}
		GUI_gen.shift_x = 0;
	} while (group != -1);
}

void gen_choose_atpa()
{
	unsigned short skill, increase;

	GUI_gen.shift_x = -80;

	do {
		/* print menu with all melee weapons skills */
		skill = GUI_gen.radio((unsigned char*)texts[78], 7,
			texts[95], texts[96], texts[97], texts[98],
			texts[99], texts[100], texts[101]) - 1;

		if (skill != 0xfffe) {
			if (hero.skills[skill] > 0) {
				increase = GUI_gen.radio((unsigned char*)texts[254], 2,
					texts[75], texts[76]);
				if (increase != 0xffff) {
					if (increase == 1) {
						/* increase attack */
						if (hero.skills[skill] >= 0 &&
							hero.pa[skill] > hero.atpa) {
							/* inc AT */
							hero.at[skill]++;
							/* dec PA */
							hero.pa[skill]--;
							gen_refresh_screen();
						} else {
							GUI_gen.radio((Bit8u*)texts[255], 0);
						}
					} else {
						if (hero.skills[skill] >= 0 &&
							hero.at[skill] > hero.atpa) {
							/* dec AT */
							hero.at[skill]--;
							/* inc PA */
							hero.pa[skill]++;
							gen_refresh_screen();
						} else {
							GUI_gen.radio((Bit8u*)texts[256], 0);
						}
					}
				}
			} else {
				GUI_gen.radio((Bit8u*)texts[260], 0);
			}
		}

	} while (skill != 0xfffe);

	GUI_gen.shift_x = 0;
}
void gen_change_head()
{
	if (gen_page == 0) {
		sm_images_do_heads(head_current,272,8);
	} else if (gen_page > 4) {
		sm_images_do_heads(head_current,272,4);
	}
	screen_f = true;
}













