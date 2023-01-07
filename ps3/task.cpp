#include "task.h"

#include "texturetools.h"
#include "blocklisttask.h"
#include "worldtask.h"
#include "settingstask.h"
#include "inventory.h"

#include <SDL/SDL.h>

//The values have to stay somewhere
Task *Task::current_task;
bool Task::key_held_down, Task::running, Task::background_saved, Task::has_touchpad, Task::keys_inverted;
TEXTURE *Task::screen, *Task::background;
const char *Task::savefile;

void Task::makeCurrent()
{
    current_task = this;
}

bool Task::keyPressed(int key)
{
	struct j
	{
		int x;
		int y;
	} joy_cord[2];
	SDL_Event event;
	SDL_Joystick *joy;
	//Uint8 *keystate = SDL_GetKeyState(NULL);
	
	if (SDL_NumJoysticks() > 0) 
	{
		joy = SDL_JoystickOpen(0);
		joy_cord[0].x = SDL_JoystickGetAxis(joy, 0);
		joy_cord[0].y = SDL_JoystickGetAxis(joy, 1);
	}
	
	SDL_JoystickUpdate();
	SDL_PollEvent(&event);
	
	#ifndef NOJOY
	if (key == 555 || key == 556 || key == SDLK_LEFT || key==SDLK_RIGHT)
	{
		if (joy_cord[0].x < -8000 && key == SDLK_LEFT)
		{
			return true;
		}
		else if (joy_cord[0].x > 8000 && key == SDLK_RIGHT)
		{
			return true;
		}
		
		if (joy_cord[0].y < -8000 && key == 555)
		{
			return true;
		}
		else if (joy_cord[0].y > 8000 && key == 556)
		{
			return true;
		}
	}
	#endif
	
	/*if (keystate[key])
	{
		return true;
	}*/
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
    deleteTexture(background);
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
