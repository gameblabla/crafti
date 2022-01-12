#include "task.h"

#include "texturetools.h"
#include "blocklisttask.h"
#include "worldtask.h"
#include "settingstask.h"
#include "inventory.h"

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdint.h>
#include <libdragon.h>

//The values have to stay somewhere
Task *Task::current_task;
bool Task::key_held_down, Task::running, Task::background_saved, Task::has_touchpad, Task::keys_inverted;
TEXTURE *screen, *background;
const char *Task::savefile;

void Task::makeCurrent()
{
    current_task = this;
}


#define NUM_BUFFERS         3
extern void *__safe_buffer[NUM_BUFFERS];
#define __get_buffer( x ) __safe_buffer[(x)-1]
extern display_context_t disp;
extern unsigned char pad_game[14];
extern void drawString(const char *str, COLOR color, TEXTURE &tex, unsigned int x, unsigned int y);

bool Task::keyPressed(int key)
{
	switch(key)
	{
		case CONT_DPAD_UP:
			if (pad_game[0] == 1) return true;
		break;
		case CONT_DPAD_DOWN:
			if (pad_game[0] == 2) return true;
		break;
		case CONT_DPAD_LEFT:
			if (pad_game[1] == 1) return true;
		break;
		case CONT_DPAD_RIGHT:
			if (pad_game[1] == 2) return true;
		break;
		
		case 556:
			if (pad_game[2] == 1) return true;
		break;
		case 557:
			if (pad_game[2] == 2) return true;
		break;
		
		case CONT_A:
			if (pad_game[8]) return true;
		break;
		case CONT_B:
			if (pad_game[9]) return true;
		break;
		case CONT_X:
			if (pad_game[10]) return true;
		break;
		case CONT_START:
			if (pad_game[11]) return true;
		break;
		
		case 600:
			if (pad_game[12] == 1) return true;
		break;
		case 601:
			if (pad_game[12] == 2) return true;
		break;
	}
	return false;
}

void Task::initializeGlobals(const char *savefile)
{
    running = true;

	screen = new TEXTURE;
	screen->width = SCREEN_WIDTH,
	screen->height = SCREEN_HEIGHT,
	screen->has_transparency = false,
	screen->transparent_color = 0,
	screen->bitmap = (COLOR*)__get_buffer( disp );
    //screen = newTexture(SCREEN_WIDTH, SCREEN_HEIGHT);
    nglSetBuffer(screen->bitmap);

    has_touchpad = is_touchpad;
    keys_inverted = is_classic;

	background = new TEXTURE;
	background->width = SCREEN_WIDTH,
	background->height = SCREEN_HEIGHT,
	background->has_transparency = false,
	background->transparent_color = 0,
	background->bitmap = (COLOR*)__get_buffer( disp );
    //background = newTexture(SCREEN_WIDTH, SCREEN_HEIGHT);
    background_saved = false;
    
	if (is_memory_expanded() == false)
	{
		for(;;)
		{
			/* Grab a render buffer */
			while( !(disp = display_lock()) )
			{
			}
			screen->bitmap = (COLOR*)__get_buffer( disp );
			background->bitmap = (COLOR*)__get_buffer( disp );
			controller_scan();
			struct controller_data keys = get_keys_down();
			if( keys.c[0].start )
			{
				break;
			}
			drawString("EXPANSION PACK HIGHLY RECOMMENDED", 0xFFFF, *screen, 16, 16);
			drawString("Failure to use one will lead to frequent", 0xFFFF, *screen, 16, 48);
			drawString("crashes.", 0xFFFF, *screen, 16, 64);
			drawString("Don't bitch about it if you can't", 0xFFFF, *screen, 16, 80);
			drawString("afford that extra memory, SCRUB !!", 0xFFFF, *screen, 16, 96);
			drawString("PUSH START to try anyway", 0xFFFF, *screen, 16, 144);
			//__n64_memcpy_ASM((uint16_t *)__get_buffer( disp ) ,real_buffer, (SCREEN_WIDTH*SCREEN_HEIGHT)*2);
			display_show(disp);
		}
	}
	
    Task::savefile = savefile;
}

void Task::deinitializeGlobals()
{
   /* deleteTexture(screen);
    deleteTexture(background);*/
}

void Task::saveBackground()
{
    copyTexture(*screen, *background);

    background_saved = true;
}

void Task::drawBackground()
{
    copyTexture(*background, *screen);
}

static constexpr int savefile_version = 5;

#define LOAD_FROM_FILE(var) if(fread(&var, sizeof(var), 1, file) != 1) { fclose(file); return false; }
#define SAVE_TO_FILE(var) if(fwrite(&var, sizeof(var), 1, file) != 1) { fclose(file); return false; }

bool Task::load()
{
    FILE *file = fopen(savefile, "rb");
    if(!file)
        return false;

    int version;
    LOAD_FROM_FILE(version);

    //For backwards compatibility
    if(version == savefile_version)
    {
        if(!settings_task.loadFromFile(file))
        {
            fclose(file);
            return false;
        }
    }
    else if(version != 4)
    {
        printf("Wrong save file version %d!\n", version);
        fclose(file);
        return false;
    }

    LOAD_FROM_FILE(current_inventory.entries)
    LOAD_FROM_FILE(world_task.xr)
    LOAD_FROM_FILE(world_task.yr)
    LOAD_FROM_FILE(world_task.x)
    LOAD_FROM_FILE(world_task.y)
    LOAD_FROM_FILE(world_task.z)
    LOAD_FROM_FILE(current_inventory.current_slot)

    LOAD_FROM_FILE(block_list_task.current_selection)

    const bool ret = world.loadFromFile(file);

    fclose(file);

    world.setPosition(world_task.x, world_task.y, world_task.z);

    return ret;
}

bool Task::save()
{
    FILE *file = fopen(savefile, "wb");
    if(!file)
        return false;

    SAVE_TO_FILE(savefile_version)
    if(!settings_task.saveToFile(file))
    {
        fclose(file);
        return false;
    }
    SAVE_TO_FILE(current_inventory.entries)
    SAVE_TO_FILE(world_task.xr)
    SAVE_TO_FILE(world_task.yr)
    SAVE_TO_FILE(world_task.x)
    SAVE_TO_FILE(world_task.y)
    SAVE_TO_FILE(world_task.z)
    SAVE_TO_FILE(current_inventory.current_slot)
    SAVE_TO_FILE(block_list_task.current_selection)

    const bool ret = world.saveToFile(file);

    fclose(file);

    return ret;
}
