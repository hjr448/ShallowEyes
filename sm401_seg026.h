#ifndef SM401_SEG27_H
#define SM401_SEG27_H
//0x25
void split_textbuffer(Bit8u **dst, Bit8u *src, Bit32u len);
//void split_textbuffer(Bit8u *, RealPt, Bit32u);
//0x2a
void load_ggsts_nvf();
//0x3e
void write_chr_temp(unsigned short);
//0x48
void load_in_head(Bit16s);
//0x4d
void load_city_ltx(signed short);
//0x57
void load_buffer_1(signed short);
//0x57
void load_ltx(unsigned short);
//0x61
void prepare_chr_name(char*, char*);
//0x61
void prepare_sg_name(char*, char*);
//0x66
void load_tempicon(unsigned short);
#endif