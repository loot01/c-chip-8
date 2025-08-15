.SILENT:
NAME = chip-8
CC = gcc
CPPFLAGS = -Iincludes
CFLAGS = -Wall -Wextra -g
SRCS = $(wildcard srcs/*.c)
OBJS = $(SRCS:.c=.o)
all: $(NAME) clean
$(NAME): $(OBJS)
	$(LINK.o) $(OBJS) -lraylib -o $(NAME)
clean:
	rm -f $(OBJS)
fclean: clean
	rm -f $(NAME) 
re: fclean all
