CFLAGS = -Wall
FINAL_NAME = main
CLEANUP = del /F /Q

SRC_DIR = ./src
OBJ_DIR = ./build

SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES))

final: $(OBJ_FILES)
	@gcc $(CFLAGS) $(OBJ_FILES) -o $(OBJ_DIR)/$(FINAL_NAME).exe
	@make run
	@make clean

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@gcc $(CFLAGS) -c -o $@ $<

run:
	$(OBJ_DIR)/$(FINAL_NAME).exe

.PHONY: clean
clean: 
	@$(CLEANUP) $(subst /,\, $(OBJ_FILES))
	@$(CLEANUP) $(subst /,\, $(OBJ_DIR)/$(FINAL_NAME).exe)
