#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bmp_handler.h"
#include "filters.h"

void printUsage(char* programName) {
    printf("=================================================\n");
    printf("  PROCESSADOR DE IMAGENS BMP - Versão 1.0\n");
    printf("=================================================\n\n");
    printf("Uso: %s <entrada.bmp> <saida.bmp> <filtro>\n\n", programName);
    printf("Filtros disponiveis:\n");
    printf("  median   - Filtro de Mediana (3x3)\n");
    printf("  mode     - Filtro de Moda (3x3)\n");
    printf("  lowpass  - Filtro Passa-Baixa (Box Blur 3x3)\n");
    printf("  highpass - Filtro Passa-Alta (Laplaciano 3x3)\n\n");
    printf("Exemplo:\n");
    printf("  %s imagem.bmp saida.bmp median\n\n", programName);
}

int main(int argc, char* argv[]) {
    // Verificar argumentos
    if (argc != 4) {
        fprintf(stderr, "Erro: Numero incorreto de argumentos!\n\n");
        printUsage(argv[0]);
        return 1;
    }

    char* inputFile = argv[1];
    char* outputFile = argv[2];
    char* filterName = argv[3];

    printf("\n=================================================\n");
    printf("  PROCESSADOR DE IMAGENS BMP\n");
    printf("=================================================\n\n");

    // Carregar imagem de entrada
    printf("Carregando imagem: %s\n", inputFile);
    Image* inputImage = loadImage(inputFile);
    if (!inputImage) {
        fprintf(stderr, "Erro: Falha ao carregar a imagem de entrada\n");
        return 1;
    }

    // Aplicar o filtro apropriado
    Image* outputImage = NULL;
    
    if (strcmp(filterName, "median") == 0) {
        outputImage = applyMedianFilter(inputImage);
    } 
    else if (strcmp(filterName, "mode") == 0) {
        outputImage = applyModeFilter(inputImage);
    } 
    else if (strcmp(filterName, "lowpass") == 0) {
        outputImage = applyLowPassFilter(inputImage);
    } 
    else if (strcmp(filterName, "highpass") == 0) {
        outputImage = applyHighPassFilter(inputImage);
    } 
    else {
        fprintf(stderr, "Erro: Filtro '%s' nao reconhecido!\n\n", filterName);
        printUsage(argv[0]);
        freeImage(inputImage);
        return 1;
    }

    // Verificar se o filtro foi aplicado com sucesso
    if (!outputImage) {
        fprintf(stderr, "Erro: Falha ao aplicar o filtro\n");
        freeImage(inputImage);
        return 1;
    }

    // Salvar imagem de saída
    printf("Salvando imagem: %s\n", outputFile);
    if (!saveImage(outputFile, outputImage)) {
        fprintf(stderr, "Erro: Falha ao salvar a imagem de saida\n");
        freeImage(inputImage);
        freeImage(outputImage);
        return 1;
    }

    printf("\n=================================================\n");
    printf("  Processamento concluido com sucesso!\n");
    printf("=================================================\n\n");

    // Liberar memória
    freeImage(inputImage);
    freeImage(outputImage);

    return 0;
}