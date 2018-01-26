##
## EPITECH PROJECT, 2018
##
## File description:
##
##

#-ansi -pedantic

SRC	=	malloc.c

FLAGS	=	-W -Wall -Wextra -Werror

CPL	=	gcc

NAME    =	libmy_malloc.so

OBJ	=	$(SRC:.c=.o)

all:	$(NAME)

$(NAME):
	$(CPL) -c -fPIC $(SRC) $(FLAGS)
	$(CPL) -shared -o $(NAME) $(OBJ)

clean:
	rm -f $(OBJ)
	rm -f *~

fclean: clean
	rm $(NAME)

re:	fclean all
