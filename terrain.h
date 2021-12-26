#ifndef TERRAIN_H
#define TERRAIN_H

#include <cstdint>

#include "texturetools.h"

#define BLOCK_SIZE 128

typedef uint8_t BLOCK;
typedef uint16_t BLOCK_WDATA;

constexpr BLOCK BLOCK_AIR = 0;
constexpr BLOCK BLOCK_STONE = 1;
constexpr BLOCK BLOCK_DIRT = 2;
constexpr BLOCK BLOCK_SAND = 3;
constexpr BLOCK BLOCK_WOOD = 4;
constexpr BLOCK BLOCK_LEAVES = 5;
constexpr BLOCK BLOCK_PLANKS_NORMAL = 6;
constexpr BLOCK BLOCK_WALL = 7;
constexpr BLOCK BLOCK_COAL_ORE = 8;
constexpr BLOCK BLOCK_GOLD_ORE = 9;
constexpr BLOCK BLOCK_IRON_ORE = 10;
constexpr BLOCK BLOCK_DIAMOND_ORE = 11;
constexpr BLOCK BLOCK_REDSTONE_ORE = 12;
constexpr BLOCK BLOCK_TNT = 13;
constexpr BLOCK BLOCK_SPONGE = 14;
constexpr BLOCK BLOCK_PLANKS_DARK = 15;
constexpr BLOCK BLOCK_PLANKS_BRIGHT = 16;
constexpr BLOCK BLOCK_FURNACE = 17;
constexpr BLOCK BLOCK_CRAFTING_TABLE = 18;
constexpr BLOCK BLOCK_BOOKSHELF = 19;
constexpr BLOCK BLOCK_GRASS = 20;
constexpr BLOCK BLOCK_PUMPKIN = 21;
constexpr BLOCK BLOCK_BEDROCK = 22;
constexpr BLOCK BLOCK_GLASS = 23;
constexpr BLOCK BLOCK_COBBLESTONE = 24;
constexpr BLOCK BLOCK_GLOWSTONE = 25;
constexpr BLOCK BLOCK_IRON = 26;
constexpr BLOCK BLOCK_GOLD = 27;
constexpr BLOCK BLOCK_DIAMOND = 28;
constexpr BLOCK BLOCK_NETHERRACK = 29;
constexpr BLOCK BLOCK_NORMAL_LAST = BLOCK_NETHERRACK;

//Special blocks begin here
constexpr int BLOCK_SPECIAL_START = 127;
constexpr BLOCK BLOCK_TORCH = 127; //Data: Direction (BLOCK_SIDE)
constexpr BLOCK BLOCK_FLOWER = 128; //Data: Flower type
constexpr BLOCK BLOCK_SPIDERWEB = 129;
constexpr BLOCK BLOCK_CAKE = 130;
constexpr BLOCK BLOCK_MUSHROOM = 131; //Data: Mushroom type
constexpr BLOCK BLOCK_DOOR = 132; //Data: (top: 1<<3) | BLOCK_SIDE
constexpr BLOCK BLOCK_WATER = 133; //Data: range
constexpr BLOCK BLOCK_LAVA = 134; //Data: range
constexpr BLOCK BLOCK_WHEAT = 135; //Data: growth
constexpr BLOCK BLOCK_REDSTONE_LAMP = 146; //Data: On/Off
constexpr BLOCK BLOCK_REDSTONE_SWITCH = 147; //Data: BLOCK_SIDE
constexpr BLOCK BLOCK_REDSTONE_WIRE = 148; //Data: (active: 1<<6, visited: 1<<5)
constexpr BLOCK BLOCK_REDSTONE_TORCH = 149; //Data: See BLOCK_TORCH
constexpr BLOCK BLOCK_PRESSURE_PLATE = 150; //Data: How many ticks kept active
constexpr BLOCK BLOCK_SPECIAL_LAST = BLOCK_REDSTONE_TORCH;

constexpr uint8_t RANGE_WATER = 5;
constexpr uint8_t RANGE_LAVA = 3;

constexpr BLOCK getBLOCK(BLOCK_WDATA bd) { return bd & 0xFF; }
constexpr uint8_t getBLOCKDATA(BLOCK_WDATA bd) { return (bd >> 8) & 0x7F; }
constexpr bool getPOWERSTATE(BLOCK_WDATA bd) { return bd & (1 << 15); }
constexpr BLOCK_WDATA getBLOCKWDATAPower(BLOCK b, uint8_t data, bool powering) { return (data << 8) | b | (powering ? 1 << 15 : 0); }
constexpr BLOCK_WDATA getBLOCKWDATA(BLOCK b, uint8_t data) { return (data << 8) | b; }

enum BLOCK_SIDE{
    BLOCK_FRONT=0,
    BLOCK_BACK,
    BLOCK_LEFT,
    BLOCK_RIGHT,
    BLOCK_TOP,
    BLOCK_BOTTOM
};
constexpr int BLOCK_SIDE_LAST = BLOCK_BOTTOM;
constexpr int BLOCK_SIDE_BITS = 0b111;

typedef uint8_t BLOCK_SIDE_BITFIELD;
constexpr BLOCK_SIDE_BITFIELD BLOCK_FRONT_BIT = 1;
constexpr BLOCK_SIDE_BITFIELD BLOCK_BACK_BIT = 2;
constexpr BLOCK_SIDE_BITFIELD BLOCK_LEFT_BIT = 4;
constexpr BLOCK_SIDE_BITFIELD BLOCK_RIGHT_BIT = 8;
constexpr BLOCK_SIDE_BITFIELD BLOCK_TOP_BIT = 16;
constexpr BLOCK_SIDE_BITFIELD BLOCK_BOTTOM_BIT = 32;

constexpr BLOCK_SIDE_BITFIELD blockSideToBit(const BLOCK_SIDE side)
{
    return 1 << side;
}

//There may be more than one resolution, so compute TextureAtlasEntries for both
struct TerrainAtlasEntry {
    TextureAtlasEntry current; //For blocks
    TextureAtlasEntry resized; //For GUI and other elements which shouldn't scale to the texture
};

extern const char *block_names[];
extern TerrainAtlasEntry block_textures[BLOCK_NORMAL_LAST + 1][BLOCK_SIDE_LAST + 1];
extern TerrainAtlasEntry terrain_atlas[16][16];

//terrain_resized is always 256x256 pixels
//The included texture or loaded texture without any modifications
extern TEXTURE *terrain_current;
//A resized copy (256x256) for non-scalable stuff, like GUI
extern TEXTURE *terrain_resized;
/*Contains four times the texture for skipping some triangles:
* 8 triangles -> 2 triangles!
*  ___      ___
* |\|\|    |\  |
* |\|\| => |  \|
*  ‾‾‾‾      ‾‾‾‾
* It's quite space consuming, so only selected textures are used*/
extern TEXTURE *terrain_quad;
struct TerrainQuadEntry {
    bool has_quad;
    TextureAtlasEntry tae;
    COLOR color; //Has nothing to do with quads, but applies to the same textures
    COLOR darker; //So you can distinguish adjacent blocks
};

//The glass texture resized to 32x32
extern TEXTURE *glass_big;

constexpr int DIR_HORIZONTAL = 0;
constexpr int DIR_VERTICAL = 1;
extern TerrainQuadEntry quad_block_textures[BLOCK_NORMAL_LAST + 1][BLOCK_SIDE_LAST + 1];

void terrainInit(const char *texture_path);
void terrainUninit();

#endif // TERRAIN_H
