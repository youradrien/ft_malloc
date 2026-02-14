# =============================
ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

HOSTTYPE ?= $(shell uname -m)_$(shell uname -s)
NAME = libft_malloc_$(HOSTTYPE).so


CC = gcc
CFLAGS = -Wall -Wextra -Werror
## ----------------------------- ------------ -----------
## -fPIC: ensures .obj files are position independent and can be loaded anywhere in memory..
## -shared: make shared library, not normal executable.. output will be .so (shared object), not standalone program
## ------------------------ --------------- ---------
C_SHARED= -fPIC -shared
SRCS = src/malloc.c src/calloc.c src/free.c
INCLUDES = includes
OBJS = $(SRCS:.c=.o)
UNAME_S := $(shell uname -s)

all: $(NAME) link

# compile the shared library
$(NAME): $(OBJS)
	@echo " - making $(NAME)"
	@$(CC) $(C_SHARED) -I$(INCLUDES) -o $@ $(OBJS)

# ---> will giveshared library : libft_malloc_x86_64-Linux.so

# compile object files only if changed (cuh)
%.o: %.c
	@$(CC) $(CFLAGS) -I$(INCLUDES) -c $< -o $@

# symbolic link libft_malloc.so -> libft_malloc_$HOSTTYPE.so
link: $(NAME)	
	@echo " - linking $(NAME) -> libft_malloc.so"
	@ln -sf $(NAME) libft_malloc.so



#  macOS only
ifeq ($(UNAME_S),Darwin)
    TEST_CC = cc -Iincludes src/main.c -o test_exec -Wl,-undefined,dynamic_lookup \
	-nodefaultlibs -lc
else
    TEST_CC = $(CC) -Iincludes src/main.c -o test_exec
endif



run: 
	@echo " - compilation OS: $(UNAME_S)"
	$(TEST_CC)
	DYLD_INSERT_LIBRARIES=./libft_malloc.so DYLD_FORCE_FLAT_NAMESPACE=1 ./test_exec

clean:
	rm -f $(OBJS)
	rm -f libft_malloc*.so
	rm -f test_exec

fclean: clean
	rm -f $(NAME) $(LINK)
	@echo " - removing shared library..."
	rm -f libft_malloc*.so

re: fclean all

# Optional: test your malloc with LD_PRELOAD
test: all
	LD_PRELOAD=./$(LINK) ls

