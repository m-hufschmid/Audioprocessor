#ifndef DMA_H
#define	DMA_H


#include "constants.h"

extern int txBufferA[BUFFERLENGTH];
extern  int txBufferB[BUFFERLENGTH];
extern  int rxBufferA[BUFFERLENGTH];
extern  int rxBufferB[BUFFERLENGTH];

__eds__ extern int testVar[10] __attribute__((space(dma),eds));

void dma_init(void);

#endif	/* DMA_H */

