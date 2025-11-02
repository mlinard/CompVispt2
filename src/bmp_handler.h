#ifndef BMP_HANDLER_H
#define BMP_HANDLER_H

#include <stdint.h>

// Estrutura para armazenar os dados de uma imagem BMP
typedef struct {
    int width;
    int height;
    uint8_t* data; // Array de pixels no formato BGR (Blue, Green, Red)
} Image;

// Estrutura do cabeçalho de arquivo BMP (14 bytes)
#pragma pack(push, 1)
typedef struct {
    uint16_t type;          // Tipo de arquivo (deve ser 0x4D42 para 'BM')
    uint32_t size;          // Tamanho do arquivo em bytes
    uint16_t reserved1;     // Reservado (deve ser 0)
    uint16_t reserved2;     // Reservado (deve ser 0)
    uint32_t offset;        // Offset para os dados da imagem
} BMPFileHeader;

// Estrutura do cabeçalho de informação BMP (40 bytes)
typedef struct {
    uint32_t size;          // Tamanho deste cabeçalho (40 bytes)
    int32_t width;          // Largura da imagem
    int32_t height;         // Altura da imagem
    uint16_t planes;        // Número de planos de cor (deve ser 1)
    uint16_t bits;          // Bits por pixel (24 para este projeto)
    uint32_t compression;   // Tipo de compressão (0 = não comprimido)
    uint32_t imagesize;     // Tamanho da imagem em bytes
    int32_t xresolution;    // Resolução horizontal
    int32_t yresolution;    // Resolução vertical
    uint32_t ncolours;      // Número de cores na paleta
    uint32_t importantcolours; // Cores importantes
} BMPInfoHeader;
#pragma pack(pop)

// Funções públicas
Image* loadImage(char* filename);
int saveImage(char* filename, Image* img);
void freeImage(Image* img);

#endif // BMP_HANDLER_H