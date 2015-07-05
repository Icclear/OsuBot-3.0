#ifndef PLAY_HPP
#define PLAY_HPP

#include <windows.h>

class Play
{
public:
    Play() {}
    virtual ~Play() {}
    virtual void StartPlaying() = 0;

    void activateHR() { HR = true; }
    void activateDT() { DT = true; }
    void stopPlaying() { Playing = false; }

protected:

    bool Playing = true;
    bool HR = false;
    bool DT = false;
};

#endif // PLAY_HPP
