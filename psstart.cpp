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
using namespace std;//-leasyx -std=c++14
void seticon(HWND hwnd,string icon) {
	HICON hIcon = (HICON)LoadImage(NULL,icon.c_str(),IMAGE_ICON,0,0,LR_LOADFROMFILE | LR_DEFAULTSIZE);
    if (hIcon) {
        SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
        SendMessage(hwnd, WM_SETICON, ICON_SMALL2, (LPARAM)hIcon);
        SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
    }
}
void psstart(){
	HWND hwnd = initgraph(1415/2,936/2,SHOWCONSOLE);
	seticon(hwnd,"lib\\ps.ico");
	IMAGE imgstart;
	loadimage(&imgstart,"lib\\psstart.png",1415/2,936/2);
	vector<string> v = {
	"加载基本模块","加载滤镜",
	"加载控件","获取系统信息",
	"请稍后","准备开启"};
	settextstyle(36,0,"Cascadia Code");
	setbkcolor(WHITE);
	settextcolor(BLACK);
	for(int i = 0;i<v.size();i++){
		putimage(0,0,&imgstart,SRCCOPY);
		string s = v[i]+"   ";
		outtextxy(40/2,440/2,s.c_str());
		Sleep(100);
	}
	closegraph();
}
char* u82ansi(wchar_t* pWCStrKey){
	int pSize = WideCharToMultiByte(CP_OEMCP, 0, pWCStrKey, wcslen(pWCStrKey), NULL, 0, NULL, NULL);
	char* pCStrKey = new char[pSize + 1];
	WideCharToMultiByte(CP_OEMCP, 0, pWCStrKey, wcslen(pWCStrKey), pCStrKey, pSize, NULL, NULL);
	pCStrKey[pSize] = '\0';
	return pCStrKey;
}
wchar_t* readstrbox(int n,wchar_t* pPrompt,wchar_t* pTitle){
	wchar_t s[n+2];
	memset(s,0,sizeof s);
	InputBox((LPTSTR)s,n,(LPCTSTR)pPrompt,(LPCTSTR)pTitle);
	return s;
}
