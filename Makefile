# # =============================
# ifeq ($(HOSTTYPE),)
# 	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
# endif

# HOSTTYPE ?= $(shell uname -m)_$(shell uname -s)
# NAME = libft_malloc_$(HOSTTYPE).so
# LIB_NAME = libft_malloc.so


# CC = gcc
# CFLAGS = -Wall -Wextra -Werror

# ifeq ($(UNAME_S),Darwin)
# 	C_SHARED= -fPIC -shared
# else
# 	C_SHARED= -dynamiclib -fPIC
# endif

# SRCS = src/malloc.c src/calloc.c src/free.c src/realloc.c 
# INCLUDES = includes
# OBJS = $(SRCS:.c=.o)
# UNAME_S := $(shell uname -s)

# all: $(NAME) link

# # compile the shared library : libft_malloc_x86_64-Linux.so
# $(NAME): $(OBJS)
# 	@rm -f $(LIB_NAME)
# 	@echo " - [making] $(NAME)"
# 	@$(CC) $(CFLAGS) $(C_SHARED) $(OBJS) -o $@




# # compile object files only if changed (cuh)
# %.o: %.c
# 	@$(CC) $(CFLAGS) -fPIC -I$(INCLUDES) -c $< -o $@

# # symbolic link libft_malloc.so -> libft_malloc_$HOSTTYPE.so
# link: $(NAME)	
# 	@echo " - [linking] $(NAME) -> libft_malloc.so"
# 	@ln -sf $(NAME) libft_malloc.so



CC = gcc
CFLAGS = -Wall -Wextra -fPIC
HOSTTYPE ?= $(shell uname -m)_$(shell uname -s)

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
SRCS_LIB = src/malloc.c src/free.c src/realloc.c
OBJ_LIB = $(SRCS_LIB:.c=.o)

# main
SRC_MAIN = src/main.c
OBJ_MAIN = $(SRC_MAIN:.c=.o)

CFLAGS += -Iincludes
# def rule
all: $(LIB_NAME) link $(PROG_NAME)

# compile dynamic lib (-shared ou -dynamiclib)
$(LIB_NAME): $(OBJ_LIB)
	$(CC) $(LDFLAGS) -o $@ $^

# compile main avec -rpath
$(PROG_NAME): $(OBJ_MAIN) $(LIB_NAME)
	$(CC) $(OBJ_MAIN) -L. -lft_malloc -Wl,-rpath,@executable_path -o $@

# symbolic link host lib -> default lib name
link: $(LIB_NAME)
	@echo " - [linking] $(LIB_NAME) -> $(LINK_NAME)"
	@ln -sf $(LIB_NAME) $(LINK_NAME)


# obj du main
$(OBJ_MAIN): $(SRC_MAIN)
	$(CC) $(CFLAGS) -Iincludes -c $< -o $@
# obj de la librairie
# $(OBJ_LIB): $(SRCS_LIB)
# 	$(CC) $(CFLAGS) -c $< -o $@
%.o: %.c
	$(CC) $(CFLAGS) -Iincludes -c $< -o $@


# cible pour exécuter directement
run: all
	./$(PROG_NAME)

# nettoyer tous les fichiers générés
clean:
	rm -f $(OBJ_LIB) $(OBJ_MAIN) $(LIB_NAME) $(PROG_NAME)

# reconstruire proprement
re: clean all
