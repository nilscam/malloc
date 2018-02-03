##
## EPITECH PROJECT, 2018
##
## File description:
##
##

#-ansi -pedantic

SRC	=	malloc.c	\
		free.c		\
		allocate.c

MODULES	=	btree/btree.c			\
		    manage_heap/manage_heap.c	\
		    manage_heap/debug.c

FLAGS	=	-W -Wall -Wextra -Werror -Wno-unused-result -pedantic -ansi

CPL	=	gcc

CPL_DEBUG   =   gcc -D M_DEBUG

NAME    =	libmy_malloc.so

OBJ	=	$(SRC:%.c=%.o) btree.o manage_heap.o debug.o

all:	$(NAME)

$(NAME):
	$(CPL) -c -fPIC $(SRC) $(MODULES) $(FLAGS)
	$(CPL) -shared -o $(NAME) $(OBJ) -lpthread

debug:
	$(CPL_DEBUG) -c -fPIC $(SRC) $(MODULES) $(FLAGS)
	$(CPL_DEBUG) -shared -o $(NAME) $(OBJ) -lpthread

clean:
	rm -f $(OBJ)
	rm -f *~

fclean: clean
	rm -f $(NAME)

re:	fclean all
