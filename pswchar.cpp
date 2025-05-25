#include<bits/stdc++.h>
using namespace std;
std::string ansi_to_utf8(const std::string& ansiStr) {
    int wideSize = MultiByteToWideChar(CP_ACP, 0, ansiStr.c_str(), -1, NULL, 0);
    std::vector<wchar_t> wideStr(wideSize);
    MultiByteToWideChar(CP_ACP, 0, ansiStr.c_str(), -1, &wideStr[0], wideSize);
    int utf8Size = WideCharToMultiByte(CP_UTF8, 0, &wideStr[0], -1, NULL, 0, NULL, NULL);
    std::vector<char> utf8Str(utf8Size);
    WideCharToMultiByte(CP_UTF8, 0, &wideStr[0], -1, &utf8Str[0], utf8Size, NULL, NULL);
    return std::string(utf8Str.begin(), utf8Str.end());
}
std::string utf8_to_ansi(const std::string& utf8_str) {
    int wlen = MultiByteToWideChar(CP_UTF8, 0, utf8_str.c_str(), -1, nullptr, 0);
    std::wstring wide_str(wlen, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, utf8_str.c_str(), -1, &wide_str[0], wlen);
    UINT codePage = CP_ACP;
    int ansi_len = WideCharToMultiByte(codePage, 0, wide_str.c_str(), -1, nullptr, 0, nullptr, nullptr);
    std::string ansi_str(ansi_len, '\0');
    WideCharToMultiByte(codePage, 0, wide_str.c_str(), -1, &ansi_str[0], ansi_len, nullptr, nullptr);
    return ansi_str;
}
