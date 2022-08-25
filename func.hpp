#ifndef FUN
#define FUN

void zero_array(int **x2, int ***x3, int N);
void reverse(int *orient);
void coord_next_cell(int k, int i, int j, int *i1, int *j1);
void check_boundary(int *k, int i, int j, int *i1, int *j1, int N);
void first_try(int **a1, int ***a2, int **count, int N);
void second_try(int **a1, int ***a2, int **count, int N);
void third_try(int **a1, int ***a2, int **count, int N);

#endif
