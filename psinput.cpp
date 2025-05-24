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
using namespace std;
int inputint(string prompt) {
    char s[15];
    memset(s,0,sizeof s);
	InputBox(s, 10,prompt.c_str(),"Photoshop");
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
