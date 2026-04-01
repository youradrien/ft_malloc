CC = gcc
CFLAGS = -Wall -Wextra -fPIC
HOSTTYPE ?= $(shell uname -m)_$(shell uname -s)
# ifeq ($(HOSTTYPE),)
# 	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
# endif

# NAME = libft_malloc_$(HOSTTYPE).so
# ## ----------------------------- ------------ -----------
# ## -fPIC: ensures .obj files are position independent and can be loaded anywhere in memory..
# ## -shared: make shared library, not normal executable.. output will be .so (shared object), not standalone program
# ## ------------------------ --------------- ---------
ifeq ($(shell uname),Darwin)
    LDFLAGS = -dynamiclib
    LIB_NAME = libft_malloc_$(HOSTTYPE).dylib
    LINK_NAME = libft_malloc.dylib
else
    LDFLAGS = -shared
    LIB_NAME = libft_malloc_$(HOSTTYPE).so
    LINK_NAME = libft_malloc.so
endif
PROG_NAME = MALLOC_TESTS

# lib
SRCS_LIB = src/malloc.c src/free.c src/realloc.c src/display.c
OBJ_LIB = $(SRCS_LIB:.c=.o)

# tests
SRC_TEST = tests/main.c
OBJ_TEST = $(SRC_TEST:.c=.o)

CFLAGS += -Iincludes
# def rule
all: $(LIB_NAME) link $(PROG_NAME)

# compile dynamic lib (-shared ou -dynamiclib)
$(LIB_NAME): $(OBJ_LIB)
	$(CC) $(LDFLAGS) -o $@ $^

# compile main avec -rpath
$(PROG_NAME): $(OBJ_TEST) $(LIB_NAME)
	$(CC) $(OBJ_TEST) -L. -lft_malloc -Wl,-rpath,@executable_path -o $@

# symbolic link host lib -> default lib name
link: $(LIB_NAME)
	@echo " - [linking] $(LIB_NAME) -> $(LINK_NAME)"
	@ln -sf $(LIB_NAME) $(LINK_NAME)


# obj main
$(OBJ_TEST): $(SRC_TEST)
	$(CC) $(CFLAGS) -Iincludes -c $< -o $@
# obj de la librairie
%.o: %.c
	$(CC) $(CFLAGS) -Iincludes -c $< -o $@


# tests files
TEST_DIR = tests/test_files
TEST_SRCS = $(wildcard $(TEST_DIR)/*.c)
TEST_BINS = $(TEST_SRCS:.c=)

# cible pour exécuter directement
run: all
	./$(PROG_NAME)

# nettoyer tous les fichiers générés
clean:
	rm -f $(OBJ_LIB) $(OBJ_TEST) $(LIB_NAME) $(PROG_NAME) $(LINK_NAME)
	rm -f $(TEST_BINS)

# reconstruire proprement
re: clean all
