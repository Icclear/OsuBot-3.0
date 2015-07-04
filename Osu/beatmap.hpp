#ifndef BEATMAP_HPP
#define BEATMAP_HPP

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include "Include/splitstring.hpp"

struct MsPB
{
    int Time;
    double Duration;
};

struct HitObject
{
    int x, y;
    int Time;
    int Type;
    char SliderType;
    std::vector<int> SliderX;
    std::vector<int> SliderY;
    int Repetition;
    double PixelLength;
    double SliderDuration;
    int SpinEndTime;
};

class Beatmap
{
public:
    Beatmap();
    void LoadBeatmap(std::string BeatmapPath);

    int getError();
    bool isLoaded() { return BeatmapLoaded; }

    int getVersion()                { return MapVersion; }
    int getMode()                   { return MapMode; }
    double getMapCircleSize()       { return MapCircleSize; }
    double getMapApproachRate()     { return MapApproachRate; }
    double getMapSliderMultiplier() { return MapSliderMultiplier; }
    double getMapOverallDifficulty(bool HR);

    std::vector<MsPB> *getMsPBs() { return &MsPBs; }
    std::vector<HitObject> *getHitObjects() { return &HitObjects; }

    std::string getPath();
    std::string getName();


private:
    bool BeatmapLoaded;
    int Error;
    int MapVersion;
    int MapMode;
    double MapCircleSize;
    double MapOverallDifficulty;
    double MapApproachRate;
    double MapSliderMultiplier;

    std::string MapPath;
    std::string MapName;
    std::vector<std::string> BeatmapFile;
    std::vector<MsPB> MsPBs;
    std::vector<HitObject> HitObjects;

    void readBeatmap();
    void readVersion();
    void readMode();
    void readDifficulty();
    void readMsPBs();
    void readHitObjects();

    int ParseInt(std::string Line);
    double ParseDouble(std::string Line);
};

#endif // BEATMAP_HPP
