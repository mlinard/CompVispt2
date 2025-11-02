#include "filters.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Função auxiliar para clamping de valores entre 0 e 255
static inline uint8_t clamp(int value) {
    if (value < 0) return 0;
    if (value > 255) return 255;
    return (uint8_t)value;
}

// Função de comparação para qsort
static int compare_uint8(const void* a, const void* b) {
    return (*(uint8_t*)a - *(uint8_t*)b);
}

// Função auxiliar para obter pixel com tratamento de bordas (espelhamento)
static uint8_t getPixel(Image* img, int x, int y, int channel) {
    // Tratamento de bordas por espelhamento
    if (x < 0) x = -x;
    if (x >= img->width) x = 2 * img->width - x - 2;
    if (y < 0) y = -y;
    if (y >= img->height) y = 2 * img->height - y - 2;
    
    // Garantir que está dentro dos limites
    if (x < 0) x = 0;
    if (x >= img->width) x = img->width - 1;
    if (y < 0) y = 0;
    if (y >= img->height) y = img->height - 1;
    
    return img->data[(y * img->width + x) * 3 + channel];
}

Image* applyMedianFilter(Image* input) {
    if (!input || !input->data) {
        fprintf(stderr, "Erro: Imagem de entrada inválida\n");
        return NULL;
    }

    // Criar nova imagem para saída
    Image* output = (Image*)malloc(sizeof(Image));
    output->width = input->width;
    output->height = input->height;
    output->data = (uint8_t*)malloc(input->width * input->height * 3);

    printf("Aplicando filtro de mediana...\n");

    // Processar cada pixel
    for (int y = 0; y < input->height; y++) {
        for (int x = 0; x < input->width; x++) {
            // Para cada canal (B, G, R)
            for (int c = 0; c < 3; c++) {
                uint8_t window[9];
                int idx = 0;
                
                // Coletar valores do kernel 3x3
                for (int dy = -1; dy <= 1; dy++) {
                    for (int dx = -1; dx <= 1; dx++) {
                        window[idx++] = getPixel(input, x + dx, y + dy, c);
                    }
                }
                
                // Ordenar os valores
                qsort(window, 9, sizeof(uint8_t), compare_uint8);
                
                // Mediana é o valor do meio
                output->data[(y * output->width + x) * 3 + c] = window[4];
            }
        }
    }

    printf("Filtro de mediana aplicado com sucesso!\n");
    return output;
}

Image* applyModeFilter(Image* input) {
    if (!input || !input->data) {
        fprintf(stderr, "Erro: Imagem de entrada inválida\n");
        return NULL;
    }

    // Criar nova imagem para saída
    Image* output = (Image*)malloc(sizeof(Image));
    output->width = input->width;
    output->height = input->height;
    output->data = (uint8_t*)malloc(input->width * input->height * 3);

    printf("Aplicando filtro de moda...\n");

    // Processar cada pixel
    for (int y = 0; y < input->height; y++) {
        for (int x = 0; x < input->width; x++) {
            // Para cada canal (B, G, R)
            for (int c = 0; c < 3; c++) {
                uint8_t window[9];
                int idx = 0;
                
                // Coletar valores do kernel 3x3
                for (int dy = -1; dy <= 1; dy++) {
                    for (int dx = -1; dx <= 1; dx++) {
                        window[idx++] = getPixel(input, x + dx, y + dy, c);
                    }
                }
                
                // Encontrar a moda (valor mais frequente)
                int maxCount = 0;
                uint8_t mode = window[0];
                
                for (int i = 0; i < 9; i++) {
                    int count = 0;
                    for (int j = 0; j < 9; j++) {
                        if (window[j] == window[i]) {
                            count++;
                        }
                    }
                    if (count > maxCount) {
                        maxCount = count;
                        mode = window[i];
                    }
                }
                
                output->data[(y * output->width + x) * 3 + c] = mode;
            }
        }
    }

    printf("Filtro de moda aplicado com sucesso!\n");
    return output;
}

Image* applyLowPassFilter(Image* input) {
    if (!input || !input->data) {
        fprintf(stderr, "Erro: Imagem de entrada inválida\n");
        return NULL;
    }

    // Criar nova imagem para saída
    Image* output = (Image*)malloc(sizeof(Image));
    output->width = input->width;
    output->height = input->height;
    output->data = (uint8_t*)malloc(input->width * input->height * 3);

    printf("Aplicando filtro passa-baixa (Box Blur 3x3)...\n");

    // Kernel de média 3x3 (todos os pesos são 1/9)
    // Processar cada pixel
    for (int y = 0; y < input->height; y++) {
        for (int x = 0; x < input->width; x++) {
            // Para cada canal (B, G, R)
            for (int c = 0; c < 3; c++) {
                int sum = 0;
                
                // Aplicar kernel 3x3
                for (int dy = -1; dy <= 1; dy++) {
                    for (int dx = -1; dx <= 1; dx++) {
                        sum += getPixel(input, x + dx, y + dy, c);
                    }
                }
                
                // Calcular média
                output->data[(y * output->width + x) * 3 + c] = clamp(sum / 9);
            }
        }
    }

    printf("Filtro passa-baixa aplicado com sucesso!\n");
    return output;
}

Image* applyHighPassFilter(Image* input) {
    if (!input || !input->data) {
        fprintf(stderr, "Erro: Imagem de entrada inválida\n");
        return NULL;
    }

    // Criar nova imagem para saída
    Image* output = (Image*)malloc(sizeof(Image));
    output->width = input->width;
    output->height = input->height;
    output->data = (uint8_t*)malloc(input->width * input->height * 3);

    printf("Aplicando filtro passa-alta (Laplaciano 3x3)...\n");

    // Kernel Laplaciano 3x3:
    // [ 0,  1,  0]
    // [ 1, -4,  1]
    // [ 0,  1,  0]
    int kernel[3][3] = {
        {0,  1,  0},
        {1, -4,  1},
        {0,  1,  0}
    };

    // Processar cada pixel
    for (int y = 0; y < input->height; y++) {
        for (int x = 0; x < input->width; x++) {
            // Para cada canal (B, G, R)
            for (int c = 0; c < 3; c++) {
                int sum = 0;
                
                // Aplicar kernel 3x3
                for (int dy = -1; dy <= 1; dy++) {
                    for (int dx = -1; dx <= 1; dx++) {
                        int pixelValue = getPixel(input, x + dx, y + dy, c);
                        sum += pixelValue * kernel[dy + 1][dx + 1];
                    }
                }
                
                // Adicionar 128 para visualização (offset) e clampar
                output->data[(y * output->width + x) * 3 + c] = clamp(sum + 128);
            }
        }
    }

    printf("Filtro passa-alta aplicado com sucesso!\n");
    return output;
}