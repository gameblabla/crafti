#include <libndls.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <kos.h>
#include "gl.h"
#include "terrain.h"
#include "worldtask.h"
#include "settingstask.h"
#include "sound.h"

KOS_INIT_FLAGS(INIT_DEFAULT | INIT_MALLOCSTATS);
#ifdef NAOMI
extern uint8 romdisk[];
KOS_INIT_ROMDISK(romdisk);
#endif

/*
 * Hack to apply Distance at 4, otherwise it won't work.
*/
static inline void Apply()
{
	world_task.makeCurrent();
	world.setDirty();
	world.setFieldOfView(3);
}

int main(int argc, char *argv[])
{
    nglInit();
    //snprintf(tmp, sizeof(tmp), "%s/crafti.ppm", home);
    //terrainInit("/documents/ndless/crafti.ppm.tns");
    terrainInit("");
    glBindTexture(terrain_current);

    glLoadIdentity();

    //If crafti has been started by the file extension association, use the first argument as savefile path
    //snprintf(tmp, sizeof(tmp), "%s/crafti.map", home);
    //Task::initializeGlobals(argc > 1 ? argv[1] : tmp);
    //Task::initializeGlobals(argc > 1 ? argv[1] : "/documents/ndless/crafti.map.tns");
    Task::initializeGlobals("");
	Task::load();
	
    //Start with WorldTask as current task
    world_task.makeCurrent();
    Apply();
    
    Init_Sound();

    while(Task::running)
    {
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
