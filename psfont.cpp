#include<bits/stdc++.h>
#include <shlobj.h>//-lgdi32
#include <windows.h>
using namespace std;
int addfont(string s = "lib\\tiwate.ttf"){
	LPCSTR fontFilePath = s.c_str();
    if (AddFontResourceEx(fontFilePath, FR_PRIVATE, NULL)) {
        return 0;//ok
    } else {
        return 1;//error
    }
}
int delfont(string s = "lib\\tiwate.ttf"){
	LPCSTR fontFilePath = s.c_str();
    if (RemoveFontResourceEx(fontFilePath, FR_PRIVATE, NULL)) {
        return 0;//ok
    } else {
        return 1;//error
    }
}
