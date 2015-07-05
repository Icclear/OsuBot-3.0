#ifndef PROGRAM_HPP
#define PROGRAM_HPP

#include <iostream>
#include <sys/stat.h> //For listing directories
#include <stdio.h>
#include <thread>
#include <mutex>

#include "Osu/osumanagement.hpp"
#include "Osu/beatmap.hpp"
#include "Play/playmanagement.hpp"

class Program
{
public:
    Program();
    ~Program();

    int getError() { return Error; }
    std::string getSongPath() { return Osu.getSongPath(); }
    void LoadSongList();
    void LoadBeatmap(std::string BeatmapPath);

    void checkForPlaying();

    std::vector<std::string> *getSongList() { return &SongFolderList; }

    void setRelax(bool active) { RelaxEnabled = active; }
    void setAuto(bool active) { AutoEnabled = active; }

private:
    int Error = 0;
    bool Playing;

    bool Running = true;

    bool RelaxEnabled = true;
    bool AutoEnabled = false;

    std::thread *PlayingThread;
    Playmanagement *PlayManagerCloser;

    OsuManagement Osu;
    Beatmap LoadedBeatmap;  //Test this stuff

    std::vector<std::string> SongFolderList;
};

#endif // PROGRAM_HPP
