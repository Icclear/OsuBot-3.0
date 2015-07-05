#include "auto.hpp"

Auto::Auto(Playmanagement &playmanager)
{    
    Playmanager = &playmanager;

    GetClientRect(Playmanager->getGame()->getWindow(), &OsuRec);

    WinRatio.Height = OsuRec.bottom - OsuRec.top;
    WinRatio.Width = OsuRec.right - OsuRec.left;

    WinRatio.scale = static_cast<double>(WinRatio.Height) / 480.;
    WinRatio.OffsetTop = static_cast<int>(static_cast<double>(480 - WinRatio.MaxY) * WinRatio.scale / 2.);
    WinRatio.OffsetLeft = static_cast<int>(static_cast<double>(640 - WinRatio.MaxX) * WinRatio.scale / 2.
                                           + (static_cast<double>(WinRatio.Width) - 800.
                                              * static_cast<double>(WinRatio.Height) / 600.) / 2.);

    if(WinRatio.Width == GetSystemMetrics(SM_CXSCREEN)
            && WinRatio.Height == GetSystemMetrics(SM_CYSCREEN))
        WinRatio.OffsetTop += 12;   //Todo: Find out the actual number

    autoData.ActiveMode = Idle;
}


void Auto::StartPlaying()
{
    int Time = 0;
    int LastTime = 0;

    GetCursorPos(&MousePos);    //Mouse startpos
    autoData.coords.x = MousePos.x;
    autoData.coords.y = MousePos.y;

    while(Playmanager->isPlaying())
    {
        Time = Playmanager->getTime();

        if(LoadNextHit)
        {
            if((Playmanager->getNextHit()->Type & 1) > 0)
                autoData.NextMode = Circle;

            else if((Playmanager->getNextHit()->Type & 2) > 0)
                autoData.NextMode = Slider;

            else if((Playmanager->getNextHit()->Type & 8) > 0)
                autoData.NextMode = Spin;

            autoData.AutoHitObject = Playmanager->getNextHit();
            LoadNextHit = false;
        }

        if(Time > Playmanager->getNextHit()->Time - Playmanager->getPreHit() * 5
                && Time < Playmanager->getNextHit()->Time - Playmanager->getPreHit())
        {
            if(autoData.NextMode != Spin)
            {
                autoData.coords.x = autoData.AutoHitObject->x;
                autoData.coords.y = autoData.AutoHitObject->y;
            }
        }

        else if(Time > autoData.AutoHitObject->Time - Playmanager->getPreHit()
                && Time < autoData.AutoHitObject->Time)
            autoData.ActiveMode = autoData.NextMode;
        else if(Time < autoData.AutoHitObject->Time + autoData.AutoHitObject->Duration)
            LoadNextHit = true;

        if(autoData.ActiveMode == Circle)
        {
            //Do I even have to do anything?
        }
        else if(autoData.ActiveMode == Slider)
        {
            //Todo: DO some fancy slider stuff

            if(autoData.AutoHitObject->SliderType == 'P')   //Passthrough / CircumscribedCircle
            {

            }
            else if(autoData.AutoHitObject->SliderType == 'C')  //CATMULL
            {

            }
            else if(autoData.AutoHitObject->SliderType == 'B')  //BEZIER
            {

            }
            else if(autoData.AutoHitObject->SliderType == 'L')  //Linear
            {

            }
        }
        else if(autoData.ActiveMode == Spin)
        {
            //Todo: DO some fancy spin stuff
        }
        else if(autoData.ActiveMode == Idle)
        {
            //Todo: Move to next hitobject
        }

        if(Time != LastTime)
            MoveMouse(autoData.coords.x, autoData.coords.y);

        LastTime = Time;
        Sleep(5);
    }
}

void Auto::MoveMouse(int x, int y)
{
    SetCursorPos(static_cast<int>(OsuRec.left + (x * WinRatio.scale) + WinRatio.OffsetLeft),
                 static_cast<int>(OsuRec.top + (y * WinRatio.scale) + WinRatio.OffsetTop));
}
