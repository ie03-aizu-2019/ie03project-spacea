#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int identifer;
    int c[2]; //coordinate[x,y]
} Point;

typedef struct
{
    int connect[2]; //idenfiter [0] and [1] are connected
} Connection;

Point *p;
Connection *c;

int main()
{
    int N, M, P, Q;
    int i; //loop variable

    scanf("%d %d %d %d", &N, &M, &P, &Q);

    //make array
    p = (Point *)malloc(sizeof(Point) * N);
    c = (Connection *)malloc(sizeof(Connection) * M);

    //input data
    for (i = 0; i < N; i++)
    {
        scanf("%d %d", &p[i].c[0], &p[i].c[1]);
        p[i].identifer = i;
    }

    for (i = 0; i < M; i++)
    {
        scanf("%d %d", &c[i].connect[0], &c[i].connect[1]);
    }

/*  Check whether data is saved

    printf("Point\n");
    for (i = 0; i < N; i++)
    {
        printf("%d, %d, %d\n", p[i].c[0], p[i].c[1], p[i].identifer);
    }
    printf("Connection\n");
    for (i = 0; i < M; i++)
    {
        printf("%d, %d\n", c[i].connect[0], c[i].connect[1]);
    }
*/

/*
    Calculate
*/

    //release mmemories
    free(p);
    free(c);

    return 0;
}