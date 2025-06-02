#include<bits/stdc++.h>
#include<windows.h>
#include<urlmon.h>
#include <tchar.h>
#include<thread>
#pragma comment(lib,"urlmon.lib")//-lurlmon
#ifndef PSWEB
#define PSWEB
//psweb.cpp
using namespace std;
const IID IID_IUnknown =
    {0x00000000,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}};
const IID IID_IBindStatuscallback =
    {0x79eac9c1,0xbaf9,0x11ce,{0x8c,0x82,0x00,0xaa,0x00,0x4b,0xa9,0x0b}};
float _lastdownlandprt[33];
int _lasttime[33];
int _fuintbyte[33];
int _lastbyte[33];
int _rettype[33];
mutex _dxcdown[33];
const float _downlandprt = 0.5;
class CBindStatusCallback : public IBindStatusCallback {
public:
	bool norep = 0;
	int threadnum = 0;
    STDMETHOD(OnProgress)(ULONG ulProgress, ULONG ulProgressMax,
                         ULONG ulStatusCode, LPCWSTR szStatusText) {
        if (ulProgressMax > 0) {
			try{
	        	if(norep)_dxcdown[threadnum].lock();
	            float percent = (ulProgress * 100.0) / ulProgressMax;
	            if(percent-_lastdownlandprt[threadnum]>=_downlandprt){
	            	_lastdownlandprt[threadnum] = percent;
	            	int tmp = GetTickCount();
	            	int spdtime = (tmp-_lasttime[threadnum]);
	            	int spdbyte = ulProgress-_lastbyte[threadnum];
	            	_lasttime[threadnum] = tmp;
	            	_fuintbyte[threadnum] = ulProgressMax;
	            	_lastbyte[threadnum] = ulProgress;
					int speed = spdbyte/(spdtime/1000.0);
					int last = ulProgressMax-ulProgress;
					int need = 0;
					if(last>0){
						need = last/speed;
					}
	            	if(norep==0)printf("已下载%03.2f%% (%10ld/%10ld)字节%8d字节每秒 剩余%5d秒         \r",percent,ulProgress,ulProgressMax,speed,need);
				}
	        	if(norep)_dxcdown[threadnum].unlock();
			}
			catch(const char*e){
				//
			}

        }
        return S_OK;
    }
    STDMETHOD(OnStartBinding)(DWORD, IBinding*) { return S_OK; }
    STDMETHOD(GetPriority)(LONG*) { return E_NOTIMPL; }
    STDMETHOD(OnLowResource)(DWORD) { return S_OK; }
    STDMETHOD(OnStopBinding)(HRESULT, LPCWSTR) { return S_OK; }
    STDMETHOD(GetBindInfo)(DWORD*, BINDINFO*) { return S_OK; }
    STDMETHOD(OnDataAvailable)(DWORD, DWORD, FORMATETC*, STGMEDIUM*) { return S_OK; }
    STDMETHOD(OnObjectAvailable)(REFIID, IUnknown*) { return S_OK; }
    STDMETHOD_(ULONG, AddRef)() { return 1; }
    STDMETHOD_(ULONG, Release)() { return 1; }
    STDMETHOD(QueryInterface)(REFIID riid, void** ppv) {
        if (riid == IID_IUnknown || riid == IID_IBindStatuscallback) {
            *ppv = this;
            return S_OK;
        }
        return E_NOINTERFACE;
    }
};
int downlandpro(string addr,string fn,bool norep=0,int threadnum = 0) {
    CBindStatusCallback callback;
    callback.norep = norep;
    callback.threadnum = threadnum;
    _lastdownlandprt[threadnum] = 0;
    _lasttime[threadnum] = GetTickCount();
    _lastbyte[threadnum] = 0;
    _fuintbyte[threadnum] = 0;
    _rettype[threadnum] = 0;
    if(norep==0)cout<<"下载文件"<<addr<<"->"<<fn<<endl;
    HRESULT r = URLDownloadToFile(NULL,_T(addr.c_str()),_T(fn.c_str()),0,&callback);
    if(norep){
    	return !SUCCEEDED(r);
	}
    cout<<"\r                                                                                   ";
    if (SUCCEEDED(r)) {
        std::cout << "\n成功下载" << std::endl;
    	return 0;
    } else {
        std::cout << "\n错误:0x" << std::hex << r << std::endl;
    }
	if(r==E_OUTOFMEMORY){
		cout<<"内存不足\n";
		return 1;
	}
	else if(r==INET_E_DOWNLOAD_FAILURE){
		cout<<"资源无效\n";
		return 1;
	}
	else if(r==INET_E_CONNECTION_TIMEOUT){
		cout<<"INET_E_CONNECTION_TIMEOUT\n";
		return 1;
	}
	else if(r==INET_E_RESOURCE_NOT_FOUND){
		cout<<"INET_E_RESOURCE_NOT_FOUND\n";
		return 1;
	}
	else if(r==INET_E_INVALID_URL){
		cout<<"INET_E_INVALID_URL\n";
		return 1;
	}
	else{
		cout<<"unknown error:"<<r<<endl;
		return 1;
	}
}
int urldownland(string addr,string fn,bool norep=0,int threadnum = 0){
	int r = downlandpro(addr,fn,norep,threadnum);
	if(norep==0)cout<<endl;
	return r;
}
float o_lastdownlandprt;
int o_lasttime;
int o_fuintbyte;
int o_lastbyte;
const float o_downlandprt = 0.5;
class CBindStatusCallbackOLD : public IBindStatusCallback {
public:
    STDMETHOD(OnProgress)(ULONG ulProgress, ULONG ulProgressMax,
                         ULONG ulStatusCode, LPCWSTR szStatusText) {
        if (ulProgressMax > 0) {
            float percent = (ulProgress * 100.0) / ulProgressMax;
            if(percent-o_lastdownlandprt>=o_downlandprt){
            	o_lastdownlandprt = percent;
            	int tmp = GetTickCount();
            	int spdtime = (tmp-o_lasttime);
            	int spdbyte = ulProgress-o_lastbyte;
            	o_lasttime = tmp;
            	o_fuintbyte = ulProgressMax;
            	o_lastbyte = ulProgress;
				int speed = spdbyte/(spdtime/1000.0);
				int last = ulProgressMax-ulProgress;
				int need = 0;
				if(last>0){
					need = last/speed;
				}
            	printf("已下载%03.2f%% (%10ld/%10ld)字节%8d字节每秒 剩余%5d秒         \r",percent,ulProgress,ulProgressMax,speed,need);
			}
        }
        return S_OK;
    }
    STDMETHOD(OnStartBinding)(DWORD, IBinding*) { return S_OK; }
    STDMETHOD(GetPriority)(LONG*) { return E_NOTIMPL; }
    STDMETHOD(OnLowResource)(DWORD) { return S_OK; }
    STDMETHOD(OnStopBinding)(HRESULT, LPCWSTR) { return S_OK; }
    STDMETHOD(GetBindInfo)(DWORD*, BINDINFO*) { return S_OK; }
    STDMETHOD(OnDataAvailable)(DWORD, DWORD, FORMATETC*, STGMEDIUM*) { return S_OK; }
    STDMETHOD(OnObjectAvailable)(REFIID, IUnknown*) { return S_OK; }
    STDMETHOD_(ULONG, AddRef)() { return 1; }
    STDMETHOD_(ULONG, Release)() { return 1; }
    STDMETHOD(QueryInterface)(REFIID riid, void** ppv) {
        if (riid == IID_IUnknown || riid == IID_IBindStatuscallback) {
            *ppv = this;
            return S_OK;
        }
        return E_NOINTERFACE;
    }
};
int urldownlandold(string addr,string file){
    int r = URLDownloadToFile(NULL,addr.c_str(),file.c_str(),0,NULL);
    //https://docs.microsoft.com/en-us/previous-versions/windows/internet-explorer/ie-developer/platform-apis/ms775123(v=vs.85)
	if(r==S_OK){
		cout<<"下载已成功开始\n";
		return 0;
	}
	else if(r==E_OUTOFMEMORY){
		cout<<"内存不足\n";
		return 1;
	}
	else if(r==INET_E_DOWNLOAD_FAILURE){
		cout<<"资源无效\n";
		return 1;
	}
	else if(r==INET_E_CONNECTION_TIMEOUT){
		cout<<"INET_E_CONNECTION_TIMEOUT\n";
		return 1;
	}
	else if(r==INET_E_RESOURCE_NOT_FOUND){
		cout<<"INET_E_RESOURCE_NOT_FOUND\n";
		return 1;
	}
	else if(r==INET_E_INVALID_URL){
		cout<<"INET_E_INVALID_URL\n";
		return 1;
	}
	else{
		cout<<"unknown error:"<<r<<endl;
		return 1;
	}
}
int urldownland2(string addr,string fn) {
    CBindStatusCallbackOLD callback;
    o_lastdownlandprt = 0;
    o_lasttime = GetTickCount();
    o_lastbyte = 0;
    o_fuintbyte = 0;
    cout<<"下载文件"<<addr<<"->"<<fn<<endl;
    HRESULT r = URLDownloadToFile(NULL,_T(addr.c_str()),_T(fn.c_str()),0,&callback);
    cout<<"\r                                                                                ";
    if (SUCCEEDED(r)) {
        std::cout << "\n成功下载" << std::endl;
    	return 0;
    } else {
        std::cout << "\n错误:0x" << std::hex << r << std::endl;
    }
	if(r==E_OUTOFMEMORY){
		cout<<"内存不足\n";
		return 1;
	}
	else if(r==INET_E_DOWNLOAD_FAILURE){
		cout<<"资源无效\n";
		return 1;
	}
	else if(r==INET_E_CONNECTION_TIMEOUT){
		cout<<"INET_E_CONNECTION_TIMEOUT\n";
		return 1;
	}
	else if(r==INET_E_RESOURCE_NOT_FOUND){
		cout<<"INET_E_RESOURCE_NOT_FOUND\n";
		return 1;
	}
	else if(r==INET_E_INVALID_URL){
		cout<<"INET_E_INVALID_URL\n";
		return 1;
	}
	else{
		cout<<"unknown error:"<<r<<endl;
		return 1;
	}
}
int _threadend=0;
mutex _report;
void threaddownland(string addr,string fn,int threadnum = 0){
	_report.lock();
	cout<<"threadstart num:"<<threadnum<<endl;
	_report.unlock();
	int r = urldownland(addr,fn,1,threadnum);//don't report to console
	_report.lock();
	_dxcdown[threadnum].lock();
	_threadend++;
	_rettype[threadnum] = r;
	_report.unlock();
	_dxcdown[threadnum].unlock();
}
void dxcdown(vector<string>addr,vector<string>fn){
	_threadend=0;
	int l = addr.size();
	thread runt[32];
	for(int i = 0;i<l;i++){
		_report.lock();
		cout<<"start thread "<<i<<"for "<<fn[i]<<endl;
		runt[i] = thread(threaddownland,addr[i],fn[i],i);
		_report.unlock();
		runt[i].detach();
	}
	while(1){
		_report.lock();
		int end = _threadend;
		_report.unlock();
		cout<<"finished thread "<<end<<endl;
		for(int i = 0;i<l;i++){
			_dxcdown[i].lock();
			printf("%2d %10ld/%10ldbyte ret:%3d\n",i,_lastbyte[i],_fuintbyte[i],_rettype[i]);
			_dxcdown[i].unlock();
		}
		Sleep(1000);
		if(end==l) break;
	}
	cout<<"all ok\n";
	_threadend=0;
	return ;
}
#endif
