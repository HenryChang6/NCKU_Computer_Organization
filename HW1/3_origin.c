#include <stdio.h>
/*
 * description:          matrix - multiply benchmarking
 *
 *|h11 h12 h13| |x1 x2| |y1 y2| | h11*x1+h12*x3+h13*x5 h11*x2+h12*x4+h13*x6|
 *|h21 h22 h23|*|x3 x4|=|y3 y4|=| h21*x1+h22*x3+h23*x5 h21*x2+h22*x4+h23*x6|
 *|h31 h32 h33| |x5 x6| |y5 y6| | h31*x1+h32*x5+h33*x5 h31*x2+h32*x4+h33*x6|
 *
 * Element are to store in following order:
 *
 * matrix h[9]={h11,h12,h13, h21,h22,h23, h31,h32,h33}
 * vector x[6]={x1,x2, x3,x4, x5,x6}
 * vector y[6]={y1,y2, y3,y4, y5,y6}
 */
int main()
{
    int f, i, j;
    int h[9] = {0}, x[6] = {0}, y[6] = {0};
    FILE *input = fopen("../input/3.txt", "r");
    for(i = 0; i < 9; i++) fscanf(input, "%d", &h[i]);
    for(i = 0; i < 6; i++) fscanf(input, "%d", &x[i]);
    for(i = 0; i < 6; i++) fscanf(input, "%d", &y[i]);
    fclose(input);
    int *p_x = &x[0];
    int *p_h = &h[0];
    int *p_y = &y[0];

    for (i = 0; i < 3; i++) // h
    {
        /* p_x points to the beginning of the input vector */
        p_x = &x[0];
        /* do matrix multiply */
        for (j = 0; j < 2; j++) // x
        {
            p_h = &h[i * 3];
            for (f = 0; f < 3; f++)
            {
                *p_y += (*p_h++) * (*p_x);
                p_x += 2; 
            }

            /* next element */
            p_x = &x[1];
            p_y++;
        }
    }
    p_y = &y[0];
    for(i = 0; i < 6; i++) printf("%d ", *p_y++);
    printf("\n");
    return 0;
}