##
## EPITECH PROJECT, 2018
##
## File description:
##
##

#-ansi -pedantic

SRC	=	malloc.c	\
		free.c		\
		allocate.c	\
		manage_heap.c	\
		debug.c

FLAGS	=	-W -Wall -Wextra -Werror -ansi -pedantic

CPL	=	gcc

NAME    =	libmy_malloc.so

OBJ	=	$(SRC:.c=.o)

all:	$(NAME)

$(NAME):
	$(CPL) -c -fPIC $(SRC) $(FLAGS)
	$(CPL) -shared -o $(NAME) $(OBJ) -lpthread

clean:
	rm -f $(OBJ)
	rm -f *~

fclean: clean
	rm -f $(NAME)

re:	fclean all
