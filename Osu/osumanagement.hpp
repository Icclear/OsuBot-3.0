#ifndef OSUMANAGEMENT_HPP
#define OSUMANAGEMENT_HPP

#include <windows.h>
#include <iostream>
#include <string>
#include <psapi.h> // For access to GetModuleFileNameEx
#include "Include/aobscan.hpp"

class OsuManagement
{
public:
    OsuManagement();
    ~OsuManagement();

    int getError();

    void readTime(int &Time);
    void readPlaying(bool &Playing);
    std::string getSongPath();
    std::string getWindowTitle();
    HWND &getWindow();


private:
    HWND OsuWindow;
    DWORD OsuProcessID;
    HANDLE OsuProcessHandle;
    std::string Filepath;

    LPVOID TimeAdress;
    LPVOID PlayingAdress;

    int Error;
};

#endif // OSUMANAGEMENT_HPP
