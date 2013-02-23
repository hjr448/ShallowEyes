#ifndef sm401_seg009_h
#define sm401_seg009_h


void decomp_pp20(Bit8u *dst, Bit8u *src_data, Bit8u *src, unsigned int len);
void decomp_rle(unsigned short width, unsigned short height,
	Bit8u *dst, Bit8u *src, Bit8u *tmp_buffer, unsigned short mode);



#endif