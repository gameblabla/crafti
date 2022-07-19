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

extern int b;
#define INPUT_ANALOG_OFFSET 16

bool Task::keyPressed(int key)
{
	extern s8 xpad;
	extern s8 ypad;
	extern s8 cxpad;
	extern s8 cypad;
	
	if (key == 555 || key == 556 || key == KEY_NSPIRE_LEFT || key==KEY_NSPIRE_RIGHT)
	{
		if ((cxpad < -INPUT_ANALOG_OFFSET || xpad < -INPUT_ANALOG_OFFSET) && key == KEY_NSPIRE_LEFT)
		{
			return true;
		}
		else if ((cxpad > INPUT_ANALOG_OFFSET || xpad > INPUT_ANALOG_OFFSET) && key == KEY_NSPIRE_RIGHT)
		{
			return true;
		}
		
		if (cypad > INPUT_ANALOG_OFFSET && key == 555)
		{
			return true;
		}
		else if (cypad < -INPUT_ANALOG_OFFSET && key == 556)
		{
			return true;
		}
	}
	
	if (key == PAD_BUTTON_UP || key == PAD_BUTTON_DOWN)
	{
		if (ypad > INPUT_ANALOG_OFFSET && key == PAD_BUTTON_UP)
		{
			return true;
		}
		else if (ypad < -INPUT_ANALOG_OFFSET && key == PAD_BUTTON_DOWN)
		{
			return true;
		}
	}
	
	
	switch(key)
	{
		case PAD_BUTTON_UP:
			if (b & PAD_BUTTON_UP) return true;
		break;
		case PAD_BUTTON_DOWN:
			if (b & PAD_BUTTON_DOWN) return true;
		break;
		case PAD_BUTTON_LEFT:
			if (b & PAD_BUTTON_LEFT) return true;
		break;
		case PAD_BUTTON_RIGHT:
			if (b & PAD_BUTTON_RIGHT) return true;
		break;
		
		/*case 556:
			if (b & PAD_TRIGGER_L) return true;
		break;
		case 557:
			if (b & PAD_TRIGGER_R) return true;
		break;*/
		
		case PAD_BUTTON_A:
			if (b & PAD_BUTTON_A) return true;
		break;
		case PAD_BUTTON_B:
			if (b & PAD_BUTTON_B) return true;
		break;
		case PAD_BUTTON_X:
			if (b & PAD_BUTTON_X) return true;
		break;
		case PAD_BUTTON_START:
			if (b & PAD_BUTTON_START) return true;
		break;
		
		case 600:
			if (b & PAD_TRIGGER_L) return true;
		break;
		case 601:
			if (b & PAD_TRIGGER_R) return true;
		break;
	}
	return false;
}


void Task::initializeGlobals(const char *savefile)
{
	extern gfx_tex_t tex;
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
