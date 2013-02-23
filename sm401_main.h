#ifndef sm401_main_h
#define sm401_main_h




typedef         double		Real64;
/* The internal types */
typedef  unsigned char		Bit8u;
typedef    signed char		Bit8s;
typedef unsigned short		Bit16u;
typedef   signed short		Bit16s;
typedef  unsigned long		Bit32u;
typedef    signed long		Bit32s;
//typedef uint64_t	Bit64u;
//typedef   int64_t	Bit64s;
typedef unsigned int		Bitu;
typedef signed int			Bits;

static inline unsigned char readb(unsigned char* off) {
	return off[0];
}
static inline unsigned short readw(unsigned char* off) {
	return off[0] | (off[1] << 8);
}
static inline unsigned int readd(unsigned char* off) {
	return off[0] | (off[1] << 8) | (off[2] << 16) | (off[3] << 24);
}
static inline unsigned int swap_u32(unsigned int v) {
	return ((v >> 24) & 0xff) | ((v >> 16) & 0xff) << 8 |
		((v >> 8) & 0xff) << 16 | (v&0xff) << 24;

}
static inline int NOT_NULL(Bit8u* p)
{
	//return (p != NULL && p != MemBase);
	return (p != NULL);
}
static inline void writeb(unsigned char*  off,unsigned char  val) {
	off[0]=val;
}
static inline void writew(unsigned char*  off,unsigned short val) {
	off[0]=(unsigned char)(val);
	off[1]=(unsigned char)(val >> 8);
}
static inline void writed(unsigned char*  off,unsigned int val) {
	off[0]=(unsigned char)(val);
	off[1]=(unsigned char)(val >> 8);
	off[2]=(unsigned char)(val >> 16);
	off[3]=(unsigned char)(val >> 24);
}








#endif
