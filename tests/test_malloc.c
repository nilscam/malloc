/*
** EPITECH PROJECT, 2018
** testmalloc
** File description:
** test
*/

#include <time.h>
#include <stdlib.h>
#include <unistd.h>

struct s_box {
    struct s_box *next;
};

typedef struct s_box s_box;

static int  cmpt_size = 0;

s_box     *init_box()
{
    s_box   *box;

    int r = rand() % 1000;
    cmpt_size += r;
    box = malloc(sizeof(box) + r);
    box->next = NULL;
    return(box);
}

int size_list(s_box *list)
{
    return(list ? 1 + size_list(list->next) : 0);
}

s_box *ramdom_free(s_box *list, int size)
{
    int i = 0;
    int r = rand() % size;
    s_box *tmp = list;
    s_box   *prev;

    if (!r) {
        tmp = list->next;
        free(list);
        return(tmp);
    }
    while (i++ < r) {
        prev = tmp;
        tmp = tmp->next;
    }
    prev->next = tmp->next;
    free(tmp);
    return list;
}

s_box    *clear_list(s_box *list, int rest)
{
    s_box *tmp = list;
    int         size = size_list(list);

    while(size > rest) {
	    list = ramdom_free(list, size);
            size -= 1;
    }
    return(list);
}

void tester(void)
{
    s_box   *list = NULL;
    s_box   *tmp;

    write(1,"start malloc\n", 13);
    while (cmpt_size < 10000000) {
	    tmp = init_box();
	    tmp->next = list;
	    list = tmp;
    }
    write(1,"start clear\n", 12);
    list = clear_list(list, 10);
	write(1, "list_cleared 1\n", 16);
	cmpt_size = 0;
	while (cmpt_size < 10000000) {
		tmp = init_box();
		tmp->next = list;
		list = tmp;
	}
	list = clear_list(list, 50);
	cmpt_size = 0;
	write(1, "list_cleared 2\n", 16);
	while (cmpt_size < 100000) {
		tmp = init_box();
		tmp->next = list;
		list = tmp;
	}
	list = clear_list(list, 0);
    write(1, "list_cleared 3\n", 16);
}

int main()
{
   // srand(time(NULL));
    tester();
    return 0;
}
