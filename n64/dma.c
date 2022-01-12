//
// helloworld/src/main.c: Threading test cart (entry point).
//
// n64chain: A (free) open-source N64 development toolchain.
// Copyright 2014-16 Tyler J. Stachecki <stachecki.tyler@gmail.com>
//
// This file is subject to the terms and conditions defined in
// 'LICENSE', which is part of this source code package.
//

#include <stdint.h>

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ **
** N64 DMA                         **
** ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

typedef struct
{
	/* Pointers to data */
	uint32_t ramp;
	uint32_t romp;
	
	/* Filesizes (8-byte aligned) */
	uint32_t size_ramrom; /* RAM -> ROM */
	uint32_t size_romram; /* RAM <- ROM */
	
	/* Status register */
	uint32_t status;
} DMA_REG;

/* DMA status flags */
enum
{
	DMA_BUSY  = 0x00000001,
	DMA_ERROR = 0x00000008
};

/* DMA registers ptr */
static volatile DMA_REG * dmaregs = (DMA_REG*)0xA4400010;

/* Copy data from ROM to RAM */
void dma_write_ram ( uint32_t ram_ptr, uint32_t rom_ptr, uint32_t length )
{
	/* Check that DMA is not busy already */
	while( dmaregs->status & DMA_BUSY );
	
	/* Write addresses */
	dmaregs->ramp = ram_ptr & 0x1FFFFFFF; /* ram pointer */
	dmaregs->romp = (rom_ptr | 0x10000000) & 0x1FFFFFFF; /* rom pointer */
	
	/* Write size */
	dmaregs->size_romram = length - 1;
	
	/* Wait for transfer to finish */
	while( dmaregs->status & DMA_BUSY );
}

