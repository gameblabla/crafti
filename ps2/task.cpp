#include "task.h"

#include "texturetools.h"
#include "blocklisttask.h"
#include "worldtask.h"
#include "settingstask.h"
#include "inventory.h"

//The values have to stay somewhere
Task *Task::current_task;
bool Task::key_held_down, Task::running, Task::background_saved, Task::has_touchpad, Task::keys_inverted;
TEXTURE *Task::screen, *Task::background;
const char *Task::savefile;

void Task::makeCurrent()
{
    current_task = this;
}

extern unsigned char pad_game[14];

bool Task::keyPressed(int key)
{
	/*
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
*/
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
