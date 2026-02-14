# =============================
ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

HOSTTYPE ?= $(shell uname -m)_$(shell uname -s)
NAME = libft_malloc_$(HOSTTYPE).so
LIB_NAME = libft_malloc.so


CC = gcc
CFLAGS = -Wall -Wextra -Werror
## ----------------------------- ------------ -----------
## -fPIC: ensures .obj files are position independent and can be loaded anywhere in memory..
## -shared: make shared library, not normal executable.. output will be .so (shared object), not standalone program
## ------------------------ --------------- ---------
C_SHARED= -fPIC -shared
SRCS = src/malloc.c src/calloc.c src/free.c
INCLUDES = ./includes
OBJS = $(SRCS:.c=.o)
UNAME_S := $(shell uname -s)

all: $(NAME) link

# compile the shared library
$(NAME): $(OBJS)
	@echo " - making $(NAME)"
	@rm -f $(LIB_NAME)
	@$(CC) $(CFLAGS) $(C_SHARED) -I$(INCLUDES) -o $@ $(OBJS)

# ---> will giveshared library : libft_malloc_x86_64-Linux.so

# compile object files only if changed (cuh)
%.o: %.c
	@$(CC) $(CFLAGS) -I$(INCLUDES) -c $< -o $@

# symbolic link libft_malloc.so -> libft_malloc_$HOSTTYPE.so
link: $(NAME)	
	@echo " - linking $(NAME) -> libft_malloc.so"
	@ln -sf $(NAME) libft_malloc.so




test_exec: src/main.c includes/malloc.h
#  macOS only
ifeq ($(UNAME_S),Darwin)
	@echo " - macOS test executable"
	@cc -Iincludes src/main.c -o test_exec -Wl,-undefined,dynamic_lookup -nodefaultlibs -lc
else
	@echo " - Linux test executable"
	@$(CC) -Iincludes src/main.c -o test_exec
endif


# run test executable w/ malloc injection
run: test_exec
ifeq ($(UNAME_S),Darwin)
	@echo "===> Running test_exec with MY MALLOC (macOS)"
	@export DYLD_LIBRARY_PATH=$$PWD; \
	export DYLD_INSERT_LIBRARIES=$$PWD/libft_malloc.so; \
	export DYLD_FORCE_FLAT_NAMESPACE=1; \
	./test_exec
else
	@echo "===> Running test_exec with MY MALLOC (Linux)"
	@LD_PRELOAD=./libft_malloc.so ./test_exec
endif


clean:
	rm -f $(OBJS)
	rm -f libft_malloc*.so
	rm -f test_exec

fclean: clean
	rm -f $(NAME) $(LIB_NAME)
	@echo " - removing shared library..."
	rm -f libft_malloc*.so

re: fclean all
