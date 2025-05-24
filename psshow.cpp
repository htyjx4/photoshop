#include<bits/stdc++.h>
#include<fstream>
#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>
#include<thread>
#include<mutex>
#include<windows.h>
#include<winuser.h>
#include<cmath>
#include<graphics.h>
#include<easyx.h>
#include"psstart.cpp"
#include"psimage.cpp"
#include"psfile.cpp"
#include"psinput.cpp"
using namespace std;
struct POS{
	short x,y;
	POS(){};
	POS(int lx,int ly){
		x=lx;y=ly;
	}
	POS(ExMessage msg){
		x=msg.x;
		y=msg.y;
	}
	POS(MOUSEMSG msg){
		x=msg.x;
		y=msg.y;
	}
};
typedef POS *PPOS;
bool hquality = 0;
int pdx=0,pdy=0;//photo display x,y
int pmx=500,pmy=400;//photo max dusplay x,y
int wnx=600,wny=450;//window max x,y
const double math_pi = 3.14159265;
const double deg2rad = 0.01745329251994329576923690F;
float displaysize = 1;
string imagefn = "lib\\psdef.png";
string savefile = "img\\tmp.png";
const string backgroundfn = "lib\\bk.png";
int setdisplayxy(POS pos){
	POS p1;
	cout<<"pdx wait for moving\n";
	while(1){
		MOUSEMSG msg = GetMouseMsg();
		if(msg.uMsg==WM_MBUTTONUP){
			p1 = POS(msg);
			break;
		}
	}
	pdx += p1.x-pos.x;
	pdy += p1.y-pos.y;
	printf("pdx:%4d pdy:%4d\n",pdx,pdy);
	return 0;
}
IMAGE img,showimg,background;
const int toolcnt = 8;//all tools cnt
IMAGE toolimg[toolcnt+2];//image of tools
int usetool = 0;//using tool number
COLORREF chcolor = RED;//choose color
int imgw,imgh;//img w/h
bool getpos(POS* ipos,POS spos){//screen pos
	float dp = 1/displaysize;
	int ix = (spos.x-pdx)*dp;
	int iy = (spos.y-pdy)*dp;
	if(ix>=0&&iy>=0&&
	ix<(imgw*dp)&&iy<(imgh*dp)){
		ipos->x = ix;
		ipos->y = iy;
		return 1;
	}
	else{
		return 0;
	}
}
int loadimg(bool ifn=1){
	if(ifn)imagefn = getopenimgname();
	IMAGE t;
	loadimage(&t,imagefn.c_str());
	img =  t;
	imgw = img.getwidth();
	imgh = img.getheight();
	pdx=pdy=0;displaysize = 1;
}
int tmp_drawcnt = 0;
void printcolor(COLORREF c){
	int r = GetRValue(c);
	int g = GetGValue(c);
	int b = GetBValue(c);
	printf("COLORREF(%3dr,%3dg,%3db)\n",r,g,b);
}
string strsetw(string s,int w){
	int l = s.length();
	if(l==w) return s;
	if(l>w) return s.substr(0,l);
	if(l<w){
		string t = s;
		for(int i = 0;i<w-l;i++) t+=" ";
		return t;
	}
	return "";
}
int defpc = 5;
int paint_d = 1;
void displaytool(){
	for(int i = 0;i<toolcnt;i++){
		putimage(i*50,pmy,&toolimg[i]);
		setlinecolor(usetool==i ? YELLOW : BLUE);
		rectangle(i*50,pmy,i*50+49,pmy+49);
	}
	setfillcolor(chcolor);
	setlinecolor(RGB(148,148,148));
	fillrectangle(pmx+10,10,pmx+40,40);
	settextstyle(19,0,"");
	outtextxy(pmx+50,0,"width");
	string s = strsetw(to_string(paint_d),4);
	outtextxy(pmx+50,24,s.c_str());
}
bool colorequ (COLORREF a,COLORREF b,int pc){
	bool t1 = abs(GetRValue(a)-GetRValue(b))<=pc;
	bool t2 = abs(GetGValue(a)-GetGValue(b))<=pc;
	bool t3 = abs(GetBValue(a)-GetBValue(b))<=pc;
	return t1&&t2&&t3;
}
const int _dx[4] = {0,1,0,-1};
const int _dy[4] = {1,0,-1,0};
long long dfscnt = 0;

void displaypic(){
	zoomimage(&showimg,&img,displaysize,hquality);
	putimage(0,0,pmx,pmy,&background,0,0);
	putimage(pdx,pdy,pmx-pdx,pmy-pdy,&showimg,0,0);
}
bool _vis[5000][3600];
void dfsfull(COLORREF color,COLORREF cput,POS pos,int pc = 5,int c = 0){
	int x = pos.x;
	int y = pos.y;
	if(x>=0&&y>=0&&x<imgw&&y<imgh&&c<25000&&!_vis[x][y]){
		bool f = colorequ(getpixel(x,y),color,pc);
		_vis[x][y] = 1;
		if(f){
			putpixel(x,y,cput);
			dfscnt++;
			if(dfscnt%1000==0){
				printf("cnt:%7lld x%4d y%4d c:%4d\n",dfscnt,x,y,c);
				//displaypic();
			}
			for(int i = 0;i<4;i++){
				int dx = x+_dx[i];
				int dy = y+_dy[i];
				dfsfull(color,cput,POS(dx,dy),pc,c+1);
			}
		}
	}
	else{
		return;
	}
}
void mouseld(POS pos){
	if(pos.x<pmx&&pos.y<pmy&&usetool==0){
		POS imgpos;
		if(getpos(&imgpos,pos)){
			SetWorkingImage(&img);
			if(paint_d==1){
				putpixel(imgpos.x,imgpos.y,chcolor);
			}
			else{
				int rad = paint_d/2.0;
				setfillcolor(chcolor);
				solidcircle(imgpos.x,imgpos.y,rad);
			}
			SetWorkingImage(NULL);
			tmp_drawcnt++;
		}
	}
	else if(usetool==1){
		savefile = getsaveimgname();
		string s = "是否保存为\n"+savefile;
		int t = MessageBox(NULL,s.c_str(),"Photoshop",MB_ICONINFORMATION|MB_YESNO);
		if(t==IDNO&&s!=""){
			cout<<"choose IDNO\n";
		}
		else{
			cout<<"choose IDYES\n";
			saveimage(savefile.c_str(),&img);
		}
		usetool = 999;displaytool();
	}
	else if(usetool==3){
		loadimg();
		usetool = 999;displaytool();
	}
	else if(usetool==2){
		int t = MessageBox(NULL,"是否退出Photoshop","Photoshop",MB_ICONINFORMATION|MB_YESNO);
		if(t==IDNO){
			cout<<"choose IDNO\n";
		}
		else{
			cout<<"choose IDYES\n";
			closegraph();
			exit(0);
		}
		usetool = 999;displaytool();
	}
	else if(usetool==4){
		chcolor = choosecolor();
		printcolor(chcolor);
		usetool = 999;displaytool();
	}
	else if(usetool==7){
		float rj = inputfloat("输入旋转角度(角度制)\n请慎重使用,不当操作会永久影响画质")*deg2rad;
		if(rj!=0&&rj!=360){
			SetWorkingImage(&img);
			IMAGE tmp = img;
			rotateimage(&img,&tmp,rj,BLACK,true,true);
			SetWorkingImage(NULL);
			imgw = img.getwidth();
			imgh = img.getheight();
			usetool = 999;displaytool();
			displaypic();
		}
	}
	else if(pos.x<pmx&&pos.y<pmy&&usetool==5){
		POS imgpos;
		getpos(&imgpos,pos);
		int x = imgpos.x;
		int y = imgpos.y;
		SetWorkingImage(&img);
		chcolor = getpixel(x,y);
		SetWorkingImage(NULL);
		printcolor(chcolor);
		displaytool();
	}
	else if(pos.x<pmx&&pos.y<pmy&&usetool==6){
		POS imgpos;
		getpos(&imgpos,pos);
		int x = imgpos.x;
		int y = imgpos.y;
		SetWorkingImage(&img);
		COLORREF col = getpixel(x,y);
		dfscnt=0;
		cout<<"memset\n";
		memset(_vis,0,sizeof _vis);
		dfsfull(col,chcolor,imgpos,defpc);
		cout<<"reset working img\n";
		SetWorkingImage(NULL);
		cout<<"try fill "<<dfscnt<<" pixels\n";
		dfscnt=0;
		displaypic();
	}
}
void mouserd(POS pos){
	if(usetool==6){
		defpc = inputint("输入容错\n默认:5");
		displaytool();
	}
	if(pos.x>pmx+50&&pos.x<pmx+100&&pos.y>0&&pos.y<50){
		paint_d = inputint("输入画笔粗细\n默认:1");
		displaytool();
	}
}
bool mouse_sti_ld = 0;//still left down
const int show_once = 25;//卡顿(skyedu)指数
int mainrange(){
	MOUSEMSG msg;
	msg = GetMouseMsg();
	if(msg.uMsg==WM_LBUTTONDOWN){
		mouse_sti_ld = 1;
		if(msg.y>pmy&&msg.x<pmx){
			usetool = (msg.x/50);
			displaytool();
		}
	}
	if(msg.uMsg==WM_RBUTTONDOWN){
		mouserd(POS(msg));
	}
	if(msg.uMsg==WM_LBUTTONUP){
		mouse_sti_ld = 0;
		tmp_drawcnt = 0;
	}
	if(mouse_sti_ld){
		mouseld(POS(msg));
	}
	if(msg.uMsg==WM_MBUTTONDOWN){
		setdisplayxy(POS(msg));//修改图形位置
	}
	if(msg.uMsg==WM_MOUSEWHEEL){
		int row = msg.wheel/120;
		double oldsize = displaysize;
		displaysize += row/20.0;
		if(displaysize<=0) displaysize = 1;
		printf("wheel row:%2d size:%.2f\n",row,displaysize);
		displaypic();
	}
	if(!mouse_sti_ld || tmp_drawcnt%show_once==0)displaypic();
	return 0;
}
int initps(){
	cout<<"init photoshop\n";
	HWND hwnd = initgraph(wnx,wny,SHOWCONSOLE);
	seticon(hwnd,"lib\\ps.ico");
	loadimage(&background,backgroundfn.c_str());
	cout<<"loading tool\n";
	for(int i = 0;i<toolcnt;i++){
		string fn = "lib\\tool"+to_string(i)+".png";
		loadimage(&toolimg[i],fn.c_str());
	}
	displaytool();
	cout<<"loading image\n";
	loadimg(0);
	displaypic();
	return 0;
}
int main(){
	//psstart();
	initps();
	while(1)mainrange();
	return 0;
}

