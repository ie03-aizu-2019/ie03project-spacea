#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

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

Point *p;
Connection *c;
int intersectionnumber;

int main()
{
    FILE *f;
    char filename[100], str[10];
    int MAX_COO, N, M, P, Q;
    int i, X, Y;

    printf("Write file name\n");
    scanf("%s", filename);
    strcat(filename, ".txt");

    f = fopen(filename, "w");

    if (f == NULL)
    {
        fscanf(stderr, "Could not open the file.");
        exit(1);
    }

    printf("Define the maximum coordinate x&y\n");
    scanf("%d", &MAX_COO);

    printf("Write number of N\n");
    scanf("%d", &N);
    fprintf(f, "%d ", N);
    printf("Write number of M\n");
    scanf("%d", &M);
    fprintf(f, "%d ", M);
    printf("Write number of P\n");
    scanf("%d", &P);
    fprintf(f, "%d ", P);
    printf("Write number of Q\n");
    scanf("%d", &Q);
    fprintf(f, "%d\n", Q);

    //make array
    p = (Point *)malloc(sizeof(Point) * N);
    c = (Connection *)malloc(sizeof(Connection) * M);

    //Reset rand seed
    srand((unsigned int)time(NULL));

    printf("Randomizing coordinate X and Y ...\n");
    for (i = 0; i < N; i++)
    {
        X = rand() % MAX_COO;
        fprintf(f, "%d ", X);
        p[i].coo[0] = X;

        Y = rand() % MAX_COO;

        while (Y == X)
        {
            Y = rand() % MAX_COO;
        }
        fprintf(f, "%d\n", Y);
        p[i].coo[1] = Y;

        p[i].identifer = i + 1;
    }
    printf("done\n");

    printf("Randomizing connection A and B ...\n");
    for (i = 0; i < M; i++)
    {
        X = 1 + rand() % N;
        fprintf(f, "%d ", X);
        c[i].connect[0] = X;

        Y = 1 + rand() % N;
        while (Y == X)
        {
            Y = 1 + rand() % N;
        }
        fprintf(f, "%d\n", Y);
        c[i].connect[1] = Y;
    }
    printf("done\n");

    deter(c, p, M);
    printf("Number of intersection is %d\n", intersectionnumber);

    printf("Making request of shortest route\n");
    for (i = 0; i < Q; i++)
    {

        Y = rand() % 2;

        if (Y == 0)
        {
            X = 1 + rand() % intersectionnumber;
            sprintf(str, "%d", X);
            fprintf(f, "C%s ", str);
        }
        else
        {
            X = 1 + rand() % N;
            sprintf(str, "%d", X);
            fprintf(f, "%s ", str);
        }

        if (Y == 0)
        {
            X = 1 + rand() % intersectionnumber;
            sprintf(str, "%d", X);
            fprintf(f, "C%s ", str);
        }
        else
        {
            X = 1 + rand() % N;
            sprintf(str, "%d", X);
            fprintf(f, "%s ", str);
        }
        fprintf(f, "1\n");
    }
    printf("done\n");

    printf("Making file is succeeded");

    fclose(f);

    return 0;
}

void deter(Connection c[], Point p[], int numline)
{
    int combi, i, j, lines[numline][2][2]; //[linenumber][point][coordinate]
    float intersection[2];
    float det, s, t;
    //make lines
    for (i = 0; i < numline; i++)
    {
        lines[i][0][0] = p[c[i].connect[0] - 1].coo[0];
        lines[i][0][1] = p[c[i].connect[0] - 1].coo[1];
        lines[i][1][0] = p[c[i].connect[1] - 1].coo[0];
        lines[i][1][1] = p[c[i].connect[1] - 1].coo[1];
    }

    for (i = 0; i < numline; i++)
    {
        for (j = i + 1; j < numline; j++)
        {
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

            /*
            printf("p1x = %d, p1y = %d\nq1x = %d, q1y = %d\np2x = %d, p2y = %d\nq2x = %d, q2y = %d\n\n",
           xp1, yp1, xq1, yq1, xp2, yp2, xq2, yq2);
            */
            det = (xq1 - xp1) * (yp2 - yq2) + (xq2 - xp2) * (yq1 - yp1);

            ////////////////
            //det[0] = (lines[0][1][0]-lines[0][0][0])*(lines[1][0][1]-lines[1][1][1])+(lines[1][1][0]-lines[1][0][0])*(lines[0][1][1]-lines[0][0][1]);
            if (det < 0)
            {
                det *= -1;
            }

            if (pow(10, -7) <= det && det <= pow(10, -7))
            {
                //printf("NA1\n");
            }
            else
            {
                s = ((yp2 - yq2) * (xp2 - xp1) + (xq2 - xp2) * (yp2 - yp1)) / det;
                t = ((yp1 - yq1) * (xp2 - xp1) + (xq1 - xp1) * (yp2 - yp1)) / det;

                if ((0 < s && s < 1) && (0 < t && t < 1))
                {
                    intersection[0] = xp2 + (xq2 - xp2) * t;
                    intersection[1] = yp2 + (yq2 - yp2) * t;
                    intersectionnumber++;
                    //printf("number = %d, x = %f, y = %f, \n", intersectionnumber, intersection[0], intersection[1]);
                }
                else
                {
                    //printf("NA2\n");
                }
            }
        }
    }
    /*
    if (sizeof(intersection) != 0)
    {
        for (i = 0; i < k; i++)
        {
            printf("%f, %f\n", intersection[i][0], intersection[i][1]);
        }
    }
    else
    {
        printf("NA\n");
    }
    */
}