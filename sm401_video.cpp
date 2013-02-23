

#include <irrlicht.h>

#include "sm401_main.h"
#include "sm401_seg002.h"
#include "sm401_GUI.h"
#include "sm401_video.h"


#ifndef WIN32
#include <unistd.h>
#else
#include <windows.h>
#endif
static int sm_sleep( unsigned long _t ) {
#ifdef WIN32
Sleep( _t );
#else
usleep( 1000 * _t );
#endif
}




using namespace irr;
//using namespace core;
//using namespace scene;
//using namespace video;
//using namespace io;
//using namespace gui;

unsigned short sm_w=320;
unsigned short sm_h=200;
unsigned short dst_w=960;
unsigned short dst_h=600;
unsigned short f_w=3;
unsigned short f_h=3;


struct struct_image{
	irr::video::ITexture *dataTexture;
	core::rect<s32> rec_in_t;
	core::position2d<s32> pos_in_s;
	bool active;
};
struct struct_text {
	gui::IGUIStaticText *dataText;
	wchar_t *text;
	//SColor PixCol;
	core::rect<s32> pos_in_s;
	bool active;
};

class MyEventReceiver : public irr::IEventReceiver{
public:
    MyEventReceiver(){KeyIsReleased=0;}
    virtual bool OnEvent(const irr::SEvent& event);
    unsigned short WasKeyPressed();
	void WasMousePressed(struct_sm_mouse *sm_mouse);
private:
    unsigned short KeyIsReleased;
    unsigned short mx;
    unsigned short my;
    unsigned short mr;
    unsigned short ml;
};

MyEventReceiver receiver;
IrrlichtDevice *device;
video::IVideoDriver* driver;	
gui::IGUIEnvironment* guienv;

u32 timeStamp;

struct_color gen_palette[256]; //di dsa-Palette, um Farben zu übersetzen
unsigned char mat[320][200];   //temporärer Speicher für die originale Pixelmatrix
char name[9];
irr::video::IImage *dataImage; //hier kommt das Bild hinein, bevor es in dit Textur umgewandelt wird
irr::video::IImage *dataImage32; //hier kommt das Bild hinein, bevor es in dit Textur umgewandelt wird
irr::video::IImage *dataImage128; //hier kommt das Bild hinein, bevor es in dit Textur umgewandelt wird
irr::video::SColor	fi_color;

irr::video::ITexture *smt_popup;
struct_image  smi_popup;
GUI_class* GUI;
struct_text  sm_popup_texts[20];
struct_text  sm_texts[50];

struct_image smi_heads[62];
struct_image smi_sex;
struct_image smi_flecha;
struct_image smi_dmenge[14];
struct_image smi_page[11];


void sm_init_video(){
	device = createDevice( video::EDT_SOFTWARE, core::dimension2d<u32>(dst_w,dst_h), 16, false, false, false, &receiver);
	device->setWindowCaption(L"Schicksalskline a la Boheme");
	driver = device->getVideoDriver();	
    guienv = device->getGUIEnvironment();
	gui::IGUISkin* skin = guienv->getSkin();
    //IGUIFont* font = guienv->getFont("C:\\Users\\hjr\\cpp\\BE\\smallfont.xml"); 
    gui::IGUIFont* font = guienv->getFont("arial14.xml"); 
    if(font)
        skin->setFont(font);
	skin->setColor (gui::EGDC_BUTTON_TEXT, video::SColor(255,255,255,255));
	dataImage   = driver->createImage(irr::video::ECF_A8R8G8B8 ,core::dimension2d<u32> (  dst_w,  dst_h));	
	dataImage32 = driver->createImage(irr::video::ECF_A8R8G8B8 ,core::dimension2d<u32> ( 32*f_w, 32*f_h));	
	dataImage128= driver->createImage(irr::video::ECF_A8R8G8B8 ,core::dimension2d<u32> (128*f_w,184*f_h));	
	
	timeStamp = device->getTimer()->getTime();
	device->getTimer()->start();
};
void sm_exit_video(){
    device->drop();
};
void sm_set_palette(Bit8u *ptr, unsigned char first_color, unsigned short colors){
	unsigned short i;
	for (i = 0; i < colors; i++){
		gen_palette[first_color+i].r=4*ptr[i*3];
		gen_palette[first_color+i].g=4*ptr[i*3+1];
		gen_palette[first_color+i].b=4*ptr[i*3+2];
	};
};

bool MyEventReceiver::OnEvent(const irr::SEvent& event){
	
    if (event.EventType == irr::EET_KEY_INPUT_EVENT)
		if (event.KeyInput.PressedDown){
			KeyIsReleased=event.KeyInput.Key;
		};
	if (event.EventType == irr::EET_MOUSE_INPUT_EVENT ){
		mx=event.MouseInput.X/f_w;
		my=event.MouseInput.Y/f_h;
		ml=0;
		mr=0;
		if (event.MouseInput.isLeftPressed()) ml=1;
		if (event.MouseInput.isRightPressed()) mr=1;
		if (event.MouseInput.Event==EMIE_LMOUSE_LEFT_UP) ml=2;
		if (event.MouseInput.Event==EMIE_RMOUSE_LEFT_UP) mr=2;
	};
    return false;
};
unsigned short MyEventReceiver::WasKeyPressed(){
	unsigned short K;
	K=KeyIsReleased;
    KeyIsReleased = 0;
	return K;
};
unsigned short sm_KeyPressed(){
	device->run();
	return receiver.WasKeyPressed();
};
void MyEventReceiver::WasMousePressed(struct_sm_mouse *sm_mouse){
	sm_mouse->x=mx;
	sm_mouse->y=my;
	sm_mouse->event = 0;
	if (ml==1) sm_mouse->event = MOUSE_LEFT_DOWN;
	if (ml==2) sm_mouse->event = MOUSE_LEFT_UP;
	if (mr==1) sm_mouse->event = MOUSE_RIGHT_DOWN;
	if (mr==2) sm_mouse->event = MOUSE_RIGHT_UP;
//if (sm_mouse->event>0) fprintf(stdout,"mevent: %d\n",sm_mouse->event);
	ml=0;
	mr=0;
};
void sm_GetMouse(struct_sm_mouse *sm_mouse){
	device->run();
	receiver.WasMousePressed(sm_mouse);
};
void wait_for_tick(){
	sm_images_draw_all(true);
	while (device->getTimer()->getTime() - timeStamp < 15){
		device->run();
		sm_sleep(1);
	}
	timeStamp = device->getTimer()->getTime();
};
unsigned short sm_delay_or_keypress(unsigned int delay){
	unsigned short k=0;
	struct_sm_mouse sm_mouse;
	while (delay>0){
		wait_for_tick();
		delay--;
		if (k=receiver.WasKeyPressed()) return k;
		receiver.WasMousePressed(&sm_mouse);
		if (sm_mouse.event!=0) return 0xffff;
	}
	return 0;
};
void find_color(double x, double y, unsigned short alpha_black){
	unsigned short x1,x2,y1,y2,quad,q1,q2,q3,q4,q5;
	double xy,r,g,b;
	x= (x - (f_w -1.0)/2.0)/f_w;
	if (x<0.  ) x=0.;
	if (x>319.) x=319.;
	y= (y - (f_h -1.0)/2.0)/f_h;
	if (y<0. ) y=0.;
	if (y>199.) y=199.;
	x1=(unsigned short)floor(x); x2=x1+1; if (x2>319) x2=319;
	y1=(unsigned short)floor(y); y2=y1+1; if (y2>319) y2=199;


	x -=x1;
	y -=y1;
	if (x<0.5){
		if(y<0.5) quad=1;
		else quad=4;
	}else{
		if(y<0.5) quad=2;
		else quad=3;
	}
	q1=mat[x1][y1];
	q2=mat[x2][y1];
	q3=mat[x2][y2];
	q4=mat[x1][y2];
	while (quad>1){
		quad--;
		q5=q1;
		q1=q2;
		q2=q3;
		q3=q4;
		q4=q5;
		xy = y;
		y = 1.0-x;
		x = xy;
		xy = 1.0 - x - y;
//fprintf(stdout,"%d, %d, %f, %f, %f\n",quad,q1,x,y,xy);
	}
//if ((x1>50)&&(x1<55)&&(y1>50)&&(y1<55))
//fprintf(stdout,"%f, %f, %f, %f,",x,y,xy,1-x-y);

	if (x+y<0.5){
		r = gen_palette[q1].r;
		g = gen_palette[q1].g;
		b = gen_palette[q1].b;
	}else{
		if((q1!=q3)&&(q2==q4)){
			r = gen_palette[q2].r;
			g = gen_palette[q2].g;
			b = gen_palette[q2].b;
		}else{
			r = gen_palette[q1].r;
			g = gen_palette[q1].g;
			b = gen_palette[q1].b;
		}
	}

	if (r+b+g==0) fi_color.set(alpha_black,r,g,b);
	else fi_color.set(255,r,g,b);
	
}
void sm_images_draw_popup_tile(unsigned short w, unsigned short h, unsigned short w1, unsigned short h1, unsigned char* src){
	unsigned short i,j,col;
	irr::video::SColor PixCol;
	for (j = 0; j<h; j++) {
		for (i = 0; i<w; i++) {
			col = readb(src++);
			mat[i][j]=col;
		}
	}
	for (j = 0; j<h*f_h; j++) {
		for (i = 0; i<w*f_w; i++) {
			find_color((double)i, (double)j,0);
			dataImage->setPixel(i+w1*f_w,j+h1*f_h, fi_color, false);
		}
	}
};

void sm_images_init_popup(unsigned char* src){
	irr::video::SColor PixCol(0,0,0,0);
	unsigned short  i,j;

	/* links oben */
    sm_images_draw_popup_tile(16,8,0,0,(unsigned char*) (src + 0));
	/* rechts oben */
    sm_images_draw_popup_tile(16,8,48,0,(unsigned char*)(src + 0x80));
	/* Mitte oben */
    sm_images_draw_popup_tile(32,8,16,0,(unsigned char*)(src + 0x380));
	
	/* links Text */
    sm_images_draw_popup_tile(16,8,0,8,src + 0x100);
	/* Mitte Text */
    sm_images_draw_popup_tile(32,8,16,8,src + 0x480);
	/* rechts Text */
    sm_images_draw_popup_tile(16,8,48,8,src + 0x180);
	/* links Option1 */
    sm_images_draw_popup_tile(16,8,0,16,src + 0x200);
	
	PixCol.set(255,gen_palette[0xd8].r,gen_palette[0xd8].g,gen_palette[0xd8].b);
	for (j = 0; j<4*f_h; j++) {
		for (i = 0; i<4*f_w; i++) {
			dataImage->setPixel(i+6*f_w,j+18*f_h, PixCol, false);			
		}
	}
	/* links Option2 */
    sm_images_draw_popup_tile(16,8,0,24,src + 0x200);
	PixCol.set(255,gen_palette[0xd9].r,gen_palette[0xd9].g,gen_palette[0xd9].b);
	for (j = 0; j<4*f_h; j++) {
		for (i = 0; i<4*f_w; i++) {
			dataImage->setPixel(i+6*f_w,j+26*f_h, PixCol, false);			
		}
	}

	/* links unten */
    sm_images_draw_popup_tile(16,8,0,32,src + 0x280);
	/* Mitte unten */
    sm_images_draw_popup_tile(32,8,16,32,src + 0x580);
	/* rechts unten */
    sm_images_draw_popup_tile(16,8,48,32,src + 0x300);
	/**/
	smt_popup = driver->addTexture("popup",dataImage);
};
void sm_images_draw_popup(){
	unsigned short x=GUI->left_border*f_w;
	unsigned short y=GUI->upper_border*f_h;
	unsigned short i,j;
	video::SColor PixCol(255,255,255,255);
	driver->draw2DImage(smt_popup, core::position2d<s32>(x,y), core::rect<s32>(0*f_w,0*f_h,16*f_w,8*f_h), 0, PixCol, true);
	x += 16*f_w;
	for ( j=0; j<GUI->menue_tiles; j++){
		driver->draw2DImage(smt_popup, core::position2d<s32>(x,y), core::rect<s32>(16*f_w,0*f_h,48*f_w,8*f_h), 0, PixCol, true);
		x += 32*f_w;
	}
	driver->draw2DImage(smt_popup, core::position2d<s32>(x,y), core::rect<s32>(48*f_w,0*f_h,64*f_w,8*f_h), 0, PixCol, true);
	y += 8*f_h; x = GUI->left_border*f_w;

	for ( i=0; i<GUI->header_lines; i++){
		driver->draw2DImage(smt_popup, core::position2d<s32>(x,y), core::rect<s32>(0*f_w,8*f_h,16*f_w,16*f_h), 0, PixCol, true);
		x += 16*f_w;
		for ( j=0; j<GUI->menue_tiles; j++){
			driver->draw2DImage(smt_popup, core::position2d<s32>(x,y), core::rect<s32>(16*f_w,8*f_h,48*f_w,16*f_h), 0, PixCol, true);
			x += 32*f_w;
		}
		driver->draw2DImage(smt_popup, core::position2d<s32>(x,y), core::rect<s32>(48*f_w,8*f_h,64*f_w,16*f_h), 0, PixCol, true);
		y += 8*f_h; x = GUI->left_border*f_w;
	}
  
	for ( i=0; i<GUI->options; i++){
		if (i+1==GUI->button){
			driver->draw2DImage(smt_popup, core::position2d<s32>(x,y), core::rect<s32>(0*f_w,24*f_h,16*f_w,32*f_h), 0, PixCol, true);
		}else{
			driver->draw2DImage(smt_popup, core::position2d<s32>(x,y), core::rect<s32>(0*f_w,16*f_h,16*f_w,24*f_h), 0, PixCol, true);
		};
		x += 16*f_w;
		for ( j=0; j<GUI->menue_tiles; j++){
			driver->draw2DImage(smt_popup, core::position2d<s32>(x,y), core::rect<s32>(16*f_w,8*f_h,48*f_w,16*f_h), 0, PixCol, true);
			x += 32*f_w;
		}
		driver->draw2DImage(smt_popup, core::position2d<s32>(x,y), core::rect<s32>(48*f_w,8*f_h,64*f_w,16*f_h), 0, PixCol, true);
		y += 8*f_h; x = GUI->left_border*f_w;
	}

	driver->draw2DImage(smt_popup, core::position2d<s32>(x,y), core::rect<s32>(0*f_w,32*f_h,16*f_w,40*f_h), 0, PixCol, true);
	x += 16*f_w;
	for ( j=0; j<GUI->menue_tiles; j++){
		driver->draw2DImage(smt_popup, core::position2d<s32>(x,y), core::rect<s32>(16*f_w,32*f_h,48*f_w,40*f_h), 0, PixCol, true);
		x += 32*f_w;
	}
	driver->draw2DImage(smt_popup, core::position2d<s32>(x,y), core::rect<s32>(48*f_w,32*f_h,64*f_w,40*f_h), 0, PixCol, true);
	
}
void sm_clean_text(char *text){
	unsigned char p;
	unsigned short i=0;
	while (p=text[i]){
		switch(p){
			case 0x40:{
				text[i]=0x20;
				i++;
				break;
				}
			case 0x8e:{
				text[i]=0xc4;
				i++;
				break;
				}
			case 0x99:{
				text[i]=0xd6;
				i++;
				break;
				}
			case 0x9a:{
				text[i]=0xdc;
				i++;
				break;
				}
			default:{
				i++;
				break;
				}
		}
	}
}
unsigned short sm_popup_line(unsigned short n,char *text,unsigned short x1,unsigned short y1,unsigned short x2,unsigned short y2){
	sm_clean_text(text);

	sm_popup_texts[n].pos_in_s = core::rect<s32>(x1*f_w,y1*f_h,x2*f_w,y2*f_h);

	const size_t cSize = strlen(text)+1;
	if (sm_popup_texts[n].text) free(sm_popup_texts[n].text);
	sm_popup_texts[n].text = (wchar_t*) malloc(2*cSize);
	mbstowcs (sm_popup_texts[n].text, text, cSize);

	if (! sm_popup_texts[n].dataText){
		sm_popup_texts[n].dataText = guienv->addStaticText(sm_popup_texts[n].text,sm_popup_texts[n].pos_in_s, false);
	}else{
		sm_popup_texts[n].dataText->setRelativePosition(sm_popup_texts[n].pos_in_s);
		sm_popup_texts[n].dataText->setText(sm_popup_texts[n].text);
	}
	sm_popup_texts[n].active = true;
	return (sm_popup_texts[n].dataText-> getTextHeight()/f_h + 8)/8;
};
unsigned short sm_print_line(unsigned short n,char *text,unsigned short x1,unsigned short y1,unsigned short x2,unsigned short y2){
	sm_clean_text(text);
	sm_texts[n].pos_in_s = core::rect<s32>(x1*f_w,y1*f_h,x2*f_w,y2*f_h);

	const size_t cSize = strlen(text)+1;
	if (sm_texts[n].text) free(sm_texts[n].text);
	sm_texts[n].text = (wchar_t*) malloc(2*cSize);
	mbstowcs (sm_texts[n].text, text, cSize);

	if (! sm_texts[n].dataText){
		sm_texts[n].dataText = guienv->addStaticText(sm_texts[n].text,sm_texts[n].pos_in_s, false);
	}else{
		sm_texts[n].dataText->setRelativePosition(sm_texts[n].pos_in_s);
		sm_texts[n].dataText->setText(sm_texts[n].text);
	}
	sm_texts[n].active = true;
	return (sm_texts[n].dataText-> getTextHeight()/f_h + 8)/8;
};
unsigned short sm_print_ral_line(unsigned short n,char *text,unsigned short x1,unsigned short y1){
	sm_clean_text(text);

	const size_t cSize = strlen(text)+1;
	if (sm_texts[n].text) free(sm_texts[n].text);
	sm_texts[n].text = (wchar_t*) malloc(2*cSize);
	mbstowcs (sm_texts[n].text, text, cSize);

	sm_texts[n].pos_in_s = core::rect<s32>(x1*f_w,y1*f_h,(x1+8*cSize)*f_w,(y1+8)*f_h);

	if (! sm_texts[n].dataText){
		sm_texts[n].dataText = guienv->addStaticText(sm_texts[n].text,sm_texts[n].pos_in_s, false);
	}else{
		sm_texts[n].dataText->setRelativePosition(sm_texts[n].pos_in_s);
		sm_texts[n].dataText->setText(sm_texts[n].text);
	}
	sm_texts[n].active = true;

	sm_texts[n].pos_in_s = core::rect<s32>(x1*f_w-sm_texts[n].dataText->getTextWidth(),y1*f_h,x1*f_w,(y1+8)*f_h);	
	sm_texts[n].dataText->setRelativePosition(sm_texts[n].pos_in_s);
	return (sm_texts[n].dataText-> getTextHeight()/f_h+8)/8;
};
void sm_activate_popup(GUI_class* GUI_){
	smi_popup.active = true;
	GUI=GUI_;
};
void sm_images_close_popup(){
	unsigned short i;
	smi_popup.active = false;
	for (i = 0; i<20; i++) {
		if (sm_popup_texts[i].active){
			sm_popup_texts[i].active = false;
		}
	}
};
void sm_images_init_heads(unsigned short n, unsigned char* src)
{
	video::SColor PixCol(0,0,0,0);
	unsigned short i,j,col;

	for (j = 0; j<32; j++) {
		for (i = 0; i<32; i++) {
			col = readb(src++);
			mat[i][j]=col;
		}
	}
	
	for (j = 0; j<32*f_h; j++) {
		for (i = 0; i<32*f_w; i++) {
			find_color((double)i, (double)j,0);
			dataImage32->setPixel(i,j, fi_color, false);
		};
	};		

	sprintf(name,"head%d", n+200);
	smi_heads[n].rec_in_t = core::rect<s32>(0,0,32*f_w,32*f_h); 
	smi_heads[n].pos_in_s = core::position2d<s32>(0,0);
	smi_heads[n].active =false;
	smi_heads[n].dataTexture = driver->addTexture(name,dataImage32);
};
void sm_images_init_sex(unsigned char* src)
{
	video::SColor PixCol(0,0,0,0);
	unsigned short i,j;
	
	for (j = 0; j<16; j++) 
		for (i = 0; i<16; i++) 
			mat[i][j]=readb(src++);
	for (j = 0; j<16*f_h; j++) {
		for (i = 0; i<16*f_w; i++) {
			find_color((double)i, (double)j,0);
			dataImage32->setPixel(i,j, fi_color, false);
		};
	};
	
	for (j = 0; j<16; j++) 
		for (i = 0; i<16; i++) 
			mat[i][j]=readb(src++);
	for (j = 0; j<16*f_h; j++) {
		for (i = 0; i<16*f_w; i++) {
			find_color((double)i, (double)j,0);
			dataImage32->setPixel(i+16*f_w,j, fi_color, false);
		};
	};
	
	for (j = 0; j<15; j++)
		for (i = 0; i<20; i++)
			mat[i][j]=readb(src++);
	for (j = 0; j<15*f_h; j++) {
		for (i = 0; i<20*f_w; i++) {
			find_color((double)i, (double)j,255);
			dataImage32->setPixel(i,j+16*f_h, fi_color, false);
		};
	};		

	sprintf(name,"sex%d", 12);
	smi_sex.rec_in_t = core::rect<s32>(16*f_w,0,32*f_w,16*f_h); 
	smi_sex.pos_in_s = core::position2d<s32>(305*f_w,7*f_h);
	smi_sex.active = false;
	smi_sex.dataTexture = driver->addTexture(name,dataImage32);
	sprintf(name,"sey%d", 12);
	smi_flecha;
	smi_flecha.rec_in_t = core::rect<s32>(0,16*f_h,20*f_w,31*f_h); 
	smi_flecha.pos_in_s = core::position2d<s32>(284*f_w,178*f_h);
	smi_flecha.active = false;
	smi_flecha.dataTexture = smi_sex.dataTexture;
};
void sm_images_init_typus(unsigned short n, unsigned char* src)
{
	video::SColor PixCol(0,0,0,0);
	unsigned short i,j,col;
	unsigned char* s;
	s=src+0x5c02;
	for (i = 0; i < 32; i++){
		gen_palette[i].r=4*readb(s++);
		gen_palette[i].g=4*readb(s++);
		gen_palette[i].b=4*readb(s++);
	};
	
	for (j = 0; j<184; j++)
		for (i = 0; i<128; i++)
			mat[i][j]=readb(src++);
	for (j = 0; j<184*f_h; j++) {
		for (i = 0; i<128*f_w; i++) {
			find_color((double)i, (double)j,0);
			dataImage128->setPixel(i,j, fi_color, false);
		}
	}
	sprintf(name,"typ%d", n);
	smi_dmenge[n].rec_in_t = core::rect<s32>(0,0,128*f_w,184*f_h); 
	smi_dmenge[n].pos_in_s = core::position2d<s32>(16*f_w,8*f_h);
	smi_dmenge[n].active = false;
	smi_dmenge[n].dataTexture = driver->addTexture(name,dataImage128);
};
void sm_images_init(unsigned short n, unsigned short w, unsigned short h, unsigned char* src)
{
	video::SColor PixCol(0,0,0,0);
	unsigned short i,j,col;

	for (j = 0; j<h; j++)
		for (i = 0; i<w; i++)
			mat[i][j]=readb(src++);
	for (j = 0; j<h*f_h; j++) {
		for (i = 0; i<w*f_w; i++) {
			find_color((double)i, (double)j,0);
			dataImage->setPixel(i,j, fi_color, false);
//fprintf(stdout,"col: %x, %x, %x\n",fi_color.getRed(),fi_color.getGreen(),fi_color.getBlue());
		}
	}
	sprintf(name,"p%d", n);
	smi_page[n].rec_in_t = core::rect<s32>(0,0,320*f_w,200*f_h); 
	smi_page[n].pos_in_s = core::position2d<s32>(0,0);
	smi_page[n].active = false;
	smi_page[n].dataTexture = driver->addTexture(name,dataImage);
fprintf(stdout,"init page: %d\n",n);
//for (i = 0; i<256; i++) fprintf(stdout,"col: %x, %x, %x\n",gen_palette[i].r,gen_palette[i].g,gen_palette[i].b);

};
void sm_images_do_typus(unsigned short n)
{
	unsigned short i;
	for (i = 0; i<11; i++) smi_dmenge[i].active=false;
	smi_dmenge[n].active = true;
};
void sm_images_do_page(unsigned short n)
{
	smi_page[n].active = true;
};
void sm_images_do_heads(unsigned short n, unsigned short x, unsigned short y)
{
	unsigned short i;
	for (i = 0; i<62; i++) smi_heads[i].active=false;
	smi_heads[n].active = true;
	smi_heads[n].pos_in_s = core::position2d<s32>(x*f_w,y*f_h);
};
void sm_images_do_sex(unsigned short sex, unsigned short lev)
{
	smi_sex.active = (sex==1);
	smi_flecha.active = (lev==1);
};










void sm_images_draw_all(bool remain){
	unsigned short i;
	driver->beginScene(true, true, video::SColor(255,0,0,0));
//struct_image smi_sex;

	for (i = 0; i<11; i++) {/* Hintergrund*/
		if (smi_page[i].active){
			driver->draw2DImage(smi_page[i].dataTexture, smi_page[i].pos_in_s,
				smi_page[i].rec_in_t, 0,video::SColor(255,255,255,255), true);
			smi_page[i].active = remain;
		}
	}
	if (smi_sex.active){
		driver->draw2DImage(smi_sex.dataTexture, smi_sex.pos_in_s,
			smi_sex.rec_in_t, 0,video::SColor(255,255,255,255), true);
		smi_sex.active = remain;
	}
	if (smi_flecha.active){
		driver->draw2DImage(smi_flecha.dataTexture, smi_flecha.pos_in_s,
			smi_flecha.rec_in_t, 0,video::SColor(255,255,255,255), true);
		smi_flecha.active = remain;
	}

	
	for (i = 0; i<14; i++) {/*Typ*/
		if (smi_dmenge[i].active){
			driver->draw2DImage(smi_dmenge[i].dataTexture, smi_dmenge[i].pos_in_s,
				smi_dmenge[i].rec_in_t, 0,video::SColor(255,255,255,255), true);
			smi_dmenge[i].active = remain;
		}
	}
	for (i = 0; i<62; i++) {/*Kopf*/
		if (smi_heads[i].active){
			driver->draw2DImage(smi_heads[i].dataTexture, smi_heads[i].pos_in_s,
				smi_heads[i].rec_in_t, 0,video::SColor(255,255,255,255), true);
			smi_heads[i].active = remain;
		}
	}



	for (i = 0; i<50; i++) {
		if (sm_texts[i].active){
			sm_texts[i].dataText->draw();
			sm_texts[i].active = remain;
		}
	}
	
	/*popup*/
	if (smi_popup.active){
		sm_images_draw_popup();
		smi_popup.active = remain;
	}
	for (i = 0; i<20; i++) {
		if (sm_popup_texts[i].active){
			sm_popup_texts[i].dataText->draw();
			sm_popup_texts[i].active = remain;
		}
	}

	driver->endScene();

}

