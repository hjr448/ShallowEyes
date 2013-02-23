/*
	Rewrite of DSA1 v3.02_de functions of seg027 (file loader)
	Functions rewritten: 8/8 (complete)
*/

#include "sm401_main.h"
#include "sm401_seg002.h"
#include "sm401_seg009.h"
#include "sm401_seg027.h"
#include "sm401_video.h"


bool anis_loaded[36];
bool tempicon_loaded[13];


Cseg027::Cseg027(Bit8u* _buffer,Bit8u* _gataosao,char* _buffer_games_nam, struct_globvars* _globvars){
	buffer=_buffer;
	gataosao=_gataosao;
	buffer_games_nam=_buffer_games_nam;
	globvars=_globvars;
	
	Bit16u i;
	for (i=0;i<36;i++)
		anis_loaded[36] = false;
}

void Cseg027::load_pp20(Bit16u index)
{
	//unsigned char buffer_ptr;//RealPt buffer_ptr;
	//Bit16u fd;
	//Bit16u bi;
	struct sm_file smf;
	unsigned short bytes;

	//if (index <= 5 || index == 0xd6 || index == 0x14 || index == 0xd7) {
		/* These pictures are buffered for faster access /
		bi = index;

		/* PLAYM_US /
		if (index == 0xd6)
			bi = 6;
		/* ZUSTA_UK /
		if (index == 0x14)
			bi = 7;
		/* ZUSTA_US /
		if (index == 0xd7)
			bi = 8;
*/
		if (sm_index_inited(index)){//  ds_readd(0x5e6a + bi * 4)) {
			sm_index_do(index);
			/* already buffered, just decomp */
			//decomp_pp20(Real2Host(ds_readd(0x5e6a + bi * 4)),
			//	Real2Host(ds_readd(0xd303)),
			//	Real2Host(ds_readd(0x5e6a + bi * 4)) + 4,
			//	ds_readd(0x5e8e + bi * 4));
		} else {
			load_archive_file(index,&smf);
			bytes = read_archive_file(&smf, buffer, 0xFDE8);
			fclose(smf.fd);

			decomp_pp20(buffer,gataosao,buffer+4,bytes);
			sm_index_init(index,320,200,gataosao);
			sm_index_do(index);
			//fd = load_archive_file(index);
			//buffer_ptr = schick_alloc_emu(get_readlength2(fd));
/*
			if (buffer_ptr) {
				/* successful allocation */

				/* save pointer /
				ds_writed(0x5e6a + bi * 4, buffer_ptr);
				/* save length /
				ds_writed(0x5e8e + bi * 4, get_readlength2(fd));

				/* read pic /
				read_archive_file(fd,
					Real2Host(ds_readd(0x5e6a + bi * 4)),
					ds_readw(0x5e8e + bi * 4));

				/* decompress /
				decomp_pp20(Real2Host(ds_readd(0x5e6a + bi * 4)),
					Real2Host(ds_readd(0xd303)),
					Real2Host(ds_readd(0x5e6a + bi * 4)) + 4,
					ds_readd(0x5e8e + bi * 4));
				bc_close(fd);
			} else {
				/* failed allocation */

				/* read it directly /
				read_archive_file(fd,
					Real2Host(ds_readd(0xd303)) - 8,
					64000);

				/* decompress it /
				decomp_pp20(Real2Host(ds_readd(0xd303)) - 8,
					Real2Host(ds_readd(0xd303)),
					Real2Host(ds_readd(0xd303)) - 8 + 4,
					get_readlength2(fd));
				bc_close(fd);
			}*/
		}
	//} /*else {
		/* unbuffered picture /

		fd = load_archive_file(index);

		read_archive_file(fd, Real2Host(ds_readd(0xd303)) - 8, 64000);

		/* decompress it /
		decomp_pp20(Real2Host(ds_readd(0xd303)) - 8,
			Real2Host(ds_readd(0xd303)),
			Real2Host(ds_readd(0xd303)) - 8 + 4,
			get_readlength2(fd));

		bc_close(fd);
	}*/
}

/**
 * load_fight_figs() - load fight sprites of actors
 * @fig_old:	the fig number
 *
 * Returns a pointer to the location where the data is.
 *
 */
/* Original-Bug: when using EMS for caching something starge happens. /
RealPt load_fight_figs(signed short fig_old)
{
	RealPt src;
	signed short index;
	Bit8u *p_tab;
	Bit8u *mem_slots;
	signed short max_entries;
	unsigned short fd;
	unsigned int len;
	unsigned int offset;
	unsigned short ems_handle;
	RealPt dst;
	signed short i;
	signed short fig = fig_old;

	/* check if fig is at a known place /
	if (fig == (signed short)ds_readw(0x2cd1))
		return ds_readd(0xd2df);
	if (fig == (signed short)ds_readw(0x2cd3))
		return ds_readd(0xd2db);

#ifdef M302de_ORIGINAL_BUGFIX
	if (fig == -1)
		return ds_readd(0xd2df);
#endif

	if ((signed short)ds_readw(0x2cd3) != -1) {
		ds_writew(0x2cd1, ds_readw(0x2cd3));
		memcpy(Real2Host(ds_readd(0xd2df)),
			Real2Host(ds_readd(0xd2db)), 20000);
		src = ds_readd(0xd2db);
		ds_writew(0x2cd3, fig);
	} else if ((signed short)ds_readw(0x2cd1) != -1) {
		src = ds_readd(0xd2db);
		ds_writew(0x2cd3, fig);
	} else {
		src = ds_readd(0xd2df);
		ds_writew(0x2cd1, fig);
	}

	/* prepare archive access... /

	if (fig >= 88) {
		/* ...for foes /
		max_entries = 36;
		mem_slots = Real2Host(ds_readd(MEM_SLOTS_MON));
		p_tab = p_datseg + 0xd01d;
		index = 16;
		fig -= 88;
	} else {
		/* ...for heroes *
		max_entries = 43;

		if (fig >= 44) {
			/* female /
			p_tab = p_datseg + 0xd0ad;
			index = 234;
			mem_slots = Real2Host(ds_readd(MEM_SLOTS_WFIG));
			fig -= 44;
		} else {
			/* male /
			p_tab = p_datseg + 0xd159;
			index = 24;
			mem_slots = Real2Host(ds_readd(MEM_SLOTS_MFIG));
		}
	}

	/* check if fig is already in memory /
	for (i = 0; i < max_entries; i++) {
		if (host_readw(mem_slots + i * 12) == fig)
			break;
	}

	if (i != max_entries) {
		/* Yes, it is /

		if (host_readw(mem_slots + i * 12 + 6) != 0) {
			/* is in EMS /
			ems_handle = host_readw(mem_slots + i * 12 + 6);
			from_EMS(src, ems_handle, host_readd(mem_slots + i * 12 + 8));
		} else {
			/* is in HEAP /
			D1_LOG("cached from HEAP %d\n", fig);
			memcpy(Real2Host(src),
				Real2Host(host_readd(mem_slots + i * 12 + 2)),
				host_readw(mem_slots + i * 12 + 8));
		}
	} else {
		D1_LOG("not cached\n");

		/* read fig from file /
		offset = host_readd(p_tab + (fig - 1) * 4);
		len = host_readd(p_tab + fig * 4) - offset;

		fd = load_archive_file(index);

		seg002_0c72(fd, offset);

		read_archive_file(fd, Real2Host(src), (unsigned short)len);

		bc_close(fd);

		dst = schick_alloc_emu(len);

		if (dst != 0) {
			D1_LOG("use HEAP for fig %d\n", fig_old);
			/* use heap /

			for (i = 0; i < max_entries - 1; i++) {
				if (host_readw(mem_slots + i * 12) == 0)
					break;
			}

			host_writew(mem_slots + i * 12, fig);
			host_writed(mem_slots + i * 12 + 2, dst);
			host_writew(mem_slots + i * 12 + 6, 0);
			host_writed(mem_slots + i * 12 + 8, len);

			memcpy(Real2Host(dst), Real2Host(src),
				(unsigned short)len);

		} else if (ds_readb(EMS_ENABLED) != 0) {
			D1_LOG("use EMS for fig %d\n", fig_old);

			ems_handle = alloc_EMS(len);

			if (ems_handle != 0) {

				/* find a free slot /
				for (i = 0; i < max_entries - 1; i++) {
					if (host_readw(mem_slots + i * 12) == 0)
						break;
				}

				/* write slot /
				host_writew(mem_slots + i * 12, fig);
				host_writew(mem_slots + i * 12 + 6, ems_handle);
				host_writed(mem_slots + i * 12 + 2, 0);
				host_writed(mem_slots + i * 12 + 8, len);

				/* copy to EMS /
				to_EMS(ems_handle, src, len);
			}
		}
	}

	return src;
}

/**
 *	load_ani() -	loads an animation
 *	@nr:	number of the animation
 */
void Cseg027::load_ani(const signed short nr)
{
	struct sm_file smf;
	unsigned short bytes;
	Bit32u off_imdata, off_palette;
	Bit16u width, height;

	Bit8u compression_type;
	Bit8u palette_size;
	Bit8u num_elements;
	int off_elements;
	Bit8u* ptr_elements;
	
	//signed int area_size;
	int len;
	signed int len_3, len_4;
	unsigned int offset, offset_2;
	Bit8u *dst, *p6;
	int ani_off, ani_len;/*
	Bit8u *p5;
	signed int p4;
	signed int p3;*/
	Bit8u *p2;
	Bit8u *p1;
	Bit32u ani_buffer;
	Bit8u *ani_main_ptr;
	Bit8u *palette_ptr;
	/*
	unsigned short ems_handle;
	Bit8u *p_area;
	unsigned short i;
	unsigned short fd;
	signed short area_changes;
	signed short area_pics;
	unsigned short i_area;
	signed short di;


	/* sanity check */
	if (nr == -1)
		return;
	if (nr > 36)
		return;

	/* no need to reload  the same ani*/
	//if (nr == globvars->current_ani)
	//	return;

	//ds_writew(0x29ae, 0);
	/* set the new ani nr*/
	globvars->current_ani=nr;
	/* clear the old ani */
	//clear_ani();    seg004

	if (!anis_loaded[nr-1]){
		/* load ANIS.TAB */
		load_archive_file(0x17,&smf);
		bytes = read_archive_file(&smf, buffer, 148);
		fclose(smf.fd);

		ani_off = readd(buffer - 4 + nr * 4);
		ani_len = readd(buffer + nr * 4) - ani_off;

		/* load ANIS */
		load_archive_file(0x16,&smf);//fd = load_archive_file(0x16);
		/* seek to ordered ani */
		fseek(smf.fd, smf.off + ani_off, SEEK_SET);//seg002_0c72(fd, ani_off);
	
		bytes = read_archive_file(&smf, buffer,	(unsigned short)ani_len);//fd, Real2Host(ds_readd(0xc3db)),
		fclose(smf.fd);

		off_imdata = readd(buffer);//ds_readd(0xc3db);
		off_palette = readd(buffer+4);
		width = readw(buffer+8);
		height = readb(buffer+10);
		num_elements = readb(buffer+11);

		//für jedes Element:
		off_elements = readd(buffer+12);
	
		ptr_elements = buffer + off_elements;
		//posx = readw(ptr_elements+4);
		//posy = readb(ptr_elements+6);
		//width = readb(ptr_elements+7);
		//height = readb(ptr_elements+8);
		//anz = readb(ptr_elements+11);
		//for i<anz
		//posx = readb(ptr_elements+12+i*4);
		//posy = readb(ptr_elements+12+i*4+1);
		
		//anzanim = readw(ptr_elements+12+anz*4);
		//for i<anzanim
		//index = readw(ptr_elements+12+anz*4 + 4*i);
		//delay = readw(ptr_elements+12+anz*4 + 4*i+2);

		
		
		
		
		
		

		/* set start of picture data */
		ani_main_ptr = buffer + off_imdata;
		//ds_writed(ANI_MAIN_PTR,
			//host_readd(Real2Host(ds_readd(0xc3db))) + ani_buffer);
		/* set start of palette */
		
		palette_ptr = buffer + off_palette + 6;//palette ist hier  //??? src_rle = ani_main_ptr + readd(buffer+4) + 6;
		//ds_writed(0xce3b,
			//host_readd(Real2Host(ds_readd(0xc3db)) + 4) + ani_buffer + 6);
		/* read some bytes between data and palette */
		//word_313A9 = readw(src_rle-6);
		//ds_writew(0xc3e9,
			//host_readw(Real2Host(ds_readd(0xce3b)) - 6));
		//word_313AB = readw(src_rle-4);
		//ds_writew(0xc3eb,
			//host_readw(Real2Host(ds_readd(0xce3b)) - 4));
		/* compression type */
		compression_type = readw(palette_ptr-1);
		//ds_writeb(0xce39,
			//host_readb(Real2Host(ds_readd(0xce3b)) - 1));
		/* palette size*/
		palette_size = readw(palette_ptr-2);
		sm_set_palette(palette_ptr,0,palette_size);
		//ds_writeb(0xce3a,
			//host_readb(Real2Host(ds_readd(0xce3b)) - 2));

		//p6 = src_rle + palette_size * 3; //pointer auf end
		/* set picture size /
00 0d
	ds_writew(0xc3e7, host_readw(Real2Host(ds_readd(0xc3db) + 8)));
87
	ds_writeb(0xc3ed, host_readb(Real2Host(ds_readd(0xc3db) + 10)));
	/* set number of areas /
01
	ds_writeb(0xc3ee, host_readb(Real2Host(ds_readd(0xc3db) + 11)));

		/* Process Main Picture */
		if (compression_type != 0) {
			len_4 = readd(ani_main_ptr);//host_readd(Real2Host(ds_readd(ANI_MAIN_PTR)));
			//p1 = ani_main_ptr;

			//p1 += (len_4 - 4);

			//len_3 = readd(p1);
			//len_3 = swap_u32(len_3) >> 8;
		
			decomp_pp20(ani_main_ptr,gataosao,ani_main_ptr+4,len_4);

			//decomp_pp20(Real2Host(ds_readd(ANI_MAIN_PTR)),
			//	Real2Host(ds_readd(0xd303)),
			//	Real2Host(ds_readd(ANI_MAIN_PTR)) + 4,
			//	len_4);

			//offset = len_3 - len_4;
			//dst = ani_main_ptr;
			//dst += len_4;
			//len = p6 - dst;
//memcpy(p6 + offset, dst, len);

			//memcpy(Real2Host(ds_readd(ANI_MAIN_PTR)),
				//Real2Host(ds_readd(0xd303)), len_3);
//memcpy(ani_main_ptr, gataosao, len_3);
			//dst += offset;
//memcpy(dst, p6 + offset, len);
			//src_rle = src_rle + offset;
			//ds_writed(0xce3b, ds_readd(0xce3b) + offset);
			//p6 += offset;
printf("anis decompressed: %d\n",nr);
		}
		sm_images_init_anis(nr-1,gataosao);
		anis_loaded[nr-1]=true;
printf("anis loaded: %d\n",nr);
	}
	
		
		
	/* Process the Areas */
	//for (i_area = 0; (signed char)ds_readb(0xc3ee) > i_area; i_area++) {
	//for (i_area = 0; i_area < num_elements; i_area++) {
		//p5 = Real2Host(RealMake(datseg, 0xc3ef + i_area * 0x107));
		//p3 = host_readd(Real2Host(ds_readd(0xc3db) + i_area * 4 + 0xc));
		//p_area = Real2Host(ds_readd(0xc3db) + p3);
		//p_area = buffer + off_elements + i_area * 4;
		//strncpy((char*)p5, (char*)p_area, 4);

		//host_writew(p5 + 5, host_readw(p_area + 4));
		//host_writeb(p5 + 7, host_readb(p_area + 6));
		//host_writeb(p5 + 8, host_readb(p_area + 7));
		//host_writew(p5 + 9, host_readw(p_area + 8));
		//host_writeb(p5 + 0x0b, host_readb(p_area + 0x0a));
/*
		area_pics = (signed char)host_readb(p_area + 0x0b);
		host_writeb(p5 + 0x0c, (unsigned char)area_pics);
		if (ds_readb(0xce39) != 0) {

			p4 = host_readd(p_area + 0xc);
			p4 += offset;
			p1 = Real2Host(ds_readd(0xc3db) + p4);

			len_4 = host_readd(p1);
			p1 += (len_4 - 4);
			area_size = host_readd(p1);
			area_size = swap_u32(area_size) >> 8;

			decomp_pp20(Real2Host(ds_readd(0xc3db)) + p4,
				Real2Host(ds_readd(0xd303)),
				Real2Host(ds_readd(0xc3db)) + p4 + 4,
				len_4);

			offset_2 = area_size - len_4;
			offset += offset_2;

			dst = Real2Host(ds_readd(0xc3db));
			dst += p4;
			dst += len_4;
			len = p6 - dst;
			memcpy(p6 + offset_2, dst, (unsigned short)len);

			memcpy(Real2Host(ds_readd(0xc3db)) + p4,
				Real2Host(ds_readd(0xd303)), (unsigned short)area_size);
			dst += offset_2;
			memcpy(dst, p6 + offset_2, (unsigned short)len);
			ds_writed(0xce3b, ds_readd(0xce3b) + offset_2);
			p6 += offset_2;
			area_size = (unsigned char)host_readb(p5 + 8)
				* (signed short)host_readw(p5 + 9);

			for (di = 0; di < area_pics; di++) {
				host_writed(p5 + di * 4 + 0xd,
					ds_readd(0xc3db) + p4 + di * area_size);
			}
		} else {
			for (di = 0; di < area_pics; di++) {
				p4 = host_readd(p_area + di * 4 + 0xc);
				host_writed(p5 + di * 4 + 0x0d,
					ds_readd(0xc3db) + p4);
			}
		}
/*
		area_changes = host_readw(p_area + area_pics * 4 + 0x0c);
		host_writew(p5 + 0x5d, area_changes);

		p2 = p_area + area_pics * 4 + 0x0e;
		for (di = 0; di < area_changes; di++) {
			host_writew(p5 + 0x5f + di * 4,
				host_readw(p2 + di * 4));
			host_writew(p5 + 0x61 + di * 4,
				host_readw(p2 + di * 4 + 2));
		}
	}

	ani_len = p6 - Real2Host(ds_readd(0xc3db));
	/* this is always true /
	if (ani_len > (signed int)ds_readd(0xce43)) {
		ds_writew(0x2ccb, 0xffff);
	}*/
}
/*
void load_scenario(signed short nr)
{
	unsigned short fd;
	signed short n;
	char buf[2];

	n = nr;

	/* load SCENARIO.LST /
	fd = load_archive_file(0xc8);

	/* read the first two bytes == nr of scenarios /
	read_archive_file(fd, (Bit8u*)buf, 2);

	/* check if scenario nr is valid /
	if ((host_readw((Bit8u*)buf) < n) || (n < 1))
		nr = 1;

	/* seek to the scenario /
	seg002_0c72(fd, (n - 1) * 621 + 2);

	/* read scenario /
	read_archive_file(fd, Real2Host(ds_readd(SCENARIO_BUF)), 621);

	/* close archive /
	bc_close(fd);
}

unsigned short count_fight_enemies(signed short nr)
{
	Bit8u *buf;
	unsigned short fd;
	unsigned short max;
	unsigned short retval;
	unsigned short i;

	retval = 0;

	buf = Real2Host(ds_readd(0xd2f3));

	/* load FIGHT.LST from TEMP dir /
	fd = load_archive_file(0x8000 | 0xcd);

	/* read the first 2 bytes (max number of fights) /
	bc__read(fd, (Bit8u*)&max, 2);

	/* sanity check for parameter nr /
	if ((max - 1) < nr || nr <= 0)
		nr = 0;

	/* seek to file position /
	bc_lseek(fd, nr * 216 + 2, DOS_SEEK_SET);

	/* read the fight entry /
	bc__read(fd, buf, 216);

	/* close FIGHT.LST /
	bc_close(fd);

	/* check all enemies /
	for (i = 0; i < 20; i++) {
		/* no enemy /
		if (host_readb(buf + 0x16 + i * 5) == 0)
			continue;
		/* enemy does not appear in the first round /
		if (host_readb(buf + 0x1a + i * 5) != 0)
			continue;
		/* increment counter /
		retval++;
	}

	return retval;
}

void read_fight_lst(signed short nr)
{
	char fight_name[21];
	unsigned short fd;
	unsigned short max;

	/* load FIGHT.LST from TEMP dir /
	fd = load_archive_file(0x8000 | 0xcd);

	/* read the first 2 bytes (max number of fights) /
	bc__read(fd, (Bit8u*)&max, 2);

	/* sanity check for parameter nr /
	if ((max - 1) < nr || nr <= 0)
		nr = 0;

	/* write the fight number to a global var /
	ds_writew(0x5eb2, nr);

	/* seek to file position /
	bc_lseek(fd, nr * 216 + 2, DOS_SEEK_SET);

	/* read the fight entry /
	bc__read(fd, Real2Host(ds_readd(PTR_FIGHT_LST)), 216);

	/* Improvement /
	strncpy(fight_name, (char*)Real2Host(ds_readd(PTR_FIGHT_LST)), 20);
	fight_name[20] = '\0';
	D1_INFO("Lade Kampf Nr %3d\t Name \"%s\"\n", nr, fight_name);
	/* Improvement end */

	/* close FIGHT.LST /
	bc_close(fd);
}

void write_fight_lst(void)
{
	signed short nr;
	unsigned short fd;

	nr = ds_readw(0x5eb2);

	/* load FIGHT.LST from TEMP dir /
	fd = load_archive_file(0x8000 | 0xcd);

	/* seek to the entry /
	bc_lseek(fd, nr * 216 + 2, DOS_SEEK_SET);

	/* write it /
	bc__write(fd, ds_readd(PTR_FIGHT_LST), 216);

	/* close the file /
	bc_close(fd);
}*/
void Cseg027::init_common_buffers(){

	struct nvf_desc nvf;
	struct sm_file smf;
	unsigned short bytes;

	/* load POPUP.DAT */
	load_archive_file(0x99,&smf);
	//bytes = read_archive_file(&smf, Real2Host(ds_readd(0xd2ad) - 8), 500);buffer_popup_dat
	bytes = read_archive_file(&smf, buffer, 500);
	fclose(smf.fd);

	/* decompress POPUP.DAT */
	decomp_pp20(buffer,gataosao,buffer+4,bytes);
	sm_images_init_popup(gataosao);

	/* load COMPASS */
	load_archive_file(0x12,&smf);
	bytes = read_archive_file(&smf, buffer, 5000);//bytes = read_archive_file(&smf, Real2Host(ds_readd(0xd2b1)), 5000);
	fclose(smf.fd);
	nvf.src = buffer;
	nvf.type = 0;
	nvf.height = 0;
	nvf.width = 0;
	nvf.dst = gataosao;
	nvf.nr = 1;
	process_nvf(&nvf);
	nvf.dst = gataosao + 22*52;
	nvf.nr = 2;
	process_nvf(&nvf);
	nvf.dst = gataosao + 22*52*2;
	nvf.nr = 3;
	process_nvf(&nvf);
	nvf.dst = gataosao + 22*52*3;
	nvf.nr = 4;
	process_nvf(&nvf);
	sm_images_init_compass(gataosao);
	
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
	smf.fd = fopen("GAMES.NAM","rb");//load_regular_file//index 0xcf;
	fread(buffer_games_nam, 1, 45, smf.fd);//bc__read(fd, Real2Host(RealMake(datseg, 0xe2da)), 45);
	fclose(smf.fd);

	/* load TOWNPAL.DAT */
	load_archive_file(0x8d,&smf);//fd = load_archive_file(0x8d);
	bytes = read_archive_file(&smf, buffer, 288);
	//read_archive_file(fd, Real2Host(ds_readd(0xd321)), 288);
	fclose(smf.fd);//bc_close(fd);

}


void Cseg027::load_icons()//Bit16u fileindex, Bit16s icon, Bit16s pos)
{
	Bit16u fd;
	struct sm_file smf;
	unsigned short bytes;

	
	load_archive_file(0x0f,&smf);//
	bytes = read_archive_file(&smf, buffer, 64000);//55
	fclose(smf.fd);
	sm_icons_init(buffer);

	load_archive_file(0x06,&smf);//
	bytes = read_archive_file(&smf, buffer, 64000);//9
	fclose(smf.fd);

	sm_bicons_init(buffer);
	
	
}

void Cseg027::load_tempicon(unsigned short nr)//aus seg026
{
	struct nvf_desc nvf;
	struct sm_file smf;
	Bit32u len;

	unsigned short fd;

	if (nr == 14)
		nr = 7;

	if (!tempicon_loaded[nr]){
		
		/* load TEMPICON */
		load_archive_file(0xb4,&smf);
		len = read_archive_file( &smf, buffer, 7000);
		fclose(smf.fd);

		nvf.src = buffer;
		nvf.type = 0;
		nvf.width = 0;
		nvf.height = 0;
		nvf.dst = gataosao;//=0xd2e3
		nvf.nr = nr;
		process_nvf(&nvf);
		sm_images_init_tempicon(nr,gataosao);
	}
	sm_images_do_tempicon(nr);
}

void Cseg027::init_ani(const signed short v1)//aus seg004
{

	Bit16u i;
/*
	/* return if current ani == -1 */
	if (globvars->current_ani == 0xffff)
		return;
/*
	if ((v1 & 0x7f) != 2) {
		for (i = 0; i < 10; i++) {
			ds_writew(0xe260 + i * 2, 0);
			ds_writew(0xe24c + i * 2, 0xffff);
			ds_writew(0xe238 + i * 2, 1);
		}

		if (v1 & 0x80)
			ds_writeb(0x2cca, 0);
		else
			ds_writeb(0x2cca, 1);

		//update_mouse_cursor();

		//clear_ani_pal();

		/* set flag for pic_copy() /
		ds_writew(0x4a92, 1);

		/* set uppter left coordinates /
		ds_writew(0xc011, ds_readw(0xce41));
		ds_writew(0xc013, ds_readw(0xce3f));
		*/
		sm_images_do_anis(globvars->current_ani-1,globvars->ani_x,globvars->ani_y);
		/* set lower right coordinates /
		ds_writew(0xc015, ds_readw(0xce41) + ds_readw(0xc3e7) - 1);
		ds_writew(0xc017, ds_readw(0xce3f) + ds_readb(0xc3ed) - 1);

		/* copy pointer /
		ds_writed(0xc019, ds_readd(0xce35));

		/* copy the main ani picture /
		do_pic_copy(1);

		set_ani_pal(Real2Host(ds_readd(0xce3b)));

		/* reset flag for pic_copy() /
		ds_writew(0x4a92, 0);

		refresh_screen_size();
	}

	if ((v1 & 0x7f) != 1) {
		wait_for_vsync();
		ds_writew(0x29ae, 1);
	} else {
		wait_for_vsync();
	}*/
}

