#include "playmanagement.hpp"

Playmanagement::Playmanagement(Beatmap &beatmap, OsuManagement &osu)
{
    RelaxThread = nullptr;
    AutoThread = nullptr;

    LoadedBeatmap = &beatmap;
    Osu = &osu;

    RelaxPlay = new Relax(*this);
    AutoPlay = new Auto(*this);

    HitObjects = LoadedBeatmap->getHitObjects();
    MsPBs = LoadedBeatmap->getMsPBs();

    double dtMult = 1;
    if(DT)
        dtMult = 1.4;

    //preKlick = static_cast<int>(dtMult * (3 + (78 - (LoadedBeatmap->getMapOverallDifficulty(HR * 6)))));
    preKlick = 20;
    extraPressTime = 35;
}

Playmanagement::~Playmanagement()
{
    //Exit Stuff
    while(RelaxThread != nullptr && !RelaxThread->joinable())
        Sleep(100);

    if(RelaxThread != nullptr)
        RelaxThread->join();

    delete RelaxThread;
    delete RelaxPlay;


    while(AutoThread != nullptr && !AutoThread->joinable())
        Sleep(100);

    if(AutoThread != nullptr)
        AutoThread->join();

    delete AutoThread;
    delete AutoPlay;
}

void Playmanagement::StartPlaying()
{
    bool LoadNextHit = true;

    currentMsPB = MsPBs->at(0).Duration;
    double LastMsPB = currentMsPB;

    unsigned int HitObjectsIterator = 0;
    unsigned int MsPBsIterator = 0;

    //UI?

    do
    {
        Sleep(100);

        //Todo: Add some check if the game is still running
        Osu->readPlaying(Playing);
        if(!Playing)
        {
            return;
        }
        Osu->readTime(Time);
    }while(Time < HitObjects->at(0).Time - 400 || Time > HitObjects->at(0).Time);

    //Start Playthreads
    RelaxThread = new std::thread(Relax::StartPlaying, RelaxPlay);
    AutoThread = new std::thread(Auto::StartPlaying, AutoPlay);

    std::cout << "Started playing" << std::endl;

    while(Playing)
    {
        //Todo: Add some check if the game is still running
        Osu->readTime(Time);

        //MsPB
        while(MsPBsIterator < MsPBs->size() - 1 && HitObjectsIterator < HitObjects->size()
              && HitObjects->at(HitObjectsIterator).Time > MsPBs->at(MsPBsIterator + 1).Time - preKlick)
        {
            MsPBsIterator++;
            if(MsPBs->at(MsPBsIterator).Duration < 0)
                currentMsPB = -1 * MsPBs->at(MsPBsIterator).Duration / 100 * LastMsPB;
            else
                currentMsPB = LastMsPB = MsPBs->at(MsPBsIterator).Duration;
            break;
        }

        while(LoadNextHit || NextHit.Time - preKlick < Time)
        {
            if(HitObjectsIterator >= HitObjects->size())
            {
                //Finished
                FinishedSong = true;

                if(Time < NextHit.Time + NextHit.Duration + preKlick)
                    break;  //Don't stop until it's over

                Playing = false;
                break;
            }
            //if(Time < HitObjects->at(HitObjectsIterator).Time - preKlick)
            else
            {
                LoadNextHit = false;
                NextHit = HitObjects->at(HitObjectsIterator);

                if((NextHit.Type & 1) > 0)  //Circle duration
                    NextHit.Duration = extraPressTime;

                else if((NextHit.Type & 2) > 0)  //Slider duration
                    NextHit.Duration = currentMsPB * NextHit.Repetition * NextHit.PixelLength
                            / LoadedBeatmap->getMapSliderMultiplier() / 100;

                else if((NextHit.Type & 8) > 0) //Spin duration
                    NextHit.Duration = NextHit.SpinEndTime - NextHit.Time;
            }
            HitObjectsIterator++;
        }

        Sleep(1);
        if(Playing)
            Osu->readPlaying(Playing);
    }
    std::cout << "Stopped playing" << std::endl;
}
