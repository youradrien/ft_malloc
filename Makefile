# =============================
ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME = libft_malloc_$(HOSTTYPE).so
LINK = libft_malloc.so

CC = gcc
CFLAGS = -Wall -Wextra -Werror -fPIC
## ----------------------------- ------------ -----------
## -fPIC: ensures .obj files are position independent and can be loaded anywhere in memory..
## -shared: make shared library, not normal executable.. output will be .so (shared object), not standalone program
## ------------------------ --------------- ---------
C_SHARED= -fPIC -shared
SRCS = main.c malloc.c free.c realloc.c calloc.c
OBJS = $(SRCS:.c=.o)

all: $(NAME) link

# compile the shared library
$(NAME): $(OBJS)
	$(CC) $(CC_SHARED) -o $@ $(OBJS)
# ---> will giveshared library : libft_malloc_x86_64-Linux.so

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

