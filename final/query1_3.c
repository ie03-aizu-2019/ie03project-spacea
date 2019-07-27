///exclude when s or t is equal to 1 or 0
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <omp.h>

#define QUEUE_MAX 30000
//use for route search
#define WHITE 0
#define BLACK 1
#define MAX_INTERSECTION 3000000
#define MAX_ALIGNLIST 10000
#define TRUE 1
#define FALSE -1

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
    float coo[2];
} Intersection;

typedef struct
{
    char ID[10];
    //float coo[2];

    int status;
    float dist;
    char from_ID[10];
} Node;

typedef struct
{
    char node[2][10];
    float cost; //length between 2 nodes
    int is_exist;
} Edge;

typedef struct
{
    int alignpoints[MAX_ALIGNLIST];
    float dist_list[MAX_ALIGNLIST];
} Align;

typedef struct Link
{
    struct Link *next;
    int edge_number;
    char tonode[10];
} LinkedList;

//task5 & 6---------------//
typedef struct
{
    char ID[10];
    float dist;
} Queuedist;

typedef struct
{
    char routeIDs[2000][10];
    int routecount;
    float dist;
} Queueroute;
//task5&6-------------------//
/*
typedef struct
{
    char id;
    float coo[30][2];
    float dist[30];
} Segments;
*/

void deter(Connection c[], Point p[], int numline);
// void sort(int low, int high, Intersection aa[]);
// void merge(int low, int mid, int high, Intersection aa[]);
void mergesort(Intersection A[], int left, int right);
void deleteSameIntersection(Intersection *);

void writefile_inter(char *filename, char *otherfilename, Point p[], Intersection inter[], Connection c[]); //writing file
void writefile_newp(char *filename, char *otherfilename, char *otherfilename2, Point p[], Point new_p[], Intersection split_p[], Connection c[]);
void writefile_highway(char *filename, char *otherfilename, char *otherfilename2, Point p[], Intersection inter[], Connection c[]);

//-------make nodes and edges-----//
void makeEdges();
void sortAlign(Align *, int, int);

int isSameEdgeExist(char *, char *);
void addLinkedList(char *, char *, int);
void freeLinkedList();

void makeGraph(Point p[], Intersection inter[], int k, int n);
//--------------------------------//
//task7--------------------------//
void newroad(Connection c[], Point p[], Point new_p[], int num_new_p, int numline);
//task3,4------------------------//
void resetNodeStatus();
void searchRoute(char *, char *);
int getNodeindex(char *);
void showRoute(char *);

//Priority queue//
void pushDist(char *, float);
void Qd_MinHeapify();
Queuedist popDist();
//-----------------------------//

//task5 & 6-----------------------//
void pushRoute(char *);
Queueroute popRoute();
int getEdgeindex(char *, char *);
int isSameRoute(Queueroute, Queueroute, int);
void searchK_route(char *, char *, int);
//--------------------------------//

//task8----------------------------//
void searchHighways();
int isRouteExist(char *, char *);
//---------------------------------//
Point *p, *new_p;
Intersection *split_p;
Connection *c, *highways;
Intersection *inter;
Intersection *tmp_inter; //data memory used for merge sort
Node *nodes;
Edge *edges;
LinkedList **linkedlist;
int intersectionnumber; // intersection number
int edgenumber;
int nodenumber;
int N, M, P, Q;
//task5--------//
int k_short;
Queuedist *qd;
Queueroute *qr;
Queueroute *k_route;
int qd_tale, qr_tale;
//------------//
int number_of_highway;
int *tmp_highway;
//interface----//
int option;

int main()
{
    double st, et; //for checking runtime
    int i, j;      //loop variable
    clock_t start, end;

    //////interface
    int firsttime = 1;
    char again[4];
    int intype; //for data input type (by hand or file)
    FILE *fp;
    char filename[100], datac;
    int datan;   //read from file to get number of characters input
    char *datas; //data in string format for file input
    ///////

    //task4
    char str_from[10], str_to[10];
    int from_index, to_index;

    //interface: need to 1.add function to case, 2.add remind for inputdata, 3.add input part to each functions.
    while (1)
    {
        if (firsttime != 1)
        {
            printf("Do you want to do it again?(yes/no): ");
            scanf("%s", &again);
            for (int i = 0; again[i]; i++)
            {
                again[i] = tolower(again[i]);
            }
            char no[4] = "no";
            if (strcmp(again, "no") == 0 || strcmp(again, "n") == 0)
            {
                return 0;
            }
        }
        firsttime = 0;
        printf("What kind of function do you want to use?\n");
        printf("Input the number of the function.\n");
        printf("1------------------------------------shortest path\n");
        printf("2-----optimal road construction to the new points.\n");
        printf("3-------------------------------highway detection.\n");
        printf("0--------------------------------------------quit.\n");

        printf("\n(Please refer to https://github.com/ie03-aizu-2019/ie03project-spacea\nfor how to use and more detail.)\n\n");
        printf("Option selection:");
        scanf("%d", &option);
        /*
    printf("Input format:\n\n");
    printf("----------------------\n");
    printf("N M P Q\n");
    printf("x_1 y_1\nx_2 y_2\n:\nx_n y_n\n");
    printf("b_1 e_1\nb_2 e_2\n:\nb_m e_m\n");
    printf("new_x_1 new_y_1\nnew_x_2 new_y_2\n:\nnew_x_p new_y_p\n");
    printf("s_1 d_1 k_1\ns_2 d_2 k_2\n:\ns_q d_q k_q\n");
    printf("-----------------------\n\n");
    printf("N is number of locations.\n");
    printf("M is number of roads.\n");
    printf("P is number of new locations.\n");
    printf("Q is number of path queries.\n");
    printf("From x_1 y_1 to x_n y_n are coordinates of locations\n");
    printf("From b_1 e_1 to b_m e_m are start and end location IDs of line segments\n");
    printf("From new_x_1 new_y_1 to new_x_p new_y_p are coordinates of new locations\n");
    printf("From s_1 d_1 k_1 to s_q d_q k_q are start and end location IDs and number of k-th shortest paths for query.\n\nfile");
//	printf("please input you data first\n");
//	printf("The first line contains the number of locations N, the number of roads M, the number of additional locations P, and the number of path queries Q separated by a space.\n");
 */
        switch (option)
        {
        case 1:
            printf("Finding shortest paths.\n");
            //Segments *segments;
            printf("Input data by hand or file?\n");
            printf("1. File input\n");
            printf("2. Hand input\n:");
            scanf("%d", &intype);
            if (intype == 1)
            {
                printf("Enter the file name:");
                scanf("%100s", filename);
                printf("\n");
                fp = fopen(filename, "r");
                if (fp == NULL)
                {
                    printf("error: file not found.\n");
                    continue;
                }
                fscanf(fp, "%d%d%d%d", &N, &M, &P, &Q);
                // printf("%d %d %d %d",N,M,P,Q);

                //make array
                p = (Point *)malloc(sizeof(Point) * N);
                if (p == NULL)
                {
                    printf("Error 0: Coundn't make p array\n");
                    exit(0);
                }
                c = (Connection *)malloc(sizeof(Connection) * M);
                if (c == NULL)
                {
                    printf("Error 1: Coundn't make c array\n");
                    exit(1);
                }
                inter = (Intersection *)malloc(sizeof(Intersection) * MAX_INTERSECTION);
                if (inter == NULL)
                {
                    printf("Error 2: Coundn't make inter array\n");
                    exit(2);
                }
                tmp_inter = (Intersection *)malloc(sizeof(Intersection) * MAX_INTERSECTION); //use for merge sort
                if (tmp_inter == NULL)
                {
                    printf("Error 3: Coundn't make tmp_inter array\n");
                    exit(3);
                }
                for (i = 0; i < N; i++)
                {
                    fscanf(fp, "%d%d", &p[i].coo[0], &p[i].coo[1]);
                    p[i].identifer = i + 1;
                }
                for (i = 0; i < M; i++)
                {
                    // printf("Line number %d: ",i+1);
                    fscanf(fp, "%d%d", &c[i].connect[0], &c[i].connect[1]);
                }
            }
            else if (intype == 2)
            {
                scanf("%d %d %d %d", &N, &M, &P, &Q);
                //make array
                p = (Point *)malloc(sizeof(Point) * N);
                if (p == NULL)
                {
                    printf("Error 0: Coundn't make p array\n");
                    exit(0);
                }
                c = (Connection *)malloc(sizeof(Connection) * M);
                if (c == NULL)
                {
                    printf("Error 1: Coundn't make c array\n");
                    exit(1);
                }
                inter = (Intersection *)malloc(sizeof(Intersection) * MAX_INTERSECTION);
                if (inter == NULL)
                {
                    printf("Error 2: Coundn't make inter array\n");
                    exit(2);
                }
                tmp_inter = (Intersection *)malloc(sizeof(Intersection) * MAX_INTERSECTION); //use for merge sort
                if (tmp_inter == NULL)
                {
                    printf("Error 3: Coundn't make tmp_inter array\n");
                    exit(3);
                }
                // float segments[M][30][2];
                //segments = (Segments *)malloc(sizeof(Segments) * M);

                //input data
                for (i = 0; i < N; i++)
                {
                    // printf("Coordinate number %d: ",i+1);
                    scanf("%d %d", &p[i].coo[0], &p[i].coo[1]);
                    p[i].identifer = i + 1;
                }

                for (i = 0; i < M; i++)
                {
                    // printf("Line number %d: ",i+1);
                    scanf("%d %d", &c[i].connect[0], &c[i].connect[1]);
                }
            }
            start = clock();

            //calc intersection
            deter(c, p, M);
            writefile_inter("../testdata/dwrite.txt", "../testdata/dwrite2.txt", p, inter, c);
            // end = clock();
            // printf("Process time %.8f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);
            //make graph
            printf("Making graph...");
            makeGraph(p, inter, intersectionnumber, N); // (point, intersection, num_intersection, num_point)
            makeEdges();
            system("python3 mat.py ../testdata/dwrite.txt ../testdata/dwrite2.txt");

            //search route
            if (intype == 1)
            {
                for (i = 0; i < Q; i++)
                {
                    // printf("[start point] [destination] [number of route]: ");
                    fscanf(fp, "%s%s%d", str_from, str_to, &k_short);
                    searchK_route(str_from, str_to, k_short);
                }
            }
            else if (intype == 2)
            {
                for (i = 0; i < Q; i++)
                {
                    // printf("[start point] [destination] [number of route]: ");
                    scanf("%s %s %d", str_from, str_to, &k_short);
                    searchK_route(str_from, str_to, k_short);
                }
            }
            free(p);
            free(c);
            free(inter);
            free(nodes);
            free(edges);
            freeLinkedList();
            end = clock();
            printf("Process time %.8f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);

            printf("end of the program\n\n");
            break;
        case 2:
            printf("Finding point to construct new road.\n");
            printf("Input data by hand or file?\n");
            printf("1. File input\n");
            printf("2. Hand input\n:");
            scanf("%d", &intype);
            //Segments *segments;
            // printf("[number of point] [number of line] [number of new point] [number of queries for asking shortest routes]:\n");
            if (intype == 1)
            {
                printf("Enter the file name:");
                scanf("%100s", filename);
                printf("\n");
                fp = fopen(filename, "r");
                if (fp == NULL)
                {
                    printf("error: file not found.\n");
                    continue;
                }
                fscanf(fp, "%d%d%d%d", &N, &M, &P, &Q);
                // //make array
                p = (Point *)malloc(sizeof(Point) * N);
                if (p == NULL)
                {
                    printf("Error 0: Coundn't make p array\n");
                    exit(0);
                }
                c = (Connection *)malloc(sizeof(Connection) * M);
                if (c == NULL)
                {
                    printf("Error 1: Coundn't make c array\n");
                    exit(1);
                }
                inter = (Intersection *)malloc(sizeof(Intersection) * MAX_INTERSECTION);
                if (inter == NULL)
                {
                    printf("Error 2: Coundn't make inter array\n");
                    exit(2);
                }
                new_p = (Point *)malloc(sizeof(Point) * P);
                split_p = (Intersection *)malloc(P * sizeof(Intersection));
                if (new_p == NULL)
                {
                    printf("Error 4: Coundn't make new_p array\n");
                    exit(4);
                }
                //input data
                for (i = 0; i < N; i++)
                {
                    // printf("Coordinate number %d: ",i+1);
                    fscanf(fp, "%d%d", &p[i].coo[0], &p[i].coo[1]);
                    p[i].identifer = i + 1;
                }

                for (i = 0; i < M; i++)
                {
                    // printf("Line number %d: ",i+1);
                    fscanf(fp, "%d%d", &c[i].connect[0], &c[i].connect[1]);
                }
                for (i = 0; i < P; i++)
                {
                    // printf("New coordinate number %d: ",i+1);
                    fscanf(fp, "%d%d", &new_p[i].coo[0], &new_p[i].coo[1]);
                }
            }
            else if (intype == 2)
            {
                scanf("%d %d %d %d", &N, &M, &P, &Q);
                // //make array
                p = (Point *)malloc(sizeof(Point) * N);
                if (p == NULL)
                {
                    printf("Error 0: Coundn't make p array\n");
                    exit(0);
                }
                c = (Connection *)malloc(sizeof(Connection) * M);
                if (c == NULL)
                {
                    printf("Error 1: Coundn't make c array\n");
                    exit(1);
                }
                inter = (Intersection *)malloc(sizeof(Intersection) * MAX_INTERSECTION);
                if (inter == NULL)
                {
                    printf("Error 2: Coundn't make inter array\n");
                    exit(2);
                }
                new_p = (Point *)malloc(sizeof(Point) * P);
                if (new_p == NULL)
                {
                    printf("Error 4: Coundn't make new_p array\n");
                    exit(4);
                }
                //input data
                for (i = 0; i < N; i++)
                {
                    // printf("Coordinate number %d: ",i+1);
                    fscanf(fp, "%d%d", &p[i].coo[0], &p[i].coo[1]);
                    p[i].identifer = i + 1;
                }

                for (i = 0; i < M; i++)
                {
                    // printf("Line number %d: ",i+1);
                    fscanf(fp, "%d%d", &c[i].connect[0], &c[i].connect[1]);
                }
                for (i = 0; i < P; i++)
                {
                    // printf("New coordinate number %d: ",i+1);
                    fscanf(fp, "%d%d", &new_p[i].coo[0], &new_p[i].coo[1]);
                }
                //input data
                for (i = 0; i < N; i++)
                {
                    printf("Coordinate number %d: ", i + 1);
                    scanf("%d %d", &p[i].coo[0], &p[i].coo[1]);
                    p[i].identifer = i + 1;
                }

                for (i = 0; i < M; i++)
                {
                    printf("Line number %d: ", i + 1);
                    scanf("%d %d", &c[i].connect[0], &c[i].connect[1]);
                }
                for (i = 0; i < P; i++)
                {
                    printf("New coordinate number %d: ", i + 1);
                    scanf("%d %d", &new_p[i].coo[0], &new_p[i].coo[1]);
                }
            }
            start = clock();

            newroad(c, p, new_p, P, M);
            writefile_newp("../testdata/dwrite.txt", "../testdata/dwrite2.txt", "../testdata/dwrite3.txt", p, new_p, split_p, c);
            system("python3 mat.py ../testdata/dwrite.txt ../testdata/dwrite2.txt ../testdata/dwrite3.txt 0");
            free(p);
            free(new_p);
            free(split_p);
            free(c);
            end = clock();
            printf("Process time %.8f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);

            printf("end of the program\n\n");
            break;
        case 3:
            printf("Detecting Highways.\n");
            printf("Input data by hand or file?\n");
            printf("1. File input\n");
            printf("2. Hand input\n:");
            scanf("%d", &intype);
            //Segments *segments;
            // printf("[number of point] [number of line] [number of new point] [number of queries for asking shortest routes]:\n");
            if (intype == 1)
            {
                printf("Enter the file name:");
                scanf("%100s", filename);
                printf("\n");
                fp = fopen(filename, "r");
                if (fp == NULL)
                {
                    printf("error: file not found.\n");
                    continue;
                }
                fscanf(fp, "%d%d%d%d", &N, &M, &P, &Q);
                //make array
                p = (Point *)malloc(sizeof(Point) * N);
                if (p == NULL)
                {
                    printf("Error 0: Coundn't make p array\n");
                    exit(0);
                }
                c = (Connection *)malloc(sizeof(Connection) * M);
                if (c == NULL)
                {
                    printf("Error 1: Coundn't make c array\n");
                    exit(1);
                }
                inter = (Intersection *)malloc(sizeof(Intersection) * MAX_INTERSECTION);
                if (inter == NULL)
                {
                    printf("Error 2: Coundn't make inter array\n");
                    exit(2);
                }
                tmp_inter = (Intersection *)malloc(sizeof(Intersection) * MAX_INTERSECTION); //use for merge sort
                if (tmp_inter == NULL)
                {
                    printf("Error 3: Coundn't make tmp_inter array\n");
                    exit(3);
                }
                //input data
                for (i = 0; i < N; i++)
                {
                    // printf("Coordinate number %d: ",i+1);
                    fscanf(fp, "%d%d", &p[i].coo[0], &p[i].coo[1]);
                    p[i].identifer = i + 1;
                }

                for (i = 0; i < M; i++)
                {
                    // printf("Line number %d: ",i+1);
                    fscanf(fp, "%d%d", &c[i].connect[0], &c[i].connect[1]);
                }
            }
            else if (intype == 2)
            {
                scanf("%d %d %d %d", &N, &M, &P, &Q);

                //make array
                p = (Point *)malloc(sizeof(Point) * N);
                if (p == NULL)
                {
                    printf("Error 0: Coundn't make p array\n");
                    exit(0);
                }
                c = (Connection *)malloc(sizeof(Connection) * M);
                if (c == NULL)
                {
                    printf("Error 1: Coundn't make c array\n");
                    exit(1);
                }
                inter = (Intersection *)malloc(sizeof(Intersection) * MAX_INTERSECTION);
                if (inter == NULL)
                {
                    printf("Error 2: Coundn't make inter array\n");
                    exit(2);
                }
                tmp_inter = (Intersection *)malloc(sizeof(Intersection) * MAX_INTERSECTION); //use for merge sort
                if (tmp_inter == NULL)
                {
                    printf("Error 3: Coundn't make tmp_inter array\n");
                    exit(3);
                }
                // float segments[M][30][2];
                //segments = (Segments *)malloc(sizeof(Segments) * M);

                //input data
                for (i = 0; i < N; i++)
                {
                    printf("Coordinate number %d: ", i + 1);
                    scanf("%d %d", &p[i].coo[0], &p[i].coo[1]);
                    p[i].identifer = i + 1;
                }

                for (i = 0; i < M; i++)
                {
                    printf("Line number %d: ", i + 1);
                    scanf("%d %d", &c[i].connect[0], &c[i].connect[1]);
                }
            }
            start = clock();

            //calc intersection
            deter(c, p, M);
            // writefile_inter("../testdata/dwrite.txt","../testdata/dwrite2.txt",p,inter,c);
            //make graph
            makeGraph(p, inter, intersectionnumber, N); // (point, intersection, num_intersection, num_point)
            makeEdges();

            //search Highway
            searchHighways();
            writefile_highway("../testdata/dwrite.txt", "../testdata/dwrite2.txt", "../testdata/dwrite3.txt", p, inter, c);
            system("python3 mat.py ../testdata/dwrite.txt ../testdata/dwrite2.txt ../testdata/dwrite3.txt 1");
            free(p);
            free(c);
            free(inter);
            free(nodes);
            free(edges);
            freeLinkedList();
            end = clock();
            printf("Process time %.8f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);

            printf("end of the program\n\n");
            break;
        case 0:
            return 0;
        }
    }
}

void deter(Connection c[], Point p[], int numline)
{
    int combi, i, j, k = 0, n;
    // int ***lines; //[linenumber][point][coordinate]
    int n_p = 10; //max number of point on a segment
    float dist1, dist2, tmpd1, tmpd2, tmpx, tmpy;
    float det, s, t;
    int flag;
    for (i = 0; i < numline; i++)
    {
        for (j = i + 1; j < numline; j++)
        {
            //printf("%d, %d\n", i, j);
            //easy to see
            int xp1, yp1, xp2, yp2, xq1, yq1, xq2, yq2;
            xp1 = p[c[i].connect[0] - 1].coo[0];
            yp1 = p[c[i].connect[0] - 1].coo[1];
            xq1 = p[c[i].connect[1] - 1].coo[0];
            yq1 = p[c[i].connect[1] - 1].coo[1];
            xp2 = p[c[j].connect[0] - 1].coo[0];
            yp2 = p[c[j].connect[0] - 1].coo[1];
            xq2 = p[c[j].connect[1] - 1].coo[0];
            yq2 = p[c[j].connect[1] - 1].coo[1];

            det = (xq1 - xp1) * (yp2 - yq2) + (xq2 - xp2) * (yq1 - yp1);
            ////////////////
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
                        //printf("%d\n", k);
                        int tempx = xp2 + (xq2 - xp2) * t;
                        int tempy = yp1 + (yq1 - yp1) * s;

                        inter[k].coo[0] = xp2 + (xq2 - xp2) * t;
                        inter[k].coo[1] = yp1 + (yq1 - yp1) * s;
                        //printf("found\n");
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
    if (k != 0)
    {
        intersectionnumber = k;

        mergesort(inter, 0, k - 1);
        free(tmp_inter);
        deleteSameIntersection(inter);
        // printf("intersection\n");
        for (i = 0; i < intersectionnumber; i++)
        {
            printf("C%d, (%f, %f) \n", inter[i].ID, inter[i].coo[0], inter[i].coo[1]);
        }
        // printf("\n");
    }
    else
    {
        printf("%d,NA3\n", k);
        intersectionnumber = 0;
    }
}
void mergesort(Intersection A[], int left, int right)
{
    int i, j, k, mid;
    if (right > left)
    {
        mid = (right + left) / 2; /*DIVIDE*/
        mergesort(A, left, mid);
        mergesort(A, mid + 1, right);
        for (i = left; i <= mid; i++)
        {
            tmp_inter[i] = A[i];
            // printf("A1: %f, %f\n",A[i].coo[0],A[i].coo[1]);
        }

        for (i = mid + 1, j = right; i <= right; i++, j--)
        {
            tmp_inter[i] = A[j];
            // printf("A2: %f, %f\n",A[j].coo[0],A[j].coo[1]);
        }
        i = left;
        j = right; /*CONQUER*/
        for (k = left; k <= right; k++)
        {
            if (tmp_inter[i].coo[0] < tmp_inter[j].coo[0])
                A[k] = tmp_inter[i++];
            else if (tmp_inter[i].coo[0] == tmp_inter[j].coo[0] && tmp_inter[i].coo[1] < tmp_inter[j].coo[1])
                A[k] = tmp_inter[i++];
            else
                A[k] = tmp_inter[j--];
        }
    }
}

void deleteSameIntersection(Intersection *inter)
{
    int i, delnum;
    Intersection temp;
    temp = inter[0];
    delnum = 0;
    for (i = 1; i < intersectionnumber; i++)
    {
        if (temp.coo[0] == inter[i].coo[0] && temp.coo[1] == inter[i].coo[1])
        {
            delnum++;
        }
        else
        {
            inter[i - delnum] = inter[i];
            temp = inter[i];
        }
    }

    intersectionnumber = intersectionnumber - delnum;

    for (i = 0; i < intersectionnumber; i++)
    {
        inter[i].ID = i + 1;
    }
}
void makeEdges()
{
    int *allignpoints;
    int i, j, l, flag;
    int alignnumber, edgeindex = 0;
    float ax, ay, bx, by, px, py;
    float dist1, dist2, dist3;
    char tmpstr[2][10];

    Align List;

    //always number of edge < M + P + k*2 (maybe it's wrong...)
    edges = (Edge *)malloc(sizeof(Edge) * (M + N + intersectionnumber * 2));
    if (edges == NULL)
    {
        printf("Error 5: Coundn't make edges array\n");
        exit(5);
    }
    linkedlist = (LinkedList **)malloc(sizeof(LinkedList *) * (N + intersectionnumber));
    if (linkedlist == NULL)
    {
        printf("Error 6: Coundn't make linkedlist array\n");
        exit(6);
    }

    for (i = 0; i < N + intersectionnumber; i++)
    {
        linkedlist[i] = NULL;
    }

    //check all lines
    for (i = 0; i < M; i++)
    {
        // printf("%d\n", i);
        for (j = 0; j < MAX_ALIGNLIST; j++)
        {
            //reset
            List.alignpoints[j] = 0;
            List.dist_list[j] = 0;
        }

        List.alignpoints[0] = c[i].connect[0];
        List.alignpoints[1] = c[i].connect[1];

        alignnumber = 2;

        ax = p[c[i].connect[0] - 1].coo[0];
        ay = p[c[i].connect[0] - 1].coo[1];
        bx = p[c[i].connect[1] - 1].coo[0];
        by = p[c[i].connect[1] - 1].coo[1];

        dist3 = sqrt(pow(bx - ax, 2) + pow(by - ay, 2));
        List.dist_list[1] = dist3;

        //for all intersections
        for (j = 0; j < intersectionnumber; j++)
        {
            //To see earily, save point coordinate
            px = inter[j].coo[0];
            py = inter[j].coo[1];

            //printf("A %d, B %d, P %d\n", c[i].connect[0], c[i].connect[1], p[j].identifer);
            //printf("(%f, %f) (%f, %f) (%f, %f)\n", ax, ay, bx, by, px, py);

            //check whether point is on the line[i] or not
            dist1 = sqrt(pow(px - ax, 2) + pow(py - ay, 2));
            dist2 = sqrt(pow(px - bx, 2) + pow(py - by, 2));

            if ((dist1 + dist2 - dist3) < pow(10, -5))
            {
                //on the line
                //dist1 = sqrt(pow(p[allignpoints[0] - 1].coo[0] - px, 2) + pow(p[allignpoints[0] - 1].coo[1] - py, 2));

                List.alignpoints[alignnumber] = -inter[j].ID;
                List.dist_list[alignnumber++] = dist1;
            }
        }

        //check point
        //for all points
        for (j = 0; j < N; j++)
        {
            //point is used for the line[i]
            if (p[j].identifer == c[i].connect[0] || p[j].identifer == c[i].connect[1])
            {
                continue;
            }
            //To see earily, save point coordinate
            px = p[j].coo[0];
            py = p[j].coo[1];

            //printf("A %d, B %d, P %d\n", c[i].connect[0], c[i].connect[1], p[j].identifer);
            //printf("(%f, %f) (%f, %f) (%f, %f)\n", ax, ay, bx, by, px, py);

            //check whether point is on the line[i] or not
            dist1 = sqrt(pow(px - ax, 2) + pow(py - ay, 2));
            dist2 = sqrt(pow(px - bx, 2) + pow(py - by, 2));

            if ((dist1 + dist2 - dist3) < pow(10, -5))
            {
                //on the line
                //dist1 = sqrt(pow(p[allignpoints[0] - 1].coo[0] - px, 2) + pow(p[allignpoints[0] - 1].coo[1] - py, 2));

                if ((dist1 + dist2 - dist3) < pow(10, -5))
                {
                    //on the line
                    //dist1 = sqrt(pow(p[allignpoints[0] - 1].coo[0] - px, 2) + pow(p[allignpoints[0] - 1].coo[1] - py, 2));

                    List.alignpoints[alignnumber] = p[j].identifer;
                    List.dist_list[alignnumber++] = dist1;
                }
            }
        }
        /*
        printf("{");
        for (j = 0; j < alignnumber; j++)
        {
            //positive number -> point ID
            //negative number -> intersection ID
            printf(" (%d, %f) ", List.alignpoints[j], List.dist_list[j]);
        }
        printf("}\n");
        printf("align number = %d\n", alignnumber);
        */

        //Sort List
        //printf("sort\n");
        //printf("%d \n", alignnumber);
        sortAlign(&List, 0, alignnumber - 1);

        // printf("end\n");
        // printf("{");
        // for (j = 0; j < alignnumber; j++)
        // {
        //     //positive number -> point ID
        //     //negative number -> intersection ID
        //     printf(" (%d, %f) ", List.alignpoints[j], List.dist_list[j]);
        // }
        // printf("}\n");

        //make edges
        for (j = 0; j < alignnumber - 1; j++)
        {

            if (List.alignpoints[j] > 0)
            {
                sprintf(tmpstr[0], "%d", List.alignpoints[j]);
                ax = p[List.alignpoints[j] - 1].coo[0];
                ay = p[List.alignpoints[j] - 1].coo[1];
            }
            else
            {
                sprintf(tmpstr[0], "C%d", -List.alignpoints[j]);
                ax = inter[-List.alignpoints[j] - 1].coo[0];
                ay = inter[-List.alignpoints[j] - 1].coo[1];
            }

            if (List.alignpoints[j + 1] > 0)
            {
                sprintf(tmpstr[1], "%d", List.alignpoints[j + 1]);
                bx = p[List.alignpoints[j + 1] - 1].coo[0];
                by = p[List.alignpoints[j + 1] - 1].coo[1];
            }
            else
            {
                sprintf(tmpstr[1], "C%d", -List.alignpoints[j + 1]);
                bx = inter[-List.alignpoints[j + 1] - 1].coo[0];
                by = inter[-List.alignpoints[j + 1] - 1].coo[1];
            }

            //check whether edge is already exist.

            if (isSameEdgeExist(tmpstr[0], tmpstr[1]) != FALSE)
            {
                continue;
            }

            //save edges
            strcpy(edges[edgeindex].node[0], tmpstr[0]);
            strcpy(edges[edgeindex].node[1], tmpstr[1]);
            edges[edgeindex].cost = List.dist_list[j + 1] - List.dist_list[j];
            edges[edgeindex].is_exist = TRUE;
            addLinkedList(tmpstr[0], tmpstr[1], edgeindex);
            addLinkedList(tmpstr[1], tmpstr[0], edgeindex);
            //printf("{%s, %s, %f}\n", edges[edgeindex].node[0], edges[edgeindex].node[1], edges[edgeindex].cost);
            edgeindex++;
        }
    }
    edgenumber = edgeindex;
    // printf("edgenumber %d\n", edgenumber);

    // for (i = 0; i < edgenumber; i++)
    // {
    //     printf("edge[%d] {%s, %s, %f}\n", i, edges[i].node[0], edges[i].node[1], edges[i].cost);
    // }
}

void sortAlign(Align *list, int left, int right)
{
    int i = left;
    int j = right;
    int temp_p;
    float temp_d;
    float pivot;
    /*
    printf("{");
    for (int k = 0; k < right; k++)
    {
        //positive number -> point ID
        //negative number -> intersection ID
        printf(" (%d, %f) ", list->alignpoints[k], list->dist_list[k]);
    }
    printf("}\n");
*/

    pivot = list->dist_list[(left + right) / 2];
    //printf("left = %d, right = %d, piv %f\n", left, right, pivot);

    while (1)
    {
        while (list->dist_list[i] < pivot)
        {
            i++;
        }
        while (list->dist_list[j] > pivot)
        {
            j--;
        }
        if (i >= j)
        {
            break;
        }

        //swap
        temp_d = list->dist_list[i];
        list->dist_list[i] = list->dist_list[j];
        list->dist_list[j] = temp_d;

        temp_p = list->alignpoints[i];
        list->alignpoints[i] = list->alignpoints[j];
        list->alignpoints[j] = temp_p;

        i++;
        j--;
    }
    /*
    for (int k = 0; k < right; k++)
    {
        printf("%d ", list->alignpoints[k]);
    }
    printf("\n");
    */
    if (left < right)
    {
        sortAlign(list, left, i - 1);
        sortAlign(list, j + 1, right);
    }
}

int isSameEdgeExist(char *from, char *to)
{
    int from_index = getNodeindex(from);
    LinkedList *l;

    for (l = linkedlist[from_index]; l != NULL; l = l->next)
    {
        if (strcmp(l->tonode, to) == 0)
        {
            return l->edge_number;
        }
    }
    return FALSE;
}

void addLinkedList(char *node1, char *node2, int edgenumber)
{
    int from_index = getNodeindex(node1);
    LinkedList *l, *new;

    new = (LinkedList *)malloc(sizeof(LinkedList));
    if (linkedlist == NULL)
    {
        printf("Error 7: Coundn't make new array\n");
        exit(7);
    }
    strcpy(new->tonode, node2);
    new->edge_number = edgenumber;
    new->next = NULL;

    if (linkedlist[from_index] == NULL)
    {
        linkedlist[from_index] = new;
    }
    else
    {
        l = linkedlist[from_index];
        while (l->next != NULL)
        {
            l = l->next;
        }
        l->next = new;
    }
}

void freeLinkedList()
{
    LinkedList *p, *q;
    for (int i = 0; i < N + intersectionnumber; i++)
    {
        for (p = linkedlist[i]; p != NULL;)
        {
            q = p->next;
            free(p);
            p = q;
        }
    }
    free(linkedlist);
}

void makeGraph(Point p[], Intersection inter[], int k, int n)
{
    int i, j;
    int len_node = n + intersectionnumber;
    //Node *nodes; -> global variable
    nodes = (Node *)malloc(sizeof(Node) * len_node);
    if (nodes == NULL)
    {
        printf("Error 8: Coundn't make nodes array\n");
        exit(8);
    }

    // printf("%d, %d, %d\n",n,k,len_node);
    for (i = 0; i < n; i++)
    {
        sprintf(nodes[i].ID, "%d", i + 1);
        //nodes[i].coo[0] = p[i].coo[0];
        //nodes[i].coo[1] = p[i].coo[1];
    }
    for (j = 0; j < intersectionnumber; i++, j++)
    {
        sprintf(nodes[i].ID, "C%d", j + 1);
        //nodes[i].coo[0] = inter[j].coo[0];
        //nodes[i].coo[1] = inter[j].coo[1];
    }

    nodenumber = len_node;
    // for (i = 0; i < nodenumber; i++)
    // {
    //     printf("node[%d] %s\n", i, nodes[i].ID); // nodes[i].coo[0], nodes[i].coo[1]);
    // }
    // printf("nodes: %d\n\n", nodenumber);
}

void resetNodeStatus()
{
    for (int i = 0; i < nodenumber; i++)
    {
        nodes[i].dist = powf(10, 10);
        nodes[i].status = WHITE;
        strcpy(nodes[i].from_ID, "0");
        //printf("%s reset\n", nodes[i].ID);
    }
}

//Base: Dijkstra
void searchRoute(char *from, char *to)
{
    float shortest_dist = powf(10, 10);
    int from_index, to_index, index;
    int flag;
    int i, j;
    Queuedist q;
    LinkedList *l;
    qd = (Queuedist *)malloc(sizeof(Queuedist) * QUEUE_MAX);
    if (qd == NULL)
    {
        printf("Error 9: Coundn't make qd array\n");
        exit(9);
    }
    qd_tale = 0;
    from_index = getNodeindex(from);

    pushDist(nodes[from_index].ID, nodes[from_index].dist);

    //printf("search %s %s\n", from, to);

    while (qd_tale != 0)
    {

        q = popDist();

        //printf("%f\n", q.dist);
        from_index = getNodeindex(q.ID);

        if (nodes[from_index].status == BLACK)
        {
            //already know shortest distance
            //printf("continue\n");
            continue;
        }
        nodes[from_index].status = BLACK;

        if (strcmp(nodes[from_index].ID, to) == 0)
        {
            break;
        }

        //printf("from %s, to %s\n", nodes[from_index].ID, to);

        //search node which has connection with from node
        for (l = linkedlist[from_index]; l != NULL; l = l->next)
        {
            i = l->edge_number;
            if (edges[i].is_exist == FALSE)
            {
                //printf("edge[%d] FALSE\n", i);
                continue;
            }

            flag = 0;
            if (strcmp(nodes[from_index].ID, edges[i].node[0]) == 0)
            {
                flag = 1;
                index = getNodeindex(edges[i].node[1]);
            }
            else if (strcmp(nodes[from_index].ID, edges[i].node[1]) == 0)
            {
                flag = 2;
                index = getNodeindex(edges[i].node[0]);
            }

            if (flag)
            {
                //has connection
                //printf("%f, %f\n", nodes[index].dist, nodes[from_index].dist + edges[i].cost);
                if (nodes[index].dist > nodes[from_index].dist + edges[i].cost)
                {
                    //update shortest dist
                    nodes[index].dist = nodes[from_index].dist + edges[i].cost;
                    strcpy(nodes[index].from_ID, nodes[from_index].ID);

                    pushDist(nodes[index].ID, nodes[index].dist);
                }
            }
        }
    }

    pushRoute(to);

    free(qd);
}

int getNodeindex(char *nodeid)
{
    int i;
    int index = 0;

    if (nodeid[0] == 'C')
    {
        //intersection node
        for (i = 1; nodeid[i] != '\0'; i++)
        {
            index = 10 * index + (nodeid[i] - '0');
        }
        if (N + index - 1 < nodenumber)
        {
            return N + index - 1;
        }
        else
        {
            return -1;
        }
    }
    else
    {
        //point node
        for (i = 0; nodeid[i] != '\0'; i++)
        {
            index = 10 * index + (nodeid[i] - '0');
        }
        if (index - 1 < nodenumber)
        {
            return index - 1;
        }
        else
        {
            return -1;
        }
    }
}

void pushDist(char *nodeid, float dist)
{
    int n = qd_tale;
    int i;
    Queuedist tmp;

    strcpy(qd[qd_tale].ID, nodeid);
    qd[qd_tale].dist = dist;
    // printf("push qd[%d] %s, %f\n", qd_tale, qd[qd_taleyes].ID, qd[qd_tale].dist);
    qd_tale++;

    while (n != 0)
    {
        i = (n - 1) / 2;
        if (qd[n].dist < qd[i].dist)
        {
            tmp = qd[n];
            qd[n] = qd[i];
            qd[i] = tmp;
        }
        else if (qd[n].dist == qd[i].dist)
        {
            if (qd[n].ID < qd[i].ID)
            {
                tmp = qd[n];
                qd[n] = qd[i];
                qd[i] = tmp;
            }
        }
        n = i;
    }
    // for (i = 0; i < qd_tale; i++)
    // {
    //     printf("%f, %s\n", qd[i].dist, qd[i].ID);
    // }
    // printf("\n");
}

Queuedist popDist()
{
    int n, i;
    Queuedist searchdata, tmp;

    searchdata = qd[0];
    // printf("pop qd[0] %s, %f\n", qd[0].ID, qd[0].dist);

    qd[0] = qd[qd_tale - 1];
    qd_tale--;
    n = qd_tale;

    while (n != 0)
    {
        i = (n - 1) / 2;
        if (qd[n].dist < qd[i].dist)
        {
            tmp = qd[n];
            qd[n] = qd[i];
            qd[i] = tmp;
        }
        else if (qd[n].dist == qd[i].dist)
        {
            if (qd[n].ID > qd[i].ID)
            {
                tmp = qd[n];
                qd[n] = qd[i];
                qd[i] = tmp;
            }
        }
        n = i;
    }
    // for (i = 0; i < qd_tale; i++)
    // {
    //     printf("%f, %s\n", qd[i].dist, qd[i].ID);
    // }
    // printf("\n");

    return searchdata;
}

void showRoute(char *goalID)
{
    int index = getNodeindex(goalID);
    if (strcmp(nodes[index].from_ID, "0") != 0)
    {
        showRoute(nodes[index].from_ID);
    }
    printf("%8s ", nodes[index].ID);
}

void pushRoute(char *goalID)
{
    int index, num = 1;
    int i, n = qr_tale;
    Queueroute tmp;

    index = getNodeindex(goalID);
    qr[qr_tale].dist = nodes[index].dist;
    while (strcmp(nodes[index].from_ID, "0") != 0)
    {
        num++;
        index = getNodeindex(nodes[index].from_ID);
    }

    qr[qr_tale].routecount = num;
    index = getNodeindex(goalID);

    for (i = num - 1; i >= 0; i--)
    {
        strcpy(qr[qr_tale].routeIDs[i], nodes[index].ID);
        index = getNodeindex(nodes[index].from_ID);
    }

    while (n != 0)
    {
        i = (n - 1) / 2;
        if (qr[n].dist < qr[i].dist)
        {
            tmp = qr[n];
            qr[n] = qr[i];
            qr[i] = tmp;
        }
        n = i;
    }
    qr_tale++;

    // for (int j = 0; j < qr_tale-1; j++)
    // {
    //     printf("qr[%d] dist %f\n", j, qr[j].dist);
    //     for (i = 0; i < num; i++)
    //     {
    //         printf("%s ", qr[j].routeIDs[i]);
    //     }
    //     printf("\n");
    // }
}

Queueroute popRoute()
{
    int min, i, n;
    Queueroute searchdata, tmp;

    searchdata = qr[0];

    qr[0] = qr[qr_tale-1];
    qr_tale--;
    n = qr_tale;

    while (n != 0)
    {
        i = (n - 1) / 2;
        if (qr[n].dist < qr[i].dist)
        {
            tmp = qr[n];
            qr[n] = qr[i];
            qr[i] = tmp;
        }
        n = i;
    }

    return searchdata;
}

int getEdgeindex(char *node1, char *node2)
{
    return isSameEdgeExist(node1, node2);
}

int isSameRoute(Queueroute q1, Queueroute q2, int checknum)
{
    int i;
    for (i = 0; i < checknum; i++)
    {
        if (strcmp(q1.routeIDs[i], q2.routeIDs[i]) != 0)
        {
            //printf("false\n");
            return FALSE;
        }
    }
    //printf("true\n");
    return TRUE;
}

void searchK_route(char *from, char *to, int k_num)
{
    int i, j, k; //loop variable
    int from_index, to_index;
    int edgeindex, tmpedgecount;
    int tmpedgelist[150], k_route_issame[15];
    float tmpdist;

    from_index = getNodeindex(from);
    to_index = getNodeindex(to);

    printf("Search from: %s to: %s\n", from, to);

    if (from_index != -1 && to_index != -1)
    {
        qr = (Queueroute *)malloc(sizeof(Queueroute) * QUEUE_MAX);

        if (qr == NULL)
        {
            printf("Error 10: Coundn't make qr array\n");
            exit(10);
        }
        k_route = (Queueroute *)malloc(sizeof(Queueroute) * k_num);
        if (k_route == NULL)
        {
            printf("Error 11: Coundn't make k_route array\n");
            exit(11);
        }

        //printf("search 1 shortest\n");
        resetNodeStatus();
        qr_tale = 0;

        //search 1st shortst route
        nodes[from_index].dist = 0;
        searchRoute(from, to);
        //input 1st shortest to k_route[0]
        k_route[0] = popRoute();
        if (k_route[0].routecount <= 1)
        {
            printf("NA\n\n");
            free(qr);
            free(k_route);

            return;
        }

        printf("No.1 shortest\ndistance: %f\nRoute\n", k_route[0].dist);
        for (j = 0; j < k_route[0].routecount; j++)
        {
            printf("%8s", k_route[0].routeIDs[j]);
            if (j % 15 == 14)
            {
                printf("\n");
            }
        }
        printf("\n");

        tmpdist = k_route[0].dist;

        for (i = 1; i < k_num; i++)
        {
            /*
            printf("%d search start\npreviousroute\n", i + 1);
            for (j = 0; j < i; j++)
            {
                for (k = 0; k < k_route[j].routecount; k++)
                {
                    printf("%s ", k_route[j].routeIDs[k]);
                }
                printf("\n");
            }
            */

            tmpedgecount = 0;

            for (j = 0; j < i; j++)
            {
                k_route_issame[j] = TRUE;
                //printf("%d reset is same\n", k);
            }

            for (j = 1; j < k_route[i - 1].routecount; j++)
            {
                //printf("j = %d\n", j);
                //for all k_shortest
                for (k = 0; k < i; k++)
                {
                    if (k_route_issame[k] == TRUE && isSameRoute(k_route[i - 1], k_route[k], j) == TRUE)
                    {
                        edgeindex = getEdgeindex(k_route[k].routeIDs[j - 1], k_route[k].routeIDs[j]);
                        if (edges[edgeindex].is_exist == FALSE)
                        {
                            continue;
                        }
                        edges[edgeindex].is_exist = FALSE;
                        tmpedgelist[tmpedgecount] = edgeindex;
                        tmpedgecount++;
                        //printf("edge[%d] {%s, %s, %f} false\n", edgeindex, edges[edgeindex].node[0], edges[edgeindex].node[1], edges[edgeindex].cost);
                    }
                    else
                    {
                        k_route_issame[k] = FALSE;
                        //printf("k_route_issame[%d] false\n", k);
                    }
                }

                resetNodeStatus();
                nodes[from_index].dist = 0;
                for (k = 0; k < j - 1; k++)
                {

                    int nodeindex1 = getNodeindex(k_route[i - 1].routeIDs[k]);
                    int nodeindex2 = getNodeindex(k_route[i - 1].routeIDs[k + 1]);
                    int edgeindex2 = getEdgeindex(k_route[i - 1].routeIDs[k], k_route[i - 1].routeIDs[k + 1]);

                    nodes[nodeindex1].status = BLACK;
                    strcpy(nodes[nodeindex2].from_ID, nodes[nodeindex1].ID);
                    nodes[nodeindex2].dist = nodes[nodeindex1].dist + edges[edgeindex2].cost;
                }

                searchRoute(k_route[i - 1].routeIDs[j - 1], to);
                for (k = 0; k < tmpedgecount; k++)
                {
                    edges[tmpedgelist[k]].is_exist = TRUE;
                    //printf("edge[%d] {%s, %s, %f} true\n", tmpedgelist[k], edges[tmpedgelist[k]].node[0], edges[tmpedgelist[k]].node[1], edges[tmpedgelist[k]].cost);
                }
                tmpedgecount--;
            }

            while (qr_tale != 0)
            {
                k_route[i] = popRoute();
                if (k_route[i].dist > tmpdist)
                {
                    break;
                }
            }
            //show route

            if (k_route[i].routecount <= 1)
            {
                //printf("no route\n");
                break;
            }

            printf("No.%d shortest\ndistance: %f\nRoute\n", i + 1, k_route[i].dist);
            for (j = 0; j < k_route[i].routecount; j++)
            {
                printf("%8s", k_route[i].routeIDs[j]);
                if (j % 15 == 14)
                {
                    printf("\n");
                }
            }
            printf("\n");
            tmpdist = k_route[i].dist;
        }
        free(qr);
        free(k_route);
    }
    else
    {
        printf("NA\n");
    }
    printf("\n");

    return;
}

void searchHighways()
{
    int i, from_index, highwaynumber = 0;
    Queueroute q;

    qr = (Queueroute *)malloc(sizeof(Queueroute) * 10);
    if (qr == NULL)
    {
        printf("Error 9: Coundn't make qd array\n");
        exit(9);
    }
    qr_tale = 0;
    tmp_highway = (int *)malloc(sizeof(int) * edgenumber);
    if (tmp_highway == NULL)
    {
        printf("Error 9: Coundn't make tmp_highway array\n");
        exit(11);
    }
    for (i = 0; i < edgenumber; i++)
    {
        // printf("%d\n", i);
        edges[i].is_exist = FALSE;
        //printf("edge[%d] {%s, %s, %f} false\n", i, edges[i].node[0], edges[i].node[1], edges[i].cost);
        //printf("search {%s, %s} \n", edges[i].node[0], edges[i].node[1]);
        resetNodeStatus();

        // from_index = getNodeindex(edges[i].node[0]);
        // nodes[from_index].dist = 0;

        // searchRoute(edges[i].node[0], edges[i].node[1]);
        // q = popRoute();
        if (qr == NULL)
        {
            printf("Error 9: Coundn't make qd array\n");
            exit(9);
        }
        if (isRouteExist(edges[i].node[0], edges[i].node[1]) == FALSE)
        {
            printf("Highway[%d](%s , %s)\n", highwaynumber, edges[i].node[0], edges[i].node[1]);
            tmp_highway[highwaynumber] = i;
            highwaynumber++;
        }

        number_of_highway = highwaynumber;

        edges[i].is_exist = TRUE;
    }

    free(qr);
}

int isRouteExist(char *from, char *to)
{
    int from_index, to_index, index;
    int i, j;
    int flag;
    int *cirqueue;
    int c_h = 0, c_t = 0;
    LinkedList *l;

    cirqueue = (int *)malloc(sizeof(int) * QUEUE_MAX);
    if (cirqueue == NULL)
    {
        printf("Error 11: Coundn't make cirqueue array\n");
        exit(11);
    }
    from_index = getNodeindex(from);

    cirqueue[c_t] = from_index;
    c_t++;
    //printf("search %s %s\n", from, to);

    while (c_h != c_t)
    {

        from_index = cirqueue[c_h];
        c_h = (c_h + 1) % QUEUE_MAX;
        //printf("%f\n", q.dist);

        if (nodes[from_index].status == BLACK)
        {
            //already know shortest distance
            //printf("continue\n");
            continue;
        }
        nodes[from_index].status = BLACK;

        if (strcmp(nodes[from_index].ID, to) == 0)
        {

            free(cirqueue);
            return TRUE;
        }

        //printf("from %s, to %s\n", nodes[from_index].ID, to);

        //search node which has connection with from node
        for (l = linkedlist[from_index]; l != NULL; l = l->next)
        {
            i = l->edge_number;
            if (edges[i].is_exist == FALSE)
            {
                //printf("edge[%d] FALSE\n", i);
                continue;
            }

            if (strcmp(nodes[from_index].ID, edges[i].node[0]) == 0)
            {
                index = getNodeindex(edges[i].node[1]);
            }
            else
            {
                index = getNodeindex(edges[i].node[0]);
            }

            cirqueue[c_t] = index;
            c_t = (c_t + 1) % QUEUE_MAX;
        }
    }
    free(cirqueue);
    return FALSE;
}

void newroad(Connection c[], Point p[], Point new_p[], int num_new_p, int numline)
{
    int combi, i, j, m, n;

    float dist1, dist2, tmpd = 0, tmpd1, tmpd2, tmpx = 0, tmpy = 0;
    float det, s, t;
    if (num_new_p > 0)
    {
        for (i = 0; i < num_new_p; i++)
        {
            int new_x, new_y;
            tmpd = 0;
            tmpx = 0;
            tmpy = 0;
            new_x = new_p[i].coo[0];
            new_y = new_p[i].coo[1];
            new_p[i].identifer = i + 1;
            for (j = 0; j < numline; j++)
            {
                //easy to see
                int xp1, yp1, xq1, yq1;
                xp1 = p[c[j].connect[0] - 1].coo[0];
                yp1 = p[c[j].connect[0] - 1].coo[1];
                xq1 = p[c[j].connect[1] - 1].coo[0];
                yq1 = p[c[j].connect[1] - 1].coo[1];

                t = -(((xp1 - new_x) * (xq1 - xp1) + (yp1 - new_y) * (yq1 - yp1)) / (pow(xq1 - xp1, 2) + pow(yq1 - yp1, 2)));
                if (t >= 0 && t <= 1)
                {
                    dist1 = abs((xq1 - xp1) * (yp1 - new_y) - (yq1 - yp1) * (xp1 - new_x)) / sqrt(pow(xq1 - xp1, 2) + pow(yq1 - yp1, 2));
                    if (tmpd == 0)
                    {
                        tmpd = dist1;
                        tmpx = xp1 + (xq1 - xp1) * t;
                        tmpy = yp1 + (yq1 - yp1) * t;
                    }
                    else if (tmpd > dist1)
                    {
                        tmpd = dist1;
                        tmpx = xp1 + (xq1 - xp1) * t;
                        tmpy = yp1 + (yq1 - yp1) * t;
                    }
                }
                else
                {
                    dist1 = sqrt(pow(xp1 - new_x, 2) + pow(yp1 - new_y, 2));
                    dist2 = sqrt(pow(xq1 - new_x, 2) + pow(yq1 - new_y, 2));
                    if (tmpd == 0)
                    {
                        if (dist1 < dist2)
                        {
                            tmpd = dist1;
                            tmpx = xp1;
                            tmpy = yp1;
                        }
                        else
                        {
                            tmpd = dist2;
                            tmpx = xq1;
                            tmpy = yq1;
                        }
                    }
                    else if (tmpd > dist1 || tmpd > dist2)
                    {
                        if (dist1 < dist2)
                        {
                            tmpd = dist1;
                            tmpx = xp1;
                            tmpy = yp1;
                        }
                        else
                        {
                            tmpd = dist2;
                            tmpx = xq1;
                            tmpy = yq1;
                        }
                    }
                }
            }
            printf("%f, %f, dist = %f\n", tmpx, tmpy, tmpd);
            split_p[i].ID = i + 1;
            split_p[i].coo[0] = tmpx;
            split_p[i].coo[1] = tmpy;
        }
    }
    else
    {
        printf("Please check data input again\n\n");
    }
}
void writefile_inter(char *filename, char *otherfilename, Point p[], Intersection inter[], Connection c[])
{
    int i;
    // create a FILE typed pointer
    FILE *file_pointer, *file_pointer2;

    // open the file "name_of_file.txt" for writing
    file_pointer = fopen(filename, "w");

    // Write to the file
    for (i = 0; i < N; i++)
    {
        fprintf(file_pointer, "%d %d %d\n", p[i].identifer, p[i].coo[0], p[i].coo[1]);
    }
    for (i = 0; i < intersectionnumber; i++)
    {
        fprintf(file_pointer, "C%d %f %f\n", inter[i].ID, inter[i].coo[0], inter[i].coo[1]);
    }
    file_pointer2 = fopen(otherfilename, "w");
    for (i = 0; i < M; i++)
    {
        fprintf(file_pointer2, "%d %d\n", c[i].connect[0], c[i].connect[1]);
    }

    // Close the file
    fclose(file_pointer);
    fclose(file_pointer2);
}
void writefile_newp(char *filename, char *otherfilename, char *otherfilename2, Point p[], Point new_p[], Intersection split_p[], Connection c[])
{
    int i;
    // create a FILE typed pointer
    FILE *file_pointer, *file_pointer2, *file_pointer3;

    // open the file "name_of_file.txt" for writing
    file_pointer = fopen(filename, "w");

    // Write to the file
    for (i = 0; i < N; i++)
    {
        fprintf(file_pointer, "%d %d %d\n", p[i].identifer, p[i].coo[0], p[i].coo[1]);
    }
    for (i = 0; i < P; i++)
    {
        fprintf(file_pointer, "N%d %d %d\n", new_p[i].identifer, new_p[i].coo[0], new_p[i].coo[1]);
    }
    for (i = 0; i < P; i++)
    {
        fprintf(file_pointer, "K%d %f %f\n", split_p[i].ID, split_p[i].coo[0], split_p[i].coo[1]);
    }
    file_pointer2 = fopen(otherfilename, "w");
    for (i = 0; i < M; i++)
    {
        fprintf(file_pointer2, "%d %d\n", c[i].connect[0], c[i].connect[1]);
    }
    file_pointer3 = fopen(otherfilename2, "w");
    for (i = 0; i < P; i++)
    {
        fprintf(file_pointer3, "N%d K%d\n", i + 1, i + 1);
        printf("N%d K%d\n", i + 1, i + 1);
    }

    // Close the file
    fclose(file_pointer);
    fclose(file_pointer2);
    fclose(file_pointer3);
}
void writefile_highway(char *filename, char *otherfilename, char *otherfilename2, Point p[], Intersection inter[], Connection c[])
{
    int i;
    // create a FILE typed pointer
    FILE *file_pointer, *file_pointer2, *file_pointer3;

    // open the file "name_of_file.txt" for writing
    file_pointer = fopen(filename, "w");

    // Write to the file
    for (i = 0; i < N; i++)
    {
        fprintf(file_pointer, "%d %d %d\n", p[i].identifer, p[i].coo[0], p[i].coo[1]);
    }
    for (i = 0; i < intersectionnumber; i++)
    {
        fprintf(file_pointer, "C%d %f %f\n", inter[i].ID, inter[i].coo[0], inter[i].coo[1]);
    }
    file_pointer2 = fopen(otherfilename, "w");
    for (i = 0; i < M; i++)
    {
        fprintf(file_pointer2, "%d %d\n", c[i].connect[0], c[i].connect[1]);
    }
    file_pointer3 = fopen(otherfilename2, "w");
    for (i = 0; i < number_of_highway; i++)
    {
        fprintf(file_pointer3, "%s %s\n", edges[tmp_highway[i]].node[0], edges[tmp_highway[i]].node[1]);
    }
    // for(int j = 0; j < number_of_highway; j++){
    //     printf("tmp Highway[%d](%s , %s)\n", j, edges[tmp_highway[j]].node[0], edges[tmp_highway[j]].node[1]);
    // }
    // Close the file
    fclose(file_pointer);
    fclose(file_pointer2);
    fclose(file_pointer3);
}