#include "task.h"

#include "texturetools.h"
#include "blocklisttask.h"
#include "worldtask.h"
#include "settingstask.h"
#include "inventory.h"

#include <SDL.h>
#include <kos.h>
#define LEFT_PRESSED (state->buttons & CONT_DPAD_LEFT)
#define RIGHT_PRESSED (state->buttons & CONT_DPAD_RIGHT)
#define UP_PRESSED (state->buttons & CONT_DPAD_UP)
#define DOWN_PRESSED (state->buttons & CONT_DPAD_DOWN)

//The values have to stay somewhere
Task *Task::current_task;
bool Task::key_held_down, Task::running, Task::background_saved, Task::has_touchpad, Task::keys_inverted;
TEXTURE *Task::screen, *Task::background;
const char *Task::savefile;

void Task::makeCurrent()
{
    current_task = this;
}

mouse_state_t *mstate;
maple_device_t *cont, *kbd,  *mouse;
cont_state_t *state;	
kbd_state_t* first_kbd_state; 

extern int rv;
extern unsigned char key_g[5];

bool Task::keyPressed(int key)
{
#ifdef DREAMCAST
	if (state)
	{
		switch (key)
		{
			case 556:
			// Analog Up Stick
			if(state->joyy < -64)
				return true;
			break;
			case 557:
			// Analog Down Stick
			if(state->joyy > 64)
				return true;
			break;
				
			case CONT_DPAD_LEFT:
				// Analog Left Stick
				if(state->joyx < -64)
					return true;
				else if ((state->buttons & key))
					return true;	
				break;
				case CONT_DPAD_RIGHT:
				// Analog Right Stick
				if(state->joyx > 64)
					return true;
				else if ((state->buttons & key))
					return true;	
				break;
				
				case 600:
				// L
				if(state->ltrig > 64)
					return true;
				break;
				case 601:
				// R
				if(state->rtrig > 64)
					return true;
				break;
				
				default:
				if ((state->buttons & key))
					return true;
				break;
		}
	}
	
	if (mstate)
	{
			if (mstate->dy < -1)
			{
				if (key == 556) return true;
			}
			else if (mstate->dy > 1)
			{
				if (key == 557) return true;
			}
			
			if (mstate->dx < -1)
			{
				if (key == KEY_NSPIRE_LEFT) return true;
			}
			else if (mstate->dx > 1)
			{
				if (key == KEY_NSPIRE_RIGHT) return true;
			}
			
			if (mstate->buttons & MOUSE_LEFTBUTTON)
			{
				if (key == KEY_NSPIRE_CLICK) return true;
			}
			if (mstate->buttons & MOUSE_RIGHTBUTTON)
			{
				if (key == KEY_NSPIRE_9) return true;
			}
	}
	
	if (first_kbd_state)
	{
		if (rv > -1)
		{
			switch(rv)
			{
				case 19200:
					if (key == 600) return true;
				break;
				case 19968:
					if (key == 601) return true;
				break;
					
				case 106: // J
				case 120: // X
					if (key == CONT_A) return true;
				break;
					
				case 107: // K
				case 99: // C
					if (key == CONT_B) return true;
				break;
					
				case 108: // L
				case 118: // V
					if (key == CONT_X) return true;
				break;
					
				case 10: // Return
				case 27: // ESC
					if (key == CONT_START) return true;
				break;
					
				default:
				break;
			}
		}
		
		if (key_g[3] == 1 && key == CONT_DPAD_UP) return true;
		if (key_g[2] == 1 && key == CONT_DPAD_DOWN) return true;
		if (key_g[1] == 1 && key == CONT_DPAD_LEFT) return true;
		if (key_g[0] == 1 && key == CONT_DPAD_RIGHT) return true;
	}
#endif
	return false;
}

void Task::initializeGlobals(const char *savefile)
{
    running = true;

    screen = newTexture(SCREEN_WIDTH, SCREEN_HEIGHT);
    nglSetBuffer(screen->bitmap);

    has_touchpad = is_touchpad;
    keys_inverted = is_classic;

    background = newTexture(SCREEN_WIDTH, SCREEN_HEIGHT);
    background_saved = false;

    Task::savefile = savefile;
}

void Task::deinitializeGlobals()
{
    deleteTexture(screen);
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
   /* FILE *file = fopen(savefile, "rb");
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

    return ret;*/
        return false;
}

bool Task::save()
{
   /* FILE *file = fopen(savefile, "wb");
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

    return ret;*/
    return false;
}
