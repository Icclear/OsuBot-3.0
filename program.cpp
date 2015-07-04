#include "program.hpp"

Program::Program()
{
    Error = Osu.getError();

    LoadSongList();

    PlayingThread = new std::thread(Program::checkForPlaying, this);

    PlayManagerCloser = nullptr;
}

Program::~Program()
{
    if(Running && PlayManagerCloser != nullptr)
    {
        PlayManagerCloser->StopPlaying();
    }

    Running = false;

    while(!PlayingThread->joinable())
        Sleep(10);

    PlayingThread->join();  //Todo: Somehow terminate that thread.

    delete PlayingThread;
}

void Program::LoadSongList()
{
    SongFolderList.clear();

     char szBuffer[MAX_PATH];
     GetCurrentDirectory(MAX_PATH, szBuffer);

     SetCurrentDirectory(Osu.getSongPath().c_str());

     WIN32_FIND_DATA fd;
     HANDLE hFind = FindFirstFile("*.", &fd);

     // Get all sub-folders:
     if (hFind != INVALID_HANDLE_VALUE)
     {
         do
         {
             char* pszName = fd.cFileName;

             if (_stricmp(pszName, ".") != 0 && _stricmp(pszName, "..") != 0)
             {
                 SongFolderList.push_back(pszName);
             }

         } while (FindNextFile(hFind, &fd));

         FindClose(hFind);
     }

     // Set the current folder back to what it was:
     SetCurrentDirectory(szBuffer);
}

void Program::LoadBeatmap(std::string BeatmapPath)
{
    LoadedBeatmap.LoadBeatmap(BeatmapPath);
}


void Program::checkForPlaying()
{
    while(Running)
    {
        if(LoadedBeatmap.isLoaded())
        {
            Osu.readPlaying(Playing);
            if(Playing)
            {
                std::cout << "Playing." << std::endl;
                Playmanagement Play(LoadedBeatmap, Osu);
                PlayManagerCloser = &Play;
                Play.StartPlaying();

                PlayManagerCloser = nullptr;
            }
        }
        Sleep(100);
    }
}
