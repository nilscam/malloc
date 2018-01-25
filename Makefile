##
## EPITECH PROJECT, 2018
##
## File description:
##
##

SRC	=	malloc.c

FLAGS	=	-ansi -pedantic -fpic -c -W -Wall -Wextra -Werror

CPL	=	gcc

NAME    =	libmy_malloc.so

OBJ	=	$(SRC:.c=.o)

all:	$(NAME)

$(NAME):$(OBJ)
	$(CPL) $(OBJ) $(FLAGS)
	$(CPL) -o $(NAME) $(OBJ) -shared

clean:
	rm -f *.o
	rm -f *~

fclean: clean
	rm $(NAME)

re:	fclean all
