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
#pragma once
using namespace std;
int inputint(string prompt,string pDefault = "") {
    char s[15];
    memset(s,0,sizeof s);
	InputBox(s, 10,prompt.c_str(),"Photoshop",pDefault.c_str());
	bool f=0;
	for(int i = 0;i<10;i++){
		if(!(s[i]>='0'&&s[i]<='9') && s[i]!='\0' &&!(s[i]=='-'&&i==0)) f = 1;
	}
	if(f) return 0;
	int r = atoi(s);
	cout<<"inputint return:"<<r<<endl;
    return r;
}
double inputfloat(string prompt) {
    char s[15];
    memset(s,0,sizeof s);
	InputBox(s, 10,prompt.c_str(),"Photoshop");
	double r = atof(s);
	cout<<"inputfloat return:"<<r<<endl;
    return r;
}
string inputstr(string prompt,bool duohang = 0,string mr ="") {
    char s[16385];
    memset(s,0,sizeof s);
	InputBox(s, 16384,prompt.c_str(),"Photoshop",mr.c_str(),0,(duohang?200:0),1);
    return (string)s;
}
struct HTYFONT{
	LOGFONT lf;
	string name;
	HTYFONT(){}
	void put(){
		printf("===HTYFONT INFO===\nh%d w%d esc%d ori%d wei%d\nitalic%d underline%d strike%d\ncharset%d pre%d quality%d family%d"
		,lf.lfHeight,lf.lfWidth,lf.lfEscapement
		,lf.lfOrientation,lf.lfWeight,lf.lfItalic,
		lf.lfUnderline,lf.lfStrikeOut,lf.lfCharSet,
		lf.lfOutPrecision,lf.lfClipPrecision,
		lf.lfQuality,lf.lfPitchAndFamily);
		cout<<"\nname:"<<name<<endl;
		switch(lf.lfCharSet){
			case ANSI_CHARSET:
				cout<<"ANSI_CHARSET"<<endl;
				break;
			case CHINESEBIG5_CHARSET:
				cout<<"CHINESEBIG5_CHARSET"<<endl;
				break;
			case DEFAULT_CHARSET:
				cout<<"DEFAULT_CHARSET"<<endl;
				break;
			case GB2312_CHARSET:
				cout<<"GB2312_CHARSET"<<endl;
				break;
			case MAC_CHARSET:
				cout<<"CMAC_CHARSETOUT"<<endl;
				break;
			case BALTIC_CHARSET:
				cout<<"BALTIC_CHARSET"<<endl;
				break;
			case EASTEUROPE_CHARSET:
				cout<<"EASTEUROPE_CHARSET"<<endl;
				break;
			case GREEK_CHARSET:
				cout<<"GREEK_CHARSET"<<endl;
				break;
			case HANGUL_CHARSET:
				cout<<"HANGUL_CHARSET"<<endl;
				break;
			case OEM_CHARSET:
				cout<<"OEM_CHARSET"<<endl;
				break;
			case RUSSIAN_CHARSET:
				cout<<"RUSSIAN_CHARSET"<<endl;
				break;
			case SHIFTJIS_CHARSET:
				cout<<"SHIFTJIS_CHARSET"<<endl;
				break;
			case SYMBOL_CHARSET:
				cout<<"SYMBOL_CHARSET"<<endl;
				break;
			case TURKISH_CHARSET:
				cout<<"TURKISH_CHARSET"<<endl;
				break;
		}
	}
};
typedef HTYFONT *PHTYFONT;
HTYFONT inputfont(){
	CHOOSEFONT cf;
    LOGFONT lf;
    memset(&lf, 0, sizeof(LOGFONT));
    lf.lfHeight = 24;
    strcpy(lf.lfFaceName,"Cascadia Code");
    memset(&cf, 0, sizeof(CHOOSEFONT));
    cf.lStructSize = sizeof(CHOOSEFONT);
    cf.hwndOwner = NULL; // 可以设置为父窗口句柄
    cf.lpLogFont = &lf;
    cf.Flags = CF_BOTH | CF_INITTOLOGFONTSTRUCT | CF_EFFECTS;
	HTYFONT font;
    if (ChooseFont(&cf)) {
    	font.name = lf.lfFaceName;
    	font.lf = lf;
    	font.put();
    } else {
    	cout<<"not ok\n";
    }
    return font;
}
