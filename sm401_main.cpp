#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdio.h>


#include "sm401_main.h"
#include "sm401_seg002.h"
#include "sm401_seg007.h"
#include "sm401_seg009.h"
#include "sm401_seg026.h"
#include "sm401_seg027.h"
#include "sm401_seg029.h"

#include "sm401_video.h"
#include "sm401_audio.h"
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

char buffer_games_nam[45];
char dialogTextPtr02[256];

struct_globvars* globvars;

unsigned short wantExitGame;
GUI_class GUI_var;
Cseg027 *seg027;
Cseg029 *seg029;

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

void load_objects_nvf(void){
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




void refresh_colors(void){
	sm_set_palette(PAL_BL,0,1);
	sm_set_palette(PAL_WH,255,1);
	sm_set_palette(PAL_301F0,0xd8, 8);//popup
	sm_set_palette(PAL_30208,0xc8, 3);
	sm_set_palette(PAL_GGSTS,0x40, 0x20);
	sm_set_palette(PAL_30271,0x20, 0x20);//heads
	sm_set_palette(PAL_276E3,0x60, 0x20);
}
void set_to_ff() {
	unsigned i;

	//for (i = 0; i < 9; i++)
	//	ds_writeb(0xbd38 + i, 0xff);
}

void init_game_state(void){
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
	/* Travia Temple in Thorwal */
	globvars->location = 2;			//ds_writeb(LOCATION, 2);
	globvars->typeindex = 1;			//ds_writeb(TYPEINDEX, 1);
	/*
	ds_writew(0x2d83, 9);
	ds_writew(0x2d85, 9);
	ds_writew(X_TARGET, 9);
	ds_writew(Y_TARGET, 8);
	ds_writeb(0x2d7c, 0);*/
	globvars->direction=0;//DIRECTION, 0);
	globvars->dungeon_index=0;//(DUNGEON_INDEX, 0);

	/* var = current_town = 1; Thorwal*/
	globvars->current_town = 1; //ds_writeb(CURRENT_TOWN, 1);
	//ds_writeb(0x2da6, 1);

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

	globvars->current_ani=0xffff;//CURRENT_ANI, -1);
	//ds_writew(0xe113, 1);

	//ds_writed(0xbff9, ds_readd(0xd303));*/
	//load_splashes();
}


void sm_init_vocs(){
	struct sm_file smf;
	unsigned short bytes;

	/* load FX1.VOC */
	load_archive_file(288+1,&smf);
	bytes = read_archive_file(&smf, buffer, 20000);
	fclose(smf.fd);
	sm_sounds_init(0,buffer);

	/* load FX3.VOC */
	load_archive_file(288+3,&smf);
	bytes = read_archive_file(&smf, buffer, 20000);
	fclose(smf.fd);
	sm_sounds_init(2,buffer);
}


unsigned short LoadSaveGame(){
	sm_file smf;
	char sm_fname[25];  //tmp_buffer
	unsigned short var_12;
	char var_4c[30];
	char var_2e[14];
	char buf[5];
	char version[3];
printf("%s\n",buffer_games_nam+9);
	signed short offs = //var_4 = 
		GUI_var.radio(string_array_text_ltx[0], 6,
		buffer_games_nam+0x00,
		buffer_games_nam+0x09,
		buffer_games_nam+0x12,
		buffer_games_nam+0x1b,
		buffer_games_nam+0x24, 
		string_array_text_ltx[737]) -1;
	if (offs==5 || offs==0xffff){//loc_36608   -> loc_36192  ax=var_16 -> loc_3660B -> ret
		return 0;
	}
	
/*
	offs *= 9;
	strcpy(sm_fname,buffer_games_nam[offs]);
	strcat(sm_fname,".gam");
	smf->fd = fopen(sm_fname,"rb");
	if (!smf->fd){
		GUI_output(string_array_text_ltx[635]);
		return 0xffff;
	}
//47D                 mov     ax, word_3138B
//480                 mov     [bp+var_6], ax
//483                 mov     word_3138B, 0
//489                 mov     ax, word_2796E
//48C                 mov     [bp+var_18], ax
//48F                 mov     word_2796E, 0
//???               ordner Löschen, Herstellen, Füllen
	sprintf(sm_fname,"TEMP\\%s","*.*");
	var_12 = sub_13040();//(sm_fname,var_4C,ss,0)//char var_4c [20]
	while (var_12=0){
		sprintf(sm_fname,"TEMP\\%s",var_2e);//,ss);
		sub_111A7(sm_fname);
		var_12=sub_13073(var_4C);
	}
	dword_33292 =dialogTextPtr02;
	memset(dword_33292,0,0x0478);
//???	
	
	
	fread(sm_fname,1,0x0c,smf->fd);     //Lese "DSA VERSION\0"           	//bc read handle buf count
	fread(buf,1,1,smf->fd); 
	fread(buf+1,1,1,smf->fd); 
	fread(version,1,1,smf->fd); 
	fread(version+1,1,1,smf->fd); 
	fread(gameDone,1,4,smf->fd); 
	var_A=gameDone;
// tagebuch
//5A4                 mov     word ptr [bp+var_E], (offset src+0B0h)
//5A9                 mov     dx, word ptr [bp+var_E+2]
//5AC                 mov     ax, word ptr [bp+var_E]
//5AF                 mov     cx, word ptr [bp+var_A+2]
//5B2                 mov     bx, word ptr [bp+var_A]
//5B5                 var_10 = call    sub_10A10
	fread(gameDone,1,var_10,smf->fd); 
//5CD                 mov     byte_29578, 1
	fread(dword_33292,1,0x0478,smf->fd); 
	di=0;
	while (di<0x11e){
		ax=  &(dword_33292+di*4)  ||  &(dword_33292+di*4 + 2);
		if (ax!=0){
			sprintf(sm_fname,"TEMP\\%s",filenames[di*4]);
			file2=fopen(sm_fname,"wb");
			fread(buffer,1,&(dword_33292+di*4),file2);
			fwrite(buffer,1,&(dword_33292+di*4),file2);
			fclose(file2);
		}
		di++;
	
	}
	
	
	di=0;//helden laden
	while (di<6) {
		memset(ptr_heros,0,0x06da);
		di++;
		ptr_heros ++=0x06da;
	}
	//loc_363E1:
	
	do{//6CB loc_363EB:
		var_14 = fread(buffer,1,0x06da,smf->fd); 
	
		if (var_14!=0){
		
			prepare_chr_name(&var_60,buffer);
			sprintf(sm_fname,"TEMP\\%s",var_60);
			file2=fopen(sm_fname,"wb");
			fwrite(buffer,1,0x06da,file2);
			fclose(file2);
	
			if (buffer[0x8a]!=0){
				prepare_chr_name(&var_60,buffer);
				Read_CHR_TMP(&var_60,buffer[0x8a]-1,buffer[0x8a]);
			}
		}
	}while (var_14!=0);
	fclose(sm_fname);
	sub_13040(var_4c,"*.CHR",0);
	
	while(var_12==0){
		sprintf(sm_fname,"TEMP\\%s",var_2e);
		file1=fopen(sm_fname,"rb");//0x8004
		if (file==0xffff){
			file2=fopen(var_2e,"rb");//0x8004
			fread(buffer,1,0x06da,file2);
			fclose(file2);
			file1=fopen(sm_fname,"wb");
			fwrite(buffer,1,0x06da,file1);
		}
		fclose(file1);
		sub_13073(var_4c);
	}
	
	di=0xe2;
	while (di<0xe7){
		load_npc(di);
		if(ptr_heros+0x29a6!=7){
			memset(heros+291,0,6da);
		}else{
			continue;
		}
		di++;
	}
	loc_365BD
//89D                 mov     word_27C8B, 0FFFFh
//8A3                 mov     ax, 1
//8A6                 mov     [bp+var_16], ax
//8A9
//8A9 loc_365C9:
//8A9                 mov     word_27806, ax
//8AC                 mov     word_27679, 0
//8B2                 mov     word_2767B, 0
//8B8                 mov     byte_29435, 3
	if (location!=2){
//8C4                 mov     al, 0FFh
//8C6                 mov     byte_30CF8+8, al
//8C9                 mov     byte_30CF8+7, al
//8CC                 mov     byte_30CF8+6, al
	}
	load_area_description(2);
//8D7                 mov     ax, [bp+var_6]
//8DA                 mov     word_3138B, ax
//8DD                 mov     ax, [bp+var_18]
//8E0                 mov     word_2796E, ax
//8E3                 call    refresh_screen_size
	return 1;*/
	return 0;
}

void do_Location(){
	unsigned short var_8=globvars->traveling;
	//var_6=word_30FBD;
	//var_2=word_27C62;
	//var_4=word_27C64;
	//word_27C62=0;
	//word_27C64=0;
	globvars->traveling=0;
	//word_30FBD=3
	//p_current_cursor=ds:byte_27808
	switch(globvars->location){
		case 1: 
			//do_Automap();
			break;
		case 2: 
			do_Temple();
			break;/*//
		case 3: 
			do_Tavern();
			break;
		case 4: 
			do_Healer();
			break;
		case 5: 
			do_Merchant();
			break;
		case 6: 
			do_Camp();
			break;
		case 7: 
			do_Inn();
			break;
		case 8: 
			do_Smith();
			break;
		case 9: 
			do_Market();
			break;
		case 10: 
			do_Citizen();
			break;
		case 11: 
			do_Harbour();
			break;
		case 12: 
			enter_map();
			break;
		case 13: 
			do_Informer();
			break;
		case 14: 
			do_Entrance();
			break;
		case 15: 
			do_Nothing();
			break;
		case 16: 
			do_House();
			break;
		case 17: 
			do_Dialog();
			break;
		case 18: 
			do_Citycamp();
			break;*/
		default:
			break;
	};
	//word_30FBD=var_6;
	//word_27C62=var_2;
	//word_27C64=var_4;
	if (globvars->traveling==0)
		globvars->traveling=var_8;
	//byte_27C68 = 0xFF;
}



void MainLoop(){
	while (wantExitGame==0){
		if (globvars->location!=0)
			do_Location();/*
		else if (current_town!=0)
			do_Town();
		else if (dungeon_index!=0)
			do_Dungeon();
		else if (traveling!=0)
			do_Travel_Mode();
		//loc_17104
		if (gameDone==0x63)
			wantExitGame=5;
		if (word_27679!=0)
			sub_24EF0();		
		if (word_2767B!=0)
			sub_24F80();

		if (check_party!=0){
			check_party = 0;
			ax = count_heros_available();
			if (ax!=0){
				ax = count_heros_available();
				if (ax>1 || check_hero(ptr_heros+0x291C)!=0?){
					//loc_17167
					ax = count_heroes_available_in_group();
					if (ax=0){
						sub_2356E(2);
					}else{
						ax = count_heroes_available_in_group();
						if (ax==1){
							ax = is_hero_available_in_group(ptr_heros+0x291C);
							if (ax!=0){
								sub_2356E(2);
							}
						}
					}
				}else{
					wantExitGame=1;
				}
			}else{
				wantExitGame=1;
			}
		}

		if (es:byte_278FD[heros]!=0)
			if (current_town!=0 || wantExitGame==0x63){ 
				if (npc_months>=1){
					if (byte_29454!=npc_months){
						npc_farewell();
						byte_29454=npc_months;
					}
				}
			}
		}
		
		if (in_fight==0){
			if (wantExitGame==0 || wantExitGame==0x63){
				if (location==0){
					sub_1891A();
				}
			}
		}
		
		if (byte_2969F!=0){
			byte_2969F=0;
			if (byte_27805==0)
				draw_status_line();
		}
		
		if (wantExitGame!=0 &&  byte_29435!=0)
			refresh_colors();
		
		if (wantExitGame==1)
			show_GameOver();

		if (wantExitGame==4)
			TimesUp();
		
		if (wantExitGame==1 || wantExitGame==2 || wantExitGame==4 || wantExitGame==5 || wantExitGame==7){
			location = 0;
			ax=0xffff;
			while (ax==0xffff){
				LoadSaveGame();
			}
			if (ax!=0){
				wantExitGame=0;
				refresh_colors();
			}
		}
		
		if (wantExitGame==0x63){
			playOutro();
			sub_24F2F();
			//exit(0);
		}*/
	}//while(wantExitGame==0)
};


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

	globvars = new(struct_globvars);
	//word_20CE5=1;//word_30CE5
	//byte_20C22=1;//byte_30C22
	sm_init_audio(1);		//call    sub_53C0 (3E80h,0)  //seg002:01E0  //init_ail
	time(&t);              	//call    sub_1123 (0,0)
	srand((unsigned int)t);	//call    sub_BAC (ax)

	sm_init_memory();		//j_schick_init_memory(); //call    sub_14F20()		   //sub_80013    //seg120:20   4x schick.dat
	sm_load_common_text();
	//short word_222C9=1;//word_322C9
	sm_init_video();
							//short word_21387=2;//mouse_enabled
	init_game_state();//geht das später?		//call    sub_14F25      //sub_80381    2x schick.dat    init_game_state
	//call    sub_B2A5		//seg004:05    SAVE_AND_SET_TIMER
	seg027 = new Cseg027(buffer,gataosao,buffer_games_nam,globvars);//call    sub_12EEF		//sub_28102       7x schick.dat, 1x games.dat, 1x schick.dat  
	seg029 = new Cseg029(seg027,globvars);

	seg027->init_common_buffers();//call    sub_12EEF		//sub_28102       7x schick.dat, 1x games.dat, 1x schick.dat
	seg027->load_icons();
	//word_20FBD=3;//word_30FBD
	//if (argc==2){//ohne Argumente -> 1
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
	//}
	//loc_AAD7
	//call    sub_14F2A  //sub_80479  //Chars -> Temp    sub_24F2A
	//call    near ptr sub_597F    sub_1597F
	sm_init_vocs();
							//push    offset aMusikAbgeschal ; "MUSIK ABGESCHALTET - NICHT GENUG SPEICH"...
							//call    sub_14459      GUI_output
							//call    sub_5186  //init CD drive   CD_init
							//if (word_15055==0){//ist 1    cd_initialized
								//call    sub_6D0    exit
	printf("GUI_Radio GameMode\n");
	globvars->GameMode= 0xFFFF;    //  word_20FC3
	while(globvars->GameMode==0xFFFF){
		globvars->GameMode =  GUI_var.radio(string_array_text_ltx[5], 2, string_array_text_ltx[6],string_array_text_ltx[7]);
							//sub_14463   GUI_Radio
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
	if (Schick_or_Gen == 2) do_call_gen(globvars->GameMode); //sub_14F52 //Generierung  do_call_gen_exe
	//call    near ptr sub_91F6 //wait_for_keyboard2();  wait_for_keyboard2
	//bildschirm löschen?
	refresh_colors();
	LoadSaveGame();		//var_6 = sub_12E7F    seg026:03B5	//gui_radio Spiel laden	//}while (var_6==0xFFFF);
	//word_20CE5= 0; // word_30CE5
	MainLoop();  			//call    sub_70C7 //main loop?   MainLoop
	//call    sub_14F2F       //seg120:2F  sub_24F2F
	//}else{
	printf("exit audio\n");
	sm_exit_audio();
	printf("exit video\n");
	sm_exit_video();
	//call    sub_1792        ; Call Procedure  sub_11792	
}



void do_Temple(){
	unsigned short var_3=0;
	unsigned short action;
	unsigned short word_333B8;
	unsigned short si;
	struct struct_sm_mouse sm_mouse;

	unsigned short ax;
	//word_27978 = 0;
	//word_27976 = 0;
	unsigned short need_refresh_screen =1; //word_27806 = 1;//need_refresh_screen
	
	seg029->set_icons(9,0,1,2,3,4,5,6,7,8);//sub_195DD(9,0,1,2,3,4,5,6,7,8);
	
	while(var_3==0){
		if (need_refresh_screen!=0){
			word_333B8 = 1;
			for(si = 1;si<15;si++){//16?
				if (!is_in_byte_array(globvars->typeindex,temple_types+si)){ //dword_2BDF6+4*si,typeindex){
					word_333B8 = si;
					break;
				}
			}
			seg029->draw_main_screen();
			seg027->load_ani(3);
			seg027->load_tempicon(word_333B8-1);
			seg027->init_ani(0);
			/*
			//sub_151F7(0x90);  sound -> cd-track
			y1=0;
			x1=0;
			x2=0x28;//40
			y2=0x16;//22
			gfx_dst=gfx_start + 0x6E63;
			gfx_src=dword_31369 + 0x1B58;
			update_mouse_cursor();
			do_pic_copy(0);
			refresh_screen_size;
			gfx_dst=gfx_start;
			/*/
			if (globvars->typeindex!=0x3A){
				sprintf(dialogTextPtr02,(char*)string_array_text_ltx[0x3AC/4],(char*)string_array_text_ltx[word_333B8+0x15],
					(char*)string_array_text_ltx[globvars->current_town+0xEB]);
			}else{
				sprintf(dialogTextPtr02,(char*)string_array_text_ltx[0x3AC/4],(char*)string_array_text_ltx[word_333B8+0x15],
					(char*)string_array_text_ltx[0x9B8/4]);
			}
			GUI_print_loc_line(dialogTextPtr02);
			need_refresh_screen = 0;
		}
		//sub_16C14();//handle input?
		action = sm_KeyPressed();
		sm_GetMouse(&sm_mouse);//umwandeln maus in tastatur?
		wait_for_tick();

		if (sm_mouse.event==MOUSE_RIGHT_DOWN || action == KEY_PGUP){// action==0x49){
			ax=GUI_var.radio(string_array_text_ltx[0x384/4],9,
					string_array_text_ltx[0x388/4],
					string_array_text_ltx[0x38C/4],
					string_array_text_ltx[0x494/4],
					string_array_text_ltx[0x390/4],
					string_array_text_ltx[0x394/4],
					string_array_text_ltx[0x398/4],
					string_array_text_ltx[0x9B0/4],
					string_array_text_ltx[0x4A0/4],
					string_array_text_ltx[0x39C/4]);
			ax -=1;
			if (ax!=0xffff){
				action = ax + 0x81;
			}
		}
		/*
		switch(action==0x89){
		case 0x89:
			if (current_group+0x2D36==0){
				GUI_output(string_array_text_ltx[0x3A2];
			}else{
				var_3=1;
			}
			break;
		case 0x81:
			heldAufnehmen(typeindex);
			draw_status_line();
			break;
		}
		case 0x82:
			heldEntlassen(typeindex);
			draw_status_line();
			break;
		case 0x83:
			heldLoeschen();
			break;
		case 0x84:
			if (typeindex!=0x3A){
				var_E=LoadSaveGame();
				if (var_E!=0xffff){
					sprintf(dialogTextPtr02,string_array_text_ltx[0x3AC]
						,string_array_text_ltx[(word_333B8+0x15)*4],string_array_text_ltx[(current_town+0xeb)*4]

					GUI_print_loc_line(dialogTextPtr02);
					draw_status_line();
					if(location!=2){
						var_3=1;
					}
				}
			}else{
				GUI_output(string_array_text_ltx[0xCC4]);
			}
			break;
		case 0x85:
			if (typeindex!=0x3A){
				ax=current_group+0x2D36;
				if (ax==0){
					GUI_output(string_array_text_ltx[0x3a0]);
				}else{
					SaveSaveGame();
				}
			}esle{
				GUI_output(aInDiesemTempel);
			}
			break;
		case 0x86:
			ax = GUI_bool(string_array_text_ltx[0x4AC]);
			if (ax!=0){
				wantExitGame = 3;
			}
			break;		
		case 0x87:
			if (byte_27CF6+current_group==0){
				GUI_output(string_array_text_ltx[0x3A0]);
			}else{
				wunderErbitten();
			}
			break;
		case 0x87:
			if (byte_27CF6+current_group==0){
				GUI_output(string_array_text_ltx[0x3A0]);
			}else{
				var_6_8=get_party_money();
				if (var_6_8==0){
					GUI_output(string_array_text_ltx[0x644]);
				}else{
					make_valuta_str(tmp_buffer,var_6_8);
					sprintf(dialogTextPtr02,string_array_text_ltx[0x4A4],tmp_buffer);
					var_2=GUI_input(dialogTextPtr02,3);
					var_A = var_2;
					if(var_A>0){
						ax = var_A *10;
						if (ax >= var_6_8){
							((word_333B8+0x3150)*4) += var_6_8/10;
							var_6_8=0;
						}else{
							((word_333B8+0x3150)*4) += Var_A_C;
							var_6_8 -= var_A *10;
						}
						set_party_money(var_6_8);
						GUI_output(string_array_text_ltx[0x4A8]);
					}
				}
			}
			break;
		}//switch*/
	}//while
	//copy_palette();
	//turnaround();
	//word_27978=1;
	//word_27976=1;
	//return 1;
}
/*
void heldAufnehmen(But16u arg_0){
	var_4_6=buffer+0xC350;
	di = sub_22E93(var_4_6,arg_0);
	
	if (NrHeroes==7 || (NrHeroes==6 && heros+0x293D==0)){
		GUI_output(string_array_text_ltx[0x480]);
				-> loc_57086
	}else{
		do{
		
			if (di==0){
				GUI_output(string_array_text_ltx[0x488]);
				si=0xFFFF;
			}else{
				si=sub_234C3(var_4_6,di,1); //eine Art GUI radio?
				if (si!=0xFFFF){
					var_A=ptr_heros;
					var_2=0;
					for (var_2=0;var_2<6;var_2++;){
						ax=var_A+0x21;
						if (ax==0){
							prepare_chr_name(dialogTextPtr02,var_4_6 + si*32);
							ax=Read_CHR_TMP(dialogTextPtr02,var_2,current_group);
							if (ax!=0){
								NrHeroes++;
								(current_group+0x2D36)++;
								(var_A+0x8A)=var_2;
								write_chr_temp(var_2);
							}
							break;
						}
						var_A+=0x6DA;
					}
					draw_main_screen();
					init_ani(2);
					sprintf(dialogTextPtr02,string_array_text_ltx[0x3AC],
						string_array_text_ltx[(word_333B8+0x15)*4],string_array_text_ltx[(current_town+0xEB)*4]);
					GUI_print_loc_line(dialogTextPtr02);
				}
				di=sub_22E93(var_4_6,arg_0);
			}
		}while ( si!=0xFFFF && (NrHeroes<6 || (NrHeroes<7 && heros+0x293D!=0)) );
	}
}

void heldLoeschen(){
	if (byte_29BFA!=0){
		var_4=dword_3139B+0x7530
	}else{
		var_4=buffer+0xC350

	}
	di = sub_22E93(var_4,0xffff);

	do{
		if (di==0){
			si=0xffff;
		}else{
			si = sub_234C3(var_4,di,0xffff);
			if (si!=0){
				strcpy(dialogTextPtr02,var_4+32*si);
				sprintf(tmp_buffer,string_array_text_ltx[0x49C],dialogTextPtr02);
				ax = GUI_bool(tmp_buffer);
				if (ax!=0){
					prepare_chr_name(tmp_buffer,dialogTextPtr02);
					var_2 = sub_111A7(tmp_buffer);//datei löschen
					if (var_2!=0){
						GUI_output(string_array_text_ltx[0x498]);
						return 0;
					}
					sprintf(dialogTextPtr02,aTempS,tmp_buffer);//aTempS ist "TEMP\\%s"
					sub_111A7(dialogTextPtr02);//datei löschen
				}
				di = sub_22E93(var_4,0xffff);
			}else{
				return 0;
			} 
		}
	}while(si!=0xffff);

	return 1;

}
	
*/


