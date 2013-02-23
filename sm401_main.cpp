#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdio.h>


#include "sm401_main.h"
#include "sm401_seg002.h"
#include "sm401_seg009.h"
#include "sm401_GUI.h"
#include "sm401_video.h"
#include "sm401_vars.h"
#include "gen201.h"

Bit8u* buffer;
//Bit8u* tmp_buffer;
Bit8u* gataosao;
//Bit8u* buffer_font6;

Bit8u* buffer_text_ltx;
Bit8u** string_array_text_ltx;

Bit8u* buffer_monnames;
Bit8u** string_array_monnames;

Bit8u* buffer_itemnames;
Bit8u** string_array_itemnames;

Bit8u* buffer_popup_dat;
Bit8u* buffer_compass;

Bit16u GameMode; //word_20FC3
GUI_class GUI_var;


Bit16u sm_init_memory(){

	buffer_text_ltx = (Bit8u*)malloc(0xFA00);//0x6de3
	string_array_text_ltx = (Bit8u**) calloc (0x345,sizeof(Bit8u*));//0x1cb
	buffer_itemnames = (Bit8u*)malloc(0x1388);//0x1388
	string_array_itemnames = (Bit8u**) calloc (0x35a,sizeof(Bit8u*));//0x158
	//buffer_monnames = (char*)malloc(0x3b6);
	buffer_monnames = (Bit8u*)malloc(0x3a1);
	//string_array_monnames = (char*)malloc(0x134);
	string_array_monnames = (Bit8u**) calloc (0x6d,sizeof(Bit8u*));
	
	buffer = (Bit8u*)malloc(0xFDE8);
	gataosao = (Bit8u*)malloc(0xFDE8);
	//tmp_buffer = (Bit8u*)malloc(0xFDE8);
	
	buffer_popup_dat = (Bit8u*)malloc(0x689);
	buffer_compass = (Bit8u*)malloc(0x898);
	
	return 1;//Prüfung auf Erfolg von Allokierung?	
}

Bit16u sm_load_common_text(){
	struct sm_file smf;
	Bit32u len;
	
	load_archive_file(0x0E,&smf);
	len = read_archive_file( &smf, buffer_text_ltx, 0xFA00);//0x6de3
	fclose(smf.fd);
	split_textbuffer(string_array_text_ltx, buffer_text_ltx, len);
	
	load_archive_file(0x0D,&smf);
	len = read_archive_file( &smf, buffer_itemnames, 0x1388);//0x1388
	fclose(smf.fd);
	split_textbuffer(string_array_itemnames, buffer_itemnames, len);
	
	load_archive_file(0xD1,&smf);
	len = read_archive_file( &smf, buffer_monnames, 0x1388);////0x3a1
	fclose(smf.fd);
	split_textbuffer(string_array_monnames, buffer_monnames, len);
	
	return 1;//Prüfung auf Erfolg?	
}

void array_add(Bit8u* dst, unsigned short len, unsigned char op, unsigned short flag) {
	unsigned short i;
	Bit8u tmp;

	for (i = 0; i < len; i++) {
		tmp = dst[i];

		if (flag == 2 && !tmp)
			continue;

		dst[i]= tmp + op;
	}
}

void load_objects_nvf(void)
{
	struct nvf_desc nvf;
	struct sm_file smf;

	load_archive_file(0x07,&smf);//OBJECTS.NVF
	read_archive_file( &smf, buffer, 2000);//buffer=0xd303
	fclose(smf.fd);

	nvf.src = buffer;
	nvf.type = 0;
	nvf.width = 0;
	nvf.height = 0;
	nvf.dst = gataosao;//=0xd2e3
	nvf.nr = 12;
	process_nvf(&nvf);

	nvf.dst = gataosao + 0x683;
	nvf.nr = 13;
	process_nvf(&nvf);

	nvf.dst = gataosao + 0xcaf;
	nvf.nr = 14;
	process_nvf(&nvf);

	nvf.dst = gataosao + 0xcef;
	nvf.nr = 15;
	process_nvf(&nvf);

	array_add(gataosao, 0xd3f, 0xe0, 2);
	
	//sm_init_objects_grafik();
}

void init_common_buffers()
{

	struct sm_file smf;
	unsigned short bytes;

	/* load POPUP.DAT */
	load_archive_file(0x99,&smf);
	//bytes = read_archive_file(&smf, Real2Host(ds_readd(0xd2ad) - 8), 500);buffer_popup_dat
	bytes = read_archive_file(&smf, buffer, 500);
	fclose(smf.fd);

	/* decompress POPUP.DAT */
	decomp_pp20(buffer,buffer_popup_dat,buffer+4,bytes);
	sm_images_init_popup(buffer_popup_dat);

	/* load COMPASS */
	load_archive_file(0x12,&smf);
	//bytes = read_archive_file(&smf, Real2Host(ds_readd(0xd2b1)), 5000);
	bytes = read_archive_file(&smf, buffer_compass, 5000);
	fclose(smf.fd);

	/* load ITEMS.DAT */
	//fd = load_archive_file(0xdc);
	//bytes = read_archive_file(fd, Real2Host(ds_readd(0xe22b)), 3060);
	//bc_close(fd);

	/* load ANIS.TAB */
	//fd = load_archive_file(0x17);
	//read_archive_file(fd, Real2Host(RealMake(datseg, 0xd205)), 148);
	//bc_close(fd);

	/* load MFIGS.TAB */
	//fd = load_archive_file(0x19);
	//read_archive_file(fd, Real2Host(RealMake(datseg, 0xd159)), 172);
	//bc_close(fd);

	/* load WFIGS.TAB */
	//fd = load_archive_file(0xeb);
	//read_archive_file(fd, Real2Host(RealMake(datseg, 0xd0ad)), 172);
	//bc_close(fd);

	/* load MONSTER.TAB */
	//fd = load_archive_file(0x11);
	//read_archive_file(fd, Real2Host(RealMake(datseg, 0xd01d)), 144);
	//bc_close(fd);

	/* load GAMES.NAM */
	//fd = load_regular_file(0xcf);
	//bc__read(fd, Real2Host(RealMake(datseg, 0xe2da)), 45);
	//bc_close(fd);

	/* load TOWNPAL.DAT */
	//fd = load_archive_file(0x8d);
	//read_archive_file(fd, Real2Host(ds_readd(0xd321)), 288);
	//bc_close(fd);

}


void refresh_colors(void)
{
	//set_color(p_datseg + 0xb22d, 0);
	sm_set_palette((Bit8u*)&col_bl,0,1);
	//set_color(p_datseg + 0xb22a, 0xff);
	sm_set_palette((Bit8u*)&col_wh,255,1);
	//set_palette(p_datseg + 0xb230, 0xd8, 8);
	sm_set_palette((Bit8u*)&pal_301F0,0xd8, 8);
	//set_palette(p_datseg + 0xb248, 0xc8, 3);
	sm_set_palette((Bit8u*)&pal_30208,0xc8, 3);
	//set_palette(p_datseg + 0xb251, 0x40, 0x20);
	sm_set_palette((Bit8u*)&pal_ggsts,0x40, 0x20);
	//set_palette(p_datseg + 0xb2b1, 0x20, 0x20);
	sm_set_palette((Bit8u*)&pal_30271,0x20, 0x20);
	//set_palette(p_datseg + 0x2723, 0x60, 0x20);
	sm_set_palette((Bit8u*)&pal_276E3,0x60, 0x20);

}
void set_to_ff() {
	unsigned i;

	//for (i = 0; i < 9; i++)
	//	ds_writeb(0xbd38 + i, 0xff);
}

void init_game_state(void)
{
	signed short i;

	refresh_colors();
/*
	ds_writew(0xd2cb, 0xc8);
	ds_writew(0xd2cd, 0xc9);
	ds_writew(0xd2cf, 0xca);
*/
	//set_to_ff();
/*
	for (i = 0; i < 0xfe; i++) {
		ds_writeb(0xe12d + i, 0);
	}

	ds_writeb(0x2d9f, 0);
	/* Travia Temple in Thorwal *//*
	ds_writeb(LOCATION, 2);
	ds_writeb(TYPEINDEX, 1);
	ds_writew(0x2d83, 9);
	ds_writew(0x2d85, 9);
	ds_writew(X_TARGET, 9);
	ds_writew(Y_TARGET, 8);
	ds_writeb(0x2d7c, 0);
	ds_writeb(DIRECTION, 0);
	ds_writeb(DUNGEON_INDEX, 0);

	/* var = current_town = 1; *//*
	ds_writeb(CURRENT_TOWN, 1);
	ds_writeb(0x2da6, 1);
*/
	GUI_var.set_menue_tiles(3);//ds_writew(0xbffd, 3);

	/* timer *//*
	ds_writed(DAY_TIMER, 24 * 0x1518 - 1);
	timewarp_until(1);
	ds_writed(DAY_TIMER,  8 * 0x1518);
	ds_writeb(DAY_OF_WEEK, 4);
	ds_writeb(DAY_OF_MONTH, 17);
	ds_writeb(MONTH, 1);
	ds_writeb(YEAR, 15);

	ds_writed(0xc00d, ds_readd(0xd2ff));
/*/
	load_objects_nvf();
	//passages_init();   //Schiffsrouten
/*
	ds_writew(CURRENT_ANI, -1);
	ds_writew(0xe113, 1);

	ds_writed(0xbff9, ds_readd(0xd303));*/
	//load_splashes();
}








/*

short word_15055;//0095
int word_15085_15083;
short word_15083;

short word_1795A;//299a
char byte_20C22; //bc62
short word_20CE5;//bd25

short word_20FC3;//c003
short dword_21375; //c3b5
short word_21387;//c3c7

short word_222C9;//d309

short ax,bx,cx,dx,si,di;
*/
main(int argc, char *argv[]){
	//short var_2,var_4,var_6,var_8;
	time_t t;
	Bit16u Schick_or_Gen;

	//word_20CE5=1;//word_30CE5
	//byte_20C22=1;//byte_30C22

	//call    sub_53C0 (3E80h,0)  //seg002:01E0  //init_ail
	time(&t);              //call    sub_1123 (0,0)
	srand((unsigned int)t);//call    sub_BAC (ax)
	//call    sub_F1C0(d30b,ds)    //D1_GFX("SaveDisplayStat(dstat=0x%x:0x%x);\n",
	//d30b: page
	//d30d: mode

	//j_schick_init_memory(); //call    sub_14F20()		   //sub_80013    //seg120:20   4x schick.dat
	sm_init_memory();
	sm_load_common_text();
	//if (schick_init_memory()){
		//loc_AA40:
		//short word_222C9=1;//word_322C9
		// call sub_C430         //schick_set_video()
	sm_init_video();
		//short word_21387=2;//mouse_enabled
		//push    cs
		//call sub_683E          //seg002:165e  //mouse sub_1683E
		//if (word_21387==0)  word_1795A= 0xFFF6; //mouse_enabled
		//loc_AA62
	init_game_state();//call    sub_14F25      //sub_80381    2x schick.dat    init_game_state
	//call    sub_B2A5		//seg004:05    SAVE_AND_SET_TIMER
	init_common_buffers();//call    sub_12EEF		//sub_28102       7x schick.dat, 1x games.dat, 1x schick.dat  
	//word_20FBD=3;//word_30FBD
	//push    cs
	//call    near ptr sub_6B0B   //refresh_screen_size
	if (argc==2){//ohne Argumente -> 1
		//var_8 =  strlen(argv[2]);    //sub_42AD(argv[2][0],argv[3][0])??
		//si=0;
		//word_15085_15083=1;
		//while (si<var_8){
			//les     bx, [bp+arg_2]  ; Load Full Pointer to ES:xx
			//les     bx, es:[bx+4]   ; Load Full Pointer to ES:xx
			// mov     al, es:[bx]
			//word_15085_15083 = word_15085_15083 * es:[bx]
			//les     bx, [bp+arg_2]  ; Load Full Pointer to ES:xx
			//es:[bx+4] ++;
		//	si++;
		//}
	}
	//loc_AAD7
	//call    sub_14F2A  //sub_80479  //Chars -> Temp    sub_24F2A
	//call    near ptr sub_597F    sub_1597F
    //if (ax !=0){ //ax ist 1
		//call    sub_52C6 //Musik? //screen update?    ReadSoundCfg
		//call    sub_5A70(0x4e20,0) alloc_voc_buffer
	//}else{	
		//push    cs
		//call    near ptr sub_53E9 EXIT_AIL
		//push    ds
		//push    offset aMusikAbgeschal ; "MUSIK ABGESCHALTET - NICHT GENUG SPEICH"...
		//call    sub_14459      GUI_output
	//}
	//call    sub_5186  //init CD drive   CD_init
	//if (word_15055==0){//ist 1    cd_initialized
		//call    sub_14F2F     sub_24F2F 
		//push    0
		//call    sub_6D0    exit
		//pop     cx
	///}
fprintf(stdout,"GUI_Radio GameMode\n");
GameMode= 0xFFFF;    //  word_20FC3
	while(GameMode==0xFFFF){
		GameMode =  GUI_var.radio(string_array_text_ltx[5], 2, string_array_text_ltx[6],string_array_text_ltx[7]);
		//sub_14463(es:dword_21375+0x14,2,es:dword_21375+0x1C) //gui-radio: Anfänger Fortgeschrittene   GUI_Radio
	}
	// Kopierschutz //call    near ptr sub_AC61  //if (ax==0) jmp     loc_AC2F     COPY_PROTECTION
	//word_20FBD= 3;//word_30FBD
	//call    sub_14F43  sub_24F43 
	//var_2= dx;//0f9c
	//var_4= ax;//6200
	//if (var_2>0)loc_ABC6
	//if (var_2<0)loc_AB87
	//if (var_2==0 && var_4>=0)loc_ABC6
	//if ((var_2<0) || (var_2==0 && var_4<0)){//loc_AB87
		//mov     ax, [bp+var_2]
		//mov     dx, [bp+var_4]
		//neg     ax              ; Two's Complement Negation
		//neg     dx              ; Two's Complement Negation
		//sbb     ax, 0           ; Integer Subtraction with Borrow
		//push    ax
		//push    dx
		//call    sub_41D2(dword_222B3,dword_222B3+2,es:[dword_21375+0C9Ch],es:[dword_21375+0C9Eh])    sprintf
		//call    sub_14459(dword_222B3,dword_222B3+2)  GUI_output
	//}else{//loc_ABC6
fprintf(stdout,"GUI_Radio Schick_or_Gen\n");
	Schick_or_Gen = 0xFFFF; 
	while(Schick_or_Gen==0xFFFF){
		Schick_or_Gen =  GUI_var.radio(string_array_text_ltx[820], 2, string_array_text_ltx[821],string_array_text_ltx[822]);
	}
	if (Schick_or_Gen == 2) do_call_gen(GameMode); //sub_14F52 //Generierung  do_call_gen_exe
		//push    cs
		//call    near ptr sub_91F6 //wait_for_keyboard2();  wait_for_keyboard2

		//do{
			//var_6 = sub_12E7F
			//gui_radio Spiel laden
		//}while (var_6==0xFFFF);
		//word_20CE5= 0; // word_30CE5
		//push    cs
		//call    sub_70C7 //main loop?   MainLoop
	//}
//loc_AC28:     loc_AC2F:       
//call    sub_14F2F       //seg120:2F  sub_24F2F
//}else{
//call    near ptr sub_53E9 ; Call Procedure  schick_reset_videoEXIT_AIL
//call    sub_C461        ; Call Procedure  schick_reset_video
	sm_exit_video();
//call    sub_1792        ; Call Procedure  sub_11792
//}



}
