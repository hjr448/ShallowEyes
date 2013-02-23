#ifndef gen201_h
#define gen201_h


void do_call_gen(unsigned short);
static unsigned short gen_read_datfile(FILE * fd, unsigned char *buf, unsigned short len);
static FILE * gen_open_datfile(unsigned short index);


void gen_enter_name();
void gen_change_sex();
void gen_change_attribs();
void gen_clear_hero();
void gen_new_values();
void gen_select_typus();
void gen_choose_typus();
void gen_save_chr();
void gen_select_skill();
void gen_choose_atpa();
void gen_select_spell();
void gen_change_head();

#endif