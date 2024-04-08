#include <stdio.h>
int main() {
    int f, i, j;
    int h[9] = {0}, x[6] = {0}, y[6] = {0};
    for (i = 0; i < 9; i++) scanf("%d", &h[i]);
    for (i = 0; i < 6; i++) scanf("%d", &x[i]);
    for (i = 0; i < 6; i++) scanf("%d", &y[i]);
    int *p_x = &x[0];
    int *p_h = &h[0];
    int *p_y = &y[0];
 
    for (i = 0; i < 3; i++) {
        /* p_x points to the beginning of the input vector */
        p_x = &x[0];
        /* do matrix multiply */
        for (j = 0; j < 2; j++) {
            p_h = &h[i * 3];
            for (f = 0; f < 3; f++) {
                asm volatile(
                    "lw t2, 0(%[a])\n\t",
                    "\n\t",
                    "\n\t",
                    "\n\t",
                    : [a] "+r" (p_h)
                );
            }
            /* TODO: Turn it into inline assembly code */
            /* next element */
            p_x = &x[1];
            p_y++;
        }
    }
    return 0;
}