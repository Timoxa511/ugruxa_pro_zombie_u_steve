#include "TimaXLIbJustOneFunction.h"


#define ever (;;)
#define ok() ifObjectIsReal()
#define ASSERT_OK()                                     \
    if (!ok())                                          \
        {                                               \
        txSleep(50);                                    \
        printf ("\nEEEERRRROOORRRRR\n");                \
        Dump ();                                        \
        txSleep(500);                                   \
        assert (0);                                     \
        }

#define ASSERT_OBJ_OK(obj)                              \
    if (!((obj) && (obj)->Hero::ok()))                  \
        {                                               \
        txOutputDebugPrintf("ERROR )*-*(  ");           \
        txSleep(50);                                    \
        printf ("\nEEEERRRROOORRRRR\n");                \
        (obj)->Hero::Dump();                            \
        txSleep(500);                                   \
        assert (0);                                     \
        }


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
    virtual ~Steve () override;

    virtual bool ifObjectIsReal () const override;

    virtual void Dump (int LeftSpace = 0) const override;
    virtual void doAnimation () const override;
    virtual void Control   (int KeyStopMove = VK_SPACE) override;
    virtual void Logic () override;
    };

struct Zombie : public Hero
    {
    int  AnimationSpeedDivider_;
    int ControlSpeedMultiplier_;
    int AppleDisgustingTimer_;
    int SpeedMultp_;
    int MaxSpeedmlt_;
    //-----------------
    Zombie (const char *Name, Image Picture, Vec pos, Vec V, const Hero *Steve);
   ~Zombie ();
    virtual void Dump (int LeftSpace = 0) const override;
    virtual bool ifObjectIsReal () const override;
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
    void* owner_;
    //-----------------------
   ~Gun ();
    void Dump (int LeftSpace = 0) const;
    bool ifObjectIsReal() const;
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
    bool  ifObjectIsReal();
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

    steve->Dump();

    //Zombiezomb (&steve, &zombie);


    MrEngine.add (mouse);
    MrEngine.add (steve, NET);
    steve->Dump();

    printf ("vladelec %p,   MrEngine%p\n", steve->owner_, MrEngine);
    txSleep(0);
    _getch();

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


        if (steve && steve->ok())
            {
            steve->pushka_->Logic();
            if (/*!StartTimer && */GetAsyncKeyState(VK_TAB)) steve->pushka_->shoot ();
            }

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
bool Controller::ifObjectIsReal()
    {
    return (this &&
            MrEngine_ && /*MrEngine_->ifObjectIsReal() &&*/
            steve_    &&    steve_->ifObjectIsReal() &&
            MrMouse_  &&  MrMouse_->ifObjectIsReal() );
    }



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
                TheNearestApplePointer_toThe_CurrentZombie->die();

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
            obj->die();
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
    if (rand()%1000 < 35)
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

                steve_->die();

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
                gmBulletPointers[j]->die();
                gmZombiePointers[i]->die();
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
    Hero ("Apple", Picture, pos, Vec(), APPLE)
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
    {
    ASSERT_OK();
    }


//-----------------------------------------------------------------------------
Steve::~Steve ()
    {
    ASSERT_OK();

    AppleDisgustingTimer_ = INTPOISON;
    SpeedMultp_ = INTPOISON;
    MaxSpeedmlt_ = INTPOISON;
    pushka_ = NULL;
    }



//-----------------------------------------------------------------------------
void Steve::Dump (int LeftSpace) const
    {
    bool is_ok = ifObjectIsReal();


    txOutputDebugPrintf ("%*sSteve\n"                         , LeftSpace, "");
    txOutputDebugPrintf ("%*s   {\n"                          , LeftSpace, "");
    txOutputDebugPrintf ("%*s   %s\n"                         , LeftSpace, "", (is_ok)? "ok" : "ERROR");
    Hero::Dump (LeftSpace + LFTSPC);
    txOutputDebugPrintf ("%*s   AppleDisgustingTimer_ = %d\n" , LeftSpace, "", AppleDisgustingTimer_);
    txOutputDebugPrintf ("%*s   SpeedMultp_  = %d\n"          , LeftSpace, "", SpeedMultp_);
    txOutputDebugPrintf ("%*s   MaxSpeedmlt_ = %d\n"          , LeftSpace, "", MaxSpeedmlt_);
    txOutputDebugPrintf ("%*s   *pushka_     = %p\n"          , LeftSpace, "", pushka_);
    if (pushka_) pushka_->Dump (LeftSpace + LFTSPC);
    txOutputDebugPrintf ("%*s   }\n\n"                        , LeftSpace, "");
    }



//-----------------------------------------------------------------------------
bool Steve::ifObjectIsReal () const
    {
    return (this &&
            Hero::ifObjectIsReal() &&

            AppleDisgustingTimer_ != INTPOISON &&
            SpeedMultp_  != INTPOISON &&
            MaxSpeedmlt_ != INTPOISON &&
            owner_ == this &&
            pushka_ && pushka_->ifObjectIsReal()  );
    }



//-----------------------------------------------------------------------------
void Steve::doAnimation () const
    {
    ASSERT_OK();

    if (DoubleCompareWithZero (V_.x) != 0 && DoubleCompareWithZero (V_.y) != 0)
        {
        int t = Global_Timer.GetTime()/8;

        Picture_.Draw (pos_, AnimationNumber_, t);
        }

    else
        Picture_.Draw (pos_, AnimationNumber_, 0);

    ASSERT_OK();
    }




//-----------------------------------------------------------------------------
void Steve::Control (int KeyStopMove )
    {
    ASSERT_OK();

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

    ASSERT_OK();
    }




//-----------------------------------------------------------------------------
void Steve::Logic ()
    {
    ASSERT_OK();

    if (AppleDisgustingTimer_ >= 0) AppleDisgustingTimer_--;
    else SpeedMultp_ = 0;

    if (SpeedMultp_ > MaxSpeedmlt_) SpeedMultp_ = SpeedMultp_;

    ASSERT_OK();
    }




//}
//-----------------------------------------------------------------------------

//{Zombie::
Zombie::Zombie (const char *Name, Image Picture, Vec pos, Vec V, const Hero *Steve) :
    Hero (Name, Picture, pos, V, NPC, Steve),
    AnimationSpeedDivider_ (8),
    ControlSpeedMultiplier_ (5),
    AppleDisgustingTimer_ (0),
    SpeedMultp_ (0),
    MaxSpeedmlt_ (2)
    {
    ASSERT_OK();
    }


//-----------------------------------------------------------------------------
Zombie::~Zombie ()
    {
    ASSERT_OK();

    AnimationSpeedDivider_ = INTPOISON;
    ControlSpeedMultiplier_ = INTPOISON;
    AppleDisgustingTimer_ = INTPOISON;
    SpeedMultp_ = INTPOISON;
    MaxSpeedmlt_ = INTPOISON;
    }




//-----------------------------------------------------------------------------
void Zombie::Dump (int LeftSpace) const
    {
    bool is_ok = ifObjectIsReal();


    txOutputDebugPrintf ("%*sZombie\n"                           , LeftSpace, "");
    txOutputDebugPrintf ("%*s   {\n"                             , LeftSpace, "");
    txOutputDebugPrintf ("%*s   %s\n"                            , LeftSpace, "", (is_ok)? "ok" : "ERROR");
    Hero::Dump (LeftSpace + LFTSPC);
    txOutputDebugPrintf ("%*s   AnimationSpeedDivider_   = %d\n" , LeftSpace, "", AnimationSpeedDivider_);
    txOutputDebugPrintf ("%*s   ControlSpeedMultiplier_  = %d\n" , LeftSpace, "", ControlSpeedMultiplier_);
    txOutputDebugPrintf ("%*s   AppleDisgustingTimer_    = %d\n" , LeftSpace, "",  AppleDisgustingTimer_);
    txOutputDebugPrintf ("%*s   SpeedMultp_   = %d\n"            , LeftSpace, "", SpeedMultp_);
    txOutputDebugPrintf ("%*s   MaxSpeedmlt_  = %d\n"            , LeftSpace, "", MaxSpeedmlt_);
    txOutputDebugPrintf ("%*s   }\n\n"                           , LeftSpace, "");


    }



//-----------------------------------------------------------------------------
bool Zombie::ifObjectIsReal () const
    {
    return (this &&
            Hero::ifObjectIsReal() &&
            AnimationSpeedDivider_  != INTPOISON &&
            ControlSpeedMultiplier_ != INTPOISON &&
            AppleDisgustingTimer_  != INTPOISON &&
            SpeedMultp_  != INTPOISON &&
            MaxSpeedmlt_ != INTPOISON );

    }


//-----------------------------------------------------------------------------
void Zombie::doAnimation () const
    {
    ASSERT_OK();

    int t = Global_Timer.GetTime()/AnimationSpeedDivider_;
    int ActiveDirection = (V_.x >= 0)?  1 : 0;

    Picture_.Draw (pos_, ActiveDirection, t);

    ASSERT_OK();
    }




//-----------------------------------------------------------------------------
void Zombie::Control (int KeyStopMove)
    {
    ASSERT_OK();

    Vec VDist = victim_->pos_ - pos_;

    if (VDist.Len() > 10)
        {
        Vec shortVector = VDist.Normalize() * (ControlSpeedMultiplier_)*(SpeedMultp_ + 1);     //  V_ = !(Vec (txMousePos()) - pos_) * 15   | ��� ! - �������� ������������
        V_ = shortVector;
        }
    else
        V_ = Vec (0, 0);


    if (GetAsyncKeyState ( KeyStopMove ))
        V_ = Vec (0, 0);

    ASSERT_OK();
    }




//-----------------------------------------------------------------------------
void Zombie::Logic ()
    {
    ASSERT_OK();

    if (AppleDisgustingTimer_ >= 0) AppleDisgustingTimer_--;
    else SpeedMultp_ = 0;

    if (SpeedMultp_ > MaxSpeedmlt_) SpeedMultp_ = SpeedMultp_;

    ASSERT_OK();
    }



//-----------------------------------------------------------------------------
void Zombie::SpeedEff ()
    {
    ASSERT_OK();

    AppleDisgustingTimer_ += 150;
    SpeedMultp_ += 1;

    ASSERT_OK();
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
    ASSERT_OK();

    pos_ = Global_Mouse.GetClickPos();

    ASSERT_OK();
    }



//}
//-----------------------------------------------------------------------------

//{FastZombie::
FastZombie::FastZombie (const char *Name, Image Picture, Vec pos, Vec V, const Hero *Steve) :
    Zombie (Name, Picture, pos, V, Steve)
    {
    AnimationSpeedDivider_ /= 4;
    ControlSpeedMultiplier_ *= 1.5;

    ASSERT_OK();
    }


//}
//-----------------------------------------------------------------------------

//{Bullet::
Bullet::Bullet (Vec pos, Vec V) :
    Hero ("Bullet", Image (), pos, V, BULLET)
    {
    ASSERT_OK();
    }

//-----------------------------------------------------------------------------
void Bullet::doAnimation () const
    {
    ASSERT_OK();

    txSetColor (TX_BLACK);
    txSetFillColor (TX_GRAY);
    txCircle (pos_.x, pos_.y, 10);

    ASSERT_OK();
    }

//}
//-----------------------------------------------------------------------------

//{Mina::

Mina::Mina (Vec pos) :
    Hero ("Mina", Image (), pos, Vec (0, 0), MINA)
    {
    ASSERT_OK();
    }


//-----------------------------------------------------------------------------
void Mina::doAnimation () const
    {
    ASSERT_OK();

    txSetColor (TX_GREEN);
    txSetFillColor (TX_GREEN);
    txRectangle (pos_.x - 13, pos_.y - 10, pos_.x + 13, pos_.y + 10);

    ASSERT_OK();
    }

//}
//-----------------------------------------------------------------------------

//{Gun::
Gun::Gun (Engine* MrEngine, GunMode CurrentModeOfGun, Steve* steve) :
    MrEngine_ (MrEngine),
    CurrentModeOfGun_ (CurrentModeOfGun),
    MrMouse_ (dynamic_cast <gmMouse*> (MrEngine->GetMouse()) ),
    steve_(steve)
    {
    ASSERT_OK();
    }

//-----------------------------------------------------------------------------
Gun::~Gun ()
    {
    ASSERT_OK();

    MrEngine_ = NULL;
    CurrentModeOfGun_ = (GunMode) INTPOISON;
    MrMouse_ = NULL;
    steve_ = NULL;
    }



//-----------------------------------------------------------------------------
void Gun::Dump (int LeftSpace) const
{
    bool is_ok = ifObjectIsReal();


    txOutputDebugPrintf ("%*sGun\n"                       , LeftSpace, "");
    txOutputDebugPrintf ("%*s   this = %p\n"              , LeftSpace, "", this);
    txOutputDebugPrintf ("%*s   {\n"                      , LeftSpace, "");
    txOutputDebugPrintf ("%*s   %s\n"                     , LeftSpace, "", (is_ok)? "ok" : "ERROR");
    txOutputDebugPrintf ("%*s   CurrentModeOfGun_ = %d\n" , LeftSpace, "", CurrentModeOfGun_);
    txOutputDebugPrintf ("%*s   MrEngine_ = %p\n"         , LeftSpace, "", MrEngine_);
    txOutputDebugPrintf ("%*s   MrMouse_ = %p\n"          , LeftSpace, "", MrMouse_);
    if (MrMouse_) MrMouse_->Dump (LFTSPC);
    txOutputDebugPrintf ("%*s   steve_   = %p\n"          , LeftSpace, "", steve_);
    txOutputDebugPrintf ("%*s   }\n\n"                    , LeftSpace, "");


    }




//-----------------------------------------------------------------------------
bool Gun::ifObjectIsReal() const
    {
    return (this &&
            CurrentModeOfGun_ != INTPOISON &&
            MrEngine_ && /*MrEngine_->ifObjectIsReal() &&*/
            steve_    &&    /*steve_->ifObjectIsReal() &&*/
            MrMouse_  &&    MrMouse_->ifObjectIsReal() );
    }


//-----------------------------------------------------------------------------
void Gun::shoot ()
    {
    ASSERT_OK();

    assert (MrMouse_);
    if (!steve_)
        return;



    $(steve_); $n;
    $(MrMouse_); $n;



    Vec BulletDirection = steve_->V_ + (MrMouse_->pos_ - steve_->pos_).Normalize()*30 ;

    if (CurrentModeOfGun_ == PISTOL)
        MrEngine_->add  (new Bullet (steve_->pos_, BulletDirection));

    if (CurrentModeOfGun_ == BAZOOKA)
        MrEngine_->add (new Mina (steve_->pos_));

    ASSERT_OK();
    }



//-----------------------------------------------------------------------------
void Gun::Logic ()
    {
    ASSERT_OK();

    if (GetAsyncKeyState ('1'))
        CurrentModeOfGun_ = PISTOL;
    if (GetAsyncKeyState ('2'))
        CurrentModeOfGun_ = BAZOOKA;

    if (!steve_)
        CurrentModeOfGun_ = DISABLED;

    ASSERT_OK();
    }



//}
//-----------------------------------------------------------------------------


void SteveAnimationNumber_and_Moving_Connecting (Hero *Steve)
    {
    ASSERT_OBJ_OK(Steve);
    //$ (Steve->GetV().Len()); $n;
    if (Steve->GetV().Len() <= DBL_EPSILON)
        {
        return;
        }


    double AnimationNumber = (360 - (Steve->GetV().Angle() + 90) )/20;
    AnimationNumber =  round (AnimationNumber);
        Steve->SetAnimation (( (int) AnimationNumber + 18)%18);

    ASSERT_OBJ_OK(Steve);
    }

//-----------------------------------------------------------------------------
void GeneralControl (Hero *object, int KeyStopMove)
    {
    ASSERT_OBJ_OK(object);

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

    ASSERT_OBJ_OK(object);
    }
//-----------------------------------------------------------------------------

void NoAnimation (const Hero */*object*/)
    {}
//-----------------------------------------------------------------------------




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
    //txDisableAutoPause();
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

















