#include "TimaXLIbJustOneFunction.h"


#define ever (;;)


//-----------------------------------------------------------------------------
enum {APPLE = Hero::FIRSTUSERTYPE, BULLET, MINA};


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

struct Gun;

struct Steve : public Hero
    {
    int AppleDisgustingTimer_;
    int SpeedMultp_;
    int MaxSpeedmlt_;
    Gun *pushka_;
    //---------------
    Steve (const char *Name, Image Picture, Vec pos, Vec V, Engine* MrEngine);
    virtual void doAnimation () const override;
    virtual void Control   (int KeyStopMove = VK_SPACE) override;
    virtual void Logic () override;
    };

struct Zombie : public Hero
    {
    int AnimationSpeedDivider;
    int   ControlSpeedMultiplier;

    int AppleDisgustingTimer_;
    int SpeedMultp_;
    int MaxSpeedmlt_;
    //-----------------
    Zombie (const char *Name, Image Picture, Vec pos, Vec V, const Hero *Steve);
    virtual void doAnimation () const override;
    virtual void Control   (int KeyStopMove = VK_SPACE) override;
    virtual void Logic () override;

    void SpeedEff ();
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

struct Bullet : public Hero
    {

    //--------------------------------
    Bullet (Vec pos, Vec V);
    virtual void doAnimation () const override;
    };

struct Mina : public Hero
    {

    //---------------------------
    Mina (Vec pos);
    virtual void doAnimation () const override;
    };

struct Gun
    {
    enum GunMode {DISABLED, PISTOL, BAZOOKA};
    GunMode CurrentModeOfGun_;

    Engine*  MrEngine_;
    gmMouse* MrMouse_;
    Steve*   steve_;
    //-----------------------
    Gun (Engine* MrEngine, GunMode CurrentModeOfGun, Steve* steve);
    void shoot ();
    void Logic ();
    };


struct Controller
    {
    gmMouse *MrMouse_;
    Engine *MrEngine_;
    Steve *steve_;
    //-----------------------------
    Controller (Engine *MrEngine, Steve *steve_, gmMouse *MrMouse);
    void   AppleSpawn (Image texture);
    void  ZombieSpawn (Image texture);

    int   ZombieAteUrBrains (const Textures *textures, int BrainsBeingEatenRadius, int KolBoZombieNeededForUrBrainsBeingEaten);

    void  AppleZombieLogic (const Hero* EatenApple);
    Hero* AppleSteveLogic (int AppleBeingEatenRadius);
    void  GunZombieLogic ();

    void  ControlSpeedEff (int MaxSpeedmlt);
    void  EndScene ();

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
    gmMouse* mouse =  new gmMouse ("Mouse", Image (), Vec (),  Vec ());
    Engine MrEngine (mouse);

    Steve* steve   =  new Steve   ("Steve", textures->Steve,  Vec (500, 500), Vec (0, 0), &MrEngine);

    //Zombiezomb (&steve, &zombie);


    MrEngine.add (mouse);
    MrEngine.add (steve);
    MrEngine.add (new Zombie ("Zombie", textures->Zombie, Vec (700, 700), Vec (0, 0), steve));


    Controller Logic (&MrEngine, steve, mouse);


    bool StartTimer = false;
    int EndTime = 0;
    while (!GetAsyncKeyState (VK_ESCAPE))
        {
        MrEngine.Run();
        SteveAnimationNumber_and_Moving_Connecting (steve);
        if (StartTimer != true) Logic.ZombieSpawn (textures->Zombie);
        if (Logic.ZombieAteUrBrains (textures, 30, 2) == LOSE) StartTimer = true;



        if (Timer (StartTimer, 100, &EndTime) == LOSE) break;


        Hero* EatenApple = Logic.AppleSteveLogic (20);
        Logic.AppleZombieLogic (EatenApple);
        Logic.AppleSpawn (textures->Apple);
        Logic.GunZombieLogic ();

        steve->pushka_->Logic();

        if (/*!StartTimer && */GetAsyncKeyState(VK_TAB)) steve->pushka_->shoot ();

        if (StartTimer) Logic.EndScene ();


        }



    MrEngine.Destruct ();
    }



//-----------------------------------------------------------------------------

//{ Controller::


Controller::Controller (Engine *MrEngine, Steve *steve, gmMouse *MrMouse) :
    MrEngine_ (MrEngine),
    steve_ (steve),
    MrMouse_ (MrMouse)
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
void Controller::AppleZombieLogic (const Hero* EatenApple)
    {
    Hero* gmApplePointers  [N_OBJECTS] = {};
    Hero* gmZombiePointers [N_OBJECTS] = {};
    MrEngine_->Get_AllObjects_ofThe_Type (gmApplePointers, APPLE);
    MrEngine_->Get_AllObjects_ofThe_Type (gmZombiePointers, Hero::NPC);

    for (int i = 0; gmZombiePointers[i] != NULL; i++)
        {
        bool AppleWasEaten = false;
        double TheShortestDist_Between_CurrentZombie_and_TheNearestApple2 = 1e4*1e4;
        Hero*  TheNearestApplePointer_toThe_CurrentZombie = NULL;

        for (int j = 0; gmApplePointers [j]; j++)
            {
            double Dist2 = HeroHypot2 (gmApplePointers[j], gmZombiePointers[i]);

            if (Dist2 < TheShortestDist_Between_CurrentZombie_and_TheNearestApple2)
                {
                TheShortestDist_Between_CurrentZombie_and_TheNearestApple2 = Dist2;
                TheNearestApplePointer_toThe_CurrentZombie = gmApplePointers[j];
                }
            }

        if (TheNearestApplePointer_toThe_CurrentZombie)
            {
            gmZombiePointers[i]->victim_ = TheNearestApplePointer_toThe_CurrentZombie;

            if (HeroHypot2 (TheNearestApplePointer_toThe_CurrentZombie, gmZombiePointers[i]) < 50*50)
                {
                DeleteObjFromArray (TheNearestApplePointer_toThe_CurrentZombie, gmApplePointers,
                                    sizearr(gmApplePointers));
                MrEngine_->FindAndRemove (TheNearestApplePointer_toThe_CurrentZombie);

                Zombie* current = dynamic_cast <Zombie*> (gmZombiePointers[i]);
                if (current) current->SpeedEff();

                AppleWasEaten = true;
                }
            }
        if (gmZombiePointers[i]->victim_ == EatenApple && EatenApple)
            AppleWasEaten = true;

        if (AppleWasEaten)
            {
            for (int j = 0; gmZombiePointers[j]; j++)
                gmZombiePointers[j]->victim_ = steve_;
            }
        }
    }



//-----------------------------------------------------------------------------
Hero* Controller::AppleSteveLogic (int AppleBeingEatenRadius)
    {
    Hero* obj = NULL;
    Hero* eatenApple = NULL;
    for (int i = 0; i < MrEngine_->Get_KolBo_OfObjects(); i++)
        {
        Hero* obj = MrEngine_->GetObject(i);

        if ((obj->Type_ == APPLE) &&
            (HeroHypot (steve_, obj) ) < AppleBeingEatenRadius)
            {
            MrEngine_->remoov (i);
            steve_->AppleDisgustingTimer_ += 150;
            steve_->SpeedMultp_ += 1;
            eatenApple = obj;
            }
        }
    return eatenApple;
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





//-----------------------------------------------------------------------------
void Controller::GunZombieLogic ()
    {
    Hero* gmBulletPointers [N_OBJECTS] = {};
    Hero* gmZombiePointers [N_OBJECTS] = {};

    int nFilledobj = MrEngine_->Get_AllObjects_ofThe_Type (gmBulletPointers, BULLET);
    MrEngine_->Get_AllObjects_ofThe_Type (gmBulletPointers, MINA, nFilledobj);

    MrEngine_->Get_AllObjects_ofThe_Type (gmZombiePointers, Hero::NPC);

    for (int j = 0; gmBulletPointers[j]; j++)
        {
        for (int i = 0; gmZombiePointers[i]; i++)
            {
            int explodeR2 = (gmBulletPointers[j]->Type_ == MINA)? 50*50 : 13*13;
            if (HeroHypot2 (gmZombiePointers[i], gmBulletPointers[j])  <  explodeR2)
                {
                MrEngine_->FindAndRemove (gmBulletPointers[j]);
                MrEngine_->FindAndRemove (gmZombiePointers[i]);
                }
            }
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
Steve::Steve (const char *Name, Image Picture, Vec pos, Vec V, Engine* MrEngine) :
    Hero (Name, Picture, pos, V, CHARACTER),
    AppleDisgustingTimer_ (0),
    SpeedMultp_ (0),
    MaxSpeedmlt_ (5),
    pushka_ (new Gun (MrEngine, Gun::DISABLED, this))
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
        Vec shortVector = VDist.Normalize() * (5*1.5 + 0.5)*(SpeedMultp_ + 1);     //  V_ = !(Vec (txMousePos()) - pos_) * 15   | ��� ! - �������� ������������
        V_ = shortVector;
        }
    else
        V_ = Vec (0, 0);


    if (GetAsyncKeyState ( KeyStopMove ))
        V_ = Vec (0, 0);
    }




//-----------------------------------------------------------------------------
void Steve::Logic ()
    {
    if (AppleDisgustingTimer_ >= 0) AppleDisgustingTimer_--;
    else SpeedMultp_ = 0;

    if (SpeedMultp_ > MaxSpeedmlt_) SpeedMultp_ = SpeedMultp_;


    }




//}
//-----------------------------------------------------------------------------

//{Zombie::
Zombie::Zombie (const char *Name, Image Picture, Vec pos, Vec V, const Hero *Steve) :
    Hero (Name, Picture, pos, V, NPC, Steve),
    AnimationSpeedDivider (8),
    ControlSpeedMultiplier (5),
    AppleDisgustingTimer_ (0),
    SpeedMultp_ (0),
    MaxSpeedmlt_ (2)
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
        Vec shortVector = VDist.Normalize() * (ControlSpeedMultiplier)*(SpeedMultp_ + 1);     //  V_ = !(Vec (txMousePos()) - pos_) * 15   | ��� ! - �������� ������������
        V_ = shortVector;
        }
    else
        V_ = Vec (0, 0);


    if (GetAsyncKeyState ( KeyStopMove ))
        V_ = Vec (0, 0);
    }




//-----------------------------------------------------------------------------
void Zombie::Logic ()
    {
    if (AppleDisgustingTimer_ >= 0) AppleDisgustingTimer_--;
    else SpeedMultp_ = 0;

    if (SpeedMultp_ > MaxSpeedmlt_) SpeedMultp_ = SpeedMultp_;
    }



//-----------------------------------------------------------------------------
void Zombie::SpeedEff ()
    {
    AppleDisgustingTimer_ += 150;
    SpeedMultp_ += 1;
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

//{Bullet::
Bullet::Bullet (Vec pos, Vec V) :
    Hero ("Bullet", Image (), pos, V, BULLET)
    {}

//-----------------------------------------------------------------------------
void Bullet::doAnimation () const
    {
    txSetColor (TX_BLACK);
    txSetFillColor (TX_GRAY);
    txCircle (pos_.x, pos_.y, 10);
    }

//}
//-----------------------------------------------------------------------------

//{Mina::

Mina::Mina (Vec pos) :
    Hero ("Mina", Image (), pos, Vec (0, 0), MINA)
    {}


//-----------------------------------------------------------------------------
void Mina::doAnimation () const
    {
    txSetColor (TX_GREEN);
    txSetFillColor (TX_GREEN);
    txRectangle (pos_.x - 13, pos_.y - 10, pos_.x + 13, pos_.y + 10);
    }

//}
//-----------------------------------------------------------------------------

//{Gun::
Gun::Gun (Engine* MrEngine, GunMode CurrentModeOfGun, Steve* steve) :
    MrEngine_ (MrEngine),
    CurrentModeOfGun_ (CurrentModeOfGun),
    MrMouse_ (dynamic_cast <gmMouse*> (MrEngine->GetMouse()) ),
    steve_(steve)
    {}

//-----------------------------------------------------------------------------

void Gun::shoot ()
    {
    assert (MrMouse_);
    if (!steve_)
        return;



    $(steve_); $n;
    $(MrMouse_); $n;



    Vec BulletDirection = steve_->V_ + (MrMouse_->pos_ - steve_->pos_).Normalize()*7;

    if (CurrentModeOfGun_ == PISTOL)
        MrEngine_->add  (new Bullet (steve_->pos_, BulletDirection));

    if (CurrentModeOfGun_ == BAZOOKA)
        MrEngine_->add (new Mina (steve_->pos_));

    }


//-----------------------------------------------------------------------------
void Gun::Logic ()
    {
    if (GetAsyncKeyState ('1'))
        CurrentModeOfGun_ = PISTOL;
    if (GetAsyncKeyState ('2'))
        CurrentModeOfGun_ = BAZOOKA;

    if (!steve_)
        CurrentModeOfGun_ = DISABLED;
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
        Vec shortVector = VDist.Normalize () * ((object->Type_ == Hero::NPC)? 10 : 15);     //  V_ = !(Vec (txMousePos()) - pos_) * 15   | ��� ! - �������� ������������
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

















