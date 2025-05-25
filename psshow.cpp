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
#include"psabout.cpp"
#include"pswchar.cpp"
#include <shellapi.h>
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
int wnx=600,wny=475;//window max x,y
const double math_pi = 3.14159265F;
const double deg2rad = 0.01745329251994329576923690F;
const double rad2deg = 1.0/deg2rad;
float displaysize = 1;
string imagefn = "lib\\psdef.png";
string savefile = "img\\tmp.png";
const string backgroundfn = "lib\\bk.png";
HTYFONT putfont;
int _displaytmp = 20;
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
const int toolcnt = 10;//all tools cnt
IMAGE toolimg[toolcnt+2];//image of tools
int usetool = 0;//using tool number
COLORREF chcolor = RGB(72,210,230);//choose color
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
	img = t;
	imgw = img.getwidth();
	imgh = img.getheight();
	pdx=pdy=0;_displaytmp = 20;
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
const int setcnt = 10;
void displaytool(){
	for(int i = 0;i<toolcnt;i++){
		putimage(i*50,pmy+50*(i/(pmx/50)),&toolimg[i]);
		setlinecolor(usetool==i ? YELLOW : BLUE);
		rectangle(i*50,pmy+50*(i/(pmx/50)),i*50+49,pmy+50*(i/10)+49);
	}
	setfillcolor(chcolor);
	setlinecolor(RGB(148,148,148));
	fillrectangle(pmx+10,10,pmx+40,40);
	settextstyle(19,0,"Consolas");
	settextcolor(WHITE);
	outtextxy(pmx+50,0,"width");
	string s = strsetw(to_string(paint_d),4);
	outtextxy(pmx+50,24,s.c_str());
	outtextxy(pmx,50,"Font");
	outtextxy(pmx,75,"Size");
	setlinecolor(YELLOW);
	for(int i = 0;i<(setcnt+1)/2;i++){
		rectangle(pmx,i*50,pmx+49,i*50+49);
		rectangle(pmx+50,i*50,pmx+99,i*50+49);
	}
	outtextxy(pmx+50,50,"About");
	outtextxy(pmx,100,"Set");
	outtextxy(pmx,125,"pos");
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
	setlinecolor(RGB(148,148,148));
	line(0,pmy,pmx,pmy);
	line(pmx,0,pmx,pmy);
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
bool change2ansi = 0;
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
		int mode = inputint("选择处理模式\n1.按角度旋转\n2.左右翻转 bug\n3.垂直翻转 bug","1");
		if(mode==1){
			float rj = inputfloat("输入旋转角度(角度制)\n请慎重使用,不当操作会永久影响画质")*deg2rad;
			if(rj!=0&&rj!=360){
				SetWorkingImage(&img);
				IMAGE tmp = img;
				rotateimage(&img,&tmp,rj,BLACK,true,true);
				SetWorkingImage(NULL);
				imgw = img.getwidth();
				imgh = img.getheight();
			}
			usetool = 999;displaytool();
			displaypic();
		}
		else if(mode==2){
			IMAGE tmp = flipimage(img,1);
			img=tmp;
			usetool = 999;displaytool();
			displaypic();
		}
		else if(mode==3){
			IMAGE tmp = flipimage(img,0);
			img=tmp;
			usetool = 999;displaytool();
			displaypic();
		}
		else{
			cout<<"Unknown mode\n";
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
	else if(pos.x<pmx&&pos.y<pmy&&usetool==9){//draw text
		POS imgpos;
		getpos(&imgpos,pos);
		int x = imgpos.x;
		int y = imgpos.y;
		string str = inputstr("输入文字\n编码:ansi",1);
		SetWorkingImage(&img);
		settextstyle(putfont.lf.lfHeight,putfont.lf.lfWidth,putfont.name.c_str(),putfont.lf.lfEscapement
		,putfont.lf.lfOrientation,putfont.lf.lfWeight,putfont.lf.lfItalic,
		putfont.lf.lfUnderline,putfont.lf.lfStrikeOut,putfont.lf.lfCharSet,
		putfont.lf.lfOutPrecision,putfont.lf.lfClipPrecision,
		putfont.lf.lfQuality,putfont.lf.lfPitchAndFamily);
		settextcolor(chcolor);
		setbkmode(TRANSPARENT);
		string tmp = "";string ansi;
		int dy = y;
		for(char c : str){
			if(c=='\n'){
                if(change2ansi)ansi = utf8_to_ansi(tmp);
                else ansi = tmp;
				outtextxy(x,dy,ansi.c_str());
				dy+=abs(putfont.lf.lfHeight);
				cout<<"dy:"<<dy<<" tmp:"<<tmp<<"\n";
				tmp = "";
			}
			else if(c=='\t'){
				tmp+="    ";//for tab
			}
			else{
				tmp+=c;
			}
		}
        if(change2ansi)ansi = utf8_to_ansi(tmp);
        else ansi = tmp;
		outtextxy(x,dy,ansi.c_str());
		setbkmode(OPAQUE);
		SetWorkingImage(NULL);
		usetool = 999;displaytool();
		displaypic();
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
	else if(usetool==8){
		int h = inputint("修改后的高度",to_string(imgh));
		int w = inputint("修改后的宽度\n默认为等比例",to_string((int)(imgw/(imgh/(double)h))));
		int mode = inputint("缩放模式\n1.等比例缩放-高精度(仅放大)\n2.等比例缩放-低精度\n3.直接扩大缩小",(string)"2");
		SetWorkingImage(&img);
		if(mode==3){
			Resize(&img,w,h);
		}
		else if(mode==1){
			IMAGE tmp = img;
			zoomimage(&img,&tmp,h/(double)imgh,1,w/(double)imgw);
		}
		else if(mode==2){
			IMAGE tmp = img;
			zoomimage(&img,&tmp,h/(double)imgh,0,w/(double)imgw);
		}
		else{
			cout<<"unknown zoom mode\n";
		}
		SetWorkingImage(NULL);
		imgh = img.getheight();
		imgw = img.getwidth();
		usetool = 999;displaytool();
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
	if(pos.x>pmx&&pos.x<pmx+50&&pos.y>50&&pos.y<100){
		putfont = inputfont();
		displaytool();
	}
	if(pos.x>pmx+50&&pos.x<pmx+100&&pos.y>50&&pos.y<100){
		cout<<"open Zfworld website\n";
		openurl("http://123.60.188.246/group/1025/discussion-detail/624");
	}
	if(pos.x>pmx&&pos.x<pmx+50&&pos.y>100&&pos.y<150){
		pdx = - inputint("移动到x坐标","0");
		pdy = - inputint("移动到y坐标","0");
		displaypic();
		displaytool();
	}
}
bool mouse_sti_ld = 0;//still left down
const int show_once = 25;//卡顿(skyedu)指数
void displayunder(){
	settextstyle(24,0,"Consolas");
	settextcolor(WHITE);
	string s1 = "P("+strsetw(to_string(imgw),4)+","+strsetw(to_string(imgh),4)+")";
	string s2 = "S("+strsetw(to_string((int)(imgw*displaysize)),4)+","+strsetw(to_string((int)(imgh*displaysize)),4)+")";
	string s3 = "D("+strsetw(to_string(pdx),4)+","+strsetw(to_string(pdy),4)+")";
	string s = s1+" "+s2+" "+s3+" TOOL"+to_string(usetool)+"          ";
	outtextxy(0,wny-25,s.c_str());
}
int mainrange(){
	MOUSEMSG msg;
	msg = GetMouseMsg();
	if(msg.uMsg==WM_LBUTTONDOWN){
		mouse_sti_ld = 1;
		if(msg.y>pmy&&msg.x<pmx){
			usetool = (msg.x/50+((msg.y-pmy)/50)*(pmx/50));
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
		_displaytmp+=row;
		if(_displaytmp<=0) _displaytmp = 20;
		displaysize = _displaytmp*(1/20.0);
		printf("wheel row:%2d size:%.2f\n",row,displaysize);
		displaypic();
	}
	if(!mouse_sti_ld || tmp_drawcnt%show_once==0)displaypic();
	displayunder();
	return 0;
}
char **_argv = nullptr;
int _argc = 0;
int setwindow(HWND hwnd){
	SetWindowText(hwnd,"Photoshop");
	return 0;
}
int initps(){
	cout<<"init photoshop\n";
	cout<<"easyx device v:"<<GetEasyXVer()<<endl;
	HWND hwnd = initgraph(wnx,wny,SHOWCONSOLE);
	setwindow(hwnd);
	seticon(hwnd,"lib\\ps.ico");
	loadimage(&background,backgroundfn.c_str());
	cout<<"loading tool\n";
	for(int i = 0;i<toolcnt;i++){
		string fn = "lib\\tool"+to_string(i)+".png";
		loadimage(&toolimg[i],fn.c_str());
	}
	displaytool();
	cout<<"loading image\n";
	for(int i = 0;i<_argc;i++){
		cout<<"argv "<<i<<":"<<_argv[i]<<endl; 
	}
	if(_argc==2) imagefn = (string)_argv[1];
	loadimg(0);
	displaypic();
	putfont.lf.lfHeight = 32;
	strcpy(putfont.lf.lfFaceName,"Cascadia Code");
	return 0;
}
int main(int argc,char *argv[]){
	_argv = argv;
	_argc = argc;
	//psstart();
	initps();
	while(1)mainrange();
	return 0;
}
//https://blog.csdn.net/qq_33430222/article/details/104852555
