#include "osumanagement.hpp"

OsuManagement::OsuManagement()
{
    Error = 0;

    OsuWindow = FindWindow(0, (LPCSTR)"osu!");
    if(OsuWindow == NULL || OsuWindow == nullptr)
    {
        Error = 1;
        std::cerr << "Couldn't find osu's window." << std::endl;
        return;
    }

    GetWindowThreadProcessId(OsuWindow, &OsuProcessID);
    if(OsuProcessID == 0)
    {
        Error = 2;
        std::cerr << "Couldn't find ProcessID." << std::endl;
        return;
    }

    OsuProcessHandle = OpenProcess(PROCESS_QUERY_INFORMATION  | PROCESS_VM_READ, false, OsuProcessID);
    if(OsuProcessHandle == NULL || OsuProcessHandle == nullptr)
    {
        Error = 3;
        std::cerr << "Couldn't open process." << std::endl;
        return;
    }

    BYTE Pattern[] = { 0xB4, 0x17, 0x00, 0x00, 0x14, 0x13, 0x00, 0x00,
                       0xB8, 0x17, 0x00 ,0x00, 0x14, 0x13, 0x00, 0x00 };

    const int PatternLength = sizeof(Pattern) / sizeof(BYTE);

    DWORD ScanAdress = AOBScan::FindPattern(OsuProcessHandle, Pattern, PatternLength);
    if(ScanAdress == 0)
    {
        Error = 4;
        std::cerr << "Couldn't find pattern." << std::endl;
        return;
    }

    CHAR tempFileName[MAX_PATH];
    if(GetModuleFileNameEx(OsuProcessHandle, NULL, tempFileName, MAX_PATH) == 0)
    {
        Error = 5;
        std::cerr << "Error finding osu-directory. " << std::endl;
        return;
    }
    Filepath = tempFileName;

    if(Filepath.length() == 0)
    {
        Error = 6;
        std::cerr << "Couldn't find directory." << std::endl;
        return;
    }

    std::string OsuExe = "osu!.exe";

    Filepath = Filepath.substr(0, Filepath.length() - OsuExe.length()) + "Songs\\";

    TimeAdress = (LPVOID)(ScanAdress + 0xA20);
    PlayingAdress = (LPVOID)(ScanAdress + 0xA20 + 0xA34);

    std::cout << "Sucessfully initialized." << std::endl;
}

OsuManagement::~OsuManagement()
{
    CloseHandle(OsuProcessHandle);
}

void OsuManagement::readTime(int &Time)
{
    if(Error == 0)
        ReadProcessMemory(OsuProcessHandle, TimeAdress, &Time, sizeof(Time), 0);
}

void OsuManagement::readPlaying(bool &Playing)
{
    if(Error == 0)
        ReadProcessMemory(OsuProcessHandle, PlayingAdress, &Playing, sizeof(Playing), NULL);
}

int OsuManagement::getError()
{
    return Error;
}

std::string OsuManagement::getSongPath()
{
    if(Error == 0)
        return Filepath;
    return "";
}

std::string OsuManagement::getWindowTitle()
{
    if(Error == 0)
    {
        const static int Length = 256;
        char Title[Length];
        GetWindowText(OsuWindow, (LPSTR)Title, Length);
        return std::string(Title);
    }
    return "";
}

HWND &OsuManagement::getWindow()
{
    return OsuWindow;
}
