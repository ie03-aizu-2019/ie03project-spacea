#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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
    float xp1,yp1,xp2,yp2,xq1,yq1,xq2,yp2,A,x,y;

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
  
  
/*-----------------------------------*/

	for(i=0; i<M;i++)
	{
		xp1 = p[c[i].connect[0]-1].c[0];         
		yp1 = p[c[i].connect[0]-1].c[1]	;	
		xp2 = p[c[i].connect[1]-1].c[0] ;      
		yp2	= p[c[i].connect[1]-1].c[1]	;
	
	 	xq1 = p[c[i+1].connect[0]-1].c[0] ;        
		yq1 = p[c[i+1].connect[0]-1].c[1];		
		xq2 = p[c[i+1].connect[1]-1].c[0]  ;     
		yq2	= p[c[i+1].connect[1]-1].c[1];		
	}
	A = fabs((xq1-xp1)*(yp2-yq2)+(xq2-xp2)*(yq1-yp1));
	if( 0 == A)
		printf("NA");
	else{
		s = ((yp2-yq2)*(xp2-xp1)+(xq2-xp2)*(yp2-yp1))/A	;	
		t = ((yp1-yq1)*(xp2-xp1)+(yq1-yp1)*(yp2-yp1))/A	;
		if( (0<=s && s<=1) && (0<=t && t<=1) ){
			x=xp1+(xq1-xp1)*s;
			y=yp1+(yq1-yp1)*s;
			printf("%f,%f",x,y);
		}
		else
			printf("NA");


/*-----------------------------------*/
  //Check whether data is saved
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


/*
    Calculate
*/

    //release mmemories
    free(p);
    free(c);

    return 0;
}
