#include "Arduboy.h"
#include "class.h"
#include "enum.h"
#include "image.h"

Arduboy ab;

Player pList[4];
int playerCount = 2;

int cursorM = M_PSEL;
int cursorC = 0;

bool isTitle = true;

bool isSound = false;
bool isViewC = true;
bool isCursorC = false;

Dice dice;

void setup()
{
  // ab.begin();
  ab.beginNoLogo();
  ab.setFrameRate(7);

  dice.init();
  initPlayer();

  dice.diceMax = 6;
  dice.diceCount = 1;
}

void loop()
{
  if (!(ab.nextFrame()))
  {
    return;
  }

  if (isTitle)
  {
    isTitle = false;
    dispTitle();
    ab.display();
    delay(3000);
    return;
  }

  ab.clear();

  button();

  disp();

  ab.display();
}

void initPlayer()
{
  int max = sizeof(pList) / sizeof(pList[0]);
  for (int i = 0; i < max; i++)
  {
    pList[i].reset(playerCount, i);
    pList[i].pNum = i;
  }
}

void disp()
{
  drawFrame();

  dispCursor();

  dispPlayer();

  dispDice();
}

void dispTitle()
{
  ab.drawBitmap(0, 0, mtg_logo, 128, 64, WHITE);
}

void dispPlayer()
{
  switch (playerCount)
  {
    case 1:
      drawPlayer1(pList[0]);
      break;
    case 2:
      for (int i = 0; i < 2; i++)
      {
        drawPlayer2(pList[i]);
      }
      break;
    case 3:
    case 4:
      int max = sizeof(pList) / sizeof(pList[0]);
      for (int i = 0; i < max; i++)
      {
        if (pList[i].isUse)
        {
          drawPlayer(pList[i]);
        }
      }
      break;
  }
}

void dispDice()
{
  for (int i = 0; i < 4; i++)
  {
    int addX = (i == 1 || i == 3) ? 9 : 0;
    int addY = (i == 2 || i == 3) ? 9 : 0;

    if (dice.diceRec[i] != -1)
    {
      drawBigDice(104 + addX,  14 + addY, dice.diceRec[i]);
    }
    else
    {
      ab.fillRect(104 + addX,  14 + addY, 9, 9, false);
    }
  }
}

void dispCursor()
{
  switch (playerCount)
  {
    case 1:
      drawCursorPlayer1();
      break;
    case 2:
      drawCursorPlayer2();
      break;
    case 3:
    case 4:
      drawCursorPlayer();
      break;
  }

  drawCursorMenu();
}

void button()
{
  buttonSound();

  upButton();
  downButton();
  leftButton();
  rightButton();

  aButton();
  bButton();
}

void buttonSound()
{
  if (ab.pressed(UP_BUTTON)   || ab.pressed(DOWN_BUTTON)  ||
      ab.pressed(LEFT_BUTTON) || ab.pressed(RIGHT_BUTTON) ||
      ab.pressed(A_BUTTON)    || ab.pressed(B_BUTTON))
  {
    if (isSound)
    {
      ab.tunes.tone(1000, 20);
    }
  }
}

void upButton()
{
  if (!ab.pressed(UP_BUTTON))
  {
    return;
  }

  if (isViewC && isCursorC)
  {
    switch (playerCount)
    {
      case 1:
      case 2:
        switch (cursorC)
        {
          case 0:
            pList[cursorM].life++;
            break;
          case 1:
          case 2:
          case 3:
          case 4:
          case 5:
          case 6:
            if (pList[cursorM].c[cursorC - 1] < 99)
            {
              pList[cursorM].c[cursorC - 1]++;
            }
            break;
        }
        break;
      case 3:
      case 4:
        if (cursorM == cursorC)
        {
          pList[cursorM].life++;
        }
        else
        {
          if (pList[cursorM].c[cursorC] < 99)
          {
            pList[cursorM].c[cursorC]++;
            pList[cursorM].life--;
          }
        }
        break;
    }
  }
  else
  {
    switch (cursorM)
    {
      case P1_BASE:
      case P2_BASE:
      case P3_BASE:
      case P4_BASE:
        pList[cursorM].life++;
        break;
      case M_PSEL:
        if (playerCount < 4)
        {
          playerCount++;
          initPlayer();
        }
        break;
      case M_DICE:
        if (dice.diceCount != 4)
        {
          dice.diceCount++;
        }
        break;
      case M_SND:
        break;
      case M_RESET:
        break;
    }
  }
}

void downButton()
{
  if (!ab.pressed(DOWN_BUTTON))
  {
    return;
  }

  if (isViewC && isCursorC)
  {
    switch (playerCount)
    {
      case 1:
      case 2:
        switch (cursorC)
        {
          case 0:
            pList[cursorM].life--;
            break;
          case 1:
          case 2:
          case 3:
          case 4:
          case 5:
          case 6:
            if (pList[cursorM].c[cursorC - 1] > 0)
            {
              pList[cursorM].c[cursorC - 1]--;
            }
            break;
        }
        break;
      case 3:
      case 4:
        if (cursorM == cursorC)
        {
          pList[cursorM].life--;
        }
        else
        {
          if (pList[cursorM].c[cursorC] > 0)
          {
            pList[cursorM].c[cursorC]--;
            pList[cursorM].life++;
          }
        }
        break;
    }
  }
  else
  {
    switch (cursorM)
    {
      case P1_BASE:
      case P2_BASE:
      case P3_BASE:
      case P4_BASE:
        pList[cursorM].life--;
        break;
      case M_PSEL:
        if (playerCount > 1)
        {
          playerCount--;
          initPlayer();
        }
        break;
      case M_DICE:
        if (dice.diceCount != 1)
        {
          dice.diceCount--;
        }
        break;
      case M_SND:
        break;
      case M_RESET:
        break;
    }
  }
}

void leftButton()
{
  if (!ab.pressed(LEFT_BUTTON))
  {
    return;
  }

  if (isViewC && isCursorC)
  {
    cursorC--;

    switch (playerCount)
    {
      case 1:
        if (cursorC < 0)
        {
          cursorC = 6;
        }
        break;
      case 2:
        if (cursorC < 0)
        {
          cursorC = 4;
        }
        break;
      case 3:
      case 4:
        if (cursorC < 0)
        {
          cursorC = 3;
        }
        break;
    }
  }
  else
  {
    cursorM--;

    if (cursorM == P4_BASE && playerCount != 4)
    {
      cursorM = playerCount - 1;
    }
    else if (cursorM < 0)
    {
      cursorM = M_RESET;
    }
  }
}

void rightButton()
{
  if (!ab.pressed(RIGHT_BUTTON))
  {
    return;
  }

  if (isViewC && isCursorC)
  {
    cursorC++;

    switch (playerCount)
    {
      case 1:
        if (cursorC > 6)
        {
          cursorC = 0;
        }
        break;
      case 2:
        if (cursorC > 4)
        {
          cursorC = 0;
        }
        break;
      case 3:
      case 4:
        if (cursorC > 3)
        {
          cursorC = 0;
        }
        break;
    }
  }
  else
  {
    cursorM++;

    if (cursorM >= playerCount && cursorM < M_PSEL)
    {
      cursorM = M_PSEL;
    }
    else if (cursorM > M_RESET)
    {
      cursorM = P1_BASE;
    }
  }
}

void initLife(int life)
{
  pList[0].life = pList[1].life = pList[2].life = pList[3].life = life;
}

void aButton()
{
  if (!ab.pressed(A_BUTTON))
  {
    return;
  }

  if (ab.pressed(B_BUTTON))
  {
    abButton();
    return;
  }

  switch (cursorM)
  {
    case P1_BASE:
    case P2_BASE:
    case P3_BASE:
    case P4_BASE:
      isCursorC = !isCursorC;
      if (isCursorC)
      {
        switch (playerCount)
        {
          case 1:
          case 2:
            cursorC = 0;
              break;
          case 3:
          case 4:
            cursorC = cursorM;
            break;
        }
      }
      break;
    case M_DICE:
      dice.roll();
      break;
    case M_PSEL:
      isViewC = !isViewC;
      break;
    case M_SND:
      isSound = !isSound;
      break;
    case M_RESET:
      initPlayer();
      initLife(20);
      break;
  }
}

void bButton()
{
  if (!ab.pressed(B_BUTTON))
  {
    return;
  }

  if (ab.pressed(A_BUTTON))
  {
    abButton();
    return;
  }

  switch (cursorM)
  {
    case P1_BASE:
    case P2_BASE:
    case P3_BASE:
    case P4_BASE:
      isCursorC = !isCursorC;
      if (isCursorC)
      {
        cursorC = cursorM;
      }
      break;
    case M_DICE:
      dice.roll();
      break;
    case M_PSEL:
      isViewC = !isViewC;
      break;
    case M_SND:
      isSound = !isSound;
      break;
    case M_RESET:
      initPlayer();
      initLife(40);
      break;
  }
}

void abButton()
{
  if (!ab.pressed(A_BUTTON) || !ab.pressed(B_BUTTON))
  {
    return;
  }

}

void drawFrame()
{
  if (isViewC)
  {
    switch (playerCount)
    {
      case 1:
        ab.drawBitmap(0, 0, vs1, 127, 63, WHITE);
        break;
      case 2:
        ab.drawBitmap(0, 0, vs2, 127, 63, WHITE);
        break;
      case 3:
        ab.drawBitmap(0, 0, vs3, 127, 63, WHITE);
        break;
      case 4:
        ab.drawBitmap(0, 0, vs4, 127, 63, WHITE);
        break;
    }
  }
  else
  {
    switch (playerCount)
    {
      case 1:
        ab.drawBitmap(0, 0, vs1_noC, 127, 63, WHITE);
        break;
      case 2:
        ab.drawBitmap(0, 0, vs2_noC, 127, 63, WHITE);
        break;
      case 3:
        ab.drawBitmap(0, 0, vs3_noC, 127, 63, WHITE);
        break;
      case 4:
        ab.drawBitmap(0, 0, vs4_noC, 127, 63, WHITE);
        break;
    }
  }

  drawText(105, 2, 1, "PL");
  drawText(120, 2, 1, playerCount);

  drawText(105, 34, 1, String(dice.diceCount) + "D" + String(dice.diceMax));

  if (isSound)
  {
    drawText(105, 44, 1, "|||");
  }
  else
  {
    drawText(105, 44, 1, "---");
  }
  drawText(105, 54, 1, "CLR");
}

void drawPlayer1(Player p)
{
  if (isViewC)
  {
    drawSmallNumber(85,  3, p.c[0], true);
    drawSmallNumber(85, 13, p.c[1], true);
    drawSmallNumber(85, 23, p.c[2], true);
    drawSmallNumber(85, 33, p.c[3], true);
    drawSmallNumber(85, 43, p.c[4], true);
    drawSmallNumber(85, 53, p.c[5], true);
  }

  if (p.life >= 100 || p.life <= -10)
  {
    drawText(8, 18, 4, p.life);
  }
  else if (p.life >= 0 && p.life <= 9)
  {
    drawText(30, 13, 5, p.life);
  }
  else
  {
    drawText(15, 13, 5, p.life);
  }
}

void drawPlayer2(Player p)
{
  int addX = 0;
  int addY = 0;

  if (p.pNum == 1 || p.pNum == 3)
  {
    addX = 49;
  }
  if (p.pNum == 2 || p.pNum == 3)
  {
    addY = 32;
  }

  if (isViewC)
  {
    drawSmallNumber(14 + addX, 43, p.c[0], true);
    drawSmallNumber(36 + addX, 43, p.c[1], true);
    drawSmallNumber(14 + addX, 53, p.c[2], true);
    drawSmallNumber(36 + addX, 53, p.c[3], true);
  }

  if (p.life >= 100 || p.life <= -10)
  {
    drawText( 7 + addX, 18, 2, p.life);
  }
  else if (p.life >= 0 && p.life <= 9)
  {
    drawText(18 + addX, 15, 3, p.life);
  }
  else
  {
    drawText( 8 + addX, 15, 3, p.life);
  }
}

void drawPlayer(Player p)
{
  int addX = 0;
  int addY = 0;

  if (p.pNum == 1 || p.pNum == 3)
  {
    addX = 49;
  }
  if (p.pNum == 2 || p.pNum == 3)
  {
    addY = 32;
  }

  if (isViewC)
  {
    if (p.pNum != 0)
    {
      drawSmallNumber( 1 + addX,  1 + addY, p.c[0], true);
    }
    if (p.pNum != 1)
    {
      drawSmallNumber(38 + addX,  1 + addY, p.c[1], true);
    }
    if (p.pNum != 2)
    {
      drawSmallNumber( 1 + addX, 23 + addY, p.c[2], true);
    }
    if (p.pNum != 3)
    {
      drawSmallNumber(38 + addX, 23 + addY, p.c[3], true);
    }
  }

  if (p.life >= 100 || p.life <= -10)
  {
    drawText(15 + addX, 12 + addY, 1, p.life);
  }
  else if (p.life >= 0 && p.life <= 9)
  {
    drawText(20 + addX,  9 + addY, 2, p.life);
  }
  else
  {
    drawText(13 + addX,  9 + addY, 2, p.life);
  }
}


void drawCursorPlayer1()
{
  if (cursorM != P1_BASE)
  {
    return;
  }

  ab.fillRect(1, 1, 9, 7, true);

  if (isViewC && isCursorC)
  {
    switch (cursorC)
    {
      case 0:
        drawArrowTop(2, 10, true);
        break;
      case 1:
      case 2:
      case 3:
      case 4:
      case 5:
      case 6:
        drawArrowLeft(77, 3 + (10 * (cursorC - 1)), true);
        break;
    }
  }
}

void drawCursorPlayer2()
{
  if (cursorM != P1_BASE && cursorM != P2_BASE)
  {
    return;
  }

  int addX = 0;
  int addRX = 0;

  switch (cursorM)
  {
    case P1_BASE:
      addX = 0;
      addRX = 0;
      break;
    case P2_BASE:
      addX = 49;
      addRX = 37;
      break;
  }

  ab.fillRect(1 + addX + addRX, 1, 9, 7, true);

  if (isViewC && isCursorC)
  {
    switch (cursorC)
    {
      case 0:
        drawArrowTop(  2 + addX + addRX, 10, true);
        break;
      case 1:
        drawArrowLeft( 6 + addX, 42, true);
        break;
      case 2:
        drawArrowLeft(29 + addX, 42, true);
        break;
      case 3:
        drawArrowLeft( 6 + addX, 52, true);
        break;
      case 4:
        drawArrowLeft(29 + addX, 52, true);
        break;
    }
  }
}

void drawCursorPlayer()
{
  if (cursorM != P1_BASE && cursorM != P2_BASE && cursorM != P3_BASE && cursorM != P4_BASE)
  {
    return;
  }

  int addX = 0;
  int addY = 0;

  int addRX = 0;
  int addRY = 0;

  switch (cursorM)
  {
    case P1_BASE:
      addX = 0;
      addY = 0;
      addRX = 0;
      addRY = 0;
      break;
    case P2_BASE:
      addX = 49;
      addY = 0;
      addRX = 37;
      addRY = 0;
      break;
    case P3_BASE:
      addX = 0;
      addY = 33;
      addRX = 0;
      addRY = 21;
      break;
    case P4_BASE:
      addX = 49;
      addY = 33;
      addRX = 37;
      addRY = 21;
      break;
  }

  ab.fillRect(1 + addX + addRX, 1 + addY + addRY, 9, 7, true);

  if (isViewC && isCursorC)
  {
    switch (cursorC)
    {
      case 0:
        drawArrowTop( 2 + addX, 10 + addY, true);
        break;
      case 1:
        drawArrowTop(38 + addX, 10 + addY, true);
        break;
      case 2:
        drawArrowDown(2 + addX, 17 + addY, true);
        break;
      case 3:
        drawArrowDown(38 + addX, 17 + addY, true);
        break;
    }
  }
}

void drawCursorMenu()
{
  if (cursorM < M_PSEL)
  {
    return;
  }

  int addX = 0;
  int addY = 0;

  switch (cursorM)
  {
    case M_PSEL:
      addY = 0;
      break;
    case M_DICE:
      addY = 32;
      break;
    case M_SND:
      addY = 42;
      break;
    case M_RESET:
      addY = 52;
      break;
  }

  drawArrowLeft(100, 2 + addY, true);
}

void drawArrowLeft(int x, int y, bool white)
{
  ab.drawLine(x + 0, y + 0, x + 0, y + 6, white);
  ab.drawLine(x + 1, y + 1, x + 1, y + 5, white);
  ab.drawLine(x + 2, y + 2, x + 2, y + 4, white);
  ab.drawLine(x + 3, y + 3, x + 3, y + 3, white);
}

void drawArrowTop(int x, int y, bool white)
{
  ab.drawLine(x + 3, y + 0, x + 3, y + 0, white);
  ab.drawLine(x + 2, y + 1, x + 4, y + 1, white);
  ab.drawLine(x + 1, y + 2, x + 5, y + 2, white);
  ab.drawLine(x + 0, y + 3, x + 6, y + 3, white);
}

void drawArrowDown(int x, int y, bool white)
{
  ab.drawLine(x + 3, y + 3, x + 3, y + 3, white);
  ab.drawLine(x + 2, y + 2, x + 4, y + 2, white);
  ab.drawLine(x + 1, y + 1, x + 5, y + 1, white);
  ab.drawLine(x + 0, y + 0, x + 6, y + 0, white);
}

void drawText(int a, int b, int fs, String mes)
{
  ab.setCursor(a, b);
  ab.setTextSize(fs);
  ab.print(mes);
}

void drawText(int a, int b, int fs, int mes)
{
  ab.setCursor(a, b);
  ab.setTextSize(fs);
  ab.print(mes);
}

void drawBigDice(int x, int y, int number)
{
  if (number <= 0)
  {
    return;
  }

  ab.fillRect(x, y,  8,  8, 1);

  switch (number)
  {
    case 1:
      ab.drawRect(x + 3, y + 3, 2, 2, 0);
      break;
    case 2:
      ab.drawRect(x + 5, y + 1, 2, 2, 0);
      ab.drawRect(x + 1, y + 5, 2, 2, 0);
      break;
    case 3:
      ab.drawRect(x + 3, y + 3, 2, 2, 0);
      ab.drawRect(x + 5, y + 1, 2, 2, 0);
      ab.drawRect(x + 1, y + 5, 2, 2, 0);
      break;
    case 4:
      ab.drawRect(x + 1, y + 1, 2, 2, 0);
      ab.drawRect(x + 5, y + 1, 2, 2, 0);
      ab.drawRect(x + 1, y + 5, 2, 2, 0);
      ab.drawRect(x + 5, y + 5, 2, 2, 0);
      break;
    case 5:
      ab.drawRect(x + 3, y + 3, 2, 2, 0);
      ab.drawRect(x + 1, y + 1, 2, 2, 0);
      ab.drawRect(x + 5, y + 1, 2, 2, 0);
      ab.drawRect(x + 1, y + 5, 2, 2, 0);
      ab.drawRect(x + 5, y + 5, 2, 2, 0);
      break;
    case 6:
      ab.drawRect(x + 1, y + 0, 2, 2, 0);
      ab.drawRect(x + 1, y + 3, 2, 2, 0);
      ab.drawRect(x + 1, y + 6, 2, 2, 0);
      ab.drawRect(x + 5, y + 0, 2, 2, 0);
      ab.drawRect(x + 5, y + 3, 2, 2, 0);
      ab.drawRect(x + 5, y + 6, 2, 2, 0);
      break;
    case 7:
      ab.drawRect(x + 3, y + 3, 2, 2, 0);
      ab.drawRect(x + 1, y + 0, 2, 2, 0);
      ab.drawRect(x + 1, y + 3, 2, 2, 0);
      ab.drawRect(x + 1, y + 6, 2, 2, 0);
      ab.drawRect(x + 5, y + 0, 2, 2, 0);
      ab.drawRect(x + 5, y + 3, 2, 2, 0);
      ab.drawRect(x + 5, y + 6, 2, 2, 0);
      break;
  }
}

void drawSmallNumber(int x, int y, int num, bool white)
{
  int s = num;
  int maxDigit = 2;
  int digit = 0;

  if (s > 99)
  {
    digit = 2;
  }
  if (s == 0)
  {
    digit = 1;
  }
  else
  {
    while (s != 0)
    {
      s /= 10;
      digit++;
    }
  }

  int sb[digit];
  s = num;
  if (s > 99)
  {
    sb[0] = 9;
    sb[1] = 9;
  }
  else
  {
    for (int i = 0; i < maxDigit; i++)
    {
      if (maxDigit - digit > i)
      {
        sb[i] = 0;
      }

      sb[i] = (s % 10);
      s /= 10;
    }
  }

  for (int i = 0; i < digit; i++)
  {
    drawOneSmallNumber(x + (i * 4), y, sb[digit - 1 - i], white);
  }
}

void drawOneSmallNumber(int x, int y, int number, bool white)
{
  int color = (white) ? "1" : "0";

  switch (number)
  {
    case 0:
      ab.drawPixel(x + 1, y + 1, color);
      ab.drawPixel(x + 2, y + 1, color);
      ab.drawPixel(x + 3, y + 1, color);
      ab.drawPixel(x + 1, y + 2, color);
      ab.drawPixel(x + 3, y + 2, color);
      ab.drawPixel(x + 1, y + 3, color);
      ab.drawPixel(x + 3, y + 3, color);
      ab.drawPixel(x + 1, y + 4, color);
      ab.drawPixel(x + 3, y + 4, color);
      ab.drawPixel(x + 1, y + 5, color);
      ab.drawPixel(x + 2, y + 5, color);
      ab.drawPixel(x + 3, y + 5, color);
      break;
    case 1:
      ab.drawPixel(x + 3, y + 1, color);
      ab.drawPixel(x + 3, y + 2, color);
      ab.drawPixel(x + 3, y + 3, color);
      ab.drawPixel(x + 3, y + 4, color);
      ab.drawPixel(x + 3, y + 5, color);
      break;
    case 2:
      ab.drawPixel(x + 1, y + 1, color);
      ab.drawPixel(x + 2, y + 1, color);
      ab.drawPixel(x + 3, y + 1, color);
      ab.drawPixel(x + 3, y + 2, color);
      ab.drawPixel(x + 1, y + 3, color);
      ab.drawPixel(x + 2, y + 3, color);
      ab.drawPixel(x + 3, y + 3, color);
      ab.drawPixel(x + 1, y + 4, color);
      ab.drawPixel(x + 1, y + 5, color);
      ab.drawPixel(x + 2, y + 5, color);
      ab.drawPixel(x + 3, y + 5, color);
      break;
    case 3:
      ab.drawPixel(x + 1, y + 1, color);
      ab.drawPixel(x + 2, y + 1, color);
      ab.drawPixel(x + 3, y + 1, color);
      ab.drawPixel(x + 3, y + 2, color);
      ab.drawPixel(x + 1, y + 3, color);
      ab.drawPixel(x + 2, y + 3, color);
      ab.drawPixel(x + 3, y + 3, color);
      ab.drawPixel(x + 3, y + 4, color);
      ab.drawPixel(x + 1, y + 5, color);
      ab.drawPixel(x + 2, y + 5, color);
      ab.drawPixel(x + 3, y + 5, color);
      break;
    case 4:
      ab.drawPixel(x + 1, y + 1, color);
      ab.drawPixel(x + 3, y + 1, color);
      ab.drawPixel(x + 1, y + 2, color);
      ab.drawPixel(x + 3, y + 2, color);
      ab.drawPixel(x + 1, y + 3, color);
      ab.drawPixel(x + 2, y + 3, color);
      ab.drawPixel(x + 3, y + 3, color);
      ab.drawPixel(x + 3, y + 4, color);
      ab.drawPixel(x + 3, y + 5, color);
      break;
    case 5:
      ab.drawPixel(x + 1, y + 1, color);
      ab.drawPixel(x + 2, y + 1, color);
      ab.drawPixel(x + 3, y + 1, color);
      ab.drawPixel(x + 1, y + 2, color);
      ab.drawPixel(x + 1, y + 3, color);
      ab.drawPixel(x + 2, y + 3, color);
      ab.drawPixel(x + 3, y + 3, color);
      ab.drawPixel(x + 3, y + 4, color);
      ab.drawPixel(x + 1, y + 5, color);
      ab.drawPixel(x + 2, y + 5, color);
      ab.drawPixel(x + 3, y + 5, color);
      break;
    case 6:
      ab.drawPixel(x + 1, y + 1, color);
      ab.drawPixel(x + 1, y + 2, color);
      ab.drawPixel(x + 1, y + 3, color);
      ab.drawPixel(x + 2, y + 3, color);
      ab.drawPixel(x + 3, y + 3, color);
      ab.drawPixel(x + 1, y + 4, color);
      ab.drawPixel(x + 3, y + 4, color);
      ab.drawPixel(x + 1, y + 5, color);
      ab.drawPixel(x + 2, y + 5, color);
      ab.drawPixel(x + 3, y + 5, color);
      break;
    case 7:
      ab.drawPixel(x + 1, y + 1, color);
      ab.drawPixel(x + 2, y + 1, color);
      ab.drawPixel(x + 3, y + 1, color);
      ab.drawPixel(x + 3, y + 2, color);
      ab.drawPixel(x + 3, y + 3, color);
      ab.drawPixel(x + 3, y + 4, color);
      ab.drawPixel(x + 3, y + 5, color);
      break;
    case 8:
      ab.drawPixel(x + 1, y + 1, color);
      ab.drawPixel(x + 2, y + 1, color);
      ab.drawPixel(x + 3, y + 1, color);
      ab.drawPixel(x + 1, y + 2, color);
      ab.drawPixel(x + 3, y + 2, color);
      ab.drawPixel(x + 1, y + 3, color);
      ab.drawPixel(x + 2, y + 3, color);
      ab.drawPixel(x + 3, y + 3, color);
      ab.drawPixel(x + 1, y + 4, color);
      ab.drawPixel(x + 3, y + 4, color);
      ab.drawPixel(x + 1, y + 5, color);
      ab.drawPixel(x + 2, y + 5, color);
      ab.drawPixel(x + 3, y + 5, color);
      break;
    case 9:
      ab.drawPixel(x + 1, y + 1, color);
      ab.drawPixel(x + 2, y + 1, color);
      ab.drawPixel(x + 3, y + 1, color);
      ab.drawPixel(x + 1, y + 2, color);
      ab.drawPixel(x + 3, y + 2, color);
      ab.drawPixel(x + 1, y + 3, color);
      ab.drawPixel(x + 2, y + 3, color);
      ab.drawPixel(x + 3, y + 3, color);
      ab.drawPixel(x + 3, y + 4, color);
      ab.drawPixel(x + 1, y + 5, color);
      ab.drawPixel(x + 2, y + 5, color);
      ab.drawPixel(x + 3, y + 5, color);
      break;
  }
}

