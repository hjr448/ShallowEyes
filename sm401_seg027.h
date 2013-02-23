

class Cseg027{
public:
	Cseg027(Bit8u* _buffer,Bit8u* _gataosao,char* _buffer_games_nam, struct_globvars* _globvars);
	void init_common_buffers();
	void load_pp20(Bit16u);
	void load_icons();//Bit16u fileindex, Bit16s icon, Bit16s pos);
	void load_ani(const signed short nr);
	void init_ani(const signed short v1);
	void load_tempicon(unsigned short);//aus seg026
private: 
	Bit8u* buffer;
	Bit8u* gataosao;
	char* buffer_games_nam;
	struct_globvars* globvars;
};
//0x20
//void load_pp20(Bit16u);
/*0x25*/
//void load_ani(signed short);
/* 0x2a */
//RealPt load_fight_figs(signed short);
void load_fight_figs(signed short);
//0x2f
//void init_common_buffers(Bit8u* buffer,Bit8u* gataosao,char* buffer_games_nam);
//0x34
void load_scenario(signed short);
//0x39
unsigned short count_fight_enemies(signed short);
//0x3e
void read_fight_lst(signed short);
//0x43
void write_fight_lst(void);
