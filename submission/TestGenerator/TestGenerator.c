#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define TRUE 1
#define FALSE 0
#define HASH_SIZE 4096

typedef struct List
{
    struct List *next;
    int x;
    int y;
} List;

typedef struct
{
    int identifer;
    int coo[2]; //coordinate[x,y]
} Point;

typedef struct
{
    int connect[2]; //idenfiter [0] and [1] are connected
} Connection;

void deter(Connection c[], Point p[], int numline);
int combination(int, int);

/* hash */
int Hash(int, int);
int searchVal(List **, int, int);
void freeHashTable(List **);
/*      */

Point *p;
Connection *c;
int intersectionnumber;

int main()
{
    FILE *f;
    char filename[100], str[10];
    int MAX_COO, N, M, P, Q;

    int X, Y, temp, temp2, i, hashval;
    List **hashtable, **hashtable2;
    List *lp, *lq;
    clock_t start, end, temp_t, temp2_t;

    printf("Write file name\n");
    scanf("%s", filename);
    strcat(filename, ".txt");

    f = fopen(filename, "w");

    if (f == NULL)
    {
        fscanf(stderr, "Could not open the file.");
        exit(1);
    }

    while (1)
    {
        printf("Define the maximum coordinate x&y(<= 1000000)\n");
        scanf("%d", &MAX_COO);
        if (MAX_COO <= 1000000)
        {
            break;
        }
        printf("The number is too big\n");
        printf("Please write smaller number\n");
    }

    MAX_COO++;

    if (MAX_COO <= 447)
    {
        temp = MAX_COO * MAX_COO;
    }
    else
    {
        temp = 2 * 100000;
    }

    while (1)
    {
        printf("Write number of N (N <= %d)\n", temp);
        scanf("%d", &N);
        if (N <= temp)
        {
            break;
        }
        printf("The number of N is too big\n");
        printf("Please write smaller number\n");
    }
    fprintf(f, "%d ", N);

    if (N <= 447)
    {
        temp2 = combination(N, 2);
    }
    else
    {
        temp2 = 100000;
    }

    while (1)
    {
        printf("Write number of M (M <= %d)\n", temp2);
        scanf("%d", &M);
        if (M <= temp2)
        {
            break;
        }
        printf("The number of N is too big\n");
        printf("Please write smaller number\n");
    }
    fprintf(f, "%d ", M);

    if (MAX_COO <= 447)
    {
        temp2 = temp - N;
    }
    else if (MAX_COO == 448)
    {
        temp2 = 200704 - N;
        if (temp2 > 1000)
        {
            temp2 = 1000;
        }
    }
    else
    {
        temp2 = 1000;
    }

    if (temp2 < 0)
    {
        temp2 = 0;
    }

    while (1)
    {
        printf("Write number of P (P <= %d)\n", temp2);
        scanf("%d", &P);
        if (P <= temp2)
        {
            break;
        }
        printf("The number is too big\n");
        printf("Please write smaller number\n");
    }

    fprintf(f, "%d ", P);
    printf("Write number of Q\n");
    scanf("%d", &Q);
    fprintf(f, "%d\n", Q);

    start = clock();

    //make array
    p = (Point *)malloc(sizeof(Point) * N);
    c = (Connection *)malloc(sizeof(Connection) * M);

    //Reset rand seed
    srand((unsigned int)time(NULL));

    printf("Randomizing coordinate X and Y ...\n");

    hashtable = (List **)malloc(sizeof(List *) * HASH_SIZE);
    for (i = 0; i < HASH_SIZE; i++)
    {
        hashtable[i] = NULL;
    }

    i = 0;
    while (i < N)
    {
        X = (rand() * rand()) % MAX_COO;
        Y = (rand() * rand()) % MAX_COO;

        if (searchVal(hashtable, X, Y) == FALSE)
        {
            fprintf(f, "%d ", X);
            fprintf(f, "%d\n", Y);

            p[i].coo[0] = X;
            p[i].coo[1] = Y;
            p[i].identifer = i + 1;

            hashval = Hash(X, Y);

            lp = (List *)malloc(sizeof(List));
            lp->x = X;
            lp->y = Y;
            lp->next = NULL;

            if (hashtable[hashval] == NULL)
            {
                hashtable[hashval] = lp;
            }
            else
            {
                lq = hashtable[hashval];
                while (lq->next != NULL)
                {
                    lq = lq->next;
                }
                lq->next = lp;
                lp->next = NULL;
            }

            i++;
        }
        else
        {
            continue;
        }
    }

    if (P == 0)
    {
        freeHashTable(hashtable);
    }
    printf("done\n");

    printf("Randomizing connection A and B ...\n");

    hashtable2 = (List **)malloc(sizeof(List *) * HASH_SIZE);
    for (i = 0; i < HASH_SIZE; i++)
    {
        hashtable2[i] = NULL;
    }

    i = 0;
    while (i < M)
    {
        X = 1 + (rand() * rand()) % N;
        for (Y = 1 + (rand() * rand()) % N; Y == X; Y = 1 + (rand() * rand()) % N)
        {
        }

        if (X > Y)
        {
            temp = X;
            X = Y;
            Y = temp;
        }

        if (searchVal(hashtable2, X, Y) == FALSE)
        {
            fprintf(f, "%d ", X);
            fprintf(f, "%d\n", Y);
            c[i].connect[0] = X;
            c[i].connect[1] = Y;

            hashval = Hash(X, Y);
            lp = (List *)malloc(sizeof(List));
            lp->x = X;
            lp->y = Y;
            lp->next = NULL;

            if (hashtable2[hashval] == NULL)
            {
                hashtable2[hashval] = lp;
            }
            else
            {
                lq = hashtable2[hashval];
                while (lq->next != NULL)
                {
                    lq = lq->next;
                }
                lq->next = lp;
                lp->next = NULL;
            }

            i++;
        }
        else
        {
            continue;
        }
    }
    freeHashTable(hashtable2);

    printf("done\n");
    printf("Checking number of intersection\n");
    deter(c, p, M);
    printf("Number of intersection is %d\n", intersectionnumber);
    if (P != 0)
    {
        printf("Making request of new location\n");
        i = 0;
        while (i < P)
        {
            X = (rand() * rand()) % MAX_COO;
            Y = (rand() * rand()) % MAX_COO;

            if (searchVal(hashtable, X, Y) == FALSE)
            {
                fprintf(f, "%d ", X);
                fprintf(f, "%d\n", Y);

                p[i].coo[0] = X;
                p[i].coo[1] = Y;
                p[i].identifer = i + 1;

                hashval = Hash(X, Y);

                lp = (List *)malloc(sizeof(List));
                lp->x = X;
                lp->y = Y;
                lp->next = NULL;

                if (hashtable[hashval] == NULL)
                {
                    hashtable[hashval] = lp;
                }
                else
                {
                    lq = hashtable[hashval];
                    while (lq->next != NULL)
                    {
                        lq = lq->next;
                    }
                    lq->next = lp;
                    lp->next = NULL;
                }

                i++;
            }
            else
            {
                continue;
            }
        }
        freeHashTable(hashtable);

        printf("done\n");
    }
    if (Q != 0)
    {
        printf("Making request of shortest route\n");

        temp_t = clock();
        hashtable2 = (List **)malloc(sizeof(List *) * HASH_SIZE);
        for (i = 0; i < HASH_SIZE; i++)
        {
            hashtable2[i] = NULL;
        }

        i = 0;

        temp_t = clock();
        temp2_t = clock();
        while (i < Q)
        {
            temp2_t = clock();
            if ((double)(temp2_t - temp_t) / CLOCKS_PER_SEC > 5)
            {
                printf("Error: Couldn't make request of shortest route\n");
                printf("Probably the number of Q is too big\n");
                exit(0);
            }

            X = 1 + (rand() * rand()) % (N + intersectionnumber - 1);
            for (Y = 1 + (rand() * rand()) % (N + intersectionnumber - 1); Y == X; Y = 1 + (rand() * rand()) % (N + intersectionnumber - 1))
            {
            }

            if (X > Y)
            {
                temp = X;
                X = Y;
                Y = temp;
            }

            if (searchVal(hashtable2, X, Y) == FALSE)
            {
                if (X < N)
                {
                    sprintf(str, "%d", X);
                    fprintf(f, "%s ", str);
                }
                else
                {
                    sprintf(str, "%d", 1 + X - N);
                    fprintf(f, "C%s ", str);
                }

                if (Y < N)
                {
                    sprintf(str, "%d", Y);
                    fprintf(f, "%s ", str);
                }
                else
                {
                    sprintf(str, "%d", 1 + Y - N);
                    fprintf(f, "C%s ", str);
                }

                fprintf(f, "%d\n", 1 + rand() % 10);

                hashval = Hash(X, Y);
                lp = (List *)malloc(sizeof(List));
                lp->x = X;
                lp->y = Y;
                lp->next = NULL;

                if (hashtable2[hashval] == NULL)
                {
                    hashtable2[hashval] = lp;
                }
                else
                {
                    lq = hashtable2[hashval];
                    while (lq->next != NULL)
                    {
                        lq = lq->next;
                    }
                    lq->next = lp;
                    lp->next = NULL;
                }

                i++;
                temp = clock();
            }
            else
            {
                continue;
            }
        }
        freeHashTable(hashtable2);

        printf("done\n");
    }
    printf("Making file is succeeded\n");
    end = clock();
    printf("Process time %.8f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);

    fclose(f);

    return 0;
}

void deter(Connection c[], Point p[], int numline)
{
    int combi, i, j, k = 0, m, n;
    int ***lines; //[linenumber][point][coordinate]
    int n_p = 10; //max number of point on a segment
    float dist1, dist2, tmpd1, tmpd2, tmpx, tmpy;
    float det, s, t;
    int flag;

    //-------------------------------------------------------//
    //detected warnings in my laptop so I changed to make arrays
    lines = (int ***)malloc(sizeof(int **) * numline);
    for (i = 0; i < numline; i++)
    {
        lines[i] = (int **)malloc(sizeof(int *) * 2);
        for (j = 0; j < 2; j++)
        {
            lines[i][j] = (int *)malloc(sizeof(int) * 2);
        }
    }

    //make lines
    for (i = 0; i < numline; i++)
    {
        lines[i][0][0] = p[c[i].connect[0] - 1].coo[0];
        lines[i][0][1] = p[c[i].connect[0] - 1].coo[1];
        lines[i][1][0] = p[c[i].connect[1] - 1].coo[0];
        lines[i][1][1] = p[c[i].connect[1] - 1].coo[1];

        //printf("%d, %d, %d, %d\n",lines[i][0][0], lines[i][0][1],lines[i][1][0],lines[i][1][1]);
    }
    for (i = 0; i < numline; i++)
    {
        m = 0;
        for (j = i + 1; j < numline; j++)
        {
            //printf("%d, %d\n", i, j);
            //easy to see
            int xp1, yp1, xp2, yp2, xq1, yq1, xq2, yq2;
            xp1 = lines[i][0][0];
            yp1 = lines[i][0][1];
            xq1 = lines[i][1][0];
            yq1 = lines[i][1][1];
            xp2 = lines[j][0][0];
            yp2 = lines[j][0][1];
            xq2 = lines[j][1][0];
            yq2 = lines[j][1][1];
            //printf("(%d,%d)-(%d,%d), (%d,%d)-(%d,%d)\n", xp1, yp1, xq1, yq1, xp2, yp2, xq2, yq2);

            det = (xq1 - xp1) * (yp2 - yq2) + (xq2 - xp2) * (yq1 - yp1);
            //printf("%f\n",det);
            ////////////////
            //det[0] = (lines[0][1][0]-lines[0][0][0])*(lines[1][0][1]-lines[1][1][1])+(lines[1][1][0]-lines[1][0][0])*(lines[0][1][1]-lines[0][0][1]);
            /*
            if (det < 0)
            {
                det *= -1;
            }
            */
            //printf("%f\n",det);
            if (det == 0)
            {
                //printf("NA1\n");
            }
            else
            {
                if (pow(10, -8) <= det && det <= pow(10, -8))
                {
                    //printf("NA1\n");
                }
                else
                {
                    s = ((yp2 - yq2) * (xp2 - xp1) + (xq2 - xp2) * (yp2 - yp1)) / det;
                    t = ((yp1 - yq1) * (xp2 - xp1) + (xq1 - xp1) * (yp2 - yp1)) / det;
                    //printf("%f,%f\n", s, t);

                    if (0 < s && s < 1 && 0 < t && t < 1)
                    {

                        k++;
                    }
                    else
                    {
                        // printf("NA2\n");
                    }
                }
            }
        }
    }
    intersectionnumber = k;
}

int combination(int n, int m)
{
    if (m == 0 || m == n)
        return 1;
    else
        return combination(n - 1, m) + combination(n - 1, m - 1);
}

int Hash(int key1, int key2)
{
    return (key1 + 2 * key2) % HASH_SIZE;
}

int searchVal(List **T, int key1, int key2)
{
    List *p;

    for (p = T[Hash(key1, key2)]; p != NULL; p = p->next)
    {
        if (key1 == p->x && key2 == p->y)
        {
            return TRUE;
        }
    }
    return FALSE;
}

void freeHashTable(List **T)
{
    int i;
    List *p, *q;

    for (i = 0; i < HASH_SIZE; i++)
    {
        for (p = T[i]; p != NULL;)
        {
            q = p->next;
            free(p);
            p = q;
        }
    }
}