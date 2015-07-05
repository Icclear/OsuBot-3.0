#ifndef PLAYMANAGEMENT_HPP
#define PLAYMANAGEMENT_HPP

#include <thread>
#include "Osu/beatmap.hpp"
#include "Osu/osumanagement.hpp"
#include "Play/play.hpp"
#include "Play/relax.hpp"
#include "Play/auto.hpp"

class Relax;    //Declaration
class Auto;

class Playmanagement
{
public:
    Playmanagement(Beatmap &beatmap, OsuManagement &osu);
    ~Playmanagement();

    void StartPlaying();
    void StopPlaying() { Playing = false; }
    void setRelax(bool active) { RelaxEnabled = active; }
    void setAuto(bool active) { AutoEnabled = active; }

    OsuManagement *getGame() { return Osu; }
    Beatmap *getBeatmap() { return LoadedBeatmap; }
    int getTime() { return Time; }
    HitObject *getNextHit() { return &NextHit; }

    bool hasFinished() { return FinishedSong; }
    bool isPlaying() { return Playing; }
    int getPreHit() { return preKlick; }
    int getExtraPressTime() { return extraPressTime; }
    double getCurrentMsPB() { return currentMsPB; }

private:
    Beatmap *LoadedBeatmap;
    OsuManagement *Osu;

    std::vector<HitObject> *HitObjects;
    std::vector<MsPB> *MsPBs;

    bool HR, DT;
    bool FinishedSong = false;

    bool Playing = true;
    int Time;

    double currentMsPB;
    HitObject NextHit;

    int preKlick, extraPressTime;


    bool RelaxEnabled, AutoEnabled;
    std::thread *RelaxThread;
    Relax *RelaxPlay;
    std::thread *AutoThread;
    Auto *AutoPlay;
};

#endif // PLAYMANAGEMENT_HPP
