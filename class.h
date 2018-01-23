#include "Arduboy.h"

class Player
{
  public: bool isUse = false;

  public: int pNum = 0;
  public: int life = 0;
  public: int c[6];

  void Player::reset(int playerCount, int pNum)
  {
    pNum = pNum;
    
    if (playerCount != 4)
    {
      life = 20;
    }
    else
    {
      life = 40;
    }

    isUse = (playerCount >= pNum + 1);

    c[0] = 0;
    c[1] = 0;
    c[2] = 0;
    c[3] = 0;
    c[4] = 0;
    c[5] = 0;
  }
};

class Dice
{
  public: int isCrit = false;
  public: int diceMax = 10;
  public: int diceCount = 1;
  public: int diceCrit = 11;
  public: int diceValue = 0x00;
  public: int diceRec[40];

    Dice::dice()
    {
      init();
    }

    void Dice::init()
    {
      Serial.begin(9600);
      randomSeed(analogRead(0));

      int i = 0;
      for (i = 0; i < 40; i++)
      {
        diceRec[i] = -1;
      }      
    }

    int Dice::roll()
    {
      diceValue = 0x00;
      int i;
      if (isCrit)
      {
        for (i = 0; i < 20; i++)
        {
          diceRec[i] = -1;
        }

        int j = 0;
        int critCount = diceCount;
        while (critCount > 0)
        {
          int crit = 0;
          int dMax = 0;
          for (int i = 0; i < critCount; i++)
          {
            int value = random(diceMax) + 1;
            if (value >= diceCrit)
            {
              crit++;
            }

            if (dMax < value)
            {
              dMax = value;
            }
          }

          diceRec[j] = crit;

          if (crit > 0)
          {
            diceValue += 10;
            j++;
          }
          else
          {
            diceValue += dMax;
          }

          critCount = crit;
        }
      }
      else
      {
        for (i = 0; i < 20; i++)
        {
          diceRec[i] = -1;
        }

        for (i = 0; i < diceCount; i++)
        {
          int value = random(diceMax) + 1;
          diceValue += value;
          diceRec[i] = value;
        }
      }
    }
};

