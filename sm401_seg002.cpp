
#include <stdlib.h>
#include <string.h>

#include "sm401_main.h"
#include "sm401_seg002.h"
#include "sm401_seg009.h"

Bit8u tmp_buffer[0x140];

//unsigned short word_20746;


/*
D1_LOG("load_archive_file(0x%x / %s) = %x\n",
	index, get_fname(index), reg_ax);

#define FNAMES		(0x4c8c)
static inline Bit8u *get_fname(unsigned short off) {
	return Real2Host(ds_readd(FNAMES + off * 4));
}
*/

static unsigned short fd_read_datfile(FILE * fd, Bit8u *buf, Bit16u len)
{

	//if (len > flen_left)
		//len = (unsigned short)flen_left;

	len = fread(buf, 1, len, fd);

	//flen_left -= len;

	return len;
}

/**
 * open_and_seek_dat - opens SCHICK.DAT and seeks to desired position
 * @fileindex: the index of the file in SCHICK.DAT
 *
 * Returns the filehandle or 0xffff.
 */
//static
FILE* open_and_seek_dat(unsigned short fileindex, sm_file* smf) {
	Bit32u start, end;

	/* open SCHICK.DAT */
	//if (!DOS_OpenFile("SCHICK.DAT", OPEN_READ, &fd))
	//	return 0xffff;
	smf->fd = fopen("SCHICK.DAT","rb");
	if (!smf->fd) return smf->fd;
	/* seek to the fileindex position in the offset table */
	//bc_lseek(fd, fileindex * 4, DOS_SEEK_SET);
	fseek(smf->fd, fileindex * 4, SEEK_SET);

	/* read the start offset of the desired file */
	//bc__read(fd, (Bit8u*)&start, 4);
	//fd_read_datfile(fd, (Bit8u*)&start, 4);
	fread((Bit8u*)&start, 1, 4, smf->fd);
	
	/* read the start offset of the next file */
	//bc__read(fd, (Bit8u*)&end, 4);
	//fd_read_datfile(fd, (Bit8u*)&end, 4);
	fread((Bit8u*)&end, 1, 4, smf->fd);

	/* BE-Fix */
	//start = host_readd((Bit8u*)&start);
	//end = host_readd((Bit8u*)&end);

	/* seek to the desired file */
	//bc_lseek(fd, start, DOS_SEEK_SET);
	fseek(smf->fd, start, SEEK_SET);
	/* save the offset of the desired file */
	//ds_writed(0xbcdf, start);
	smf->off=start;
	
	/* save the length of the desired file in 2 variables */
	//ds_writed(0xbce3, end - start);
	//ds_writed(0xbce7, end - start);
	smf->len=end-start;
	smf->len_=end-start;
fprintf(stdout,"open and seek %d, Start: %x, Länge %x/%d\n",fileindex,start,end-start,end-start);
	return smf->fd;
}




FILE* load_archive_file(Bit16u index, sm_file* smf){

/*	unsigned short si,ax,bp;
	si=bp+index;
	call;//flushall();        ; Call Procedure
	
	if (si!=0x8000){
		ax=si and 0x7FFF;
		open_tmp(ax);//sub_5F32(ax);
	}else{*/
		open_and_seek_dat(index,smf);//sub_5D5E(si);
	//}
		return smf->fd;
}


Bit16u read_archive_file( sm_file* smf, Bit8u *buf, Bit16u readsize) {

	/* no need to read */
	//if (ds_readd(0xbce3) == 0)
	if (smf->len == 0)
		return 0;

	/* adjust number of bytes to read */
	if (readsize > smf->len)
		readsize = smf->len & 0xffff;

	smf->len -= readsize;

	//return bc__read(handle, buffer, readsize);
fprintf(stdout,"read %x\n",readsize);
	return fread(buf, 1, readsize, smf->fd);
}


signed int process_nvf(struct nvf_desc *nvf) {

	signed char nvf_type;
	Bit8u *src, *dst;
	int p_size;
	int offs;
	signed int retval;
	short va;
	short height;
	short pics;
	short width;
	short i;

	Bit8u *p = nvf->src;
	dst = nvf->dst;

	nvf_type = readb(p);
//fprintf(stdout,"type: %d\n",nvf_type);
	va = nvf_type & 0x80;
	nvf_type &= 0x7f;

	pics = readw(p + 1);
//fprintf(stdout,"pics: %d\n",pics);

	if (nvf->nr < 0)
		nvf->nr = 0;

	if (nvf->nr > pics - 1)
		nvf->nr = pics - 1;

	switch (nvf_type) {

	case 0x00:
		width = readw(p + 3);
		height = readw(p + 5);
		p_size = height * width;
		src =  p + nvf->nr * p_size + 7;
		break;

	case 0x01:
		offs = pics * 4 + 3;
		for (i = 0; i < nvf->nr; i++) {
			width = readw(p + i * 4 + 3);
			height = readw(p + i * 4 + 5);
			offs += width * height;
		}

		width = readw(p + nvf->nr * 4 + 3);
		height = readw(p + nvf->nr * 4 + 5);
		p_size = width * height;
		src = p + offs;
		break;

	case 0x02: case 0x04:
		width = readw(p + 3);
		height = readw(p + 5);
		offs = pics * 4 + 7;
		for (i = 0; i < nvf->nr; i++)
			offs += readd(p  + (i * 4) + 7);

		p_size = readd(p + nvf->nr * 4 + 7);
		src = p + offs;
		break;

	case 0x03: case 0x05:
		offs = pics * 8 + 3;
		for (i = 0; i < nvf->nr; i++)
			offs += readd(p  + (i * 8) + 7);

		width = readw(p + nvf->nr * 8 + 3);
		height = readw(p + nvf->nr * 8 + 5);
		p_size = readd(p + i * 8 + 7);
		src = p + offs;
		break;
	}

	switch (nvf->type) {

	case 0:
		/* PP20 decompression */

		if (va != 0) {

			/* get size from unpacked picture */
			retval = readd(src);
			retval = readd(src + (retval - 4));
			retval = swap_u32(retval) >> 8;

		} else
			retval = width * height;

		decomp_pp20(src, dst, src + 4, p_size);
		break;

	case 2: case 3: case 4: case 5:
		/* RLE decompression */
		decomp_rle(width, height, dst, src,
			tmp_buffer, nvf->type);
		/* retval was originally neither set nor used here.
			VC++2008 complains about an uninitialized variable
			on a Debug build, so we fix this for debuggings sake */
		/* Orig-Fix */
		retval = p_size;
		break;

	default:
		/* No decompression, just copy */
		memmove(nvf->dst, src, (short)p_size);
		retval = p_size;
	}

	nvf->width = width;
	nvf->height = height;

	return retval;
}


//versdchoben in seg 029:
//void sub_195DD(signed char icons, ...){
/*
	unsigned short i;
	va_list arguments;

	
	va_start(arguments, icons);
	for (i=0;i<icons;i++){
		byte_30CF8[i] = va_arg(arguments, signed short);
	}
	while(i<9){
		byte_30CF8[i]= -1;
		i++;
	}

	if (byte_27805==0)
		draw_icons();
	*/
//}




