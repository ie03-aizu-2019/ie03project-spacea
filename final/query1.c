///exclude when s or t is equal to 1 or 0
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

#define QUEUE_MAX 10000
//use for route search
#define WHITE 0
#define BLACK 1

#define TRUE 1
#define FALSE 0

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
    int alignpoints[1000];
    float dist_list[1000];
} Align;

//task5 & 6---------------//
typedef struct
{
    char ID[10];
    float dist;
} Queuedist;

typedef struct
{
    char routeIDs[200][10];
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
void mergesort(Intersection A[],int left,int right);

//-------make nodes and edges-----//
void makeEdges();
void sortAlign(Align *, int, int);

void makeGraph(Point p[], Intersection inter[], int k, int n);
//--------------------------------//
//task7--------------------------//
void newroad(Connection c[], Point p[], Point new_p[],int num_new_p, int numline);
//task3,4------------------------//
void resetNodeStatus();
void searchRoute(char *, char *);
int getNodeindex(char *);
void showRoute(char *);

//Priority queue//
void pushDist(char *, float);
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
//---------------------------------//
Point *p, *new_p;
Connection *c;
Intersection *inter;
Intersection *tmp_inter; //data memory used for merge sort
Node *nodes;
Edge *edges;
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

//interface----//
int option;

int main()
{
    int i, j; //loop variable

    //////interface
    int firsttime = 1;
    char again[4];
    int intype; //for data input type (by hand or file)
    FILE *fp;
    char filename[100],datac;
    int datan; //read from file to get number of characters input
    char *datas; //data in string format for file input
    ///////

    //task4
    char str_from[10], str_to[10];
    int from_index, to_index;

	//interface: need to 1.add function to case, 2.add remind for inputdata, 3.add input part to each functions.
while(1){
    if(firsttime != 1){
        printf("Do you want to do it again?(yes/no): ");
        scanf("%s",&again);
        for(int i = 0; again[i]; i++){
            again[i] = tolower(again[i]);
        }
        char no[4] = "no";
        if(strcmp(again,"no") == 0 ||strcmp(again,"n") == 0){
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
	scanf("%d",&option);
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
	switch(option){
		case 1:
            printf("Finding shortest paths.\n");
            //Segments *segments;
            printf("Input data by hand or file?\n");
            printf("1. File input\n");
            printf("2. Hand input\n:");
            scanf("%d",&intype);
            if(intype == 1){
                printf("Enter the file name:");
                scanf("%100s",filename);
                printf("\n");
                fp = fopen(filename,"r");
                if(fp == NULL){
                    printf("error: file not found.\n");
                    exit(1);
                }
                fscanf(fp,"%d%d%d%d",&N,&M,&P,&Q);
                // printf("%d %d %d %d",N,M,P,Q);

                //make array
                p = (Point *)malloc(sizeof(Point) * N);
                c = (Connection *)malloc(sizeof(Connection) * M);
                inter = (Intersection *)malloc(sizeof(Intersection) * 50000);
                tmp_inter = (Intersection *)malloc(sizeof(Intersection) * 50000); //use for merge sort
                for (i = 0; i < N; i++)
                {
                    fscanf(fp,"%d%d", &p[i].coo[0],&p[i].coo[1]);
                    p[i].identifer = i + 1;
                }
                for (i = 0; i < M; i++)
                {
                    // printf("Line number %d: ",i+1);
                    fscanf(fp,"%d%d", &c[i].connect[0], &c[i].connect[1]);
                }
            }else if(intype == 2){
                scanf("%d %d %d %d", &N, &M, &P, &Q);
                //make array
                p = (Point *)malloc(sizeof(Point) * N);
                c = (Connection *)malloc(sizeof(Connection) * M);
                inter = (Intersection *)malloc(sizeof(Intersection) * 50000);
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
            //calc intersection
            deter(c, p, M);

            //make graph
            makeEdges();
            makeGraph(p, inter, intersectionnumber, N); // (point, intersection, num_intersection, num_point)

            //search route
            if(intype == 1){
                for (i = 0; i < Q; i++)
                {
                    // printf("[start point] [destination] [number of route]: ");
                    fscanf(fp,"%s%s%d", str_from, str_to, &k_short);
                    searchK_route(str_from, str_to, k_short);
                }                
            }else if(intype == 2){
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
            printf("end of the program\n\n");
			break;
		case 2:
            printf("Finding point to construct new road.\n");
            printf("Input data by hand or file?\n");
            printf("1. File input\n");
            printf("2. Hand input\n:");
            scanf("%d",&intype);
            //Segments *segments;
            // printf("[number of point] [number of line] [number of new point] [number of queries for asking shortest routes]:\n");
            if(intype == 1){
                printf("Enter the file name:");
                scanf("%100s",filename);
                printf("\n");
                fp = fopen(filename,"r");
                if(fp == NULL){
                    printf("error: file not found.\n");
                    exit(1);
                }
                fscanf(fp,"%d%d%d%d",&N,&M,&P,&Q);
                // //make array
                p = (Point *)malloc(sizeof(Point) * N);
                c = (Connection *)malloc(sizeof(Connection) * M);
                inter = (Intersection *)malloc(sizeof(Intersection) * 50000);
                new_p = (Point *)malloc(sizeof(Point) * P);
                //input data
                for (i = 0; i < N; i++)
                {
                    // printf("Coordinate number %d: ",i+1);
                    fscanf(fp,"%d%d", &p[i].coo[0], &p[i].coo[1]);
                    p[i].identifer = i + 1;
                }

                for (i = 0; i < M; i++)
                {
                    // printf("Line number %d: ",i+1);
                    fscanf(fp,"%d%d", &c[i].connect[0], &c[i].connect[1]);
                }
                for (i = 0; i < P; i++)
                {
                    // printf("New coordinate number %d: ",i+1);
                    fscanf(fp,"%d%d", &new_p[i].coo[0], &new_p[i].coo[1]);
                }
            }else if(intype == 2){
                scanf("%d %d %d %d", &N, &M, &P, &Q);
                // //make array
                p = (Point *)malloc(sizeof(Point) * N);
                c = (Connection *)malloc(sizeof(Connection) * M);
                inter = (Intersection *)malloc(sizeof(Intersection) * 50000);
                new_p = (Point *)malloc(sizeof(Point) * P);
                //input data
                for (i = 0; i < N; i++)
                {
                    printf("Coordinate number %d: ",i+1);
                    scanf("%d %d", &p[i].coo[0], &p[i].coo[1]);
                    p[i].identifer = i + 1;
                }

                for (i = 0; i < M; i++)
                {
                    printf("Line number %d: ",i+1);
                    scanf("%d %d", &c[i].connect[0], &c[i].connect[1]);
                }
                for (i = 0; i < P; i++)
                {
                    printf("New coordinate number %d: ",i+1);
                    scanf("%d %d", &new_p[i].coo[0], &new_p[i].coo[1]);
                }
            }
            newroad(c, p,new_p, P, M);
            free(p);
            free(new_p);
            free(c);
            free(inter);
            free(nodes);
            free(edges);
            printf("end of the program\n\n");
			break;		
		case 3:
            printf("Detecting Highways.\n");
            printf("Input data by hand or file?\n");
            printf("1. File input\n");
            printf("2. Hand input\n:");
            scanf("%d",&intype);
            //Segments *segments;
            // printf("[number of point] [number of line] [number of new point] [number of queries for asking shortest routes]:\n");
            if(intype == 1){
                printf("Enter the file name:");
                scanf("%100s",filename);
                printf("\n");
                fp = fopen(filename,"r");
                if(fp == NULL){
                    printf("error: file not found.\n");
                    exit(1);
                }
                fscanf(fp,"%d%d%d%d",&N,&M,&P,&Q);
                //make array
                p = (Point *)malloc(sizeof(Point) * N);
                c = (Connection *)malloc(sizeof(Connection) * M);
                inter = (Intersection *)malloc(sizeof(Intersection) * 50000);
                tmp_inter = (Intersection *)malloc(sizeof(Intersection) * 50000); //use for merge sort

                //input data
                for (i = 0; i < N; i++)
                {
                    // printf("Coordinate number %d: ",i+1);
                    fscanf(fp,"%d%d", &p[i].coo[0], &p[i].coo[1]);
                    p[i].identifer = i + 1;
                }

                for (i = 0; i < M; i++)
                {
                    // printf("Line number %d: ",i+1);
                    fscanf(fp,"%d%d", &c[i].connect[0], &c[i].connect[1]);
                }
            }else if(intype == 2){
                scanf("%d %d %d %d", &N, &M, &P, &Q);

                //make array
                p = (Point *)malloc(sizeof(Point) * N);
                c = (Connection *)malloc(sizeof(Connection) * M);
                inter = (Intersection *)malloc(sizeof(Intersection) * 50000);
                // float segments[M][30][2];
                //segments = (Segments *)malloc(sizeof(Segments) * M);

                //input data
                for (i = 0; i < N; i++)
                {
                    printf("Coordinate number %d: ",i+1);
                    scanf("%d %d", &p[i].coo[0], &p[i].coo[1]);
                    p[i].identifer = i + 1;
                }

                for (i = 0; i < M; i++)
                {
                    printf("Line number %d: ",i+1);
                    scanf("%d %d", &c[i].connect[0], &c[i].connect[1]);
                }
            }
            //calc intersection
            deter(c, p, M);

            //make graph
            makeEdges();
            makeGraph(p, inter, intersectionnumber, N); // (point, intersection, num_intersection, num_point)
            //search Highway
            searchHighways();
            free(p);
            free(c);
            free(inter);
            free(nodes);
            free(edges);
            printf("end of the program\n\n");
			break;			
		case 0:
			return 0;
    }
}


    //Segments *segments;
    // scanf("%d %d %d %d", &N, &M, &P, &Q);

    // //make array
    // p = (Point *)malloc(sizeof(Point) * N);
    // c = (Connection *)malloc(sizeof(Connection) * M);
    // inter = (Intersection *)malloc(sizeof(Intersection) * 50000);
    // // float segments[M][30][2];
    // //segments = (Segments *)malloc(sizeof(Segments) * M);

    // //input data
    // for (i = 0; i < N; i++)
    // {
    //     scanf("%d %d", &p[i].coo[0], &p[i].coo[1]);
    //     p[i].identifer = i + 1;
    // }

    // for (i = 0; i < M; i++)
    // {
    //     scanf("%d %d", &c[i].connect[0], &c[i].connect[1]);
    // }
    // //calc intersection
    // deter(c, p, M);

    // //make graph
    // makeEdges();
    // makeGraph(p, inter, intersectionnumber, N); // (point, intersection, num_intersection, num_point)

    // //search route
    // for (i = 0; i < Q; i++)
    // {
    //     scanf("%s %s %d", str_from, str_to, &k_short);
    //     searchK_route(str_from, str_to, k_short);
    // }

    // //search Highway
    // //searchHighways();

    // //release mmemories
    // free(p);
    // free(c);
    // free(inter);
    // free(nodes);
    // free(edges);
    // printf("end of the program");
//    return 0;
}

void deter(Connection c[], Point p[], int numline)
{
    int combi, i, j, k = 0, m, n;
    int ***lines; //[linenumber][point][coordinate]
    int n_p = 10; //max number of point on a segment
    float dist1, dist2, tmpd1, tmpd2, tmpx, tmpy;
    float det, s, t;
    int flag;
/*
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
*/
    for (i = 0; i < numline; i++)
    {
        m = 0;
        for (j = i + 1; j < numline; j++)
        {
            //printf("%d, %d\n", i, j);
            //easy to see
            int xp1, yp1, xp2, yp2, xq1, yq1, xq2, yq2;
/* 
            xp1 = lines[i][0][0];
            yp1 = lines[i][0][1];
            xq1 = lines[i][1][0];
            yq1 = lines[i][1][1];
            xp2 = lines[j][0][0];
            yp2 = lines[j][0][1];
            xq2 = lines[j][1][0];
            yq2 = lines[j][1][1];
*/
            xp1 = p[c[i].connect[0] - 1].coo[0];
            yp1 = p[c[i].connect[0] - 1].coo[1];
            xq1 = p[c[i].connect[1] - 1].coo[0];
            yq1 = p[c[i].connect[1] - 1].coo[1];
            xp2 = p[c[j].connect[0] - 1].coo[0];
            yp2 = p[c[j].connect[0] - 1].coo[1];
            xq2 = p[c[j].connect[1] - 1].coo[0];
            yq2 = p[c[j].connect[1] - 1].coo[1];
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
                        //printf("%d\n", k);
                        flag = 0;
                        int tempx = xp2 + (xq2 - xp2) * t;
                        int tempy = yp1 + (yq1 - yp1) * s;
                        for (int l = 0; l < k; l++)
                        {
                            if (tempx == inter[l].coo[0] && tempy == inter[l].coo[1])
                            {
                                flag = 1;
                                break;
                            }
                        }
                        if (flag == 0)
                        {
                            inter[k].coo[0] = xp2 + (xq2 - xp2) * t;
                            inter[k].coo[1] = yp1 + (yq1 - yp1) * s;
                            //printf("found\n");
                            k++;
                        }
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
        // inter = (Intersection *)malloc(sizeof(Intersection) * k);
        //printf("num of intersection %d\n", k);
        // rearrange(inter, k);
        //sort(0,k-1,inter);
        // for (i = 0; i < k; i++)
        // {
        //     printf("C%d, (%f, %f) \n", inter[i].ID, inter[i].coo[0], inter[i].coo[1]);
        // }
        mergesort(inter,0,k-1);
        printf("intersection\n");
        for (i = 0; i < k; i++)
        {
            printf("C%d, (%f, %f) \n", inter[i].ID, inter[i].coo[0], inter[i].coo[1]);
            //printf("line %d and %d \n", inter[i].crossline[0], inter[i].crossline[1]);
            //printf("line1 point %d and %d ", c[inter[i].crosslile[0]].connect[0], c[inter[i].crosslile[0]].connect[1]);
            //printf("line2 point %d and %d\n", c[inter[i].crosslile[1]].connect[0], c[inter[i].crosslile[1]].connect[1]);
        }
        // printf("\n");
    }
    else
    {
        printf("%d,NA3\n", k);
    }
    /*
    for (n = 0; n < numline; n++)
    {
        printf("{");
        for (m = 0; m < n_p; m++)
        {
            printf("%f, ", dist[n][m]);
        }
        printf("}\n");
    }

    printf("+++++++++++++++++++++++++++++++++++\n");
*/
    intersectionnumber = k;
    // free(lines);
}
// void rearrange(Intersection aa[], int k)
// {
//     int i, j, c;
//     float a, b;
//     Intersection tmp;
//     for (i = 0; i < k; i++)
//     {
//         for (j = i; j < k; j++)
//         {
//             if (aa[i].coo[0] > aa[j].coo[0])
//             {
//                 /*
//                 a = aa[j].coo[0];
//                 b = aa[j].coo[1];
//                 aa[j].coo[0] = aa[i].coo[0];
//                 aa[j].coo[1] = aa[i].coo[1];
//                 aa[i].coo[0] = a;
//                 aa[i].coo[1] = b;
//                 */
//                 tmp = aa[i];
//                 aa[i] = aa[j];
//                 aa[j] = tmp;
//             }
//             else if (aa[i].coo[0] == aa[j].coo[0])
//             {
//                 if (aa[i].coo[1] > aa[j].coo[1])
//                 {
//                     /*
//                     a = aa[j].coo[0];
//                     b = aa[j].coo[1];
//                     aa[j].coo[0] = aa[i].coo[0];
//                     aa[j].coo[1] = aa[i].coo[1];
//                     aa[i].coo[0] = a;
//                     aa[i].coo[1] = b;
//                     */
//                     tmp = aa[i];
//                     aa[i] = aa[j];
//                     aa[j] = tmp;
//                 }
//             }
//             aa[i].ID = i + 1;
//         }
//     }
// }
// void sort(int low, int high, Intersection aa[]){
//     int mid;
//     if(low < high){
//         mid = (low + high) / 2;
//         sort(low, mid, aa);
//         sort(mid+1, high, aa);
//         merge(low,mid,high, aa);
//     }
// }
// void merge(int low, int mid, int high, Intersection aa[]){
//     int l1, l2, i;
//     Intersection tmp[30];
//     for(l1=low, l2=mid+1, i=low; l1<=mid && l2<=high; i++){
//         if (aa[l1].coo[0] < aa[l2].coo[0])
//         {
//             tmp[i] = aa[l1++];
//         }else if (aa[l1].coo[0] == aa[l2].coo[0])
//         {
//             if (aa[l1].coo[1] < aa[l2].coo[1])
//             {
//                 tmp[i] = aa[l1++];
//             }
//         }else{
//             tmp[i] = aa[l2++];
//         }
//         aa[i].ID = i + 1;
//     }
//     while(l1 <= mid){
//         tmp[i++] = aa[l1++];
//     }
//     while(l2 <= high){
//         tmp[i++] = aa[l2++];
//     }
//     for(i = low; i <= high; i++){
//         aa[i] = tmp[i];
//         aa[i].ID = i+1;
//     }
// }
void mergesort(Intersection A[],int left,int right){
    int i,j,k,mid;
    if (right>left) {
        mid=(right+left)/2; /*DIVIDE*/
        mergesort(A,left,mid);
        mergesort(A,mid+1,right);
        for (i=left;i<=mid;i++){
            tmp_inter[i]=A[i];
            // printf("A1: %f, %f\n",A[i].coo[0],A[i].coo[1]);
        } 
        
        for (i=mid+1,j=right;i<=right;i++,j--){
            tmp_inter[i]=A[j];
            // printf("A2: %f, %f\n",A[j].coo[0],A[j].coo[1]);
        } 
        i=left; j=right; /*CONQUER*/
        for (k = left; k <= right; k++){
            if (tmp_inter[i].coo[0]<tmp_inter[j].coo[0]) A[k]=tmp_inter[i++];
            else if (tmp_inter[i].coo[0]==tmp_inter[j].coo[0] && tmp_inter[i].coo[1]<tmp_inter[j].coo[1]) A[k]=tmp_inter[i++];
            else A[k]=tmp_inter[j--];
        }
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
    //check all lines
    for (i = 0; i < M; i++)
    {

        //printf("%d\n", i);
        for (j = 0; j < 1000; j++)
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

                    List.alignpoints[alignnumber] = -inter[j].ID;
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

        /* 
        printf("end\n");
        printf("{");
        for (j = 0; j < alignnumber; j++)
        {
            //positive number -> point ID
            //negative number -> intersection ID
            printf(" (%d, %f) ", List.alignpoints[j], List.dist_list[j]);
        }
        printf("}\n");
*/

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
            flag = 0;
            for (l = 0; l < edgeindex; l++)
            {
                //printf("%s, %s, %s, %s\n", edges[l].node[0], edges[l].node[1], tmpstr[0], tmpstr[1]);
                if ((strcmp(edges[l].node[0], tmpstr[0]) == 0 && strcmp(edges[l].node[1], tmpstr[1]) == 0) || (strcmp(edges[l].node[0], tmpstr[1]) == 0 && strcmp(edges[l].node[1], tmpstr[0]) == 0))
                {
                    flag = 1;
                    break;
                }
            }

            if (flag == 1)
            {
                //edge is already exist.
                //printf("edge exist\n");
                continue;
            }

            //save edges
            strcpy(edges[edgeindex].node[0], tmpstr[0]);
            strcpy(edges[edgeindex].node[1], tmpstr[1]);
            edges[edgeindex].cost = List.dist_list[j + 1] - List.dist_list[j];
            edges[edgeindex].is_exist = TRUE;
            //printf("{%s, %s, %f}\n", edges[edgeindex].node[0], edges[edgeindex].node[1], edges[edgeindex].cost);
            edgeindex++;
        }
    }
    edgenumber = edgeindex;
    //printf("edgenumber %d\n", edgenumber);

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
        sortAlign(list, right, j + 1);
    }
}


void makeGraph(Point p[], Intersection inter[], int k, int n)
{
    int i, j;
    int len_node = n + intersectionnumber;
    //Node *nodes; -> global variable
    nodes = (Node *)malloc(sizeof(Node) * len_node);

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
    // printf("nodes: %d, edges:%d\n\n", nodenumber, edgenumber);
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

    qd = (Queuedist *)malloc(sizeof(Queuedist) * QUEUE_MAX);
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
        for (i = 0; i < edgenumber; i++)
        {
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
    strcpy(qd[qd_tale].ID, nodeid);
    qd[qd_tale].dist = dist;
    //printf("push qd[%d] %s, %f\n", qd_tale, qd[qd_tale].ID, qd[qd_tale].dist);
    qd_tale++;
}

Queuedist popDist()
{
    int min, i, minid = 0, tmpid = 0;
    float min_dist = pow(10, 8);
    Queuedist searchdata;
    for (i = 0; i < qd_tale; i++)
    {
        if (min_dist > qd[i].dist)
        {
            min_dist = qd[i].dist;
            min = i;
            minid = 0;
            for (int j = 0; qd[i].ID[j] != '\0'; j++)
            {
                minid += 10*minid + qd[i].ID[j];
            }
        }
        else if (min_dist == qd[i].dist)
        {
            tmpid = 0;
            for (int j = 0; qd[i].ID[j] != '\0'; j++)
            {
                tmpid += 10*tmpid + qd[i].ID[j];
            }
            if (tmpid > minid)
            {
                min_dist = qd[i].dist;
                min = i;
            }
        }
    }
    searchdata = qd[min];
    //printf("pop qd[%d] %s, %f\n", min, qd[min].ID, qd[min].dist);
    qd[min] = qd[qd_tale - 1];
    qd_tale--;
    return searchdata;
}

void showRoute(char *goalID)
{
    int index = getNodeindex(goalID);
    if (strcmp(nodes[index].from_ID, "0") != 0)
    {
        showRoute(nodes[index].from_ID);
    }
    printf("%s ", nodes[index].ID);
}

void pushRoute(char *goalID)
{
    int index, num = 1;
    int i;

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

    //printf("qd[%d] dist %f\n", qr_tale, qr[qr_tale].dist);
    /*
    for (i = 0; i < num; i++)
    {
        printf("%s ", qr[qr_tale].routeIDs[i]);
    }
    printf("\n");
    */

    qr_tale++;
}

Queueroute popRoute()
{
    int min, i;
    float min_dist = pow(10, 8);
    Queueroute searchdata;
    for (i = 0; i < qr_tale; i++)
    {
        if (min_dist > qr[i].dist)
        {
            min_dist = qr[i].dist;
            min = i;
        }
    }
    searchdata = qr[min];
    qr[min] = qr[qr_tale - 1];
    qr_tale--;

    return searchdata;
}

int getEdgeindex(char *node1, char *node2)
{
    int i;
    for (i = 0; i < edgenumber; i++)
    {
        if ((strcmp(node1, edges[i].node[0]) == 0 && strcmp(node2, edges[i].node[1]) == 0) || (strcmp(node1, edges[i].node[1]) == 0 && strcmp(node2, edges[i].node[0]) == 0))
        {
            return i;
        }
    }

    //not found
    return -1;
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
    int tmpedgelist[15], k_route_issame[15];
    float tmpdist;

    from_index = getNodeindex(from);
    to_index = getNodeindex(to);

    printf("Search from: %s to: %s\n", from, to);

    if (from_index != -1 && to_index != -1)
    {
        qr = (Queueroute *)malloc(sizeof(Queueroute) * QUEUE_MAX);
        k_route = (Queueroute *)malloc(sizeof(Queueroute) * k_num);

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
            printf("NA\n");
            return;
        }

        printf("No.1 shortest\ndistance: %f\nRoute: ", k_route[0].dist);
        for (j = 0; j < k_route[0].routecount; j++)
        {
            printf("%s ", k_route[0].routeIDs[j]);
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

            printf("No.%d shortest\ndistance: %f\nRroute: ", i + 1, k_route[i].dist);
            for (j = 0; j < k_route[i].routecount; j++)
            {
                printf("%s ", k_route[i].routeIDs[j]);
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
    qr_tale = 0;

    for (i = 0; i < edgenumber; i++)
    {
        edges[i].is_exist = FALSE;
        //printf("edge[%d] {%s, %s, %f} false\n", i, edges[i].node[0], edges[i].node[1], edges[i].cost);
        //printf("search {%s, %s} \n", edges[i].node[0], edges[i].node[1]);
        resetNodeStatus();

        from_index = getNodeindex(edges[i].node[0]);
        nodes[from_index].dist = 0;

        searchRoute(edges[i].node[0], edges[i].node[1]);
        q = popRoute();
        if (q.routecount <= 1)
        {
            printf("Highway[%d](%s , %s)\n", highwaynumber, edges[i].node[0], edges[i].node[1]);
            highwaynumber++;
        }
        edges[i].is_exist = TRUE;
    }

    free(qr);
}
void newroad(Connection c[], Point p[], Point new_p[],int num_new_p, int numline)
{
    int combi, i, j, m, n;
    int ***lines; //lines[numline][2][2]; //[linenumber][point][coordinate]
    int n_p = 10; //max number of point on a segment
    //float lines1[numline][n_p][2]
    float **dist; //float dist[numline][n_p];
    float dist1, dist2, tmpd = 0,tmpd1, tmpd2, tmpx=0, tmpy=0;
    float det, s, t;


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
            xp1 = p[c[i].connect[0] - 1].coo[0];
            yp1 = p[c[i].connect[0] - 1].coo[1];
            xq1 = p[c[i].connect[1] - 1].coo[0];
            yq1 = p[c[i].connect[1] - 1].coo[1];

            t = -(((xp1-new_x)*(xq1-xp1)+(yp1-new_y)*(yq1-yp1))/(pow(xq1-xp1,2)+pow(yq1-yp1,2)));
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
}
