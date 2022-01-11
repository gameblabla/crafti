#ifndef BLOCKLISTTASK_H
#define BLOCKLISTTASK_H

#include "task.h"

class BlockListTask : public Task
{
public:
    BlockListTask();
    virtual ~BlockListTask();

    virtual void makeCurrent() override;

    virtual void render() override;
    virtual void logic() override;

    int current_selection;

private:
    static const int field_width = 32, field_height = 32, // Size of each field
                     pad_x = (field_width - 24) / 2, pad_y = (field_height - 24) / 2,
                     pad_y_door = (field_height - 32) / 2, // Door > field_height -> Negative!
                     fields_x = 9, fields_y = 5;

    static const unsigned int blocklist_width = fields_x * field_width + 2*pad_x, blocklist_left = (SCREEN_WIDTH - blocklist_width) / 2;
    static const unsigned int blocklist_height = fields_y * field_height + 2*pad_y;
    static unsigned int blocklist_top;
    //Black texture as background
    static TEXTURE *blocklist_background;
};

extern BlockListTask block_list_task;

#endif // BLOCKLISTTASK_H
