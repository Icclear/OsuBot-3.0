#ifndef RELAX_HPP
#define RELAX_HPP

#include "Play/playmanagement.hpp"
#include "Play/play.hpp"

class Playmanagement;   //Declaration

struct RelaxData
{
    int BeginKlick;
    int EndKlick;
};

struct KlickButton
{
    bool Pressed;
    INPUT PressButton;
    INPUT ReleaseButton;
    int ReleaseAt;
    int Keycode;
};


class Relax : public Play
{
public:
    Relax(Playmanagement &playmanager);
    //~Relax();

    void StartPlaying();
    bool ButtonPressed() { return (Button1.Pressed || Button2.Pressed); }
private:
    Playmanagement *Playmanager;

    int Time;

    RelaxData NextClick;
    KlickButton Button1, Button2;
    int LastButtonPressed = 1;
    bool Klicked;

    void InitButton(INPUT &Button, WORD Keycode, bool Press);

    void Klick();
    void ReleaseButtons();
    void ResetButtons();
    void BT1Klick();
    void BT2Klick();
};

#endif // RELAX_HPP
