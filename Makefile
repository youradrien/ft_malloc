# =============================
ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME = libft_malloc_$(HOSTTYPE).so
LINK = libft_malloc.so

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

all: $(NAME) link

# compile the shared library
$(NAME): $(OBJS)
	$(CC) $(C_SHARED) -I$(INCLUDES) -o $@ $(OBJS)

# ---> will giveshared library : libft_malloc_x86_64-Linux.so

# compile object files only if changed (cuh)
%.o: %.c
	$(CC) $(CFLAGS) -I$(INCLUDES) -c $< -o $@

# symbolic link libft_malloc.so -> libft_malloc_$HOSTTYPE.so
link: $(NAME)
	@ln -sf $(NAME) $(LINK)

clean:
	rm -f $(OBJS)
	rm -f libft_malloc*.so

fclean: clean
	rm -f $(NAME) $(LINK)
	@echo "Removing shared library..."
	rm -f libft_malloc*.so

re: fclean all

# Optional: test your malloc with LD_PRELOAD
test: all
	LD_PRELOAD=./$(LINK) ls

