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
#include<sstream>
#include"psdefine.cpp"
#include"pspaste.cpp"
#include"psres.cpp"
#include"psstart.cpp"
#include"psimage.cpp"
#include"psfile.cpp"
#include"psinput.cpp"
#include"psabout.cpp"
#include"pswchar.cpp"
#include"psfont.cpp"
#include"psweb.cpp"
#include <shellapi.h>
#include<thread>
#include<mutex>
#include<deque>
using namespace std;
const int toolcnt = 10;//all tools cnt
bool hquality = 0;//high quality mode
int pdx=0,pdy=0;//photo display x,y
const int pmx=500,pmy=400;//photo max dusplay x,y
const int wnx=600,wny=pmy+ceil(toolcnt/10.0)*50+50;//window max x,y
const int initmode = SHOWCONSOLE;
const double math_pi = 3.14159265F;
const double deg2rad = 0.01745329251994329576923690F;
const double rad2deg = 1.0/deg2rad;
deque<IMAGE> imgtmp;
const int max_imgtmp = 20;
float displaysize = 1;
string imagefn = "lib\\psdef.png";
string savefile = "img\\tmp.png";
string exedir = ""; 
string backgroundfn = "lib\\bk.png";
HTYFONT putfont;
int _displaytmp = 20;
int imgw,imgh;//img w/h
IMAGE img,showimg,background;
void save_imgtmp(IMAGE t = img){
	if(imgtmp.size()>max_imgtmp){
		imgtmp.pop_front();
	}
	imgtmp.push_back(t);
}
void getexedir(){
	char path[MAX_PATH];
    DWORD len = GetModuleFileName(NULL, path, MAX_PATH);
    string t = string(path, len);
    exedir = t.substr(0,t.find_last_of("\\"))+"\\";
    cout<<"exedir "<<exedir<<endl;
}
void load_imgtmp(){
	if(imgtmp.size()>0){
		IMAGE t = imgtmp.back();
		imgtmp.pop_back();
		imgw = t.getwidth();
		imgh = t.getheight();
		img = t;
		if(imgtmp.size()==0)save_imgtmp();
	}
	else{
		cout<<"return image:imgtmp size is 0\n";
	}
}
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
IMAGE toolimg[toolcnt+2];//image of tools
int usetool = 0;//using tool number
COLORREF chcolor = RGB(72,210,230);//choose color
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
	imgtmp.clear();
	save_imgtmp();
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
	outtextxy(pmx+50,100,"Font");
	outtextxy(pmx+50,125,"add");
	outtextxy(pmx,150,"撤销");
	outtextxy(pmx,200,"查看");
	outtextxy(pmx,225,"信息");
	outtextxy(pmx+50,150,"保存");
	outtextxy(pmx+50,175,"7z");
	outtextxy(pmx+50,200,"URL");
	outtextxy(pmx+50,225,"下载");
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
		usetool = 999;displaytool();save_imgtmp();
	}
	else if(usetool==3){
		save_imgtmp();
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
		save_imgtmp();
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
		save_imgtmp();
		POS imgpos;
		getpos(&imgpos,pos);
		int x = imgpos.x;
		int y = imgpos.y;
		string str = inputstr("输入文字\n编码:ansi",1);
		SetWorkingImage(&img);
		settextstyle(putfont.lf.lfHeight,putfont.lf.lfWidth,putfont.name.c_str(),putfont.lf.lfEscapement,putfont.lf.lfOrientation,putfont.lf.lfWeight,putfont.lf.lfItalic,putfont.lf.lfUnderline,putfont.lf.lfStrikeOut,putfont.lf.lfCharSet,putfont.lf.lfOutPrecision,putfont.lf.lfClipPrecision,putfont.lf.lfQuality,putfont.lf.lfPitchAndFamily);
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
				//cout<<"dy:"<<dy<<" tmp:"<<tmp<<"\n";
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
		save_imgtmp();
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
		SetWorkingImage(&img);save_imgtmp();
		if(mode==3){
			Resize(&img,w,h);
		}
		else if(mode==1){
			IMAGE tmp = img;
			zoomimage(&img,&tmp,w/(double)imgw,1,h/(double)imgh);
		}
		else if(mode==2){
			IMAGE tmp = img;
			zoomimage(&img,&tmp,w/(double)imgw,0,h/(double)imgh);
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
void settwtfont(){
	string s = "安装专用字体选项";
	const vector<string> v = {"lib\\consola.ttf","lib\\consolab.ttf","lib\\consolai.ttf","lib\\consolaz.ttf","lib\\simkai.ttf","lib\\tiwate.ttf"};
	const vector<string> ms = {"Consolas (NORMAL)","Consolas (BOLD)","Consolas (ITALY)","Consolas (ITALY_BOLD)","楷体 (NORMAL)","提瓦特字体 (仅英文)",};
	int cnt = 0;
	for(auto i : v){
		s+="\n";
		s+=to_string(cnt);
		s+=".";
		s+=strsetw(i,33);
		s+=ms[cnt];
		cnt++;
	}
	int font = inputint(s,"5");
	int mode = inputint("选择模式\n1.安装\n2.删除","1");
	int r = -1;
	if(mode==1 && font<v.size()&&font>=0){
		r=addfont(v[font]);
	}
	else if(mode==2 && font<v.size()&&font>=0){
		r=delfont(v[font]);
	}
	else{
		cout<<"unknown font mode\n";
	}
	if(r==0){
		cout<<"修改成功,部分修改须重启软件方可应用\n";
	}
	cout<<"ended with return type:"<<r<<endl;
	return ;
}
std::string replaceAllSubstr(const std::string& str, const std::string& from, const std::string& to) {
    std::string result;
    size_t pos = 0;
    size_t prev = 0;
    while ((pos = str.find(from, prev)) != std::string::npos) {
        result += str.substr(prev, pos - prev);
        result += to;
        prev = pos + from.length(); 
    }
    result += str.substr(prev);
    return result;
}
string cutlabel(string s,string a){
	string s1 =":"+a;
	string s2 =":"+a+":";
	int p1 = s.find(s1)+s1.size()+1;
	int p2 = s.find(s2);
	int si = p2-p1-1;
	if(si<=0){
		cout<<"error:size is 0\n";
		return "";
	}
	string msg = s.substr(p1,si);
	return msg;
}
string cutlabelU8(string u8s,string a,bool isu8=0){
	if(isu8==0){
		string ansi = utf8_to_ansi(u8s);
		ansi = replaceAllSubstr(ansi,"\\n","\n");
		ansi = replaceAllSubstr(ansi,"\\\"","\"");
		string msg = cutlabel(ansi,a);
		return msg;
	}
	else{
		string str = u8s;
		str = replaceAllSubstr(str,"\\n","\n");
		str = replaceAllSubstr(str,"\\\"","\"");
		string msg = cutlabel(str,a);
		return msg;
	}
}
struct WEBMSG{
	string msg,downland,title,start,idyes,idno,u8msg;
};
WEBMSG hwebmsg;
void peekwebmsg(bool show = 1){
	//MessageBox(NULL,"即将从网络获取信息，请保障网络连接","Photoshop",MB_OK);
	cout<<"downland json file to lib\n";
	int r = urldownland2("http://123.60.188.246/api/get-discussion-detail?did=162",(exedir+"lib\\webmsg.json").c_str());
	cout<<"wait for downland\n";
	Sleep(200);
	if(r==1){
		cout<<"urlmon error:cannot downland\n";
		return;
	}
	cout<<"read json to string\n";
	//load file
	ifstream file((exedir+"lib\\webmsg.json").c_str());
	stringstream buffer;
    buffer << file.rdbuf();
	string s = buffer.str();
	file.close();
	cout<<"文件已保存在"<<exedir<<(exedir+"lib\\webmsg.json\n").c_str();
	//put text
	hwebmsg.msg = cutlabelU8(s,"webmsg");
	hwebmsg.u8msg = cutlabelU8(s,"webmsg",1);
	hwebmsg.downland = cutlabelU8(s,"downland");
	hwebmsg.title = cutlabelU8(s,"title");
	hwebmsg.start = cutlabelU8(s,"start");
	hwebmsg.idyes = cutlabelU8(s,"idyes");
	hwebmsg.idno = cutlabelU8(s,"idno");
	cout<<"====web msg====================\n";
	cout<<hwebmsg.msg<<endl;
	cout<<"====last downland==============\n";
	cout<<hwebmsg.downland<<endl;
	cout<<"====title======================\n";
	cout<<hwebmsg.title<<endl;
	cout<<"====start======================\n";
	cout<<hwebmsg.start<<endl;
	cout<<"====IDYES======================\n";
	cout<<hwebmsg.idyes<<endl;
	cout<<"====IDNO=======================\n";
	cout<<hwebmsg.idno<<endl;
	cout<<"===============================\n";
	if(show){
		int r = MessageBox(NULL,(hwebmsg.msg+"\n点击确认复制到剪切板").c_str(),"Photoshop 服务器信息",MB_YESNO);
		if(r==IDYES){
			str2paste(hwebmsg.u8msg);
		}
	}
	return;
}
vector<string> splstring(string s,char spl = '\n'){
	std::string text = s;
    std::istringstream stream(text);
    std::string line;
    std::vector<std::string> lines;
    while (std::getline(stream, line,spl)) {
        lines.push_back(line);
    }
    return lines;
}
void urldownfile(){
	string s1 = inputstr("url列表",1,"");
	string s2 = inputstr("保存文件名列表",1,"");
	int mode = inputint("模式\n1.单线程1.0\n2.单线程2.0\n3.多线程(实验)");
	vector<string>v1 = splstring(s1);
	vector<string>v2 = splstring(s2);
	if(v1.size()!=v2.size()){
		cout<<"大小不一致！\n";
		return;
	}
	if(mode==3)dxcdown(v1,v2);
	else if(mode==2){
		for(int i = 0;i<v1.size();i++){
			if(v1[i]==""||v2[i]=="") continue;
			cout<<"downland "<<i<<" of "<<v2.size()<<endl;
			urldownland2(v1[i],v2[i]);
		}
		cout<<"finish\n";
	}
	else{
		for(int i = 0;i<v1.size();i++){
			if(v1[i]==""||v2[i]=="") continue;
			cout<<"downland "<<i<<" of "<<v2.size()<<endl;
			urldownlandold(v1[i],v2[i]);
		}
		cout<<"finish\n";
	}
}
void mouseldcd(POS pos){
	if(usetool==6){
		defpc = inputint("输入容错","24");
		displaytool();
	}
	if(pos.x>pmx+50&&pos.x<pmx+100&&pos.y>0&&pos.y<50){
		paint_d = inputint("输入画笔粗细\n默认:1",to_string(paint_d));
		displaytool();
	}
	if(pos.x>pmx&&pos.x<pmx+50&&pos.y>50&&pos.y<100){
		putfont = inputfont();
		displaytool();
	}
	if(pos.x>pmx+50&&pos.x<pmx+100&&pos.y>50&&pos.y<100){
		cout<<"open Zfworld website 123.60.188.246\n";
		openurl("http://123.60.188.246/group/1025/discussion-detail/624");
	}
	if(pos.x>pmx&&pos.x<pmx+50&&pos.y>100&&pos.y<150){
		pdx = - inputint("移动到x坐标","0");
		pdy = - inputint("移动到y坐标","0");
		displaypic();
		displaytool();
	}
	if(pos.x>pmx+50&&pos.x<pmx+100&&pos.y>100&&pos.y<150){
		cout<<"reset font\n";
		settwtfont();
	}
	if(pos.x>pmx+0&&pos.x<pmx+50&&pos.y>150&&pos.y<200){
		cout<<"return image\n";
		load_imgtmp();
	}
	if(pos.x>pmx+50&&pos.x<pmx+100&&pos.y>150&&pos.y<200){
		cout<<"save .7z file\n";
		string fn = inputstr("保存压缩包文件名\n.zip .7z",0,"photo.7z");
		string pic = inputstr("保存图像文件名\n.jpg .png .bmp",0,"photo.png");
		int upw = inputint("是否保存密码\n1/0","0");
		string pw;
		if(upw==1){
			pw = inputstr("密码",0,"");
			cout<<"cur pw:"<<pw<<endl;
		}
		saveimage(((string)""+pic).c_str(),&img);
		system(((string)"del \\s \\q \""+fn+"\"").c_str());
		if(upw!=1)system(((string)exedir+"7z.exe a -mx9 -sdel -y -sccWIN \""+fn+"\" \""+pic+"\"").c_str());
		else system(((string)exedir+"7z.exe a -mx9 -sdel -y -p"+pw+" -sccWIN \""+fn+"\" \""+pic+"\"").c_str());
	}
	if(pos.x>pmx+0&&pos.x<pmx+50&&pos.y>200&&pos.y<250){
		cout<<"peek Internet Photoshop Message(IPM)\n";
		peekwebmsg();
	}
	if(pos.x>pmx+50&&pos.x<pmx+100&&pos.y>200&&pos.y<250){
		cout<<"URL downland file\n";
		int mode = inputint("下载管理\n1.从internet下载文件\n2.资源下载(对于精简版ps)","2");
		if(mode==1)urldownfile();
		if(mode==2)respack();
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
	string s = s1+" "+s2+" "+s3+" TOOL"+strsetw(to_string(usetool),3)+" CX"+strsetw(to_string(imgtmp.size()),2)+"    ";
	outtextxy(0,wny-50,s.c_str());
	settextstyle(24,0,"Microsoft YaHei UI");
	string t = "服务器信息:"+hwebmsg.title;
	outtextxy(0,wny-25,t.c_str());
}
int _last_mouse_sti_ld = 0;
bool isdrawingtool(int n){
	return n==0;
}
void mouserd(POS pos){
	if(pos.x>pmx+0&&pos.x<pmx+50&&pos.y>150&&pos.y<200){
		_displaytmp = inputint("可撤回的图像数量","20");
		displaytool();displayunder();
	}
}
int mainrange(){
	MOUSEMSG msg;
	msg = GetMouseMsg();
	if(msg.uMsg==WM_LBUTTONDOWN){
		_last_mouse_sti_ld = mouse_sti_ld;
		mouse_sti_ld = 1;
		if(msg.y>pmy&&msg.x<pmx){
			usetool = (msg.x/50+((msg.y-pmy)/50)*(pmx/50));
			displaytool();
		}
	}
	if(msg.uMsg==WM_LBUTTONDOWN&&msg.x>pmx){
		mouseldcd(POS(msg));
	}
	if(msg.uMsg==WM_LBUTTONUP){
		_last_mouse_sti_ld = mouse_sti_ld;
		mouse_sti_ld = 0;
		if(isdrawingtool(usetool)&&msg.y<pmy&&msg.x<pmx)save_imgtmp();
		tmp_drawcnt = 0;
	}
	if(mouse_sti_ld){
		mouseld(POS(msg));
	}
	if(msg.uMsg==WM_MBUTTONDOWN){
		setdisplayxy(POS(msg));//修改图形位置
	}
	if(msg.uMsg==WM_RBUTTONDOWN){
		mouserd(POS(msg));
	}
	if(msg.uMsg==WM_MOUSEWHEEL){
		int row = msg.wheel/WHEEL_DELTA;//120
		_displaytmp+=row;
		if(_displaytmp<=0) _displaytmp = 20;
		displaysize = _displaytmp*(1/20.0);
		printf("wheel row:%2d size:%.2f\n",row,displaysize);
		displaypic();
	}
	if((_last_mouse_sti_ld==1&&mouse_sti_ld==0) || tmp_drawcnt%show_once==0){
		_last_mouse_sti_ld =mouse_sti_ld;
		displaypic();
	}
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
	cout<<"init photoshop\nget exe dir\n";
	getexedir();//exe dir
	cout<<"get msg from http\n";
	peekwebmsg(0);
	if(hwebmsg.start.size()>0){
		int r = MessageBox(NULL,hwebmsg.start.c_str(),"Photoshop 服务器信息",MB_YESNOCANCEL|MB_ICONINFORMATION);
		if(r==IDCANCEL){
			//id cancel
		}
		if(r==IDYES){
			system(hwebmsg.idyes.c_str());
		}
		if(r==IDNO){
			system(hwebmsg.idno.c_str());
		}
	}
	cout<<"init graph\n";
	cout<<"easyx device v:"<<GetEasyXVer()<<endl;
	HWND hwnd = initgraph(wnx,wny,initmode);
	setwindow(hwnd);
	seticon(hwnd,"lib\\ps.ico");
	backgroundfn = exedir+backgroundfn;
	loadimage(&background,backgroundfn.c_str());
	cout<<"loading tool\n";
	for(int i = 0;i<toolcnt;i++){
		string fn = exedir+"lib\\tool"+to_string(i)+".png";
		loadimage(&toolimg[i],fn.c_str());
	}
	cout<<"loading image\n";
	for(int i = 0;i<_argc;i++){
		cout<<"argv "<<i<<":"<<_argv[i]<<endl; 
	}
	if(_argc==2) imagefn = (string)_argv[1];
	else if(_argc>2) cout<<"usage:psshow [imgdir\\imgname]\n";
	loadimg(0);
	cout<<"set font style\n";
	putfont.lf.lfHeight = 32;
	strcpy(putfont.lf.lfFaceName,"Cascadia Code");
	cout<<"display all\n";
	displaypic();
	displaytool();
	displayunder();
	rp_init();//for res pack
	return 0;
}
int main(int argc,char *argv[]){
	_argv = argv;
	_argc = argc;
	//psstart();
	initps();
	while(1){
		mainrange();
	}
	return 0;
}
//https://blog.csdn.net/qq_33430222/article/details/104852555
