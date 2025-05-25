#include<bits/stdc++.h>
#include<iostream>
#include<cstdio>
#include<cmath>
#include<queue>
#include <shellapi.h>
using namespace std;
int openurl(string url1){
	string s= "start "+url1;
	return system(s.c_str());
}

