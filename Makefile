# Makefile para o Processador de Imagens BMP
# Projeto 2 - Processamento de Imagens

# Compilador e flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2
LDFLAGS = -lm

# Nome do executável
TARGET = processador

# --- ALTERAÇÕES AQUI ---
# Define os diretórios de código
SRC_DIR = src
# Encontra todos os arquivos .c dentro de SRC_DIR
SOURCES = $(wildcard $(SRC_DIR)/*.c)
# Cria nomes de arquivos objeto (ex: src/main.o)
OBJECTS = $(SOURCES:.c=.o)
# Define os cabeçalhos
HEADERS = $(wildcard $(SRC_DIR)/*.h)
# --- FIM DAS ALTERAÇÕES ---

# Regra padrão
all: $(TARGET)

# Regra para criar o executável
$(TARGET): $(OBJECTS)
	@echo "Linkando $(TARGET)..."
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)
	@echo "Compilação concluída com sucesso!"
	@echo "Execute com: ./$(TARGET) <entrada.bmp> <saida.bmp> <filtro>"

# --- ALTERAÇÃO AQUI ---
# Regra para compilar arquivos .c em .o (procurando em src/)
$(SRC_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS)
	@echo "Compilando $<..."
	$(CC) $(CFLAGS) -c $< -o $@
# --- FIM DA ALTERAÇÃO ---

# Limpar arquivos compilados
clean:
	@echo "Limpando arquivos compilados..."
	# --- ALTERAÇÃO AQUI ---
	rm -f $(SRC_DIR)/*.o $(TARGET)
	@echo "Limpeza concluída!"

# Limpar e recompilar
rebuild: clean all

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