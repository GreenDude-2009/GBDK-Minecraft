#include <gb/gb.h>
#include <stdio.h>
#include "Steve.c"
#include "GameCharacter.c"
#include "Map.c"
#include "Title.c"
#include "blank.c"
#include "Title_map.c"
#include "blank_map.c"
#include "MapTiles.c"
#include <gb/font.h>
#include <stdlib.h>
#include <string.h>

INT16 playerlocation[2]; // stores two INT16 x and y position of player
INT16 enemylocation[2]; // stores two INT16 x and y position of player
BYTE jumping;
BYTE walking;
INT8 gravity = -2;
double speed = 2;
double tempspeed = 0;
INT16 currentspeedY;
double currentspeedX;
INT16 floorYposition = 127;
GameCharacter player;
GameCharacter enemy;
BYTE timerdelay = 0;
INT8 hp;
int score;
char stringscore[3];
int j;
INT8 index0 = 0;
INT8 index1 = 0;


void moveplayer_to(struct Character* character, UINT8 x, UINT8 y){
    move_sprite(0, x, y);
    move_sprite(1, x, y + 8);
}
void move_enemy_to(struct Character* character, UINT8 x, UINT8 y){
    move_sprite(2, x, y);
    move_sprite(3, x, y + 8);
}

void performantdelay(UINT8 numloops){
    UINT8 i;
    for(i = 0; i < numloops; i++){
        wait_vbl_done();
    }     
}

INT8 wouldhitsurface(INT16 projectedYPosition){
    if(projectedYPosition >= floorYposition){
        
        return floorYposition;
    }
    return -1;
}

void jump(UINT8 spriteid, UINT16 spritelocation[2]){
    INT8 possiblesurfaceY;

    if(jumping==0){
        jumping=1;
        currentspeedY = 17;
    }

    // work out current speed - effect of gravities accelleration down
    currentspeedY = currentspeedY + gravity;    

    spritelocation[1] = spritelocation[1] - currentspeedY;

    possiblesurfaceY = wouldhitsurface(spritelocation[1]);

    if(possiblesurfaceY != -1){
        jumping = 0;
        move_sprite(spriteid,spritelocation[0],possiblesurfaceY);
    }
    else{
        move_sprite(spriteid,spritelocation[0],spritelocation[1]);
    }
}
UBYTE checkcollisions(GameCharacter* one, GameCharacter* two){
    return (one->x >= two->x && one->x <= two->x + two->width) && (one->y >= two->y && one->y <= two->y + two->height) || (two->x >= one->x && two->x <= one->x + one->width) && (two->y >= one->y && two->y <= one->y + one->height);
}
int main()
{
    hp = 20;

    set_bkg_data(0, 114, title_data);
    set_bkg_tiles(0, 0, 20, 18, title_map);
    SHOW_BKG;
    waitpad(J_START);
    set_bkg_data(0, 114, blank_data);
    set_bkg_tiles(0, 0, 20, 18, blank_map);
    SHOW_BKG;
	set_sprite_data(0, 16, MinecraftSteve);   /* defines the sprite data */

	set_sprite_tile(0,0);
	set_sprite_tile(1,1);
	set_sprite_tile(2,8);
	set_sprite_tile(3,9);

    playerlocation[0] = 10;
    playerlocation[1] = floorYposition;
    enemylocation[0] = 160;
    enemylocation[1] = floorYposition;
    jumping = 0;

    player.x = playerlocation[0];
    player.y = playerlocation[1];
    enemy.x = enemylocation[0];
    enemy.y = enemylocation[1];
    player.width = 8;
    player.height = 16;
    enemy.width = 8;
    enemy.height = 16;

    moveplayer_to(&player,playerlocation[0],playerlocation[1]);
    move_enemy_to(&enemy,enemylocation[0],enemylocation[1]);

    SHOW_SPRITES;
	DISPLAY_ON;						// Turn on the display

    while(hp != 0 && score != 999){
        if((joypad() & J_A) || jumping == 1){
            jump(0,playerlocation);
        }
        if(joypad() & J_LEFT){
            if (playerlocation[0] != 8)
            {
                playerlocation[0] = playerlocation[0] - 2;
                set_sprite_prop(0,S_FLIPX);
                set_sprite_prop(1,S_FLIPX);
                index0++;
            }
        }
        if(joypad() & J_RIGHT){
            if (playerlocation[0] != 160)
            {
                playerlocation[0] = playerlocation[0] + 2;
                set_sprite_prop(0,S_FLIPX);
                set_sprite_prop(0,get_sprite_prop(0) & ~S_FLIPX);
                set_sprite_prop(1,S_FLIPX);
                set_sprite_prop(1,get_sprite_prop(0) & ~S_FLIPX);
                index0++;
            }
        }
        if (playerlocation[0] > enemylocation[0])
        {
            enemylocation[0] = enemylocation[0] + 1;
            set_sprite_prop(2,S_FLIPX);
            set_sprite_prop(3,S_FLIPX);
            set_sprite_prop(2,get_sprite_prop(0) & ~S_FLIPX);
            set_sprite_prop(3,get_sprite_prop(0) & ~S_FLIPX);
            index1++;
        }
        else if (playerlocation[0] < enemylocation[0])
        {
            enemylocation[0] = enemylocation[0] - 1;
            set_sprite_prop(2,S_FLIPX);
            set_sprite_prop(3,S_FLIPX);
            index1++;
        }
        player.x = playerlocation[0];
        player.y = playerlocation[1];
        enemy.x = enemylocation[0];
        enemy.y = enemylocation[1];   
        
        

        moveplayer_to(&player,playerlocation[0],playerlocation[1]);
        move_enemy_to(&enemy,enemylocation[0],enemylocation[1]);

        if (checkcollisions(&player, &enemy))
        {
            hp = hp - 1;
            jump(0, playerlocation);
        }

        if(index0 == 4){
            index0 = 0;
        }

        if(index1 == 4){
            index1 = 0;
        }
        
	    set_sprite_tile(1, (index0 * 2) + 1);
        set_sprite_tile(3,(index1 * 2) + 9);
        
        performantdelay(3);
        score++;
        if (timerdelay != 0)
            timerdelay = timerdelay - 1;
    };
    if (score == 999)
    {
        printf("\n\n     YOU   WIN\n  Restart  system\n     to replay");
    }
    else
    {
        printf("\n\n     Game Over\n  Restart  system\n     to replay");
    }
    
}
