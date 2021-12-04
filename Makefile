CC=gcc
CFLAGS=-O2 -Wall
OBJ_DIR=objects
BIN_DIR=bin

all: project

project: project.c $(OBJ_DIR)/database.o $(OBJ_DIR)/utils.o $(OBJ_DIR)/sql.o $(OBJ_DIR)/check.o $(OBJ_DIR)/table.o $(OBJ_DIR)/expand.o $(OBJ_DIR)/query_exec.o $(OBJ_DIR)/record_list.o
	$(CC) $(CFLAGS) -o $(BIN_DIR)/project $^
	
$(OBJ_DIR)/%o: %c %h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJ_DIR)/*.o $(BIN_DIR)/project
