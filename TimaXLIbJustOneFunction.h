#include "TXlib.h"

#define CAP(action) action

FILE* Log = fopen ("Log.txt", "w");//stdout;

enum {NO = false, YES = true};
const double DT = 1.0;
const int LFTSPC = 3;

const int INTPOISON = -66666;
const double DOUBLEPOISON = -666.666666;

//-----------------------------------------------------------------------------

template <typename T = double>
class Vector
    {
    public:
    T x, y;

    //-----------

    Vector ();
    Vector (T a, T b);
   ~Vector ();
    Vector (POINT point);


    Vector <T> Normalize () const;
    double Angle () const;
    double Len   () const;

    Vector <T> operator + (Vector <T> that ) const;
    Vector <T> operator - (Vector <T> that) const;
    Vector <T> operator / (T divider) const;
    Vector <T> operator * (T multiplier) const;
    };
typedef Vector <> Vec;     //typedef какой-то_тип пробел название_синонима_к_этому_типу                      Vec = Vector <>


class Image
    {

    private:
    HDC Texture_;
    Vector <> Len_;
    Vector <int> nFrames_ ;

    //------------------------------------------

    public:
    explicit Image (const char FilePictureName [], Vector <int> nFrames = Vector <int> (1, 1)); //explicit - запрещ€ет не€вный вызов конструктора с одним параметром (ибо если не защищать то могут пойти оч сложные ошибки)
    Image ();
   ~Image ();

    Vector <int> nFramesReturn () const;
    Vec GetLen () const;
    HDC GetTexture () const;

    void  Draw (Vec pos, int AnimationNumber, int t) const;
    void  DeleteTex ();

    };

class Hero;              // € обь€вд€ю класс (еще не целиком) что бы defcontrol работал так как он работает с классом геро€
                         // и в классе геро€ же используетс€ => что бы можно было бы указывать defcontrol  в конструкторе
class Hero
    {

    public:
    const char *Name_;
    Image Picture_;

    Vec pos_;
    Vec V_;

    int AnimationNumber_;
    int Type_;

    enum LifeStatus {DEAD, ALIVE};
    LifeStatus status_;

    const Hero *victim_;

    //--------------------------------
    public:


    enum {NPC, CHARACTER, MOUSE, PICTURE, FIRSTUSERTYPE};

    //---------------------

    Hero ();
    virtual ~Hero ();
    Hero (const char *Name, Image Picture, Vec pos, Vec V, int Type,
          const Hero *victim = NULL, int AnimationNumber = 0);

    virtual void Dump (int LeftSpace = 0) const;

    virtual bool ifObjectIsReal () const;
    void Tag (LifeStatus status);
    void die ();

    Vec GetV () const;

    bool SetAnimation (int AnimationNumber);

    virtual void Move (double dt);
    virtual void Logic ();
    virtual void Control   (int KeyStopMove = VK_SPACE);
    virtual void doAnimation () const;
    };

class Mouse
    {
    public:
    Vec pos_;
    Vec LastClickPos_;
    int LastClickButtons_;

    //-----------------------

    Mouse ();

    Vec  GetPos();
    int  GetButtons();
    Vec  GetClickPos();
    int  GetClickButtons();
    void Update();
    };

class Time
    {
    private:
    int t_;
    //----
    public:
    Time ();
    int GetTime();
    void Update();
    };

struct Animation : public Hero
    {
    Animation (Image Picture, Vec pos);
    virtual void doAnimation () const override;
    virtual void Control   (int KeyStopMove = VK_SPACE) override;
    };

struct Tool : public Hero
    {

    //--------------------------------
    Tool (Vec pos, Vec V, int Type);

    virtual void Move (double dt) override;
    virtual void Logic () override;
    virtual void Control   (int KeyStopMove = VK_SPACE) override;
    virtual void doAnimation () const override;
    };


//-----------------------------------------------------------------------------


const int N_OBJECTS = 4000;

class Engine
    {
    private:
    Hero* objects_[N_OBJECTS];
    int freePlace_;
    Hero* Mouse_;

    //--------------------------------------
    void  KillTheTagged ();
    void  remoov (int numObj);
    //--------------------------------------
    public:
    Engine (Hero* Mouse = NULL);

    bool  ifObjectIsReal();
    void  PrintfAllIncludes();

    Hero* GetMouse () const;
    int   GetObjectNumber (const char *name) const;
    int   GetObjNum (const Hero *object) const;
    Hero* GetObject (int num) const;
    int   Get_KolBo_OfObjects () const;
    int   Get_KolBo_TypeObjects (int Type) const;
    int   Get_AllObjects_ofThe_Type (Hero* gmTypePointers [], int Type, int Start = 0);

    void  add    (Hero *object);



    void  Destruct ();
    void  Run ();
    };


int DoubleCompareWithZero (double number);
double HeroHypot  (const Hero *first, const Hero *Second);
double HeroHypot2 (const Hero *first, const Hero *Second);
double hypot2 (double dx, double dy);

void DeleteObjFromArray (const Hero *object, const Hero* Array [], int ArraySize);
int  GetObjArrayNum     (const Hero *object, const Hero* Array [], int ArraySize);
void DeleteFromArray (int objNum, const Hero* Array [], int ArraySize);

//-----------------------------------------------------------------------------


 //
 //
 //
 //           ___________                    ____________
 //               <(|)>                               #@
 //
 //
 Mouse Global_Mouse;
 Time  Global_Timer;
 //
 //
 //
 //
 //          )________                            ______(
 //                   \_______          _________/
 //                           \________/
 //
 //
 //
 //
 //                                                                  Bill
                                                                                  //                | имеетс€   hero | имеетс€ hero
                                                                                  //                | в форме обьекта| в форме указател€

                                                                                  // нужен в        |  hero          | *hero
                                                                                  // форме обьекта  |                |
//{ Engine::

Engine::Engine (Hero* Mouse) :                                                               // нужен в        |  &hero         | hero                         |
    objects_  ({}),                                                               // форме указател€|
    freePlace_ (N_OBJECTS),
    Mouse_ (Mouse)
    {
    }



//-----------------------------------------------------------------------------
void Engine::PrintfAllIncludes ()
    {
    txOutputDebugPrintf ("Engine\n{\n");
    txOutputDebugPrintf ("freePlace_ = %d\n", freePlace_);
    txOutputDebugPrintf (" N_OBJECTS = %d\n" , N_OBJECTS);
    txOutputDebugPrintf ("  objects_ = 0x%p\n", objects_);
    txOutputDebugPrintf ("    Mouse_ = 0x%p\n", Mouse_);
    txOutputDebugPrintf ("}\n\n");

    for (int i = 0; i < Get_KolBo_OfObjects (); i++)
        {
        txOutputDebugPrintf ("i = %d, pointer = 0x%p\n", i, objects_[i]);
        if (objects_[i]) objects_[i]->Dump();
        }
    }



//-----------------------------------------------------------------------------
bool Engine::ifObjectIsReal()
    {

    if (!(this &&
          freePlace_ != INTPOISON &&
          0 < freePlace_ && freePlace_ < N_OBJECTS &&
          Mouse_ &&
          Mouse_->ifObjectIsReal() ) ) return false;

    int i = 0;
    for (; i < Get_KolBo_OfObjects(); i++)
        {
        if (!(objects_[i] && objects_[i]->ifObjectIsReal() ) ) return false;
        }
    for (; i < N_OBJECTS; i++)
        {
        if (objects_[i]) return false;
        }

    return true;
    }



//-----------------------------------------------------------------------------
Hero* Engine::GetMouse () const
    {
    return Mouse_;
    }





//-----------------------------------------------------------------------------

int Engine::GetObjectNumber (const char *name) const
    {
    assert (name != NULL);

    for (int i = 0; i < N_OBJECTS; i++)
        {
        assert (objects_[i]->Name_ != NULL);

        if (strcmp (name, objects_[i]->Name_) == 0)
            {
            return i;
            }
        }
    return -1;
    }




//-----------------------------------------------------------------------------

Hero* Engine::GetObject (int num) const
    {
    assert (0 <= num && num < N_OBJECTS);
    return objects_[num];
    }



//-----------------------------------------------------------------------------
int Engine::GetObjNum (const Hero *object) const
    {
    for (int i = 0; i < Get_KolBo_OfObjects(); i++)
        {
        if (objects_[i] == object) return i;
        }
    return -1;
    }




//-----------------------------------------------------------------------------

int Engine::Get_KolBo_OfObjects () const
    {
    return (N_OBJECTS - freePlace_);
    }




//-----------------------------------------------------------------------------
int Engine::Get_KolBo_TypeObjects (int Type) const
    {
    int Kol_Bo_ofTypeObjects = 0;
    for (int i = 0; i < Get_KolBo_OfObjects(); i++)
        {
        if (GetObject(i)->Type_ == Type)
            Kol_Bo_ofTypeObjects++;
        }
    return Kol_Bo_ofTypeObjects;
    }




//-----------------------------------------------------------------------------
int Engine::Get_AllObjects_ofThe_Type (Hero* gmTypePointers [], int Type, int Start)
    {
    int nObjects = 0;
    for (int i = 0; i < Get_KolBo_OfObjects(); i++)
        {
        Hero* obj = GetObject(i);
        if (obj->Type_ == Type)
            {
            gmTypePointers[Start + nObjects++] = obj;
            }
        }
    return nObjects;

    }



//-----------------------------------------------------------------------------
void Engine::add (Hero *object)
    {

    assert (0 <= (Get_KolBo_OfObjects()) && (Get_KolBo_OfObjects()) < N_OBJECTS);

    objects_ [Get_KolBo_OfObjects()] = object;

    if (object->Type_ == 4)
                {
                txOutputDebugPrintf ("%s: %p i=%2d ADD\n",__TX_FUNCTION__, object, Get_KolBo_OfObjects());
                }

    freePlace_--;


    }




//-----------------------------------------------------------------------------
void Engine::remoov (int numObj)
    {
    assert (0 <= numObj && numObj < Get_KolBo_OfObjects());


    delete objects_[numObj];

    objects_[numObj] = objects_[Get_KolBo_OfObjects() - 1];
    objects_[Get_KolBo_OfObjects() - 1] = NULL;
    freePlace_ ++;
    }





void Engine::Destruct ()
    {
    for (int i = 0; i < N_OBJECTS; i++)
        delete objects_[i];
    }




//-----------------------------------------------------------------------------
void Engine::Run ()
    {
    if (!ifObjectIsReal())
        {
        PrintfAllIncludes();
        assert (0);
        }

    KillTheTagged();

    txSetFillColor (TX_WHITE);
    txClear ();
    for (int i = 0; i < Get_KolBo_OfObjects(); i++)
        {
        assert (0 <= i && i < (N_OBJECTS - freePlace_));
        objects_[i]->Control ();
        objects_[i]->Move (DT);
        objects_[i]->doAnimation ();
        objects_[i]->Logic ();
        //objects_[i]->Dump ();
        }
    txSleep (20);
    Global_Mouse.Update();
    Global_Timer.Update();
    }




//-----------------------------------------------------------------------------

void Engine::KillTheTagged ()
    {
    for (int i = 0; i < Get_KolBo_OfObjects(); i++)
        {
        assert (0 <= i && i < (N_OBJECTS - freePlace_));

        if (objects_[i]->status_ == Hero::DEAD)
            {
            $ (i); $n;
            printf ("€ убила хе\n");
            $ (objects_[i]->Type_); $n;
            if (objects_[i]->Type_ == 4)
                {
                txOutputDebugPrintf ("%p i=%2d DELETE\n", objects_[i],i);
                }
            remoov (i--);
            }
        }
    }



//}


//-----------------------------------------------------------------------------

//{ otherClasses


//{ Hero::
Hero::Hero () :
    Name_ ("Ama Default Fish, Booub"),

    Picture_ (),                    //Picture = Image ();              Image cfgyui ("fghjk"); Picture = cfgyui;         // кстати можно не писать Picture () потому что у image есть дефолт конструктор

    pos_ (),
    V_ (),

    AnimationNumber_ (0),
    Type_ (0),

    status_ (ALIVE),
    victim_ (NULL)

    {
    }

//-----------------------------------------------------------------------------

Hero::Hero (const char *Name, Image Picture, Vec pos, Vec V, int Type,
            const Hero *victim, int AnimationNumber) :

    Name_ (Name),
    Picture_ (Picture),
    pos_ (pos),
    V_ (V),
    AnimationNumber_ (AnimationNumber),
    Type_ (Type),
    victim_ (victim),
    status_ (ALIVE)
    {
    }


//-----------------------------------------------------------------------------
Hero::~Hero ()
    {
    Name_ = NULL;
    AnimationNumber_ = INTPOISON;
    Type_ = INTPOISON;
    victim_ = NULL;
    status_ = (LifeStatus) INTPOISON;
    }



//-----------------------------------------------------------------------------
void Hero::Dump (int LeftSpace) const
    {
    bool is_ok = ifObjectIsReal();

    txOutputDebugPrintf ("%*sHero\n"                     , LeftSpace, "");
    txOutputDebugPrintf ("%*s   {\n"                     , LeftSpace, "");
    txOutputDebugPrintf ("%*s   %s\n"                    , LeftSpace, "", (is_ok)? "ok" : "ERROR");
    txOutputDebugPrintf ("%*s   name = %s\n"             , LeftSpace, "", Name_);
    txOutputDebugPrintf ("%*s   pos  = {%lg, %lg}\n"     , LeftSpace, "", pos_.x, pos_.y);
    txOutputDebugPrintf ("%*s   v    = {%d, %d}\n"       , LeftSpace, "", V_.x, V_.y);
    txOutputDebugPrintf ("%*s   AnimationNumber_ = %d\n" , LeftSpace, "", AnimationNumber_);
    txOutputDebugPrintf ("%*s   Type_   = %d\n"            , LeftSpace, "", Type_);
    txOutputDebugPrintf ("%*s   status_ = %d\n"          , LeftSpace, "", status_);
    txOutputDebugPrintf ("%*s   victim  = %p\n"           , LeftSpace, "", victim_);
    if (victim_) victim_->Dump (LeftSpace + LFTSPC);
    txOutputDebugPrintf ("%*s   }\n\n"                   , LeftSpace, "");


    }
//-----------------------------------------------------------------------------
bool Hero::ifObjectIsReal () const
    {
    return (this &&
            Name_ &&
            AnimationNumber_ != INTPOISON &&
            status_ != INTPOISON );
    }



//-----------------------------------------------------------------------------
void Hero::Tag (LifeStatus status)
    {
    CAP(status_ = status);
    }



//-----------------------------------------------------------------------------
void Hero::die ()
    {
    Tag (DEAD);
    }


//-----------------------------------------------------------------------------
Vec Hero::GetV () const
    {
    return V_;
    }
//-----------------------------------------------------------------------------

void Hero::Move (double dt)
    {
    pos_ = pos_ + V_*dt;
    }

//-----------------------------------------------------------------------------
void Hero::Control (int /*KeyStopMove*/)
    {

    }
//-----------------------------------------------------------------------------

bool Hero::SetAnimation (int AnimationNumber)
    {
    if (AnimationNumber >= Picture_.nFramesReturn().y || AnimationNumber < 0)
        {
        printf ("ERROR: wrong AnimationNumber\n"
                "       %s's AnimationNumber = %d\n"
                "       trying to set AnimationNumber = %d\n\n", Name_, Picture_.nFramesReturn().y, AnimationNumber);
        return NO;
        }

    AnimationNumber_ = AnimationNumber;

    return YES;
    }

//-----------------------------------------------------------------------------
void Hero::doAnimation () const
    {
    if (Picture_.GetTexture() != NULL)
        {
        Picture_.Draw (pos_, 0, 0);
        }
    }


//-----------------------------------------------------------------------------

void Hero::Logic ()
    {}
//}
//-----------------------------------------------------------------------------


//{ Animation::

Animation::Animation (Image Picture, Vec pos) :
    Hero ("Picture", Picture, pos, Vec (), PICTURE)
    {}
//-----------------------------------------------------------------------------

void Animation::doAnimation () const
    {
    int t = Global_Timer.GetTime()/3;
    Picture_.Draw (pos_, 0, t);
    }

//-----------------------------------------------------------------------------
void Animation::Control (int KeyStopMove)
    {
    }
//}
//-----------------------------------------------------------------------------


//{ Image::
//-----------------------------------------------------------------------------
Image::Image () :
    Texture_ (NULL),
    Len_     (),
    nFrames_ ()
    {
    }
//-----------------------------------------------------------------------------

Image::Image (const char FilePictureName [], Vector <int> nFrames) :
    Texture_ (txLoadImage (FilePictureName)),
    Len_     (),
    nFrames_ (nFrames)
    {

    if (Texture_ == NULL)
        {
        char Message [999] = "";
        sprintf (Message, "”пс, %s не загружаетс€", FilePictureName);
        txMessageBox (Message, "программа не работает", MB_ICONEXCLAMATION);;
        return;
        }

    HBITMAP hbitmap = (HBITMAP) Win32::GetCurrentObject (Texture_, OBJ_BITMAP);

    BITMAP bitmap = {};

    Win32::GetObject (hbitmap, sizeof(bitmap), &bitmap);

    Len_.x = bitmap.bmWidth;
    Len_.y = bitmap.bmHeight;

    }

//-----------------------------------------------------------------------------
Image::~Image ()
    {
    //TODO Texture. DeleteTex();
    Len_ = Vec (INTPOISON, INTPOISON);
    nFrames_ = Vector <int> (INTPOISON, INTPOISON);

    }
//-----------------------------------------------------------------------------
Vector <int> Image::nFramesReturn ()   const
    {
    return nFrames_;
    }

//-----------------------------------------------------------------------------
Vec Image::GetLen () const
    {
    return Len_;
    }

//-----------------------------------------------------------------------------
HDC Image::GetTexture () const
    {
    return Texture_;
    }
//-----------------------------------------------------------------------------
void Image::DeleteTex ()
    {
    txDeleteDC (Texture_);
    Texture_ = NULL;
    }
//-----------------------------------------------------------------------------
void Image::Draw (Vec pos, int AnimationNumber, int t) const
    {
    int CurrentFrame = t % nFrames_.x;
    Vector <int> FrameLen ((int) (Len_.x/nFrames_.x), (int) (Len_.y/nFrames_.y));

    txTransparentBlt (txDC (), pos.x - FrameLen.x/2, pos.y - FrameLen.y/2, FrameLen.x, FrameLen.y,
                      Texture_, FrameLen.x * CurrentFrame, FrameLen.y * AnimationNumber);
    }

//-----------------------------------------------------------------------------
//}
//-----------------------------------------------------------------------------



//{ Vector::

template <typename T>
Vector <T>::~Vector ()
    {
    x = INTPOISON;
    y = INTPOISON;
    }

//-----------------------------------------------------------------------------

template <typename T>
Vector <T>::Vector () :
    x (0),
    y (0)
    {
    }

//-----------------------------------------------------------------------------
template <typename T>
Vector <T>::Vector (POINT point) :
    x (point.x),
    y (point.y)
    {
    }

//-----------------------------------------------------------------------------

template <typename T>
Vector <T>::Vector (T a, T b) :
    x (a),
    y (b)
    {
    }

//-----------------------------------------------------------------------------
template <typename T>
Vector <T> Vector <T>::operator - (Vector <T> that) const
                                                                            //Vector C = Minus (a, b);
                                                                            //Vector C = a.Minus (b);
    {
    Vector <T> res (x - that.x, y - that.y);

    return res;
    }


//-----------------------------------------------------------------------------
template <typename T>
Vector <T> Vector <T>::operator + (Vector <T> that ) const
                                                                            //Vector C = Minus (a, b);
                                                                            //Vector C = a.Minus (b);
    {
    Vector <T> res (this->x + that.x, this->y + that.y);

    return res;
    }
//-----------------------------------------------------------------------------

template <typename T>
Vector <T> Vector <T>::operator / (T divider) const
                                                                            //Vector C = Minus (a, b);                                                                         //Vector C = a.Minus (b);
    {
    Vector <T> res (x/divider, y/divider);

    return res;
    }


//-----------------------------------------------------------------------------

template <typename T>
double Vector <T>::Len () const
    {
    double VectorLen = hypot (x, y);
    return VectorLen;
    }

//-----------------------------------------------------------------------------
template <typename T>
Vector <T> Vector <T>::Normalize () const
    {
    double len = Len();
    if (len < DBL_EPSILON)
        return Vector <T> (0, 0);

    Vector <T> NormalizedVector = *this/len;
    return NormalizedVector;

    }

//-----------------------------------------------------------------------------
template <typename T>
Vector <T> Vector <T>::operator * (T multiplier) const
    {
    Vector <T> res (x * multiplier, y * multiplier);

    return res;
    }


//-----------------------------------------------------------------------------
template <typename T>
double Vector <T>::Angle () const
    {
    Vec promezshootochnoe_nazvanie = this->Normalize ();
    double angle = acos (promezshootochnoe_nazvanie.x);

    angle = angle/M_PI*180;
    if (y > 0) angle = -angle + 360;

    return angle;
    }


//}
//-----------------------------------------------------------------------------



//{ Mouse::
Mouse::Mouse () :
    pos_ (),
    LastClickPos_ ()
    {}


//-----------------------------------------------------------------------------
int Mouse::GetButtons()
    {
    return txMouseButtons();
    }

//-----------------------------------------------------------------------------
Vec Mouse::GetPos()
    {
    return Vec (txMousePos());
    }

//-----------------------------------------------------------------------------
Vec Mouse::GetClickPos()
    {
    return LastClickPos_;
    }

//-----------------------------------------------------------------------------
int  Mouse::GetClickButtons()
    {
    return LastClickButtons_;
    }
//-----------------------------------------------------------------------------
void Mouse::Update()
    {
    if (Global_Mouse.GetButtons() == 1)
        LastClickPos_ = GetPos();

    LastClickButtons_ = txMouseButtons ();
    }

//-----------------------------------------------------------------------------

//}
//-----------------------------------------------------------------------------



//{ Time
Time::Time () :
    t_ (0)
    {}
//-----------------------------------------------------------------------------

int Time::GetTime()
    {
    return t_;
    }

//-----------------------------------------------------------------------------

void Time::Update()
    {
    t_++;
    }
//}
//-----------------------------------------------------------------------------

//{ Tool::
Tool::Tool (Vec pos, Vec V, int Type)  :
    Hero (NULL, Image (), pos, V, Type)
    {}

//-----------------------------------------------------------------------------
void Tool::Move (double dt)
    {}

//-----------------------------------------------------------------------------
void Tool::Logic ()
    {}

//-----------------------------------------------------------------------------
void Tool::Control   (int KeyStopMove)
    {}


//-----------------------------------------------------------------------------
void Tool::doAnimation () const
    {}



//}
//-----------------------------------------------------------------------------

//}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------



int DoubleCompareWithZero (double number)
    {
    if (number > DBL_EPSILON) return +1;

    if (number < DBL_EPSILON) return -1;                  // DCWZ ( 0.0000000000000001) = 0
                                                          // DCWZ (-0.0000000000000001) = 0
    return 0;                                             // DCWZ ( 0.01) = +1
                                                          // DCWZ (-0.01) = -1
    }

//-----------------------------------------------------------------------------
double HeroHypot (const Hero *first, const Hero *Second)
    {
    return hypot (first->pos_.x - Second->pos_.x,
                  first->pos_.y - Second->pos_.y);
    }

//-----------------------------------------------------------------------------
double HeroHypot2 (const Hero *first, const Hero *Second)
    {
    return hypot2 (first->pos_.x - Second->pos_.x,
                   first->pos_.y - Second->pos_.y);
    }

//-----------------------------------------------------------------------------
double hypot2 (double dx, double dy)
    {
    return (dx*dx + dy*dy);
    }




//-----------------------------------------------------------------------------
int GetObjArrayNum (Hero *object, Hero* Array [], int ArraySize)
    {
    for (int i = 0; i < ArraySize; i++)
        {
        if (Array[i] == object) return i;
        }
    return -1;
    }

//-----------------------------------------------------------------------------
void DeleteFromArray (int objNum, Hero* Array [], int ArraySize)
    {
    assert (0 <= objNum && objNum < ArraySize);

    Array[objNum] = Array[ArraySize - 1];
    Array[ArraySize - 1] = NULL;
    }
//-----------------------------------------------------------------------------
void DeleteObjFromArray (Hero *object, Hero* Array [], int ArraySize)
    {
    DeleteFromArray (GetObjArrayNum (object, Array, ArraySize), Array, ArraySize);
    }
//-----------------------------------------------------------------------------



//{----kladbizshgzshe
/*
void Engine::FindAndRemove (const Hero *obj)
    {
    int num = GetObjNum (obj);
    if (num != -1)
        remoov (num);
    }




  */
//}











