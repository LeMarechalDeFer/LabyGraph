# Nom de l'exécutable
EXEC = labygraph

# Dossiers
SRC_DIR = src
OBJ_DIR = obj
INC_DIR = include
LIB_DIR = /usr/local/lib

# Fichiers sources et objets
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Options de compilation
CC = gcc
CFLAGS = -I$(INC_DIR) -I/usr/local/include -Wall -Wextra -std=c11
LDFLAGS = -L$(LIB_DIR) -lraylib -lm -lpthread -ldl -lrt -lX11

# Règles de construction
all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $^ -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -f $(OBJ_DIR)/*.o $(EXEC)
	rm -rf $(OBJ_DIR)

.PHONY: all clean
