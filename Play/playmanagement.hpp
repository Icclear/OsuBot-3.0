#ifndef PLAYMANAGEMENT_HPP
#define PLAYMANAGEMENT_HPP

#include <thread>
#include "Osu/beatmap.hpp"
#include "Osu/osumanagement.hpp"
#include "Play/play.hpp"
#include "Play/relax.hpp"

class Relax;    //Declaration

class Playmanagement
{
public:
    Playmanagement(Beatmap &beatmap, OsuManagement &osu);
    ~Playmanagement();

    void StartPlaying();
    void StopPlaying() { Playing = false; }

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
    HitObject LastHit;

    int preKlick, extraPressTime;

    std::thread *RelaxThread;
    Relax *RelaxPlay;
};

#endif // PLAYMANAGEMENT_HPP
