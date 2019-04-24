#include <stdio.h>
#include <stdlib.h>
#include <math.h>
typedef struct
{
    int identifer;
    int coo[2]; //coordinate[x,y]
} Point;

typedef struct
{
    int connect[2]; //idenfiter [0] and [1] are connected
} Connection;

void findout();
void deter(Connection c[], Point p[], int numline);

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
        scanf("%d %d", &p[i].coo[0], &p[i].coo[1]);
        p[i].identifer = i+1;
    }

    for (i = 0; i < M; i++)
    {
        scanf("%d %d", &c[i].connect[0], &c[i].connect[1]);
    }
///////////////////////////////////////////
deter(c,p,2);
///////////////////////////////////////////
//  Check whether data is saved

    // printf("Point\n");
    // for (i = 0; i < N; i++)
    // {
    //     printf("%d, %d, %d\n", p[i].coo[0], p[i].coo[1], p[i].identifer);
    // }
    // printf("Connection\n");
    // for (i = 0; i < M; i++)
    // {
    //     printf("%d, %d\n", c[i].connect[0], c[i].connect[1]);
    // }
/*
    Calculate
*/

    //release mmemories
    free(p);
    free(c);

    return 0;
}


void deter(Connection c[], Point p[], int numline){
    int combi,i, lines[numline][2][2]; //[linenumber][point][coordinate]
    float det,s,t,in_x,in_y;
    //make lines
    while(i<numline){     
        lines[i][0][0] = p[c[i].connect[0]-1].coo[0];
        lines[i][0][1] = p[c[i].connect[0]-1].coo[1];
        lines[i][1][0] = p[c[i].connect[1]-1].coo[0];
        lines[i][1][1] = p[c[i].connect[1]-1].coo[1];
        i++;
    }
    //easy to see
    int xp1, yp1, xp2, yp2, xq1, yq1, xq2, yq2;
    xp1 = lines[0][0][0];
    yp1 = lines[0][0][1];
    xq1 = lines[0][1][0];
    yq1 = lines[0][1][1];
    xp2 = lines[1][0][0];
    yp2 = lines[1][0][1];
    xq2 = lines[1][1][0];
    yq2 = lines[1][1][1];

    det = (xq1-xp1)*(yp2-yq2)+(xq2-xp2)*(yq1-yp1);
    ////////////////
    //det[0] = (lines[0][1][0]-lines[0][0][0])*(lines[1][0][1]-lines[1][1][1])+(lines[1][1][0]-lines[1][0][0])*(lines[0][1][1]-lines[0][0][1]);
    if(det < 0){det*=-1;}

    if(pow(10,-7)<=det && det<=pow(10,-7)){
        printf("NA\n");
    }
    else{
        ///find s = det[1] and t = det[2]
        s = ((yp2-yq2)*(xp2-xp1)+(xq2-xp2)*(yp2-yp1))/det;
        t = ((yp1-yq1)*(xp2-xp1)+(xq1-xp1)*(yp2-yp1))/det;

        if(0<=s && s<=1 && 0<=t && t<=1){
            in_x = xp2+(xq2-xp2)*t;
            in_y = yp2+(yq2-yp2)*t;
            printf("%f, %f\n",in_x,in_y);
        }else{printf("NA\n");}
    }
}
