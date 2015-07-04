#include "beatmap.hpp"

Beatmap::Beatmap()
{

}

void Beatmap::LoadBeatmap(std::string BeatmapPath)
{
    MapPath = BeatmapPath;

    std::vector<std::string> BeatPath;
    SplitString::tokenize(MapPath, BeatPath, "\\");
    MapName = BeatPath[BeatPath.size() - 1];

    std::cout << "Mapname: " << MapName << std::endl;

    Error = 0;
    BeatmapLoaded = false;

    readBeatmap();
    readVersion();
    readMode();

    if(MapMode != 0 && MapVersion > 5)
    {
        Error = 1;
        std::cerr << "Not a valid Osu!:Beatmap." << std::endl;
    }
    else
    {
        readDifficulty();
        readMsPBs();
        readHitObjects();
        BeatmapLoaded = true;
    }
}

void Beatmap::readBeatmap()
{
    std::ifstream t(MapPath);
    std::stringstream buffer;
    buffer << t.rdbuf();

    if(buffer.str().length() < 5)   //More or less random value
    {
        Error = 2;
        std::cerr << "Something went wrong loading Beatmap: " << MapPath << std::endl;
        return;
    }

    SplitString::tokenize(buffer.str(), BeatmapFile, "\n");

    if(BeatmapFile.front().find("osu file format v") == std::string::npos)
    {
        Error = 3;
        std::cerr << "Something went wrong loading Beatmap: " << MapPath << std::endl;
        return;
    }
}

void Beatmap::readVersion()
{
    if(Error != 0)
        return;

    std::vector<std::string> tmp;
    SplitString::tokenize(BeatmapFile.front(), tmp, "v");
    MapVersion = std::stoi(tmp.back());
}

void Beatmap::readMode()
{
    if(Error != 0)
        return;

    for(auto &i : BeatmapFile) //iterate through the vector
    {
        if(i.find("Mode") != std::string::npos)
        {
            MapMode = ParseInt(i);
            return;
        }
    }
}

void Beatmap::readDifficulty()
{
    if(Error != 0)
        return;

    int FoundCount = 0;

    for(auto &i : BeatmapFile)
    {
        if(i.find("CircleSize") != std::string::npos)
        {
            MapCircleSize = ParseDouble(i);
            FoundCount++;
        }
        else if (i.find("OverallDifficulty") != std::string::npos)
        {
            MapOverallDifficulty = ParseDouble(i);
            FoundCount++;
        }
        else if (i.find("ApproachRate") != std::string::npos)
        {
            MapApproachRate = ParseDouble(i);
            FoundCount++;
        }
        else if (i.find("SliderMultiplier") != std::string::npos)
        {
            MapSliderMultiplier = ParseDouble(i);
            FoundCount++;
        }
        if(FoundCount >= 4)
            return;
    }
}

void Beatmap::readMsPBs()
{
    MsPBs.clear();

    bool TimingPoints = false;
    for(auto &i : BeatmapFile)
    {
        if(TimingPoints && i.find(",") == std::string::npos)
            return;

        if(TimingPoints)
        {
            std::vector<std::string> Line;
            SplitString::tokenize(i, Line, ",");
            MsPB mspb;
            mspb.Time = std::stoi(Line.at(0));
            mspb.Duration = std::stod(Line.at(1));
            MsPBs.push_back(mspb);
        }

        if(i.find("[TimingPoints]") != std::string::npos)
            TimingPoints = true;
    }
}

void Beatmap::readHitObjects()
{
    HitObjects.clear();

    bool AtHitObjects = false;

    for(auto &i : BeatmapFile)
    {
        if(AtHitObjects && i.find(",") == std::string::npos)
            return;

        if(AtHitObjects)
        {
            std::vector<std::string> Line;
            SplitString::tokenize(i, Line, ",");

            HitObject hitobject;
            hitobject.x = std::stoi(Line.at(0));
            hitobject.y = std::stoi(Line.at(1));
            hitobject.Time = std::stoi(Line.at(2));
            hitobject.Type = std::stoi(Line.at(3));

            if((hitobject.Type & 2) > 0)    //Slider
            {
                std::vector<std::string> SliderTokens;
                SplitString::tokenize(Line.at(5), SliderTokens, "|");
                hitobject.SliderType = SliderTokens.at(0)[0];   //First char of first token

                for(auto j = SliderTokens.begin() + 1; j != SliderTokens.end(); ++j)
                {
                    std::vector<std::string> XY;
                    SplitString::tokenize(*j, XY, ":");
                    hitobject.SliderX.push_back(std::stoi(XY.at(0)));
                    hitobject.SliderY.push_back(std::stoi(XY.at(1)));
                }
                hitobject.Repetition = std::stoi(Line.at(6));
                hitobject.PixelLength = std::stod(Line.at(7));  //Todo: replace . with , ?--------------
            }
            else if((hitobject.Type & 8) > 0)
                hitobject.SpinEndTime = std::stoi(Line.at(5));

            HitObjects.push_back(hitobject);
        }

        if(i.find("[HitObjects]") != std::string::npos)
            AtHitObjects = true;
    }
}

double Beatmap::getMapOverallDifficulty(bool HR)
{
    if(Error != 0)
        return 0;
    if(HR)
    {
        double od = MapOverallDifficulty * 1.4;
        if(od > 10)
            return 10;
        return od;
    }
    return MapOverallDifficulty;
}

int Beatmap::ParseInt(std::string Line)
{
    std::vector<std::string> tmp;
    SplitString::tokenize(Line, tmp, ":");
    return std::stoi(tmp.back());
}

double Beatmap::ParseDouble(std::string Line)
{
    std::vector<std::string> tmp;
    SplitString::tokenize(Line, tmp, ":");
    return std::stod(tmp.back());
}
