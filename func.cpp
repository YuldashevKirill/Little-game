#include "class.hpp"
#include "func.hpp"
using namespace std;

void zero_array(int **x2, int ***x3, int N)
{
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
        {
            if (x2 != nullptr)
                x2[i][j] = 0;
            if (x3 != nullptr)
                for (int w = 0; w < N; w++)
                    x3[i][j][w] = 0;
        }
}

void reverse(int *orient)
{
    if (*orient >= 4)
        *orient -= 4;
    else
        *orient += 4;
}

void coord_next_cell(int k, int i, int j, int *i1, int *j1)
{
    if (k <= 1 || k == 7)
        *i1 = i - 1;
    if (k >= 3 && k <= 5)
        *i1 = i + 1;
    if (k == 2 || k == 6)
        *i1 = i;
    if (k >= 1 && k <= 3)
       *j1 = j + 1;
    if (k >= 5 && k <= 7)
        *j1 = j - 1;
    if (k == 0 || k == 4)
        *j1 = j;
}

void check_boundary(int *k, int i, int j, int *i1, int *j1, int N)
{
    if (*i1 < 0 || *i1 >= N || *j1 < 0 || *j1 >= N)
    {
        *i1 = i;
        *j1 = j;
        *k = -1;
    }   
}       
void first_try(int **a1, int ***a2, int **count, int N)
{
    int i, j;
    for (i = 0; i < N; i++)
    {
        count[i] = new int [N];
        a2[i] = new int* [N];
        for (j = 0; j < N; j++)
            a2[i][j] = new int [N];
    }
    zero_array(count, a2, N);
    for (i = 0; i < N; i++)
        for (j = 0; j < N; j++)
        {
            if (!a1[i][j])
                continue;
            int s, i1, j1, k = a1[i][j] % 10;
            coord_next_cell(k, i, j, &i1, &j1);
            check_boundary(&k, i, j, &i1, &j1, N);
            if (a1[i][j] >= 20)
            {
                count[i1][j1]++;
                s = count[i1][j1]%10;
                a2[i1][j1][N-s] = k; // k means traectory by which this comes to this cell
            }
            if (a1[i][j] >= 10 && a1[i][j] < 20)
            {
                s = count[i1][j1]/10;
                count[i1][j1] += 10;
                a2[i1][j1][s] = k;  
            }
        }
    zero_array(a1, nullptr, N);
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            int i2, j2;
            if (count[i][j] >= 11 || count[i][j]%10 > 1)
                count[i][j] -= count[i][j]%10;  // ruin bubbles
            if (count[i][j]/10 > 1) // there is no balloon, which want this cell
            {
                int num_balls = count[i][j]/10;
                for (int w = 0; w < num_balls; w++)
                {
                    int orient = a2[i][j][w];
                    if (orient == -1) //this balloon wanted to go out from scene, he should stay in his cell
                        continue;
                    reverse(&orient);
                    coord_next_cell(orient, i, j, &i2, &j2);
                    count[i][j] -= 10; //clean current cell from this balloon
                    if (count[i2][j2])
                        continue; //his previous place is not free - he must crash
                    a1[i2][j2] = 30 + orient; 
                    count[i2][j2] = 10;
                }
            }
        }
    }
}

void second_try(int **a1, int ***a2, int **count, int N)
{
    int i, j;
    zero_array(nullptr, a2, N);
    for (i = 0; i < N; i++)
        for (j = 0; j < N; j++)
        {
            if (a1[i][j] >= 30)
            {
                int i1 = 0, j1 = 0, k = a1[i][j] - 30;
                coord_next_cell(k, i, j, &i1, &j1);
                check_boundary(&k, i, j, &i1, &j1, N);
                if (count[i1][j1] == 10 || k == -1)
                {
                    if (k != -1)
                    {
                        reverse(&k);
                        a1[i][j] = 30 + k; //the selected cell is occupied => don't move and change traectory
                    }
                    else
                    {
                        a1[i][j] = 0; // the selected place is abroad scene => don't move anymore
                        count[i][j] = 10;
                    }
                    continue;
                }
                else
                {
                    count[i][j] -= 10;
                    int s = count[i1][j1]/40;
                    count[i1][j1] += 40;
                    a2[i1][j1][s] = k;
                }
            }
        }
    zero_array(a1, nullptr, N);
    for (i = 0; i < N; i++)
        for (j = 0; j < N; j++)
        {
            if (count[i][j] == 40)
            {
                count[i][j] = 10;
                continue;
            }
            if (count[i][j] > 40)
            {
                int num_balls = count[i][j]/40;
                for (int w = 0; w < num_balls; w++)
                {
                    int i2, j2, orient = a2[i][j][w];
                    if (orient == -1) //it is ball, which want to go abroad scene
                    {
                        count[i][j] -= 30; // because every balls have weigth 40, as a result count[i][j] == 10
                        continue;
                    }
                    reverse(&orient);
                    coord_next_cell(orient, i, j, &i2, &j2);
                    count[i][j] -= 40;
                    if (count[i2][j2]) //his previous place is not free - he must crash
                        continue;
                    a1[i2][j2] = 30 + orient;
                    count[i2][j2] = 10;
                }
            }
        }
}
void third_try(int **a1, int ***a2, int **count, int N)
{
    second_try(a1, a2, count, N);
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
            delete [] a2[i][j];
        delete [] a1[i];
        delete [] a2[i];
    }
}
