#ifndef AUTO_HPP
#define AUTO_HPP

#include "Play/playmanagement.hpp"
#include "Play/play.hpp"

class Playmanagement;   //Declaration

struct WindowRatio
{
    const int MaxX = 512;
    const int MaxY = 384;
    int Height;
    int Width;
    double scale;
    int OffsetTop;
    int OffsetLeft;
};

enum Mode { Idle, Circle, Slider, Spin };

struct Coords
{
    int x, y;
};

struct AutoData
{
    Coords coords;
    Mode ActiveMode;
    Mode NextMode;
    HitObject *AutoHitObject;
};


class Auto : public Play
{
public:
    Auto(Playmanagement &playmanager);
    //~Auto();
    void StartPlaying();

private:

    Playmanagement *Playmanager;
    AutoData autoData;

    RECT OsuRec;
    WindowRatio WinRatio;
    POINT MousePos;

    bool LoadNextHit = true;

    void MoveMouse(int x, int y);
};

#endif // AUTO_HPP
