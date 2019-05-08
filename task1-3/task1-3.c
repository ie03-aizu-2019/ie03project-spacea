//It's needed the option "-lm" when you compriles the program.

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//------task1-3 ReachingState--------//
#define WHITE 0
#define GRAY 1
#define BLACK 2
//-----------------------------------//

//structure declearation
typedef struct
{
    int identifer;
    int c[2]; //coordinate[x,y]

    //task1-3
    double shortest_distance;
    int previous_point;
    int reachingstate;
} Point;

typedef struct
{
    int connect[2]; //idenfiter [0] and [1] are connected
} Connection;

typedef struct Intersection
{
    //Use for binary tree structure
    struct Intersection *parent;
    struct Intersection *left;
    struct Intersection *right;

    //data
    int number;            //intersection number
    int line_point[2][2];  //line[connection][point]
    float intersection[2]; //intersection coordinate[x,y]

    //task1-3
    double shortest_distance;
    int previous_point;
    int reachingstate;
} Intersection;

//fanction declearation
void CheckIntersection(int, int);                    //variable: (connection1, connection2)
int CompareIntersection(Intersection, Intersection); //compare the coordinate

//-------binary tree fanctions--------//
void SaveIntersection(int, int, float, float);
void InsertIntersection(Intersection *);
void AssignNumber(Intersection *);
void ShowIntersection(Intersection *);
Intersection *SearchIntersection_Number(Intersection *, int);
void FreeTree(Intersection *);
//------------------------------------//

//------------task1-3----------------//
void Route_Calculation(int, int, int);
void Calc_Distance_PtoP(int);
void Calc_Distance_PtoI(int, Intersection *);
void Calc_Distance_ItoP(Intersection *);
void Calc_Distance_ItoI(Intersection *, Intersection *);
double Calc_Distance(double, double, double, double);
void *SearchIntersection_Shortest(Intersection *, double);
void Reset_Distance();
void Reset_Distance_Intersection(Intersection *);
//-----------------------------------//
//global variable
Point *p;
Connection *c;
Intersection *root;

//for Route_Calculation, SearchIntersection_Shortest
Intersection *min_intersection;

int N, M, P, Q;
int intersectionnumber = 1;

int main()
{
    int i, j; //loop variable
    //------task 1-3------//
    char str_from[10], str_to[10]; //use for scanf
    int from, to, num;             //use for processing
    //--------------------//

    Intersection *searchnode;

    scanf("%d %d %d %d", &N, &M, &P, &Q);

    //make array
    p = (Point *)malloc(sizeof(Point) * N);
    if (p == NULL)
    {
        //if memory allocation is failed, show error message.
        fprintf(stderr, "Error:Could not allocate memories.");
        exit(0);
    }

    c = (Connection *)malloc(sizeof(Connection) * M);
    if (c == NULL)
    {
        //if memory allocation is failed, show error message.
        fprintf(stderr, "Error:Could not allocate memories.");
        free(p);
        exit(1);
    }

    //input data
    for (i = 0; i < N; i++)
    {
        scanf("%d %d", &p[i].c[0], &p[i].c[1]);
        p[i].identifer = i + 1;
    }

    for (i = 0; i < M; i++)
    {
        scanf("%d %d", &c[i].connect[0], &c[i].connect[1]);
    }

    //Calculate  intersection
    for (i = 0; i < M; i++)
    {
        for (j = i + 1; j < M; j++)
        {
            CheckIntersection(i, j);
        }
    }

    //Assign number to the intersection
    AssignNumber(root);

    for (i = 0; i < Q; i++)
    {
        scanf("%s %s %d", str_from, str_to, &num);
        if (str_from[0] == 'C')
        {
            from = -atoi(&str_from[1]);
        }
        else
        {
            from = atoi(&str_from[0]);
        }

        if (str_to[0] == 'C')
        {
            to = -atoi(&str_to[1]);
        }
        else
        {
            to = atoi(&str_to[0]);
        }

        printf("from:%d to:%d\n", from, to);

        if (from > N || -from > intersectionnumber || to > N || -to > intersectionnumber)
        {
            //no exist start or destination
            printf("NA\n");
        }
        else
        {
            Reset_Distance();

            if (from > 0)
            {
                p[from - 1].shortest_distance = 0;
            }
            else
            {
                searchnode = SearchIntersection_Number(root, -from);
                searchnode->shortest_distance = 0;
            }

            Route_Calculation(from, to, num);

            if (to > 0)
            {
                if (p[to - 1].reachingstate != WHITE)
                {
                    printf("%.5f\n", p[to - 1].shortest_distance);
                }
                else
                {
                    printf("NA\n");
                }
            }
            else
            {
                searchnode = SearchIntersection_Number(root, -to);
                if (searchnode->reachingstate != WHITE)
                {
                    printf("%.5f\n", searchnode->shortest_distance);
                }
                else
                {
                    printf("NA\n");
                }
            }
        }
        printf("from:%d to:%d End\n\n", from, to);
    }

    //searchnode = SearchIntersection(root, searchnumber);

    //output intersection
    // ShowIntersection(root);

    //release mmemories
    free(p);
    free(c);
    FreeTree(root);

    printf("End of the program");
    return 0;
}

void CheckIntersection(int line1, int line2)
{

    float p1x = p[c[line1].connect[0] - 1].c[0];
    float p1y = p[c[line1].connect[0] - 1].c[1];
    float q1x = p[c[line1].connect[1] - 1].c[0];
    float q1y = p[c[line1].connect[1] - 1].c[1];
    float p2x = p[c[line2].connect[0] - 1].c[0];
    float p2y = p[c[line2].connect[0] - 1].c[1];
    float q2x = p[c[line2].connect[1] - 1].c[0];
    float q2y = p[c[line2].connect[1] - 1].c[1];
    float A, s, t, intersection_x, intersection_y;
    float EPS = powf(10, -7);

    /*input check
    printf("p1x = %f, p1y = %f\nq1x = %f, q1y = %f\np2x = %f, p2y = %f\nq2x = %f, q2y = %f\n\n",
           p1x, p1y, q1x, q1y, p2x, p2y, q2x, q2y);
    */

    //step1
    A = fabs((q1x - p1x) * (p2y - q2y) + (q2x - p2x) * (q1y - p1y));

    //check result of A
    //printf("A = %f\n", A);

    if (A <= EPS)
    {
        //assume A = 0
        //printf("Step1 == 0\n");
    }
    else
    {
        //step2
        s = ((p2y - q2y) * (p2x - p1x) + (q2x - p2x) * (p2y - p1y)) / A;
        t = ((p1y - q1y) * (p2x - p1x) + (q1x - p1x) * (p2y - p1y)) / A;

        //check result of s & t
        //printf("s = %f, t = %f\n", s, t);

        //step3
        if ((0 < s && s < 1) && (0 < t && t < 1))
        {
            //step4
            intersection_x = p1x + (q1x - p1x) * s;
            intersection_y = p1y + (q1y - p1y) * s;

            SaveIntersection(line1, line2, intersection_x, intersection_y);
        }
        else
        {
            //no intersection
            //printf("step3 no intersection\n");
        }
    }
}

void SaveIntersection(int line1, int line2, float x, float y)
{
    Intersection *new;
    //make new node
    new = (Intersection *)malloc(sizeof(Intersection));
    if (new == NULL)
    {
        //if memory allocation is failed, show error message.
        fprintf(stderr, "error: Could not allocate intersection memory");
        free(c);
        free(p);
        FreeTree(root);
        exit(2);
    }
    //input data into the new node
    new->parent = NULL;
    new->left = NULL;
    new->right = NULL;
    new->line_point[0][0] = c[line1].connect[0];
    new->line_point[0][1] = c[line1].connect[1];
    new->line_point[1][0] = c[line2].connect[0];
    new->line_point[1][1] = c[line2].connect[1];
    new->intersection[0] = x;
    new->intersection[1] = y;

    InsertIntersection(new);
}

//Insert new node into the binary tree
void InsertIntersection(Intersection *new)
{
    Intersection *x = root;
    Intersection *y = NULL;
    int compare;

    while (x != NULL)
    {
        y = x;
        compare = CompareIntersection(*new, *x);
        if (compare > -1)
        {
            x = x->left;
        }
        else
        {
            x = x->right;
        }
    }

    new->parent = y;

    if (y == NULL)
    {
        root = new;
    }
    else
    {
        if (compare > -1)
        {
            y->left = new;
        }
        else
        {
            y->right = new;
        }
    }
}

int CompareIntersection(Intersection A, Intersection B)
{
    //compare two intersections A and B
    //Refer to task2 in phase 1
    //A < B :return 1
    //A > B :return -1
    //A = B :return 0

    //check the value of coordination[x,y] in both of intersection.
    //printf("A.x = %f, A.y = %f, B.x = %f, B.y = %f\n", A.intersection[0], A.intersection[1], B.intersection[0], B.intersection[1]);

    //compare coordinate x
    if (A.intersection[0] < B.intersection[0])
    {
        return 1;
    }
    else if (A.intersection[0] > B.intersection[0])
    {
        return -1;
    }
    else
    {
        //if each x are same, compare coordinate y
        if (A.intersection[1] < B.intersection[1])
        {
            return 1;
        }
        else if (A.intersection[1] > B.intersection[1])
        {
            return -1;
        }
    }
    //if each x and y are same
    return 0;
}

//Assign number to the all intersections.
//when you call this fanction, pass "root" as the argument.
void AssignNumber(Intersection *u)
{
    if (u->left != NULL)
    {
        AssignNumber(u->left);
    }
    u->number = intersectionnumber++;
    if (u->right != NULL)
    {
        AssignNumber(u->right);
    }
}

//show all intersections
//when you call this fanction, pass "root" as the argument.
void ShowIntersection(Intersection *u)
{
    if (u->left != NULL)
    {
        ShowIntersection(u->left);
    }
    //--------------print area-----------//

    //printf("number = %d, x = %f, y = %f\n", u->number, u->intersection[0], u->intersection[1]);
    //printf("p1 = %d, p2 = %d, p3 = %d, p4 = %d\n", u->line_point[0][0], u->line_point[0][1], u->line_point[1][0], u->line_point[1][1]);

    printf("int_number = %d ", u->number);
    printf("dis = %f, ", u->shortest_distance);
    if (u->reachingstate == WHITE)
    {
        printf("WHITE\n");
    }
    else if (u->reachingstate == GRAY)
    {
        printf("GRAY\n");
    }
    else
    {
        printf("BLACK\n");
    }

    //--------------print area-----------//

    if (u->right != NULL)
    {
        ShowIntersection(u->right);
    }
}

//when you call this fanction, pass "root" as the first argument.
//num: the intersection number which you want to find
Intersection *SearchIntersection_Number(Intersection *u, int num)
{
    if (u == NULL)
    {
        //not found
        return NULL;
    }
    if (num == u->number)
    {
        //found
        return u;
    }

    //search(recursion)
    if (num < u->number)
    {
        return SearchIntersection_Number(u->left, num);
    }
    else
    {
        return SearchIntersection_Number(u->right, num);
    }
}

//release all the memories which is used for binary tree
void FreeTree(Intersection *u)
{
    if (u->left != NULL)
    {
        FreeTree(u->left);
    }
    if (u->right != NULL)
    {
        FreeTree(u->right);
    }

    //printf("noed number = %d free\n", u->number);
    free(u);
}

void Route_Calculation(int from, int to, int num)
{
    Intersection *searchnode, *minnode;
    int i, j;
    double min_dis = pow(10, 10);
    int min_point = 0;

    if (from > 0)
    {
        if (p[from - 1].reachingstate != BLACK)
        {
            p[from - 1].reachingstate = BLACK;
            //printf("start_number = %d\n", from);
        }
        else
        {
            return;
        }
    }
    else
    {
        searchnode = SearchIntersection_Number(root, -from);
        if (searchnode->reachingstate != BLACK)
        {
            searchnode->reachingstate = BLACK;
            //printf("start_number = %d\n", -searchnode->number);
        }
        else
        {
            return;
        }
    }

    if (from > 0)
    {
        //Point process
        //Point -> Point
        //printf("start PtoP\n");
        Calc_Distance_PtoP(from);

        //Point -> Intersection
        //printf("start PtoI\n");
        Calc_Distance_PtoI(from, root);
    }
    else
    {
        //Intersection
        //Intersection -> Point
        //printf("start ItoP\n");
        Calc_Distance_ItoP(searchnode);

        //Intersection -> Intersection
        //printf("start ItoI\n");
        Calc_Distance_ItoI(root, searchnode);
    }

    //printf("End calculate\n");
    //printf("start search shortest node\n");

    SearchIntersection_Shortest(root, min_dis);
    minnode = min_intersection;
    if (minnode != NULL)
    {
        min_dis = minnode->shortest_distance;
        min_point = -minnode->number;
        //printf("%f\n", min_dis);
    }

    //ShowIntersection(root);

    for (i = 0; i < N; i++)
    {
        /*
        printf("%d, %f, ", p[i].identifer, p[i].shortest_distance);
        if (p[i].reachingstate == WHITE)
        {
            printf("WHITE\n");
        }
        else if (p[i].reachingstate == GRAY)
        {
            printf("GRAY\n");
        }
        else
        {
            printf("BLACK\n");
        }
        */

        if (p[i].reachingstate == GRAY)
        {
            if (p[i].shortest_distance < min_dis)
            {
                min_dis = p[i].shortest_distance;
                min_point = p[i].identifer;
            }
        }
    }

    if (min_point != to && min_point != 0)
    {
        Route_Calculation(min_point, to, num);
    }
    else
    {
        return;
    }
}

void Calc_Distance_PtoP(int from)
{
    int i;

    for (i = 0; i < M; i++)
    {
        if ((c[i].connect[0] == from))
        {
            if (p[c[i].connect[1] - 1].reachingstate != BLACK)
            {
                double p1_x = p[c[i].connect[0] - 1].c[0];
                double p1_y = p[c[i].connect[0] - 1].c[1];
                double p2_x = p[c[i].connect[1] - 1].c[0];
                double p2_y = p[c[i].connect[1] - 1].c[1];

                if (p[c[i].connect[1] - 1].shortest_distance > Calc_Distance(p1_x, p1_y, p2_x, p2_y) + p[from - 1].shortest_distance)
                {
                    p[c[i].connect[1] - 1].shortest_distance = Calc_Distance(p1_x, p1_y, p2_x, p2_y) + p[from - 1].shortest_distance;
                    p[c[i].connect[1] - 1].previous_point = from;
                    p[c[i].connect[1] - 1].reachingstate = GRAY;
                }
            }
        }
        else if (c[i].connect[1] == from)
        {
            if (p[c[i].connect[0] - 1].reachingstate != BLACK)
            {
                int p1_x = p[c[i].connect[0] - 1].c[0];
                int p1_y = p[c[i].connect[0] - 1].c[1];
                int p2_x = p[c[i].connect[1] - 1].c[0];
                int p2_y = p[c[i].connect[1] - 1].c[1];

                if (p[c[i].connect[0] - 1].shortest_distance > Calc_Distance(p1_x, p1_y, p2_x, p2_y) + p[from - 1].shortest_distance)
                {
                    p[c[i].connect[0] - 1].shortest_distance = Calc_Distance(p1_x, p1_y, p2_x, p2_y) + p[from - 1].shortest_distance;
                    p[c[i].connect[0] - 1].previous_point = from;
                    p[c[i].connect[0] - 1].reachingstate = GRAY;
                }
            }
        }
    }
}

void Calc_Distance_PtoI(int num, Intersection *u)
{

    if (u->left != NULL)
    {
        Calc_Distance_PtoI(num, u->left);
    }
    if ((num == u->line_point[0][0]) || (num == u->line_point[0][1]) || (num == u->line_point[1][0]) || (num == u->line_point[1][1]))
    {
        //printf("p1 = %d, p2 = %d, p3 = %d, p4 = %d\n", u->line_point[0][0], u->line_point[0][1], u->line_point[1][0], u->line_point[1][1]);
        //calculate
        double p_x = p[num - 1].c[0];
        double p_y = p[num - 1].c[1];
        double int_x = u->intersection[0];
        double int_y = u->intersection[1];

        if (u->shortest_distance > Calc_Distance(p_x, p_y, int_x, int_y) + p[num - 1].shortest_distance)
        {
            u->reachingstate = GRAY;
            u->previous_point = num;
            u->shortest_distance = Calc_Distance(p_x, p_y, int_x, int_y) + p[num - 1].shortest_distance;
        }
    }

    if (u->right != NULL)
    {

        Calc_Distance_PtoI(num, u->right);
    }
}

void Calc_Distance_ItoP(Intersection *fromnode)
{
    int i, j, k;

    for (j = 0; j < 2; j++)
    {
        for (k = 0; k < 2; k++)
        {
            int point = fromnode->line_point[j][k];

            if (p[point - 1].reachingstate != BLACK)
            {
                double p1_x = fromnode->intersection[0];
                double p1_y = fromnode->intersection[1];
                double p2_x = p[point - 1].c[0];
                double p2_y = p[point - 1].c[1];

                if (p[point - 1].shortest_distance > Calc_Distance(p1_x, p1_y, p2_x, p2_y) + fromnode->shortest_distance)
                {
                    p[point - 1].shortest_distance = Calc_Distance(p1_x, p1_y, p2_x, p2_y) + fromnode->shortest_distance;
                    p[point - 1].previous_point = -fromnode->number;
                    p[point - 1].reachingstate = GRAY;
                }
            }
        }
    }
}

void Calc_Distance_ItoI(Intersection *u, Intersection *fromnode)
{

    if (u->left != NULL)
    {
        Calc_Distance_ItoI(u->left, fromnode);
    }

    if (((fromnode->line_point[0][0] == u->line_point[0][0]) && (fromnode->line_point[0][1] == u->line_point[0][1])) || ((fromnode->line_point[1][0] == u->line_point[1][0]) && (fromnode->line_point[1][1] == u->line_point[1][1])))
    {
        if (u->reachingstate != BLACK)
        {
            //calculate
            double x1 = fromnode->intersection[0];
            double y1 = fromnode->intersection[1];
            double x2 = u->intersection[0];
            double y2 = u->intersection[1];

            if (u->shortest_distance > Calc_Distance(x1, y1, x2, y2) + fromnode->shortest_distance)
            {
                u->reachingstate = GRAY;
                u->previous_point = -fromnode->number;
                u->shortest_distance = Calc_Distance(x1, y1, x2, y2) + fromnode->shortest_distance;
            }
        }
    }

    if (u->right != NULL)
    {
        Calc_Distance_ItoI(u->right, fromnode);
    }
}

double Calc_Distance(double x1, double y1, double x2, double y2)
{
    //printf("x1 = %f, y1 = %f, x2 = %f, y2 = %f\n", x1, y1, x2, y2);
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

void *SearchIntersection_Shortest(Intersection *u, double min_dir)
{
    if (u->left != NULL)
    {
        SearchIntersection_Shortest(u->left, min_dir);
    }

    if (u->reachingstate == GRAY && u->shortest_distance < min_dir)
    {
        min_intersection = u;
        min_dir = u->shortest_distance;
    }

    if (u->right != NULL)
    {
        SearchIntersection_Shortest(u->right, min_dir);
    }
}

void Reset_Distance()
{
    int i;
    for (i = 0; i < N; i++)
    {

        p[i].previous_point = 0;
        p[i].shortest_distance = pow(10, 10);
        p[i].reachingstate = WHITE;
    }
    Reset_Distance_Intersection(root);
}

void Reset_Distance_Intersection(Intersection *u)
{
    if (u->left != NULL)
    {
        Reset_Distance_Intersection(u->left);
    }
    if (u->right != NULL)
    {
        Reset_Distance_Intersection(u->right);
    }

    u->reachingstate = WHITE;
    u->previous_point = 0;
    u->shortest_distance = pow(10, 10);
}