#ifndef DMA_H
#define DMA_H

#ifdef __cplusplus
extern "C" {
#endif

void dma_write_ram ( uint32_t ram_ptr, uint32_t ram2_ptr, uint32_t length);

#ifdef __cplusplus
}
#endif

#endif
