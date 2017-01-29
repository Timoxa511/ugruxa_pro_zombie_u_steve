#include "TimaXLIbJustOneFunction.h"


#define ever (;;)


//-----------------------------------------------------------------------------
enum {APPLE = Hero::FIRSTUSERTYPE};


struct Textures
    {
    Image Steve;
    Image Zombie;
    Image Blood;
    Image Apple;
    };



struct Apple : public Hero
    {

    //-----------------------
    Apple (Image Picture, Vec pos);
    };

struct Steve : public Hero
    {
    int SteveAppleDisgustingTimer_;
    int SpeedMultp_;
    //---------------
    Steve (const char *Name, Image Picture, Vec pos, Vec V);
    virtual void doAnimation () const override;
    virtual void Control   (int KeyStopMove = VK_SPACE) override;
    };

struct Zombie : public Hero
    {
    int AnimationSpeedDivider;
    int   ControlSpeedMultiplier;
    //-----------------
    Zombie (const char *Name, Image Picture, Vec pos, Vec V, const Hero *Steve);
    virtual void doAnimation () const override;
    virtual void Control   (int KeyStopMove = VK_SPACE) override;
    };

struct gmMouse : public Hero
    {
    //---------------
    gmMouse (const char *Name, Image Picture, Vec pos, Vec V);
    virtual void doAnimation () const override;
    virtual void Control   (int KeyStopMove = VK_SPACE) override;
    };

struct FastZombie : public Zombie
    {

    //-----------------------
    FastZombie (const char *Name, Image Picture, Vec pos, Vec V, const Hero *Steve);
    };



struct Controller
    {
    Engine *MrEngine_;
    Steve *steve_;
    //-----------------------------
    Controller (Engine *MrEngine, Steve *steve_);
    void  AppleSpawn (Image texture);
    void ZombieSpawn (Image texture);
    int  ZombieAteUrBrains (const Textures *textures, int BrainsBeingEatenRadius, int KolBoZombieNeededForUrBrainsBeingEaten);
    void AppleZombieLogic ();
    void  AppleSteveLogic (int AppleBeingEatenRadius);
    void  ControlSpeedEff (int MaxSpeedmlt);
    void EndScene ();

    };
//-----------------------------------------------------------------------------


enum {LOSE, WIN, CONTINUE};

void StartCom ();
void PlsStopSpammingKeysGetYourHandsAwayFromTheKeyBoard_Arrrrrrrr ();


void SteveAnimationNumber_and_Moving_Connecting (Hero *Steve);


void GeneralControl (Hero *object, int KeyStopMove);
void MouseControl (Hero *Mouse, int KeyStopMove);

void Zombiezomb (Hero *victim, const Hero *daddy);

int Timer (bool StartTimer, int sleep, int *EndTime);

void GameProcces (const Textures *textures);

//-----------------------------------------------------------------------------

int main()
    {
    StartCom ();

    Textures textures = {Image ("steve.bmp", {4, 18}), Image ("Zombie.bmp", {3, 2}),
                         Image ("bloodtex1.bmp", {1, 1}), Image ("Apple.bmp", {1, 1})};

    GameProcces (&textures);

    textures.Steve.DeleteTex();
    textures.Zombie.DeleteTex();
    textures.Blood.DeleteTex();
    textures.Apple.DeleteTex();
    txEnd ();
    PlsStopSpammingKeysGetYourHandsAwayFromTheKeyBoard_Arrrrrrrr ();
    }


//-----------------------------------------------------------------------------
void GameProcces (const Textures *textures)
    {

    gmMouse mouse ("Mouse", Image (),    Vec (),   Vec ());
    Steve  steve  ("Steve",  textures->Steve,  Vec (500, 500), Vec (0, 0));
    Zombie zombie ("Zombie", textures->Zombie, Vec (700, 700), Vec (0, 0), &steve);
    //Zombiezomb (&steve, &zombie);

    //!!!
    Engine MrEngine;
    Controller Logic (&MrEngine, &steve);
    //!!!

    MrEngine.add (&steve);
    MrEngine.add (&zombie);
    MrEngine.add (&mouse);

    bool StartTimer = false;
    int EndTime = 0;
    while (!GetAsyncKeyState (VK_ESCAPE))
        {
        MrEngine.Run();

        SteveAnimationNumber_and_Moving_Connecting (&steve);
        if (StartTimer != true) Logic.ZombieSpawn (textures->Zombie);
        if (Logic.ZombieAteUrBrains (textures, 30, 2) == LOSE) StartTimer = true;



        if (Timer (StartTimer, 100, &EndTime) == LOSE) break;


        Logic.AppleZombieLogic ();
        Logic.AppleSpawn (textures->Apple);
        Logic.AppleSteveLogic (20);
        Logic.ControlSpeedEff (5);

        if (StartTimer) Logic.EndScene ();

        txSleep (20);
        Global_Mouse.Update();
        Global_Timer.Update();
        }
    }



//-----------------------------------------------------------------------------

//{ Controller::


Controller::Controller (Engine *MrEngine, Steve *steve) :
    MrEngine_ (MrEngine),
    steve_ (steve)
    {}


//-----------------------------------------------------------------------------
void Controller::EndScene ()
    {
    Hero* gmZombiePointers [N_OBJECTS] = {};
    MrEngine_->Get_AllObjects_ofThe_Type (gmZombiePointers, Hero::NPC);
    for (int i = 0; gmZombiePointers[i]; i++)
        {
        gmZombiePointers[i]->victim_ = steve_;
        }
    }


//-----------------------------------------------------------------------------
void Controller::ControlSpeedEff (int MaxSpeedmlt)
    {
    if (steve_->SteveAppleDisgustingTimer_ >= 0) steve_->SteveAppleDisgustingTimer_--;
    else steve_->SpeedMultp_ = 0;

    if (steve_->SpeedMultp_ > MaxSpeedmlt) steve_->SpeedMultp_ = steve_->SpeedMultp_;
    }


//-----------------------------------------------------------------------------
void Controller::AppleZombieLogic ()
    {
    Hero* gmApplePointers  [N_OBJECTS] = {};
    Hero* gmZombiePointers [N_OBJECTS] = {};
    MrEngine_->Get_AllObjects_ofThe_Type (gmApplePointers, APPLE);
    MrEngine_->Get_AllObjects_ofThe_Type (gmZombiePointers, Hero::NPC);

    for (int i = 0; gmZombiePointers[i] != NULL; i++)
        {
        bool AppleWasEaten = false;
        double TheShortestDist_Between_CurrentZombie_and_TheNearestApple = 10000;
        Hero*  TheNearestApplePointer_toThe_CurrentZombie = NULL;

        for (int j = 0; gmApplePointers [j]; j++)
            {
            double Dist = HeroHypot (gmApplePointers[j], gmZombiePointers[i]);

            if (Dist < TheShortestDist_Between_CurrentZombie_and_TheNearestApple)
                {
                TheShortestDist_Between_CurrentZombie_and_TheNearestApple = Dist;
                TheNearestApplePointer_toThe_CurrentZombie = gmApplePointers[j];
                }
            }

        if (TheNearestApplePointer_toThe_CurrentZombie)
            {
            gmZombiePointers[i]->victim_ = TheNearestApplePointer_toThe_CurrentZombie;

            if (HeroHypot (TheNearestApplePointer_toThe_CurrentZombie, gmZombiePointers[i]) < 50)
                {
                DeleteObjFromArray (TheNearestApplePointer_toThe_CurrentZombie, gmApplePointers,
                                    sizearr(gmApplePointers));
                MrEngine_->FindAndRemove (TheNearestApplePointer_toThe_CurrentZombie);
                AppleWasEaten = true;
                }
            }

        if (AppleWasEaten)
            {
            for (int j = 0; gmZombiePointers[j]; j++)
                gmZombiePointers[j]->victim_ = steve_;
            }
        }
    }



//-----------------------------------------------------------------------------
void Controller::AppleSteveLogic (int AppleBeingEatenRadius)
    {
    for (int i = 0; i < MrEngine_->Get_KolBo_OfObjects(); i++)
    if ((MrEngine_->GetObject(i)->Type_ == APPLE) &&
        (HeroHypot (steve_, MrEngine_->GetObject(i)) ) < AppleBeingEatenRadius)
        {
        MrEngine_->remoov (i);
        steve_->SteveAppleDisgustingTimer_ += 150;
        steve_->SpeedMultp_ += 1;
        }

    }




//-----------------------------------------------------------------------------
void Controller::AppleSpawn (Image texture)
    {
    if (rand()%1000 < 5)
        {
        Apple *apple = new Apple ( texture, Vec (rand()%txGetExtentX(), rand()%txGetExtentY()) );
        MrEngine_->add (apple);
        }
    }




//-----------------------------------------------------------------------------
int Controller::ZombieAteUrBrains (const Textures *textures, int BrainsBeingEatenRadius,
                                                             int KolBoZombieNeededForUrBrainsBeingEaten)
    {
    int SteveSisNumber = MrEngine_->GetObjNum (steve_);
    if (SteveSisNumber == -1) return CONTINUE;

    for (int i = 0; i < MrEngine_->Get_KolBo_OfObjects(); i++)
        {
        if ((MrEngine_->GetObject(i)->Type_ == Hero::NPC) &&
            (HeroHypot (steve_, MrEngine_->GetObject(i)) ) < BrainsBeingEatenRadius)
            {
            if (--KolBoZombieNeededForUrBrainsBeingEaten <= 0)
                {

                MrEngine_->remoov (SteveSisNumber);

                MrEngine_->add (new Animation (textures->Blood,     steve_->pos_));    //blood
                return LOSE;
                }
            }
        }
    return CONTINUE;
    }




//-----------------------------------------------------------------------------
void Controller::ZombieSpawn (Image texture)
    {
    if (txMouseButtons () == 2)
        {
        Zombie *zom = (rand()%100 > 30)? (new Zombie     ("Zomb",     texture, txMousePos(), Vec (0, 0), steve_)) :
                                         (new FastZombie ("FastZomb", texture, txMousePos(), Vec (0, 0), steve_));
        MrEngine_->add (zom);
        }
    }



//}
//-----------------------------------------------------------------------------

void Zombiezomb (Hero *victim, const Hero *daddy)
    {
    Zombie zomb ("Zombie", Image (), Vec (700, 700), Vec (0, 0), daddy);
    int t = *(int*)&zomb;
    *(int*)victim = t;
    victim->victim_ = daddy;
    }

//-----------------------------------------------------------------------------
int Timer (bool StartTimer, int sleep, int *EndTime)
    {
    if (StartTimer)
        {
        (*EndTime)++;
        if (*EndTime >= sleep) return LOSE;
        }
    return CONTINUE;
    }

//-----------------------------------------------------------------------------

//{Apple::
Apple::Apple (Image Picture, Vec pos) :
    Hero (NULL, Picture, pos, Vec(), APPLE)
    {
    }

//-----------------------------------------------------------------------------

//}
//-----------------------------------------------------------------------------

//{Steve::
Steve::Steve (const char *Name, Image Picture, Vec pos, Vec V) :
    Hero (Name, Picture, pos, V, CHARACTER),
    SteveAppleDisgustingTimer_ (0),
    SpeedMultp_ (0)
    {}
//-----------------------------------------------------------------------------
void Steve::doAnimation () const
    {
    if (DoubleCompareWithZero (V_.x) != 0 && DoubleCompareWithZero (V_.y) != 0)
        {
        int t = Global_Timer.GetTime()/8;

        Picture_.Draw (pos_, AnimationNumber_, t);
        }

    else
        Picture_.Draw (pos_, AnimationNumber_, 0);

    }

//-----------------------------------------------------------------------------
void Steve::Control (int KeyStopMove )
    {
Vec VDist = Global_Mouse.GetClickPos() - pos_;

    if (VDist.Len() > 10)
        {
        Vec shortVector = VDist.Normalize() * (5*1.5 + 0.5)*(SpeedMultp_ + 1);     //  V_ = !(Vec (txMousePos()) - pos_) * 15   | где ! - оператор нормализации
        V_ = shortVector;
        }
    else
        V_ = Vec (0, 0);


    if (GetAsyncKeyState ( KeyStopMove ))
        V_ = Vec (0, 0);
    }
//}
//-----------------------------------------------------------------------------

//{Zombie::
Zombie::Zombie (const char *Name, Image Picture, Vec pos, Vec V, const Hero *Steve) :
    Hero (Name, Picture, pos, V, NPC, Steve),
    AnimationSpeedDivider (8),
      ControlSpeedMultiplier (5)
    {}

//-----------------------------------------------------------------------------
void Zombie::doAnimation () const
 {
    int t = Global_Timer.GetTime()/AnimationSpeedDivider;
    int ActiveDirection = (V_.x >= 0)?  1 : 0;

    Picture_.Draw (pos_, ActiveDirection, t);

    }

//-----------------------------------------------------------------------------
void Zombie::Control (int KeyStopMove)
    {
    Vec VDist = victim_->pos_ - pos_;

    if (VDist.Len() > 10)
        {
        Vec shortVector = VDist.Normalize () * ControlSpeedMultiplier;     //  V_ = !(Vec (txMousePos()) - pos_) * 15   | где ! - оператор нормализации
        V_ = shortVector;
        }
    else
        V_ = Vec (0, 0);


    if (GetAsyncKeyState ( KeyStopMove ))
        V_ = Vec (0, 0);
    }
//}
//-----------------------------------------------------------------------------

//{gmMouse::
gmMouse::gmMouse (const char *Name, Image Picture, Vec pos, Vec V):
    Hero (Name, Picture, pos, V, MOUSE)
    {}

//-----------------------------------------------------------------------------
void gmMouse::doAnimation () const
    {}

//-----------------------------------------------------------------------------
void gmMouse::Control (int KeyStopMove)
    {
    pos_ = Global_Mouse.GetClickPos();
    }

//}
//-----------------------------------------------------------------------------

//{FastZombie::
FastZombie::FastZombie (const char *Name, Image Picture, Vec pos, Vec V, const Hero *Steve) :
    Zombie (Name, Picture, pos, V, Steve)
    {
    AnimationSpeedDivider /= 4;
      ControlSpeedMultiplier *= 1.5;
    }


//}
//-----------------------------------------------------------------------------

void SteveAnimationNumber_and_Moving_Connecting (Hero *Steve)
    {

    //$ (Steve->GetV().Len()); $n;
    if (Steve->GetV().Len() <= DBL_EPSILON)
        {
        return;
        }


    double AnimationNumber = (360 - (Steve->GetV().Angle() + 90) )/20;
    AnimationNumber =  round (AnimationNumber);
    Steve->SetAnimation (( (int) AnimationNumber + 18)%18);


    }
//-----------------------------------------------------------------------------
void GeneralControl (Hero *object, int KeyStopMove)
    {
    Vec VDist = object->victim_->pos_ - object->pos_;

    if (VDist.Len() > 10)
        {
        Vec shortVector = VDist.Normalize () * ((object->Type_ == Hero::NPC)? 10 : 15);     //  V_ = !(Vec (txMousePos()) - pos_) * 15   | где ! - оператор нормализации
        object->V_ = shortVector;
        }
    else
        object->V_ = Vec (0, 0);


    if (GetAsyncKeyState ( KeyStopMove ))
        object->V_ = Vec (0, 0);
    }
//-----------------------------------------------------------------------------

void NoAnimation (const Hero */*object*/)
    {

    }
//-----------------------------------------------------------------------------


//{ slaves::


/*//-----------------------------------------------------------------------------

void CircleAnimation (const Hero *object, int Moving)
    {

    txBitBlt (txDC (), object->pos.x - object->Picture.Len.x/4/2, object->pos.y - object->Picture.Len.y/18/2,
                      object->Picture.Len.x/4, object->Picture.Len.y/18, object->Picture.Texture,
                      object->Picture.Len.x/4 * Moving, object->Picture.Len.y/18 * object->AnimationNumber);

    $ (object->AnimationNumber); $n;
    //$ (object->pos.y - object->Picture.Len.y/18/2); $n;
    //$ (object->Picture.Len.x/4);   $n;
    //$ (object->Picture.Len.y/18); $n;
    //$ (object->Picture.Len.y/18 * object->AnimationNumber);  $n;
    //getch ();

    }
            */
//-----------------------------------------------------------------------------

/*void SuperAnimation (const Hero *object, int Active)
    {
    int ActiveMirror = 0;
    if (object->V.x >= 0) ActiveMirror = 1; else ActiveMirror = 0;

    txAlphaBlend (txDC (), object->pos.x - object->Picture.Len.x/2, object->pos.y - object->Picture.Len.y/2,
                      object->Picture.Len.x, object->Picture.Len.y, object->Picture.Texture,
                      object->Picture.Len.x * Active, object->Picture.Len.y *(object->AnimationNumber * 2 + ActiveMirror));
    }
         */
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------


void StartCom ()
    {
    txCreateWindow (1800, 1000);

    txSetFillColor     (RGB (255, 255, 255));
    txClear ();
    txSetColor         (RGB(0, 0, 0));

    txBegin ();
    txDisableAutoPause();
    }

//-----------------------------------------------------------------------------

void PlsStopSpammingKeysGetYourHandsAwayFromTheKeyBoard_Arrrrrrrr ()
    {
    while (GetAsyncKeyState ('A') || GetAsyncKeyState ('S') ||
           GetAsyncKeyState ('W') || GetAsyncKeyState ('D'))
        Sleep (0);
    }

//}
//-----------------------------------------------------------------------------

/*  HAAARDCOOODE

    Hero* gmApplePointers  [N_OBJECTS] = {};
    Hero* gmZombiePointers [N_OBJECTS] = {};
    engbro->Get_AllObjects_ofThe_Type (gmApplePointers, APPLE);
    engbro->Get_AllObjects_ofThe_Type (gmZombiePointers, Hero::NPC);

    for (int i = 0; gmZombiePointers[i] != NULL; i++)
        {
        bool AppleWasEaten = false;
        double TheShortestDist_Between_CurrentZombie_and_TheNearestApple = 10000;
        Hero*  TheNearestApplePointer_toThe_CurrentZombie = NULL;

        for (int j = 0; gmApplePointers [j]; j++)
            {
            double Dist = hypot (gmApplePointers[j]->pos_.x - gmZombiePointers[i]->pos_.x,
                                 gmApplePointers[j]->pos_.y - gmZombiePointers[i]->pos_.y);

            if (Dist < TheShortestDist_Between_CurrentZombie_and_TheNearestApple)
                {
                TheShortestDist_Between_CurrentZombie_and_TheNearestApple = Dist;
                TheNearestApplePointer_toThe_CurrentZombie = gmApplePointers[j];
                }
            }

        if (TheNearestApplePointer_toThe_CurrentZombie)
            {
            gmZombiePointers[i]->victim_ = TheNearestApplePointer_toThe_CurrentZombie;

            if (HeroHypot (TheNearestApplePointer_toThe_CurrentZombie, gmZombiePointers[i]) < 50)
                {
                DeleteObjFromArray (TheNearestApplePointer_toThe_CurrentZombie, gmApplePointers,
                                    sizearr(gmApplePointers));
                engbro->FindAndRemove (TheNearestApplePointer_toThe_CurrentZombie);
                AppleWasEaten = true;
                }
            }

        if (AppleWasEaten)
            {
            for (int j = 0; gmZombiePointers[j]; j++)
                gmZombiePointers[j]->victim_ = steve;
            }
        }
  */

















