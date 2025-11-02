# Makefile para o Processador de Imagens BMP
# Projeto 2 - Processamento de Imagens

# Compilador e flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2
LDFLAGS = -lm

# Nome do executável
TARGET = processador

# Arquivos fonte e objeto
SOURCES = main.c bmp_handler.c filters.c
OBJECTS = $(SOURCES:.c=.o)
HEADERS = bmp_handler.h filters.h

# Regra padrão
all: $(TARGET)

# Regra para criar o executável
$(TARGET): $(OBJECTS)
	@echo "Linkando $(TARGET)..."
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)
	@echo "Compilação concluída com sucesso!"
	@echo "Execute com: ./$(TARGET) <entrada.bmp> <saida.bmp> <filtro>"

# Regra para compilar arquivos .c em .o
%.o: %.c $(HEADERS)
	@echo "Compilando $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# Limpar arquivos compilados
clean:
	@echo "Limpando arquivos compilados..."
	rm -f $(OBJECTS) $(TARGET)
	@echo "Limpeza concluída!"

# Limpar e recompilar
rebuild: clean all

# Regra para executar (exemplo)
run: $(TARGET)
	./$(TARGET)

# Ajuda
help:
	@echo "Makefile do Processador de Imagens BMP"
	@echo ""
	@echo "Comandos disponíveis:"
	@echo "  make          - Compila o projeto"
	@echo "  make clean    - Remove arquivos compilados"
	@echo "  make rebuild  - Limpa e recompila"
	@echo "  make help     - Mostra esta mensagem"
	@echo ""
	@echo "Uso do programa:"
	@echo "  ./processador <entrada.bmp> <saida.bmp> <filtro>"
	@echo ""
	@echo "Filtros disponíveis:"
	@echo "  median, mode, lowpass, highpass"

# Declarar alvos que não são arquivos
.PHONY: all clean rebuild run help