/*
        Rewrite of DSA1 v3.02_de functions of seg029 (playmask)
        Functions rewritten: 10/10
*/

#include <stdarg.h>
#include <stdio.h>
#include "sm401_main.h"
#include "sm401_seg027.h"
#include "sm401_seg029.h"
#include "sm401_video.h"


/* DS:0x5ecc - DS:0x5ed4 */
static signed char icon_array[9] = {
	-2, -2, -2,
	-2, -2, -2,
	-2, -2, -2
};

Cseg029::Cseg029(Cseg027* _seg027,struct_globvars* _globvars){
	seg027=_seg027;
	globvars=_globvars;
}

void Cseg029::draw_playmask()
{
	//ds_writew(0xc3cb, 0);

	/* load the desired playmask */
	//if (ds_readb(0xbc62) == 0)  //byte_30C22
	//	load_pp20(0);
	//else
	sm_set_palette(PAL_GRAY, 0xe0, 0x20);
		seg027->load_pp20(0xd6);

	//ds_writeb(0x2845, 0);

	//set_color(p_datseg + 0xb22d, 0);

	globvars->ani_x = 16;//ds_writew(0xce41, 16);
	globvars->ani_y = 2;//ds_writew(0xce3f, 2);

	//set_textcolor(0x1f, 0x1b);

	//ds_writew(0xd2d9, 196);
	//ds_writew(0xd2d7, 12);
	//ds_writew(0xd2d3, 103);
	//ds_writew(0xd2d5, 113);

	//ds_writew(0xd313, 205);

	//ds_writed(0x29e0, RealMake(datseg, 0x29e8));
	//ds_writed(0x29e4, 0);

	//ds_writeb(0xc3cb, 1);
}

/**
 * copy_forename - copys the forename from a name
 * @dst:	wheres the forename should be stored
 * @name:	the full name
 *
 * A forename has a maximum length of 7 characters.
 /
void copy_forename(Bit8u *dst, Bit8u *name) {

	unsigned short i;

	for (i = 0; i < 7; i++) {
		if (host_readb(name + i) == 0x20) {
			host_writeb(dst + i, 0);
			break;
		}
		host_writeb(dst + i, host_readb(name + i));
	}

	host_writeb(dst + 7, 0);
}

/**
 *	draw_status_line() - draws the status line (pictures and names)
 */
void Cseg029::draw_status_line()
{
	Bit8u *src, *dst;
	unsigned short i, j;
	unsigned short fg_bak, bg_bak;
	Bit16s head_bak;

	//ds_writew(0xc3cb, 0);

	//get_textcolor(&fg_bak, &bg_bak);

	for (i = 0; i < 7; i++) {
		/* Clear name field */
		//do_fill_rect(ds_readd(0xd2ff),
		//	ds_readw(0x2d01 + i * 2), 190,
		//	ds_readw(0x2d01 + i * 2) + 41, 197, 0);

		//if (host_readb(get_hero(i) + 0x21) != 0) {

			//copy_forename(Real2Host(ds_readd(0xd2f3)),
			//	get_hero(i) + 0x10);

			//set_textcolor(0xff, 0);

			/* Gray the names of heros in another group */
			//if (host_readb(get_hero(i) + 0x87) != ds_readb(CURRENT_GROUP))
			//	set_textcolor(0x6f, 0);

			/* print the name */
			//GUI_print_string(Real2Host(ds_readd(0xd2f3)),
			//	GUI_get_first_pos_centered(Real2Host(ds_readd(0xd2f3)),	ds_readw(0x2d01 + i * 2), 43, 0), 190);
		//}

		//wait_for_vsync();
		//update_mouse_cursor();

		//if (host_readb(get_hero(i) + 0x21) == 0) {
		//	clear_hero_icon(i);
		//} else {
		/*
			if (host_readb(get_hero(i) + 0x87) == ds_readb(CURRENT_GROUP)) {
				ds_writew(0xc011, ds_readw(0x2d01 + 2 * i));
				ds_writew(0xc013, 157);
				ds_writew(0xc015, ds_readw(0x2d01 + 2 * i) + 31);
				ds_writew(0xc017, 188);

				head_bak = -1;

				/* load skull if hero is dead /
				if (hero_dead(get_hero(i))) {
					head_bak = ds_readw(0x515c);
					load_in_head(0x29);
				}

				/* set the src pointer of the head /
				if (hero_dead(get_hero(i)))
					ds_writed(0xc019, ds_readd(0xd2f3));
				else
					ds_writed(0xc019, ds_readd(HEROS) + i * 0x6da + 0x2da);

				do_pic_copy(0);

				if (head_bak != -1)
					load_in_head(head_bak);
			} else {

				dst = Real2Host(ds_readd(0xd303));
				head_bak = -1;

				/* load skull if hero is dead /
				if (hero_dead(get_hero(i))) {
					head_bak = ds_readw(0x515c);
					load_in_head(0x29);
				}

				/* set the src pointer of the head /
				if (hero_dead(get_hero(i)))
					src = Real2Host(ds_readd(0xd2f3));
				else
					src = get_hero(i) + 0x2da;

				/* Gray out picture /
				for (j = 0; j < 1024; src++, dst++, j++)
					*dst = *src + 0x40;

				ds_writew(0xc011, ds_readw(0x2d01 + 2 * i));
				ds_writew(0xc013, 157);
				ds_writew(0xc015, ds_readw(0x2d01 + 2 * i) + 31);
				ds_writew(0xc017, 188);
				ds_writed(0xc019, ds_readd(0xd303));

				do_pic_copy(0);

				if (head_bak == -1)
					load_in_head(head_bak);
			}

			for (j = 0; j < 6; j++)
				ds_writew(0x2c18 + i * 8 + j * 2, 0xffff);
		}

		refresh_screen_size();*/
	}

	//set_textcolor(fg_bak, bg_bak);

	//ds_writew(0xc3cb, 1);
		
}

/**
 * clear_hero_icon - fills hero icon and bars with black color
 * @pos:	position of the hero
 /
void clear_hero_icon(unsigned short pos) {

	/* fill icon area black /
	do_fill_rect(ds_readd(0xd2ff), ds_readw(0x2d01 + pos * 2), 157,
		ds_readw(0x2d01 + pos * 2) + 31, 188, 0);

	/* return if the hero has a class /
	if (host_readb(get_hero(pos) + 0x21))
		return;

	/* fill bars area black /
	do_fill_rect(ds_readd(0xd2ff), ds_readw(0x2d01 + pos * 2) + 33, 157,
		ds_readw(0x2d01 + pos * 2) + 39, 188, 0);
}

/**
 * load_icon - loads an icon to the icoc_buffer
 * @fileindex:	index of the icon file (ICONS or BICONS)
 * @icon:	index if the icon in the file
 * @pos:	position of the icon (0-8)
 */
//static -> seg 027


/**
 *	draw_icons() - draws all icons
 */
void Cseg029::draw_icons()
{
	Bit16u i;
	//if (ds_readb(0x2845) != 0)
	//	return;
	for (i = 0; i < 9; i++) {
		sm_do_icon(i,icon_array[i]);
	}
}

/**
 *	draw_main_screen() - draws the main screen
 */
void Cseg029::draw_main_screen()
{
	//ds_writew(0xe111, 0xf1);
	//ds_writew(0xe10f, 0x1f);
	//ds_writew(0xe113, 0);

	//ds_writew(0x29ae, 0);//set_var_to_zero();

	//if (readb(0x2845))// byte_27805
		draw_playmask();

	//clear_loc_line();

	draw_status_line();

	draw_icons();

	draw_compass();

	//ds_writew(0xe10d, 1);
	//ds_writew(0xe113, 1);

	//set_textcolor(0x1f, 0x1b);
	sm_images_draw_all(true);
}
/*
void clear_loc_line() {
	update_mouse_cursor();
	do_fill_rect(ds_readd(0xd2ff), 3, 140, 316, 153, 0);
	refresh_screen_size();
}

/**
 * select_hero_icon - marks a heros icon in the playmask
 * @pos:	position of the hero
 *
 * This function is only used from sell- and repair screens.
 /
void select_hero_icon(unsigned short pos) {

	unsigned short fg_bak, bg_bak;

	/* paint a blue border for the pic and bars /
	do_border(Real2Phys(ds_readd(0xd2ff)),
		ds_readw(0x2d01 + pos * 2) - 1, 156,
		ds_readw(0x2d01 + pos * 2) + 42, 189, (signed char)0xfc);

	/* paint a blue border for the name /
	do_border(Real2Phys(ds_readd(0xd2ff)),
		ds_readw(0x2d01 + pos * 2) - 1, 189,
		ds_readw(0x2d01 + pos * 2) + 42, 198, (signed char)0xfc);

	/* save the textcolors /
	get_textcolor(&fg_bak, &bg_bak);

	/* copy the heros forename /
	copy_forename(Real2Host(ds_readd(0xd2f3)), get_hero(pos) + 0x10);

	/* set the textcolors /
	set_textcolor(0xfc, 0);

	/* print forename /
	GUI_print_string(Real2Host(ds_readd(0xd2f3)),
		GUI_get_first_pos_centered(Real2Host(ds_readd(0xd2f3)),
			ds_readw(0x2d01 + pos * 2), 43, 0), 190);


	/* restore textcolors /
	set_textcolor(fg_bak, bg_bak);
}

/**
 * deselect_hero_icon - deselect a heros icon in the playmask
 * @pos:	position of the hero
 *
 * This function is only used from sell- and repair screens.
 */
/* TODO: this function an select_hero_icon() can be merged into one. /
void deselect_hero_icon(unsigned short pos) {

	unsigned short fg_bak, bg_bak;

	/* paint a gray border for the pic and bars /
	do_border(Real2Phys(ds_readd(0xd2ff)),
		ds_readw(0x2d01 + pos * 2) - 1, 156,
		ds_readw(0x2d01 + pos * 2) + 42, 189, (signed char)0xe6);

	/* paint a gray border for the name /
	do_border(Real2Phys(ds_readd(0xd2ff)),
		ds_readw(0x2d01 + pos * 2) - 1, 189,
		ds_readw(0x2d01 + pos * 2) + 42, 198, (signed char)0xe6);

	/* save the textcolors /
	get_textcolor(&fg_bak, &bg_bak);

	/* copy the heros forename /
	copy_forename(Real2Host(ds_readd(0xd2f3)), get_hero(pos) + 0x10);

	/* set the textcolors /
	set_textcolor(0xff, 0);

	/* print forename /
	GUI_print_string(Real2Host(ds_readd(0xd2f3)),
		GUI_get_first_pos_centered(Real2Host(ds_readd(0xd2f3)),
			ds_readw(0x2d01 + pos * 2), 43, 0), 190);


	/* restore textcolors /
	set_textcolor(fg_bak, bg_bak);
}
*/


//void sub_195DD(signed char icons, ...){
void Cseg029::set_icons(signed char icons, ...){
	unsigned short i;
	va_list arguments;
	
	va_start(arguments, icons);
	for (i=0;i<icons;i++){
		icon_array[i] = va_arg(arguments, int);
	}
	while(i<9){
		icon_array[i]= -1;
		i++;
	}
	//if (byte_27805==0) //2845
		draw_icons();
}

//from seg 002
void Cseg029::draw_compass() {

	/* No compass in a location */
	if (globvars->location)
		return;
	/* Has something to do with traveling */
	//if (ds_readb(0xb132))
	//	return;
	/* Not in town or dungeon */
	if (!globvars->dungeon_index && !globvars->current_town)
		return;
	/* I have no clue */
	//if (ds_readb(0x4475) == 2)
	//	return;

	sm_images_do_compass(globvars->direction);
}



