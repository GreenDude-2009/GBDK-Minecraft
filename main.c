#include <gb/gb.h>
#include <gb/font.h>
#include <stdio.h>
#include "Sprites.c"
#include "Character.c"

#define false 0
#define true 1

struct Character steve;
UBYTE spritesize = 8;
UBYTE i = 0;

void performantdelay(UINT8 numloops){
    UINT8 i;
    for(i = 0; i < numloops; i++){
        wait_vbl_done();
    }     
}

void movegamecharacter(struct Character* character, UINT8 x, UINT8 y){
    set_sprite_prop(character->ids[0],S_FLIPX);
    set_sprite_prop(character->ids[1],S_FLIPX);
    set_sprite_prop(character->ids[0],get_sprite_prop(character->ids[0]) & ~S_FLIPX);
    set_sprite_prop(character->ids[1],get_sprite_prop(character->ids[1]) & ~S_FLIPX);
    move_sprite(character->ids[0], x, y);
    move_sprite(character->ids[1], x + spritesize, y);
    move_sprite(character->ids[2], x, y + spritesize);
    move_sprite(character->ids[3], x + spritesize, y + spritesize);
}

void setupship(){
    steve.x = 80;
    steve.y = 72;
    steve.w = 16;
    steve.h = 16;
    steve.d = 0;

    // load sprites for steve
    steve.ids[0] = 0;
    steve.ids[1] = 1;
    steve.ids[2] = 2;
    steve.ids[3] = 3;
    set_sprite_tile(steve.ids[0], 0);
    set_sprite_tile(steve.ids[1], 1);
    set_sprite_tile(steve.ids[2], 2);
    set_sprite_tile(steve.ids[3], 3);

    movegamecharacter(&steve, steve.x, steve.y);
}

void main(){
	set_sprite_data(steve.ids[0], 8, Sprites);   /* defines the sprite data */
	setupship();

	DISPLAY_ON;						// Turn on the display
    SHOW_SPRITES;

    while(1){
        if(joypad() & J_RIGHT){
            steve.d = 0;
            movegamecharacter(steve.ids[0], 2, 0);
            movegamecharacter(steve.ids[1], 2, 0);
            movegamecharacter(steve.ids[2], 2, 0);
            movegamecharacter(steve.ids[3], 2, 0);
        }
        if(joypad() & J_UP){
            steve.d = 1;
            movegamecharacter(steve.ids[0], 0, -2);
            movegamecharacter(steve.ids[1], 0, -2);
            movegamecharacter(steve.ids[2], 0, -2);
            movegamecharacter(steve.ids[3], 0, -2);
        }
        if(joypad() & J_LEFT){
            steve.d = 2;
            movegamecharacter(steve.ids[0], -2, 0);
            movegamecharacter(steve.ids[1], -2, 0);
            movegamecharacter(steve.ids[2], -2, 0);
            movegamecharacter(steve.ids[3], -2, 0);
        }
        if(joypad() & J_DOWN){
            steve.d = 3;
            movegamecharacter(steve.ids[0], 0, 2);
            movegamecharacter(steve.ids[1], 0, 2);
            movegamecharacter(steve.ids[2], 0, 2);
            movegamecharacter(steve.ids[3], 0, 2);
        }
        if (steve.d == 0){
            set_sprite_tile(steve.ids[0], 0);
            set_sprite_tile(steve.ids[1], 1);
            set_sprite_prop(steve.ids[0],S_FLIPX);
            set_sprite_prop(steve.ids[1],S_FLIPX);
            set_sprite_prop(steve.ids[0],get_sprite_prop(0) & ~S_FLIPX);
            set_sprite_prop(steve.ids[0],get_sprite_prop(0) & ~S_FLIPX);
        }
        if (steve.d == 1){
            set_sprite_tile(steve.ids[0], 6);
            set_sprite_tile(steve.ids[1], 7);
        }
        if (steve.d == 2){
            set_sprite_tile(steve.ids[0], 0);
            set_sprite_tile(steve.ids[1], 1);
            set_sprite_prop(steve.ids[0],S_FLIPX);
            set_sprite_prop(steve.ids[1],S_FLIPX);
        }
        if (steve.d == 3){
            set_sprite_tile(steve.ids[0], 4);
            set_sprite_tile(steve.ids[1], 5);
        }
        
        performantdelay(1);
    }
}