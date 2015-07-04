#ifndef AOBSCAN_HPP
#define AOBSCAN_HPP

#include <windows.h>

class AOBScan
{
public:
    static DWORD FindPattern(HANDLE ProcessHandle, BYTE Signature[], unsigned const int ByteCount);
};

#endif // AOBSCAN_HPP
