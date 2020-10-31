#include "game.h"

#include <stdio.h>
#include <stdlib.h>

#include "gba.h"
#include "images/garbage.h"
#include "images/startScreen.h"
#include "images/bandit.h"
#include "images/gameOver.h"
#include "images/win.h"
/* TODO: */
// Include any header files for title screen or exit
// screen images generated by nin10kit. Example for the provided garbage
// image:
// #include "images/garbage.h"

/* TODO: */
// Add any additional states you need for your app.
State current;
State previous;
Barrier mainBarriers[7];
unsigned short colors[] = {RED, BLUE, GREEN, WHITE, CYAN, YELLOW, MAGENTA};
int main(void) {
  /* TODO: */
  // Manipulate REG_DISPCNT here to set Mode 3. //
  REG_DISPCNT = MODE3 | BG2_ENABLE;
  // Save current and previous state of button input.
  u32 previousButtons = BUTTONS;
  u32 currentButtons = BUTTONS;

  // Load initial game state
  GBAState state = START;

  while (1) {

    /* TODO: */
    // Manipulate the state machine below as needed //
    // NOTE: Call waitForVBlank() before you draw
    currentButtons = BUTTONS;
    waitForVBlank();
    switch (state) {
      case START:
        state = startGame(previousButtons, currentButtons);
        break;
      case AVATAR:
        state = avatar(previousButtons, currentButtons);
        break;
      case LEVEL1:
        state = level1(previousButtons, currentButtons);
        break;
      case LEVEL1WIN:
        state = level1Win(previousButtons, currentButtons);
        break;
      case LEVEL2:
        state = level2(previousButtons, currentButtons);
        break;
      case LEVEL2WIN:
        state = level2Win(previousButtons, currentButtons);
        break;
      case LOSE:
        state = lose(previousButtons, currentButtons);
        break;
    }

    previousButtons = currentButtons;  // Store the current state of the buttons
  }

  return 0;
}

GBAState startGame(u32 previousButtons, u32 currentButtons) {
  drawFullScreenImageDMA(startScreen);
  if (KEY_JUST_PRESSED(BUTTON_START, currentButtons, previousButtons)) {
    drawAvatar();
    return AVATAR;
  }
  return START;
}

GBAState avatar(u32 previousButtons, u32 currentButtons) {
    if (KEY_JUST_PRESSED(BUTTON_SELECT, currentButtons, previousButtons)) {
      return START;
    }
    if (KEY_JUST_PRESSED(BUTTON_A, currentButtons, previousButtons)) {
      current.mainAvatar.row = 0;
      current.mainAvatar.col = 0;
      current.mainAvatar.width = BANDIT_WIDTH;
      current.mainAvatar.height = BANDIT_HEIGHT;
      current.mainAvatar.isRectangle = 0;
      current.mainAvatar.image = bandit;
      fillScreenDMA(BLACK);
      char *str2 = "Level 1";
      drawString(70, 95, str2, RED);
      delay(2000);
      drawLevel1();
      return LEVEL1;
    }
    if (KEY_JUST_PRESSED(BUTTON_B, currentButtons, previousButtons)) {
      current.mainAvatar.row = 0;
      current.mainAvatar.col = 0;
      current.mainAvatar.width = GARBAGE_WIDTH;
      current.mainAvatar.height = GARBAGE_HEIGHT;
      current.mainAvatar.isRectangle = 0;
      current.mainAvatar.image = garbage;
      fillScreenDMA(BLACK);
      char *str2 = "Level 1";
      drawString(70, 95, str2, RED);
      delay(2000);
      drawLevel1();
      return LEVEL1;
    }
    if (KEY_JUST_PRESSED(BUTTON_L, currentButtons, previousButtons)) {
      current.mainAvatar.row = 0;
      current.mainAvatar.col = 0;
      current.mainAvatar.width = 30;
      current.mainAvatar.height = 30;
      current.mainAvatar.isRectangle = 1;
      current.mainAvatar.color = YELLOW;
      fillScreenDMA(BLACK);
      char *str2 = "Level 1";
      drawString(70, 95, str2, RED);
      delay(2000);
      drawLevel1();
      return LEVEL1;
    }
  return AVATAR;
}

GBAState level1(u32 previousButtons, u32 currentButtons) {
    previous = current;
    GBAState st = LEVEL1;
    if (KEY_JUST_PRESSED(BUTTON_SELECT, currentButtons, previousButtons)) {
      st = START;
    }
    if (KEY_JUST_PRESSED(BUTTON_UP, currentButtons, previousButtons)) {
      current.mainAvatar.row = current.mainAvatar.row - 10;
      if (current.mainAvatar.row < 0) {
        current.mainAvatar.row = 0;
      }
      if (collides(2) == 1) {
        st = LOSE;
      }
    }
    if (KEY_JUST_PRESSED(BUTTON_DOWN, currentButtons, previousButtons)) {
      current.mainAvatar.row = current.mainAvatar.row + 10;
      if (current.mainAvatar.row > 160 - current.mainAvatar.height) {
        current.mainAvatar.row = 160 - current.mainAvatar.height;
      }
      if (collides(2) == 1) {
        st = LOSE;
      }
      if ((current.mainAvatar.row == 160 - current.mainAvatar.height) && (current.mainAvatar.col >= 180)) {
        fillScreenDMA(BLACK);
        char *str = "You beat Level 1";
        drawString(10, 70, str, BLUE);
        char *str2 = "Press Start to Play Level 2";
        drawString(80, 40, str2, RED);
        st = LEVEL1WIN;
      }
    }
    if (KEY_JUST_PRESSED(BUTTON_LEFT, currentButtons, previousButtons)) {
      current.mainAvatar.col = current.mainAvatar.col - 10;
      if (current.mainAvatar.col < 0) {
        current.mainAvatar.col = 0;
      }
      if (collides(2) == 1) {
        st = LOSE;
      }
    }
    if (KEY_JUST_PRESSED(BUTTON_RIGHT, currentButtons, previousButtons)) {
      current.mainAvatar.col = current.mainAvatar.col + 10;
      if (current.mainAvatar.col > 240 - current.mainAvatar.width) {
        current.mainAvatar.col = 240 - current.mainAvatar.width;
      }
      if (collides(2) == 1) {
        st = LOSE;
      }
    }
    if (KEY_JUST_PRESSED(BUTTON_R, currentButtons, previousButtons)) {
      current.mainAvatar.color = colors[randint(0, 6)];
    }
    drawRectDMA(previous.mainAvatar.row, previous.mainAvatar.col, previous.mainAvatar.width, previous.mainAvatar.height, BLACK);
    if (current.mainAvatar.isRectangle == 0) {
      drawImageDMA(current.mainAvatar.row, current.mainAvatar.col, current.mainAvatar.width, current.mainAvatar.height, current.mainAvatar.image);
    } else {
      drawRectDMA(current.mainAvatar.row, current.mainAvatar.col, current.mainAvatar.width, current.mainAvatar.height, current.mainAvatar.color);
    }
    if (st == LEVEL1WIN) {
      drawRectDMA(current.mainAvatar.row, current.mainAvatar.col, current.mainAvatar.width, current.mainAvatar.height, BLACK);
    }
    return st;
}

GBAState level1Win(u32 previousButtons, u32 currentButtons) {
  if (KEY_JUST_PRESSED(BUTTON_SELECT, currentButtons, previousButtons)) {
      return START;
    }
  if (KEY_JUST_PRESSED(BUTTON_START, currentButtons, previousButtons)) {
      fillScreenDMA(BLACK);
      char *str2 = "Level 2";
      drawString(70, 95, str2, RED);
      delay(2000);
      current.mainAvatar.row = 0;
      current.mainAvatar.col = 0;
      drawLevel2();
      return LEVEL2;
  }
  return LEVEL1WIN;
}

GBAState level2(u32 previousButtons, u32 currentButtons) {
    previous = current;
    GBAState st = LEVEL2;
    if (KEY_JUST_PRESSED(BUTTON_SELECT, currentButtons, previousButtons)) {
      st = START;
    }
    if (KEY_JUST_PRESSED(BUTTON_UP, currentButtons, previousButtons)) {
      current.mainAvatar.row = current.mainAvatar.row - 10;
      if (current.mainAvatar.row < 0) {
        current.mainAvatar.row = 0;
      }
      if (collides(7) == 1) {
        st = LOSE;
      }
    }
    if (KEY_JUST_PRESSED(BUTTON_DOWN, currentButtons, previousButtons)) {
      current.mainAvatar.row = current.mainAvatar.row + 10;
      if (current.mainAvatar.row > 160 - current.mainAvatar.height) {
        current.mainAvatar.row = 160 - current.mainAvatar.height;
      }
      if (collides(7) == 1) {
        st = LOSE;
      }
      if ((current.mainAvatar.row + current.mainAvatar.height >= 150) && ((current.mainAvatar.col + current.mainAvatar.width >= 70) && (current.mainAvatar.col + current.mainAvatar.width <= 120))) {
        current.mainAvatar.row = 160 - current.mainAvatar.height;
        current.mainAvatar.col = 126;
      }
      if ((current.mainAvatar.row + current.mainAvatar.height > 90) && ((current.mainAvatar.col + current.mainAvatar.width >= 160) && (current.mainAvatar.col + current.mainAvatar.width <= 200))) {
        st = LEVEL2WIN;
      }
    }
    if (KEY_JUST_PRESSED(BUTTON_LEFT, currentButtons, previousButtons)) {
      current.mainAvatar.col = current.mainAvatar.col - 10;
      if (current.mainAvatar.col < 0) {
        current.mainAvatar.col = 0;
      }
      if (collides(7) == 1) {
        st = LOSE;
      }
    }
    if (KEY_JUST_PRESSED(BUTTON_RIGHT, currentButtons, previousButtons)) {
      current.mainAvatar.col = current.mainAvatar.col + 10;
      if (current.mainAvatar.col > 240 - current.mainAvatar.width) {
        current.mainAvatar.col = 240 - current.mainAvatar.width;
      }
      if (collides(7) == 1) {
        st = LOSE;
      }
      if ((current.mainAvatar.row + current.mainAvatar.height >= 150) && ((current.mainAvatar.col + current.mainAvatar.width >= 70) && (current.mainAvatar.col + current.mainAvatar.width <= 120))) {
        current.mainAvatar.row = 160 - current.mainAvatar.height;
        current.mainAvatar.col = 126;
      }

    }
    if (KEY_JUST_PRESSED(BUTTON_R, currentButtons, previousButtons)) {
      current.mainAvatar.color = colors[randint(0, 6)];
    }
    drawRectDMA(previous.mainAvatar.row, previous.mainAvatar.col, previous.mainAvatar.width, previous.mainAvatar.height, BLACK);
    if (current.mainAvatar.isRectangle == 0) {
      drawImageDMA(current.mainAvatar.row, current.mainAvatar.col, current.mainAvatar.width, current.mainAvatar.height, current.mainAvatar.image);
    } else {
      drawRectDMA(current.mainAvatar.row, current.mainAvatar.col, current.mainAvatar.width, current.mainAvatar.height, current.mainAvatar.color);
    }
  return st;
}

GBAState level2Win(u32 previousButtons, u32 currentButtons) {
  drawFullScreenImageDMA(win);
  if (KEY_JUST_PRESSED(BUTTON_SELECT, currentButtons, previousButtons)) {
      return START;
    }
  if (KEY_JUST_PRESSED(BUTTON_START, currentButtons, previousButtons)) {
      drawAvatar();
      return AVATAR;
  }
  return LEVEL2WIN;
}

GBAState lose(u32 previousButtons, u32 currentButtons) {
  drawFullScreenImageDMA(gameOver);
  if (KEY_JUST_PRESSED(BUTTON_SELECT, currentButtons, previousButtons)) {
      return START;
    }
  if (KEY_JUST_PRESSED(BUTTON_START, currentButtons, previousButtons)) {
    drawAvatar();
    return AVATAR;
  }
  return LOSE;
}

void drawAvatar(void) {
  fillScreenDMA(BLACK);
  drawImageDMA(80, 100, GARBAGE_WIDTH, GARBAGE_HEIGHT, garbage);
  drawImageDMA(80, 10, BANDIT_WIDTH, BANDIT_HEIGHT, bandit);
  drawRectDMA(80, 190, 30, 30, YELLOW);
  char *str = "Select your avatar";
  drawString(10, 65, str, BLUE);
  char *A = "A";
  drawString(60, 20, A, RED);
  char *B = "B";
  drawString(60, 110, B, RED);
  char *L = "L";
  drawString(60, 200, L, RED);
  char *banditText = "Bandit";
  drawString(125, 5, banditText, BLUE);
  char *garbageText = "Garbage";
  drawString(125, 95, garbageText, BLUE);
  char *recText = "Square";
  drawString(125, 185, recText, BLUE);
}

void drawLevel1(void) {
  fillScreenDMA(BLACK);
  if (current.mainAvatar.isRectangle == 0) {
    drawImageDMA(current.mainAvatar.row, current.mainAvatar.col, current.mainAvatar.width, current.mainAvatar.height, current.mainAvatar.image);
  } else {
    drawRectDMA(current.mainAvatar.row, current.mainAvatar.col, current.mainAvatar.width, current.mainAvatar.height, current.mainAvatar.color);
  }
  mainBarriers[0].row = 0;
  mainBarriers[0].col = 60;
  mainBarriers[0].height = 110;
  mainBarriers[0].width = 30;
  mainBarriers[0].color = BLUE;
  drawRectDMA(mainBarriers[0].row, mainBarriers[0].col, mainBarriers[0].width, mainBarriers[0].height, mainBarriers[0].color);
  mainBarriers[1].row = 40;
  mainBarriers[1].col = 150;
  mainBarriers[1].height = 120;
  mainBarriers[1].width = 30;
  mainBarriers[1].color = RED;
  drawRectDMA(mainBarriers[1].row, mainBarriers[1].col, mainBarriers[1].width, mainBarriers[1].height, mainBarriers[1].color);
  char *finish = "Finish";
  drawString(150, 190, finish, BLUE);
}

void drawLevel2(void) {
  fillScreenDMA(BLACK);
  if (current.mainAvatar.isRectangle == 0) {
    drawImageDMA(current.mainAvatar.row, current.mainAvatar.col, current.mainAvatar.width, current.mainAvatar.height, current.mainAvatar.image);
  } else {
    drawRectDMA(current.mainAvatar.row, current.mainAvatar.col, current.mainAvatar.width, current.mainAvatar.height, current.mainAvatar.color);
  }
  mainBarriers[0].row = 0;
  mainBarriers[0].col = 120;
  mainBarriers[0].height = 160;
  mainBarriers[0].width = 5;
  mainBarriers[0].color = YELLOW;
  drawRectDMA(mainBarriers[0].row, mainBarriers[0].col, mainBarriers[0].width, mainBarriers[0].height, mainBarriers[0].color);
  mainBarriers[1].row = 40;
  mainBarriers[1].col = 0;
  mainBarriers[1].height = 10;
  mainBarriers[1].width = 60;
  mainBarriers[1].color = BLUE;
  drawRectDMA(mainBarriers[1].row, mainBarriers[1].col, mainBarriers[1].width, mainBarriers[1].height, mainBarriers[1].color);
  mainBarriers[2].row = 100;
  mainBarriers[2].col = 60;
  mainBarriers[2].height = 10;
  mainBarriers[2].width = 60;
  mainBarriers[2].color = GREEN;
  drawRectDMA(mainBarriers[2].row, mainBarriers[2].col, mainBarriers[2].width, mainBarriers[2].height, mainBarriers[2].color);
  mainBarriers[3].row = 50;
  mainBarriers[3].col = 0;
  mainBarriers[3].height = 110;
  mainBarriers[3].width = 10;
  mainBarriers[3].color = BLUE;
  drawRectDMA(mainBarriers[3].row, mainBarriers[3].col, mainBarriers[3].width, mainBarriers[3].height, mainBarriers[3].color);
  mainBarriers[4].row = 100;
  mainBarriers[4].col = 125;
  mainBarriers[4].height = 10;
  mainBarriers[4].width = 75;
  mainBarriers[4].color = CYAN;
  drawRectDMA(mainBarriers[4].row, mainBarriers[4].col, mainBarriers[4].width, mainBarriers[4].height, mainBarriers[4].color);
  mainBarriers[5].row = 40;
  mainBarriers[5].col = 200;
  mainBarriers[5].height = 70;
  mainBarriers[5].width = 5;
  mainBarriers[5].color = RED;
  drawRectDMA(mainBarriers[5].row, mainBarriers[5].col, mainBarriers[5].width, mainBarriers[5].height, mainBarriers[5].color);
  mainBarriers[6].row = 40;
  mainBarriers[6].col = 170;
  mainBarriers[6].height = 10;
  mainBarriers[6].width = 35;
  mainBarriers[6].color = WHITE;
  drawRectDMA(mainBarriers[6].row, mainBarriers[6].col, mainBarriers[6].width, mainBarriers[6].height, mainBarriers[6].color);
  char *tele = "Teleport";
  drawString(150, 70, tele, RED);
  char *fin = "FINISH";
  drawString(90, 160, fin, RED);
}

int collides(int barriers) {
  for (int i = 0; i < barriers; i++) {
    if ((current.mainAvatar.col + current.mainAvatar.width >= mainBarriers[i].col) && (current.mainAvatar.col + current.mainAvatar.width <= mainBarriers[i].col + mainBarriers[i].width)) {
      if ((current.mainAvatar.row >= mainBarriers[i].row) && (current.mainAvatar.row <= mainBarriers[i].row + mainBarriers[i].height)) {
        return 1;
      }
      if ((current.mainAvatar.row + current.mainAvatar.height >= mainBarriers[i].row) && (current.mainAvatar.row + current.mainAvatar.height <= mainBarriers[i].row + mainBarriers[i].height)) {
        return 1;
      }
    }
    if ((current.mainAvatar.col >= mainBarriers[i].col) && (current.mainAvatar.col <= mainBarriers[i].col + mainBarriers[i].width)) {
      if ((current.mainAvatar.row >= mainBarriers[i].row) && (current.mainAvatar.row <= mainBarriers[i].row + mainBarriers[i].height)) {
        return 1;
      }
      if ((current.mainAvatar.row + current.mainAvatar.height >= mainBarriers[i].row) && (current.mainAvatar.row + current.mainAvatar.height <= mainBarriers[i].row + mainBarriers[i].height)) {
        return 1;
      }
    }
  }
  return 0;
}