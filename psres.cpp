#include<bits/stdc++.h>
#include<fstream>
#include"psweb.cpp"
#include"psinput.cpp"
using namespace std;
//psres.cpp
struct HRESPACK{
	string url,fn,copy_to,zip_to,name;
	int downland(){
		cout<<"downland respack from internet\n";
		cout<<"开始下载,可能会有亿点点慢(powered by IE)\n";
		urldownland2(url,fn);
		
		cout<<"copy file\n";
		string cmd = "copy \""+fn+"\" \""+copy_to+"\"";
		cout<<"cmd:"<<cmd<<endl;
		system(cmd.c_str());
		
		cmd = "7z.exe e -y -sccWIN -o"+zip_to+" -sdel \""+copy_to+"\"";
		cout<<"cmd:"<<cmd<<endl;
		system(cmd.c_str());
		
		cmd = "del /s \""+fn+"\"";
		cout<<"cmd:"<<cmd<<endl;
		system(cmd.c_str());
		cout<<"all finish\n";
	}
};
/*
HRESPACK rp_info[15] ={
	{"WAIT FOR LOAD","font.zip","lib\\font.zip","lib\\","可选字体文件"},
	{"WAIT FOR LOAD","image.zip","img\\image.zip","img\\","可选图片文件"},
};*/
vector<HRESPACK> rp_info;
int rescnt = 0;
int rp_init(){
	cout<<"respack init\n";
	ifstream ifs("setting\\respack.ini");
	while(1){
		HRESPACK tmp;
		getline(ifs,tmp.url);
		if(tmp.url=="stop") break;
		getline(ifs,tmp.fn);
		getline(ifs,tmp.copy_to);
		getline(ifs,tmp.zip_to);
		getline(ifs,tmp.name);
		rp_info.push_back(tmp);
	}
	rescnt = rp_info.size();
	ifs.close();
}
int rp_load(int n){
	if(n>=0 && n<rescnt){
		cout<<"rp_load rp_info\\"<<n<<endl;
		rp_info[n].downland();
	}
	else{
		cout<<"input error\n";
	}
}
int respack(){
	cout<<"loading respack\n";
	string s = "选择\n";
	for(int i = 0;i<rescnt;i++){
		s+=to_string(i);
		s+="."+rp_info[i].name+"\n";
	}
	s+="资源可能需要加速器(如steamcommunity302)";
	int type = inputint(s,"");
	rp_load(type);
}
