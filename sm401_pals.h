
static const struct struct_color pal_popup[8] = {
	{0x00, 0x00, 0x00 },
	{0x38, 0x30, 0x28 },
	{0x38, 0x30, 0x10 },
	{0x30, 0x28, 0x0c },
	{0x2c, 0x24, 0x08 },
	{0x28, 0x20, 0x04 },
	{0x18, 0x14, 0x00 },
	{0x0b, 0x19, 0x0c }
};
static const struct struct_color pal_misc[3] = {
	{0x28, 0x00, 0x00 },
	{0x28, 0x28, 0x00 },
	{0x00, 0x00, 0x28 }
};
static const struct struct_color pal_heads[32] = {
	{0x00, 0x00, 0x00},
	{0x00, 0x00, 0x3f},
	{0x00, 0x00, 0x24},
	{0x3c, 0x30, 0x28},
	{0x3c, 0x2c, 0x24},
	{0x3c, 0x28, 0x1c},
	{0x38, 0x24, 0x18},
	{0x34, 0x20, 0x14},
	{0x30, 0x1c, 0x14},
	{0x28, 0x18, 0x10},
	{0x24, 0x14, 0x0c},
	{0x1c, 0x10, 0x0c},
	{0x18, 0x0c, 0x08},
	{0x14, 0x0c, 0x08},
	{0x38, 0x38, 0x38},
	{0x30, 0x30, 0x30},
	{0x2c, 0x2c, 0x2c},
	{0x28, 0x28, 0x28},
	{0x20, 0x20, 0x20},
	{0x1c, 0x1c, 0x1c},
	{0x14, 0x14, 0x14},
	{0x10, 0x10, 0x10},
	{0x00, 0x3f, 0x00},
	{0x00, 0x24, 0x00},
	{0x3c, 0x38, 0x00},
	{0x34, 0x2c, 0x00},
	{0x28, 0x20, 0x00},
	{0x3c, 0x18, 0x10},
	{0x30, 0x10, 0x08},
	{0x24, 0x0c, 0x04},
	{0x10, 0x08, 0x04},
	{0x3c, 0x3c, 0x3c},
};
static const struct struct_color pal_genbg[32] = {
	{0x00, 0x00, 0x00},
	{0x38, 0x38, 0x38},
	{0x34, 0x34, 0x34},
	{0x30, 0x30, 0x30},
	{0x2c, 0x2c, 0x2c},
	{0x28, 0x28, 0x28},
	{0x24, 0x24, 0x24},
	{0x20, 0x20, 0x20},
	{0x1c, 0x1c, 0x1c},
	{0x18, 0x18, 0x18},
	{0x14, 0x14, 0x14},
	{0x10, 0x10, 0x10},
	{0x0c, 0x0c, 0x0c},
	{0x08, 0x08, 0x08},
	{0x3c, 0x3c, 0x3c},
	{0x3c, 0x34, 0x18},
	{0x3c, 0x34, 0x08},
	{0x3c, 0x34, 0x00},
	{0x38, 0x30, 0x00},
	{0x34, 0x2c, 0x00},
	{0x30, 0x28, 0x00},
	{0x2c, 0x24, 0x00},
	{0x28, 0x20, 0x00},
	{0x24, 0x1c, 0x00},
	{0x20, 0x18, 0x00},
	{0x1c, 0x14, 0x00},
	{0x38, 0x14, 0x0c},
	{0x34, 0x0c, 0x00},
	{0x2c, 0x08, 0x00},
	{0x24, 0x04, 0x00},
	{0x1c, 0x00, 0x00},
	{0x14, 0x00, 0x00},
};


struct_color col_bl={0x0,0,0};
struct_color col_wh={0x3F,0x3F,0x3F};
/*struct_color pal_301F0[] = {
{0x00,0x00,0x00},
{0x38,0x30,0x28},
{0x38,0x30,0x10},
{0x30,0x28,0x0C},
{0x2C,0x24,0x08},
{0x28,0x20,0x04},
{0x18,0x14,0x00},
{0x0B,0x19,0x0C}};*/
struct_color pal_30208[] = { //similar to misc
{0x30,0x00,0x00},
{0x30,0x30,0x00},
{0x00,0x00,0x30}};
struct_color pal_ggsts[] = {
{0x00,0x00,0x00},
{0x38,0x30,0x28},
{0x30,0x24,0x1C},
{0x24,0x18,0x10},
{0x20,0x14,0x0C},
{0x18,0x0C,0x08},
{0x14,0x08,0x04},
{0x0C,0x04,0x00},
{0x3C,0x38,0x00},
{0x38,0x28,0x00},
{0x30,0x1C,0x00},
{0x2C,0x14,0x00},
{0x3C,0x24,0x00},
{0x3C,0x0C,0x00},
{0x3C,0x00,0x08},
{0x3C,0x00,0x20},
{0x00,0x10,0x00},
{0x00,0x14,0x00},
{0x00,0x1C,0x00},
{0x04,0x20,0x00},
{0x04,0x28,0x00},
{0x1C,0x38,0x3C},
{0x14,0x2C,0x34},
{0x0C,0x1C,0x30},
{0x04,0x0C,0x28},
{0x00,0x00,0x24},
{0x10,0x10,0x10},
{0x14,0x14,0x14},
{0x1C,0x1C,0x1C},
{0x24,0x24,0x24},
{0x30,0x30,0x30},
{0x3C,0x3C,0x3C}};
/*struct_color pal_30271[] = {
{0x00,0x00,0x00},
{0x00,0x00,0x3F},
{0x00,0x00,0x24},
{0x3C,0x30,0x28},
{0x3C,0x2C,0x24},
{0x3C,0x28,0x1C},
{0x38,0x24,0x18},
{0x34,0x20,0x14},
{0x30,0x1C,0x14},
{0x28,0x18,0x10},
{0x24,0x14,0x0C},
{0x1C,0x10,0x0C},
{0x18,0x0C,0x08},
{0x14,0x0C,0x08},
{0x38,0x38,0x38},
{0x30,0x30,0x30},
{0x2C,0x2C,0x2C},
{0x28,0x28,0x28},
{0x20,0x20,0x20},
{0x1C,0x1C,0x1C},
{0x14,0x14,0x14},
{0x10,0x10,0x10},
{0x00,0x3F,0x00},
{0x00,0x24,0x00},
{0x3C,0x38,0x00},
{0x34,0x2C,0x00},
{0x28,0x20,0x00},
{0x3C,0x18,0x10},
{0x30,0x10,0x08},
{0x24,0x0C,0x04},
{0x10,0x08,0x04},
{0x3C,0x3C,0x3C}};*/
struct_color pal_276E3[] = {
{0x00,0x00,0x00},
{0x00,0x00,0x29},
{0x00,0x00,0x0E},
{0x26,0x1A,0x12},
{0x26,0x16,0x0E},
{0x26,0x12,0x06},
{0x22,0x0E,0x02},
{0x1E,0x0A,0x00},
{0x1A,0x06,0x00},
{0x12,0x02,0x00},
{0x0E,0x00,0x00},
{0x06,0x00,0x00},
{0x02,0x00,0x00},
{0x00,0x00,0x00},
{0x22,0x22,0x22},
{0x1A,0x1A,0x1A},
{0x16,0x16,0x16},
{0x12,0x12,0x12},
{0x0A,0x0A,0x0A},
{0x06,0x06,0x06},
{0x00,0x00,0x00},
{0x00,0x00,0x00},
{0x00,0x20,0x00},
{0x00,0x0E,0x00},
{0x26,0x22,0x00},
{0x1E,0x16,0x00},
{0x12,0x0A,0x00},
{0x26,0x02,0x00},
{0x1A,0x00,0x00},
{0x0E,0x00,0x00},
{0x00,0x00,0x00},
{0x26,0x26,0x26}};
struct_color pal_0_1F[] = {
{0x00,0x00,0x00},
{0x3C,0x2C,0x24},
{0x3C,0x28,0x1C},
{0x34,0x20,0x18},
{0x28,0x18,0x14},
{0x20,0x14,0x10},
{0x18,0x0C,0x08},
{0x10,0x08,0x08},
{0x0C,0x04,0x04},
{0x34,0x34,0x34},
{0x28,0x28,0x28},
{0x20,0x20,0x20},
{0x18,0x18,0x18},
{0x14,0x14,0x14},
{0x10,0x10,0x10},
{0x0C,0x0C,0x0C},
{0x08,0x08,0x08},
{0x14,0x0C,0x08},
{0x00,0x00,0x00},
{0x18,0x14,0x3C},
{0x14,0x00,0x00},
{0x1C,0x00,0x00},
{0x24,0x00,0x00},
{0x2C,0x00,0x00},
{0x3C,0x00,0x00},
{0x1C,0x10,0x10},
{0x00,0x1C,0x00},
{0x00,0x28,0x00},
{0x00,0x04,0x20},
{0x2C,0x2C,0x0C},
{0x38,0x38,0x10},
{0x3C,0x3C,0x3C}};
struct_color pal_gray[] = {
{0x00,0x00,0x00},
{0x38,0x38,0x38},
{0x34,0x34,0x34},
{0x30,0x30,0x30},
{0x2C,0x2C,0x2C},
{0x28,0x28,0x28},
{0x24,0x24,0x24},
{0x20,0x20,0x20},
{0x1C,0x1C,0x1C},
{0x18,0x18,0x18},
{0x14,0x14,0x14},
{0x10,0x10,0x10},
{0x0C,0x0C,0x0C},
{0x08,0x08,0x08},
{0x04,0x04,0x04},
{0x00,0x00,0x00},
{0x00,0x3c,0x08},
{0x3C,0x30,0x28},
{0x3C,0x2C,0x28},
{0x2C,0x1C,0x18},
{0x24,0x14,0x10},
{0x1C,0x10,0x0C},
{0x14,0x08, 8},
{0x0C,0x04, 4},
{0x0C,0x08, 0},
{0x2C,0x00, 0},
{0x10,0x14,0x3C},
{0x00,0x08,0x38},
{0x00,0x08,0x34},
{0x00,0x00,0x18},
{0x3C,0x3C,0x00},
{0x3C,0x3C,0x3C}};




