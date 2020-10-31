#ifndef GAME_H
#define GAME_H

#include "gba.h"

                    /* TODO: */

            // Create any necessary structs //

typedef enum {
  START,
  AVATAR,
  LEVEL1,
  LEVEL2,
  LEVEL1WIN,
  LEVEL2WIN,
  LOSE,
} GBAState;

struct avatar {
	int row;
	int col;
	int height;
	int width;
	int isRectangle;
	u16 color;
	const u16 *image;
};

typedef struct barrier {
	int row;
	int col;
	int height;
	int width;
	u16 color;
} Barrier;

typedef struct state {
	struct avatar mainAvatar;
} State;

GBAState startGame(u32 previousButtons, u32 currentButtons);
GBAState avatar(u32 previousButtons, u32 currentButtons);
GBAState level1(u32 previousButtons, u32 currentButtons);
GBAState level1Win(u32 previousButtons, u32 currentButtons);
GBAState level2(u32 previousButtons, u32 currentButtons);
GBAState level2Win(u32 previousButtons, u32 currentButtons);
GBAState lose(u32 previousButtons, u32 currentButtons);
void drawAvatar(void);
void drawLevel1(void);
void drawLevel2(void);
int collides(int barriers);
#endif
