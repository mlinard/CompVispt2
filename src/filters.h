#ifndef FILTERS_H
#define FILTERS_H

#include "bmp_handler.h"

// Filtro de Mediana (kernel 3x3)
Image* applyMedianFilter(Image* input);

// Filtro de Moda (kernel 3x3)
Image* applyModeFilter(Image* input);

// Filtro Passa-Baixa (Box Blur 3x3)
Image* applyLowPassFilter(Image* input);

// Filtro Passa-Alta (Laplaciano 3x3)
Image* applyHighPassFilter(Image* input);

#endif // FILTERS_H