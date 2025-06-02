#include<bits/stdc++.h>
#include <windows.h>
#include <commdlg.h>
#include <CommCtrl.h>
#include <iostream>//-lcomdlg32 -lcomctl32
#pragma comment(lib, "Comdlg32.lib")
#pragma comment(lib, "comctl32.lib")
using namespace std;
BOOL CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lp, LPARAM pData){
    // 回调函数，通常不需要实现，除非你需要处理特殊情况
    return FALSE;
}
string getsavefilename(char * filt) {
    OPENFILENAME ofn;       // 创建 OPENFILENAME 结构体
    char szFile[260];       // 用于保存文件名的缓冲区
    ZeroMemory(&ofn, sizeof(ofn));
    szFile[0] = '\0';
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL; // 可以指定父窗口句柄，这里设为NULL表示无父窗口
    ofn.lpstrFile = szFile; // 指向文件名缓冲区
    ofn.lpstrFile[0] = '\0'; // 初始化为空字符串
    ofn.nMaxFile = sizeof(szFile); // 缓冲区大小
    ofn.lpstrFilter = filt; // 文件类型过滤器
    ofn.nFilterIndex = 1; // 设置默认的文件类型过滤器索引
    ofn.lpstrFileTitle = NULL; // 不需要文件名标题，设为NULL
    ofn.lpstrInitialDir = NULL; // 初始目录，设为NULL表示使用默认目录
    ofn.Flags = OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY; // 设置对话框选项
    // 这里可以添加其他选项，例如 OFN_EXPLORER 等
    if (GetSaveFileName(&ofn)) { // 显示对话框并获取结果
        return std::string(szFile); // 返回用户选择的文件名
    } else {
        return ""; // 用户取消或出错时返回空字符串
    }
}
string getopenfilename(char * filt) {
    OPENFILENAME ofn;       // 创建 OPENFILENAME 结构体
    char szFile[260];       // 用于保存文件名的缓冲区
    ZeroMemory(&ofn, sizeof(ofn));
    szFile[0] = '\0';
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL; // 可以指定父窗口句柄，这里设为NULL表示无父窗口
    ofn.lpstrFile = szFile; // 指向文件名缓冲区
    ofn.lpstrFile[0] = '\0'; // 初始化为空字符串
    ofn.nMaxFile = sizeof(szFile); // 缓冲区大小
    ofn.lpstrFilter = filt; // 文件类型过滤器
    ofn.nFilterIndex = 1; // 设置默认的文件类型过滤器索引
    ofn.lpstrFileTitle = NULL; // 不需要文件名标题，设为NULL
    ofn.lpstrInitialDir = NULL; // 初始目录，设为NULL表示使用默认目录
    ofn.Flags = OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY; // 设置对话框选项
    // 这里可以添加其他选项，例如 OFN_EXPLORER 等
    if (GetOpenFileName(&ofn)) { // 显示对话框并获取结果
        return std::string(szFile); // 返回用户选择的文件名
    } else {
        return ""; // 用户取消或出错时返回空字符串
    }
}
string getsaveimgname(){
	char c[] = "png图片(.png)\0*.png\0gif图片(.gif)\0*.gif\0bmp图片(.bmp)\0*.bmp\0tif图片(.tif)\0*.tif\0jpeg图片(.jpg)\0*.jpg\0所有文件(*.*)\0*.*\0\0";
	return getsavefilename(c);
}
string getopenimgname(){
	char c[] = "png图片(.png)\0*.png\0gif图片(.gif)\0*.gif\0bmp图片(.bmp)\0*.bmp\0tif图片(.tif)\0*.tif\0jpeg图片(.jpg)\0*.jpg\0所有文件(*.*)\0*.*\0\0";
	return getopenfilename(c);
}
COLORREF _psfile_CustomColors[20];
COLORREF choosecolor(){
	CHOOSECOLOR cc;
    ZeroMemory(&cc, sizeof(cc));
    cc.lStructSize = sizeof(cc);
    cc.hwndOwner = NULL;
    cc.rgbResult = RGB(255,255,255);
    cc.lpCustColors = _psfile_CustomColors;
    cc.Flags = CC_FULLOPEN | CC_RGBINIT | CC_ANYCOLOR;
    BOOL bRet = ChooseColor(&cc);
    if (bRet) {
        return cc.rgbResult;
    } else {
        return RGB(255,255,255);
    }
}
