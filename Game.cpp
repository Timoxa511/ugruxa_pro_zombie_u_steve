#include "TimaXLIbJustOneFunction.h"


#define ever (;;)

const double DT = 1.0;

/*struct Vector
    {
    double x, y;
    };   */

/*struct Image
    {
    HDC Texture;
    Vector Len;
    };  */

struct Object
    {
    Image Picture;
    Vec pos;
    };

struct Textures
    {
    Image Steve;
    Image Zombie;
    Image Fon;
    Image Map;
    };

enum {LOSE, WIN, CONTINUE};
enum {NONE, WALL, BREAD, FRIDGE, DOOR};


void StartCom ();
void PlsStopSpammingKeysGetYourHandsAwayFromTheKeyBoard_Arrrrrrrr ();




void SteveAnimationNumber_and_Moving_Connecting (Hero *Steve);
void ControlSteve  (Hero *Steve,  int KeyStopMove = VK_SPACE);
void ControlZombie (Hero *Zombie, int KeyStopMove);

int ClrGreenTest (const COLORREF *Clr);
void GameProcces (const Textures *textures);

int main()
    {
    StartCom ();

    Textures textures = {Image ("steve.bmp", {4, 18}), Image ("Zombie.bmp", {3, 2})};

    GameProcces (&textures);

    textures.Map.DeleteTex();
    textures.Fon.DeleteTex();
    textures.Steve.DeleteTex();
    textures.Zombie.DeleteTex();
    txEnd ();
    PlsStopSpammingKeysGetYourHandsAwayFromTheKeyBoard_Arrrrrrrr ();
    }

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

void GameProcces (const Textures *textures)
    {

    Hero Steve ("Steve",  textures->Steve,  Vec (500, 500), Vec (0, 0), 0, Hero::CHARACTER, ControlSteve);
    Hero Zombie = {"Zombie", textures->Zombie, {700, 700}, {0, 0}, 0, Hero::CHARACTER, ControlZombie, &Steve};

    COLORREF PrevClr = -1;
    while (!GetAsyncKeyState (VK_ESCAPE))
        {
        txClear();

        txLock ();
        //textures->Map.Draw ({0, 0});
        //COLORREF PointClr = txGetPixel (Steve.pos.x, Steve.pos.y);
        //ImageDraw (&textures->Fon, {0, 0});
        txUnlock ();



        Steve.Control ();

        Steve.Move (DT);
        Steve.CircleAnimation ();

        SteveAnimationNumber_and_Moving_Connecting (&Steve);

        Zombie.PolarAnimation ();
        Zombie.Control ();
        Zombie.Move (DT);




        //PrevClr = PointClr;
        txSleep (20);
        Global_Mouse.Update();
        Global_Timer.Update();
        }
    }


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
void ControlSteve (Hero *Steve, int KeyStopMove)
    {
                                        //Vec MousePos (Global_Mouse.Pos()); ������ ����������� ��� ������� � �������� ���
                                        //�� ����������� �� ������ ��������� ����� �������� ��� ��� ���� ----> Vec MousePos = Global_Mouse.Pos();

    Vec VDistBetweenTheHeroAndAMouseClick = Global_Mouse.GetClickPos() - Steve->pos_;     // "-" �������������� �����

    if (VDistBetweenTheHeroAndAMouseClick.Len() > 10)
        {
        Vec shortVector = VDistBetweenTheHeroAndAMouseClick.Normalize () * 15;     //V_ = !(Vec (txMousePos()) - pos_) * 15   | ��� ! - �������� ������������
        Steve->V_ = shortVector;
        }
    else
        Steve->V_ = Vec (0, 0);


    if (GetAsyncKeyState ( KeyStopMove ))
        Steve->V_ = Vec (0, 0);
    }
//-----------------------------------------------------------------------------
void ControlZombie (Hero *Zombie, int KeyStopMove)
    {
    Vec VDistBetweenTheHeroAndAVictim = Zombie->victim_->pos_  - Zombie->pos_;

    if (VDistBetweenTheHeroAndAVictim.Len() > 10)
        {
        Vec shortVector = VDistBetweenTheHeroAndAVictim.Normalize () * 10;     //  V_ = !(Vec (txMousePos()) - pos_) * 15   | ��� ! - �������� ������������
        Zombie->V_ = shortVector;
        }
    else
        Zombie->V_ = Vec (0, 0);


    if (GetAsyncKeyState ( KeyStopMove ))
        Zombie->V_ = Vec (0, 0);
    }
//-----------------------------------------------------------------------------


int ClrGreenTest (const COLORREF *Clr)
    {
    int AmountOfRed  =  txExtractColor (*Clr, TX_RED);
    int AmountOfGreen = txExtractColor (*Clr, TX_GREEN);
    int AmountOfBlue  = txExtractColor (*Clr, TX_RED);

    if (AmountOfRed == 255 && AmountOfGreen == 0 && AmountOfBlue == 0)
        return WALL;

    if (AmountOfRed == 255 && AmountOfGreen == 255 && AmountOfBlue == 0)
        return DOOR;

    if (AmountOfRed == 0 && AmountOfGreen == 255 && AmountOfBlue == 0)
        return BREAD;

    if (AmountOfRed == 0 && AmountOfGreen == 0 && AmountOfBlue == 255)
        return FRIDGE;

    return NONE;
    }


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


