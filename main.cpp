#include <libndls.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include "gl.h"
#include "terrain.h"
#include "worldtask.h"
#include "settingstask.h"
#include "sound.h"
//#include "textures/loading.h"


/*
 * Hack to apply Distance at 9, otherwise it won't work.
*/
static inline void Apply()
{
	world_task.makeCurrent();
	world.setDirty();
	world.setFieldOfView(9);
}

int main(int argc, char *argv[])
{
	char home[128];
	char tmp[256];
	
    #ifdef _TINSPIRE
        //Sometimes there's a clock on screen, switch that off
        __asm__ volatile("mrs r0, cpsr;"
                        "orr r0, r0, #0x80;"
                        "msr cpsr_c, r0;" ::: "r0");
    #endif

    nglInit();
    /*if(lcd_type() == SCR_320x240_4)
        greyscaleTexture(loading);
    nglSetBuffer(loading.bitmap);
    nglDisplay();*/
    
    snprintf(home, sizeof(home), "%s/.crafti", getenv("HOME"));
    mkdir(home, 0755);

    //Early exit #1
    //(Task::keyPressed can only be used after initializeGlobals)
    if(isKeyPressed(KEY_NSPIRE_ESC))
    {
        nglUninit();
        return 0;
    }

    snprintf(tmp, sizeof(tmp), "%s/crafti.ppm", home);
    
    //terrainInit("/documents/ndless/crafti.ppm.tns");
    terrainInit(tmp);
    glBindTexture(terrain_current);

    glLoadIdentity();

    //Early exit #2
    if(isKeyPressed(KEY_NSPIRE_ESC))
    {
        terrainUninit();
        nglUninit();

        return 0;
    }

    //If crafti has been started by the file extension association, use the first argument as savefile path
    snprintf(tmp, sizeof(tmp), "%s/crafti.map", home);
    Task::initializeGlobals(argc > 1 ? argv[1] : tmp);
    //Task::initializeGlobals(argc > 1 ? argv[1] : "/documents/ndless/crafti.map.tns");

    if(Task::load())
        puts("Loaded world.");
    else
        puts("Failed to load world!");
        
    //Start with WorldTask as current task
    world_task.makeCurrent();
    Apply();
    
    Init_Sound();

    while(Task::running)
    {
		SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                {
                    Task::running = 0;
                    break;
                }
            }
        }
        //Reset "loading" message
        drawLoadingtext(-1);

        Task::current_task->render();

        nglDisplay();

        Task::current_task->logic();
    }

    Task::deinitializeGlobals();

    nglUninit();

    terrainUninit();
    
    Close_Sound();

    return 0;
}
