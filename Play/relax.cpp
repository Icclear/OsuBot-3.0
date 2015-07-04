#include "relax.hpp"

Relax::Relax(Playmanagement &playmanager)
{
    Playmanager = &playmanager;

    std::string Key1 = ".";
    std::string Key2 = "-";

    Button1.Keycode = VkKeyScanEx(Key1.at(0), GetKeyboardLayout(0)) & 0xFF;
    Button2.Keycode = VkKeyScanEx(Key2.at(0), GetKeyboardLayout(0)) & 0xFF;

    InitButton(Button1.PressButton, Button1.Keycode, true);     //Initiate buttons
    InitButton(Button1.ReleaseButton, Button1.Keycode, false);
    InitButton(Button2.PressButton, Button2.Keycode, true);
    InitButton(Button2.ReleaseButton, Button2.Keycode, false);

    Klicked = true; //Load first hit
}

void Relax::InitButton(INPUT &Button, WORD Keycode, bool Press)
{
    Button.type = INPUT_KEYBOARD;
    Button.ki.wScan = 0;
    Button.ki.dwExtraInfo = 0;
    Button.ki.wVk = Keycode;
    if(Press)
        Button.ki.dwFlags = 0;
    else
        Button.ki.dwFlags = KEYEVENTF_KEYUP;
}

void Relax::StartPlaying()
{
    int LastTime = 0;
    while(Playmanager->isPlaying())
    {
        Time = Playmanager->getTime();

        if(Klicked && Time != LastTime)
        {
            const int LastClick = NextClick.BeginKlick;
            NextClick.BeginKlick = Playmanager->getNextHit()->Time - Playmanager->getPreHit();

            if(!NextClick.BeginKlick == LastClick)
            {
                NextClick.EndKlick = NextClick.BeginKlick + Playmanager->getNextHit()->Duration;
                if((Playmanager->getNextHit()->Type & 2) > 0)  //Slider
                {
                    NextClick.EndKlick += Playmanager->getPreHit();
                }

                Klicked = false;
            }
        }

        if(Time >= NextClick.BeginKlick && !Klicked && Time != LastTime)
        {
            Klicked = true;
            Klick();
        }

        ReleaseButtons();
        LastTime = Time;
        Sleep(1);
    }
    ResetButtons();
}

void Relax::Klick()
{
    if(LastButtonPressed == 2)
        BT1Klick();
    else
        BT2Klick();
}

void Relax::ReleaseButtons()
{
    if(Button1.Pressed && Time > Button1.ReleaseAt)
    {
        SendInput(1, &Button1.ReleaseButton, sizeof(INPUT));
        Button1.Pressed = false;
    }
    if(Button2.Pressed && Time > Button2.ReleaseAt)
    {
        SendInput(1, &Button2.ReleaseButton, sizeof(INPUT));
        Button2.Pressed = false;
    }
}

void Relax::ResetButtons()
{
    SendInput(1, &Button1.ReleaseButton, sizeof(INPUT));
    Button1.Pressed = false;

    SendInput(1, &Button2.ReleaseButton, sizeof(INPUT));
    Button2.Pressed = false;
}

void Relax::BT1Klick()
{
    if (!Button1.Pressed)
    {
        SendInput(1, &Button1.PressButton, sizeof(INPUT));
        Button1.Pressed = true;
        Button1.ReleaseAt = NextClick.EndKlick;
    }
    else if (!Button2.Pressed)
    {
        SendInput(1, &Button2.PressButton, sizeof(INPUT));
        Button2.Pressed = true;
        Button2.ReleaseAt = NextClick.EndKlick;
    }
    else
    {	//forcerelease
        SendInput(1, &Button1.ReleaseButton, sizeof(INPUT));
        Button1.Pressed = false;
        Klicked = false;
        return;
    }
    LastButtonPressed = 1;
}

void Relax::BT2Klick()
{
    if (!Button2.Pressed)
    {
        SendInput(1, &Button2.PressButton, sizeof(INPUT));
        Button2.Pressed = true;
        Button2.ReleaseAt = NextClick.EndKlick;
    }
    else if (!Button1.Pressed)
    {
        SendInput(1, &Button1.PressButton, sizeof(INPUT));
        Button1.Pressed = true;
        Button1.ReleaseAt = NextClick.EndKlick;
    }
    else
    {	//forcerelease
        SendInput(1, &Button2.ReleaseButton, sizeof(INPUT));
        Button2.Pressed = false;
        Klicked = false;
        return;
    }
    LastButtonPressed = 2;
}
