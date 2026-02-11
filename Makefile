# =============================
ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME = libft_malloc_$(HOSTTYPE).so
LINK = libft_malloc.so

CC = gcc
CFLAGS = -Wall -Wextra -Werror -fPIC
## ----------------------------- ------------ -----------
## -fPIC: generate code that can be loaded anywhere in memory.. not fixed addresses
## -shared: make shared library, not normal executable.. output will be .so (shared object), not standalone program
## ------------------------ --------------- ---------
C_SHARED_FLAGS = -fPIC -shared
SRCS = main.c malloc.c free.c realloc.c calloc.c
OBJS = $(SRCS:.c=.o)

all: $(NAME) link

# compile the shared library
$(NAME): $(OBJS)
	$(CC) -o $@ $(OBJS)

# compile object files only if changed (cuh)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# symbolic link libft_malloc.so -> libft_malloc_$HOSTTYPE.so
link: $(NAME)
	@ln -sf $(NAME) $(LINK)

clean:
	rm -f $(OBJS)
	@$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME) $(LINK)
	@$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

# Optional: test your malloc with LD_PRELOAD
test: all
	LD_PRELOAD=./$(LINK) ls

