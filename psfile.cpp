#include<bits/stdc++.h>
#include <windows.h>
#include <commdlg.h>
#include <CommCtrl.h>
#include <iostream>//-lcomdlg32 -lcomctl32
#pragma comment(lib, "Comdlg32.lib")
#pragma comment(lib, "comctl32.lib")
using namespace std;
BOOL CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lp, LPARAM pData){
    // �ص�������ͨ������Ҫʵ�֣���������Ҫ�����������
    return FALSE;
}
string getsavefilename(char * filt) {
    OPENFILENAME ofn;       // ���� OPENFILENAME �ṹ��
    char szFile[260];       // ���ڱ����ļ����Ļ�����
    ZeroMemory(&ofn, sizeof(ofn));
    szFile[0] = '\0';
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL; // ����ָ�������ھ����������ΪNULL��ʾ�޸�����
    ofn.lpstrFile = szFile; // ָ���ļ���������
    ofn.lpstrFile[0] = '\0'; // ��ʼ��Ϊ���ַ���
    ofn.nMaxFile = sizeof(szFile); // ��������С
    ofn.lpstrFilter = filt; // �ļ����͹�����
    ofn.nFilterIndex = 1; // ����Ĭ�ϵ��ļ����͹���������
    ofn.lpstrFileTitle = NULL; // ����Ҫ�ļ������⣬��ΪNULL
    ofn.lpstrInitialDir = NULL; // ��ʼĿ¼����ΪNULL��ʾʹ��Ĭ��Ŀ¼
    ofn.Flags = OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY; // ���öԻ���ѡ��
    // ��������������ѡ����� OFN_EXPLORER ��
    if (GetSaveFileName(&ofn)) { // ��ʾ�Ի��򲢻�ȡ���
        return std::string(szFile); // �����û�ѡ����ļ���
    } else {
        return ""; // �û�ȡ�������ʱ���ؿ��ַ���
    }
}
string getopenfilename(char * filt) {
    OPENFILENAME ofn;       // ���� OPENFILENAME �ṹ��
    char szFile[260];       // ���ڱ����ļ����Ļ�����
    ZeroMemory(&ofn, sizeof(ofn));
    szFile[0] = '\0';
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL; // ����ָ�������ھ����������ΪNULL��ʾ�޸�����
    ofn.lpstrFile = szFile; // ָ���ļ���������
    ofn.lpstrFile[0] = '\0'; // ��ʼ��Ϊ���ַ���
    ofn.nMaxFile = sizeof(szFile); // ��������С
    ofn.lpstrFilter = filt; // �ļ����͹�����
    ofn.nFilterIndex = 1; // ����Ĭ�ϵ��ļ����͹���������
    ofn.lpstrFileTitle = NULL; // ����Ҫ�ļ������⣬��ΪNULL
    ofn.lpstrInitialDir = NULL; // ��ʼĿ¼����ΪNULL��ʾʹ��Ĭ��Ŀ¼
    ofn.Flags = OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY; // ���öԻ���ѡ��
    // ��������������ѡ����� OFN_EXPLORER ��
    if (GetOpenFileName(&ofn)) { // ��ʾ�Ի��򲢻�ȡ���
        return std::string(szFile); // �����û�ѡ����ļ���
    } else {
        return ""; // �û�ȡ�������ʱ���ؿ��ַ���
    }
}
string getsaveimgname(){
	char c[] = "pngͼƬ(.png)\0*.png\0gifͼƬ(.gif)\0*.gif\0bmpͼƬ(.bmp)\0*.bmp\0tifͼƬ(.tif)\0*.tif\0jpegͼƬ(.jpg)\0*.jpg\0�����ļ�(*.*)\0*.*\0\0";
	return getsavefilename(c);
}
string getopenimgname(){
	char c[] = "pngͼƬ(.png)\0*.png\0gifͼƬ(.gif)\0*.gif\0bmpͼƬ(.bmp)\0*.bmp\0tifͼƬ(.tif)\0*.tif\0jpegͼƬ(.jpg)\0*.jpg\0�����ļ�(*.*)\0*.*\0\0";
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
