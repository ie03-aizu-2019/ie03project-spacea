///exclude when s or t is equal to 1 or 0
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

typedef struct
{
    int identifer;
    int coo[2]; //coordinate[x,y]
} Point;

typedef struct
{
    int connect[2]; //idenfiter [0] and [1] are connected
} Connection;

typedef struct
{
    int ID;
    int crossline[2];
    float coo[2];
} Intersection;

typedef struct
{
    char ID[10];
    float coo[2];
} Node;

typedef struct
{
    char node[2][10];
    float cost; //length between 2 nodes
} Edge;
/*
typedef struct
{
    char id;
    float coo[30][2];
    float dist[30];
} Segments;
*/

void deter(Connection c[], Point p[], Point new_p[],int num_new_p, int numline);
// void rearrange(Intersection aa[], int k);

//-----------------------//
void makeEdges();
//-----------------------//
void makeGraph(Point p[], Intersection inter[], int k, int n);
Point *p, *new_p;
Connection *c;
Intersection *inter;
Node *nodes;
Edge *edges;
int k; // intersection number
int edgenumber;
int N, M, P, Q;

int main()
{
    int i; //loop variable
    //Segments *segments;

    scanf("%d %d %d %d", &N, &M, &P, &Q);

    //make array
    p = (Point *)malloc(sizeof(Point) * N);
    c = (Connection *)malloc(sizeof(Connection) * M);
    new_p = (Point *)malloc(sizeof(Point) * P);
    inter = (Intersection *)malloc(sizeof(Intersection) * 50000);
    // float segments[M][30][2];
    //segments = (Segments *)malloc(sizeof(Segments) * M);

    //input data
    for (i = 0; i < N; i++)
    {
        scanf("%d %d", &p[i].coo[0], &p[i].coo[1]);
        p[i].identifer = i + 1;
    }

    for (i = 0; i < M; i++)
    {
        scanf("%d %d", &c[i].connect[0], &c[i].connect[1]);
    }
    for (i = 0; i < P; i++)
    {
        scanf("%d %d", &new_p[i].coo[0], &new_p[i].coo[1]);
    }
    ///////////////////////////////////////////
    deter(c, p,new_p, P, M);
    // makeEdges();
    // makeGraph(p, inter, k, N); // (point, intersection, num_intersection, num_point)
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
    free(inter);
    free(nodes);
    free(edges);
    printf("end of the program\n");
    return 0;
}
void deter(Connection c[], Point p[], Point new_p[],int num_new_p, int numline)
{
    int combi, i, j, m, n;
    int ***lines; //lines[numline][2][2]; //[linenumber][point][coordinate]
    int n_p = 10; //max number of point on a segment
    //float lines1[numline][n_p][2]
    float **dist; //float dist[numline][n_p];
    float dist1, dist2, tmpd = 0,tmpd1, tmpd2, tmpx=0, tmpy=0;
    float det, s, t;

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
    /*
    dist = (float **)malloc(sizeof(float *) * numline);
    for (i = 0; i < numline; i++)
    {
        dist[i] = (float *)malloc(sizeof(float) * n_p);
    }
    */
    //-------------------------------------------------//
    /*
    for (i = 0; i < numline; i++)
    {
        for (j = 0; j < n_p; j++)
        {
            dist[i][j] = 0;
        }
    }*/

    //make lines
    for (i = 0; i < numline; i++)
    {
        lines[i][0][0] = p[c[i].connect[0] - 1].coo[0];
        lines[i][0][1] = p[c[i].connect[0] - 1].coo[1];
        lines[i][1][0] = p[c[i].connect[1] - 1].coo[0];
        lines[i][1][1] = p[c[i].connect[1] - 1].coo[1];

        // printf("%d, %d, %d, %d\n",lines[i][0][0], lines[i][0][1],lines[i][1][0],lines[i][1][1]);

        /*
        lines1[i][0][0] = p[c[i].connect[0] - 1].coo[0];
        lines1[i][0][1] = p[c[i].connect[0] - 1].coo[1];
        lines1[i][1][0] = p[c[i].connect[1] - 1].coo[0];
        lines1[i][1][1] = p[c[i].connect[1] - 1].coo[1];
        */

        /*
        dist[i][0] = sqrt(pow(lines[i][0][0] - lines[i][1][0], 2) + pow(lines[i][0][1] - lines[i][1][1], 2));
        printf("%f  ", dist[i][0]);
        printf("\n");
        */

        // dist[i][0] = pow(3,2);
        // segments[i].coo[0][0] = (float)lines[i][0][0];
        // segments[i].coo[0][1] = (float)lines[i][0][1];
        // segments[i].coo[1][1] = (float)lines[i][1][1];
        // segments[i].coo[1][0] = (float)lines[i][1][0];
    }
    for (i = 0; i < num_new_p; i++)
    {
        int new_x, new_y;
        tmpd = 0;
        tmpx = 0;
        tmpy = 0;
        new_x = new_p[i].coo[0];
        new_y = new_p[i].coo[1];
        for (j = 0; j < numline; j++)
        {
            //easy to see
            int xp1, yp1, xq1, yq1;
            xp1 = lines[j][0][0];
            yp1 = lines[j][0][1];
            xq1 = lines[j][1][0];
            yq1 = lines[j][1][1];


            t = -(((xp1-new_x)*(xq1-xp1)+(yp1-new_y)*(yq1-yp1))/(pow(xq1-xp1,2)+pow(yq1-yp1,2)));
            ////////////////
            //det[0] = (lines[0][1][0]-lines[0][0][0])*(lines[1][0][1]-lines[1][1][1])+(lines[1][1][0]-lines[1][0][0])*(lines[0][1][1]-lines[0][0][1]);
            if (t >= 0 && t <= 1)
            {
                dist1 = abs((xq1-xp1)*(yp1-new_y)-(yq1-yp1)*(xp1-new_x))/sqrt(pow(xq1-xp1,2)+pow(yq1-yp1,2));
                if(tmpd == 0 ){
                    tmpd = dist1;
                    tmpx = xp1+(xq1-xp1)*t;
                    tmpy = yp1+(yq1-yp1)*t;
                    
                }else if(tmpd > dist1){
                    tmpd = dist1;
                    tmpx =xp1+(xq1-xp1)*t;
                    tmpy = yp1+(yq1-yp1)*t;
                    
                }
            }else{
                dist1 = sqrt(pow(xp1-new_x,2)+pow(yp1-new_y,2));
                dist2 = sqrt(pow(xq1-new_x,2)+pow(yq1-new_y,2));
                if(tmpd == 0){
                    if(dist1 < dist2){
                        tmpd = dist1;
                        tmpx = xp1;
                        tmpy = yp1;
                        
                    }else{
                        tmpd = dist2;
                        tmpx = xq1;
                        tmpy = yq1;
                        
                    }
                }
                else if(tmpd > dist1 || tmpd > dist2){
                    if(dist1 < dist2){
                        tmpd = dist1;
                        tmpx = xp1;
                        tmpy = yp1;
                        
                    }else{
                        tmpd = dist2;
                        tmpx = xq1;
                        tmpy = yq1;
                        
                    }
                }
            }
        }
        printf("%f, %f, dist = %f\n",tmpx,tmpy,tmpd);
    }
     
    free(lines);
}