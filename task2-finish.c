///exclude when s or t is equal to 1 or 0
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

typedef struct 
{
    int ID;
    float coo[2];
}Intersection;

void deter(Connection c[], Point p[], int numline);
void rearrange(Intersection aa[],int k);
Point *p;
Connection *c;
Intersection *inter;
int k;

int main()
{
    int N, M, P, Q;
    int i; //loop variable

    scanf("%d %d %d %d", &N, &M, &P, &Q);

    //make array
    p = (Point *)malloc(sizeof(Point) * N);
    c = (Connection *)malloc(sizeof(Connection) * M);
    inter = (Intersection *)malloc(sizeof(Intersection) * 1000);


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
deter(c,p,M);
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
    return 0;
}


void deter(Connection c[], Point p[],int numline){
    int combi,i,j, lines[numline][2][2]; //[linenumber][point][coordinate]

    float det,s,t;
    //make lines
    for(i=0;i<numline;i++){     
        lines[i][0][0] = p[c[i].connect[0]-1].coo[0];
        lines[i][0][1] = p[c[i].connect[0]-1].coo[1];
        lines[i][1][0] = p[c[i].connect[1]-1].coo[0];
        lines[i][1][1] = p[c[i].connect[1]-1].coo[1];
    }


    for(i=0;i<numline;i++){
        for(j=i+1;j<numline;j++){
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
            
            det = (xq1-xp1)*(yp2-yq2)+(xq2-xp2)*(yq1-yp1);
            // printf("%f\n",det);
            ////////////////
            //det[0] = (lines[0][1][0]-lines[0][0][0])*(lines[1][0][1]-lines[1][1][1])+(lines[1][1][0]-lines[1][0][0])*(lines[0][1][1]-lines[0][0][1]);
            // if(det < 0){det*=-1;}
            if(det == 0)
            {
                printf("NA1\n");
            }else{
                if(pow(10,-7)<=det && det<=pow(10,-7)){
                    printf("NA1\n");
                }
                else{
                    s = ((yp2-yq2)*(xp2-xp1)+(xq2-xp2)*(yp2-yp1))/det;
                    t = ((yp1-yq1)*(xp2-xp1)+(xq1-xp1)*(yp2-yp1))/det;
                    // printf("%f,%f\n",s,t);
                    
                    if(0<s && s<1 && 0<t && t<1){
                        // printf("%d\n",k);
                        inter[k].coo[0] = xp2+(xq2-xp2)*t;
                        inter[k].coo[1] = yp1+(yq1-yp1)*s;
                        // inter[k].ID = k+1;
                        k++;
                    }else{
                        // printf("NA2\n");
                    }
                }
            }
        }
    }
    if(k != 0){
        // inter = (Intersection *)malloc(sizeof(Intersection) * k);
        //printf("%d\n",k);
        rearrange(inter,k);
        for(i=0;i<k;i++){
            // printf("%f, %f, %d\n",intersection[i][0],intersection[i][1],(int)intersection[i][2]);
            printf("%f, %f, %d\n",inter[i].coo[0],inter[i].coo[1],inter[i].ID);
        }
    }else{printf("%d,NA3\n",k);} 
}
void rearrange(Intersection aa[],int k){
    int i, j, c;
    float a, b;
    for(i=0;i<k;i++){
        for(j=i;j<k;j++)
        {
            if(aa[i].coo[0]>aa[j].coo[0])
            {
                a = aa[j].coo[0];
                b = aa[j].coo[1];
                aa[j].coo[0] = aa[i].coo[0];
                aa[j].coo[1] = aa[i].coo[1];
                aa[i].coo[0] = a;
                aa[i].coo[1] = b;
            }else if(aa[i].coo[0]==aa[j].coo[0])
            {
                if(aa[i].coo[1]>aa[j].coo[1])
                {
                    a = aa[j].coo[0];
                    b = aa[j].coo[1];
                    aa[j].coo[0] = aa[i].coo[0];
                    aa[j].coo[1] = aa[i].coo[1];
                    aa[i].coo[0] = a;
                    aa[i].coo[1] = b;
                }
            }
            aa[i].ID = i+1;
        }

    }
}
