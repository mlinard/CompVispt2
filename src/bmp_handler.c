#include "bmp_handler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Image* loadImage(char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        fprintf(stderr, "Erro: Não foi possível abrir o arquivo %s\n", filename);
        return NULL;
    }

    // Ler cabeçalho do arquivo
    BMPFileHeader fileHeader;
    if (fread(&fileHeader, sizeof(BMPFileHeader), 1, file) != 1) {
        fprintf(stderr, "Erro: Não foi possível ler o cabeçalho do arquivo\n");
        fclose(file);
        return NULL;
    }

    // Verificar se é um arquivo BMP válido
    if (fileHeader.type != 0x4D42) { // 'BM' em little-endian
        fprintf(stderr, "Erro: Arquivo não é um BMP válido\n");
        fclose(file);
        return NULL;
    }

    // Ler cabeçalho de informação
    BMPInfoHeader infoHeader;
    if (fread(&infoHeader, sizeof(BMPInfoHeader), 1, file) != 1) {
        fprintf(stderr, "Erro: Não foi possível ler o cabeçalho de informação\n");
        fclose(file);
        return NULL;
    }

    // Verificar se é 24 bits não comprimido
    if (infoHeader.bits != 24 || infoHeader.compression != 0) {
        fprintf(stderr, "Erro: Apenas BMP de 24 bits não comprimidos são suportados\n");
        fclose(file);
        return NULL;
    }

    // Criar estrutura Image
    Image* img = (Image*)malloc(sizeof(Image));
    if (!img) {
        fprintf(stderr, "Erro: Falha ao alocar memória para a estrutura Image\n");
        fclose(file);
        return NULL;
    }

    img->width = infoHeader.width;
    img->height = abs(infoHeader.height);

    // Calcular padding (BMP requer linhas alinhadas a 4 bytes)
    int padding = (4 - (img->width * 3) % 4) % 4;
    int rowSize = img->width * 3 + padding;

    // Alocar memória para os dados da imagem
    img->data = (uint8_t*)malloc(img->width * img->height * 3);
    if (!img->data) {
        fprintf(stderr, "Erro: Falha ao alocar memória para os dados da imagem\n");
        free(img);
        fclose(file);
        return NULL;
    }

    // Posicionar no início dos dados da imagem
    fseek(file, fileHeader.offset, SEEK_SET);

    // Ler os dados da imagem (considerando padding)
    uint8_t* rowBuffer = (uint8_t*)malloc(rowSize);
    for (int y = 0; y < img->height; y++) {
        if (fread(rowBuffer, 1, rowSize, file) != (size_t)rowSize) {
            fprintf(stderr, "Erro: Falha ao ler dados da imagem\n");
            free(rowBuffer);
            free(img->data);
            free(img);
            fclose(file);
            return NULL;
        }
        
        // Copiar dados sem o padding
        // BMP armazena de baixo para cima, então invertemos
        int destRow = (infoHeader.height > 0) ? (img->height - 1 - y) : y;
        memcpy(img->data + destRow * img->width * 3, rowBuffer, img->width * 3);
    }

    free(rowBuffer);
    fclose(file);

    printf("Imagem carregada: %dx%d pixels\n", img->width, img->height);
    return img;
}

int saveImage(char* filename, Image* img) {
    if (!img || !img->data) {
        fprintf(stderr, "Erro: Imagem inválida\n");
        return 0;
    }

    FILE* file = fopen(filename, "wb");
    if (!file) {
        fprintf(stderr, "Erro: Não foi possível criar o arquivo %s\n", filename);
        return 0;
    }

    // Calcular padding
    int padding = (4 - (img->width * 3) % 4) % 4;
    int rowSize = img->width * 3 + padding;
    int imageSize = rowSize * img->height;

    // Preparar cabeçalho do arquivo
    BMPFileHeader fileHeader;
    fileHeader.type = 0x4D42; // 'BM'
    fileHeader.size = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader) + imageSize;
    fileHeader.reserved1 = 0;
    fileHeader.reserved2 = 0;
    fileHeader.offset = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader);

    // Preparar cabeçalho de informação
    BMPInfoHeader infoHeader;
    infoHeader.size = sizeof(BMPInfoHeader);
    infoHeader.width = img->width;
    infoHeader.height = img->height; // Positivo = bottom-up
    infoHeader.planes = 1;
    infoHeader.bits = 24;
    infoHeader.compression = 0;
    infoHeader.imagesize = imageSize;
    infoHeader.xresolution = 2835; // 72 DPI
    infoHeader.yresolution = 2835;
    infoHeader.ncolours = 0;
    infoHeader.importantcolours = 0;

    // Escrever cabeçalhos
    fwrite(&fileHeader, sizeof(BMPFileHeader), 1, file);
    fwrite(&infoHeader, sizeof(BMPInfoHeader), 1, file);

    // Preparar buffer de padding
    uint8_t paddingBytes[3] = {0, 0, 0};

    // Escrever dados da imagem (de baixo para cima)
    for (int y = img->height - 1; y >= 0; y--) {
        fwrite(img->data + y * img->width * 3, 1, img->width * 3, file);
        if (padding > 0) {
            fwrite(paddingBytes, 1, padding, file);
        }
    }

    fclose(file);
    printf("Imagem salva: %s\n", filename);
    return 1;
}

void freeImage(Image* img) {
    if (img) {
        if (img->data) {
            free(img->data);
        }
        free(img);
    }
}