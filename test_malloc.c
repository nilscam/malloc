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

s_box *ramdom_free(s_box *list)
{
    int i = 0;
    int r = rand() % size_list(list);
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

s_box    *clear_list(s_box *list)
{
    s_box *tmp = list;

    while(size_list(list) > 0)
        list = ramdom_free(list);
    return(list);
}

void tester(void)
{
    s_box   *list = NULL;
    s_box   *tmp;

    write(1,"start malloc\n", 13);
    while (cmpt_size < 1000000){
        tmp = init_box();
        tmp->next = list;
        list = tmp;
    }
    write(1,"start clear\n", 12);
    list = clear_list(list);

}

int main()
{
    srand(time(NULL));
    tester();
    return 0;
}