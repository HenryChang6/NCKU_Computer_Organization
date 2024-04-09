#include <stdio.h>
int main() {
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
 
    for (i = 0; i < 3; i++) {
        // p_x = &x[0];
        for (j = 0; j < 2; j++) {
            // p_h = &h[i * 3];
            for (f = 0; f < 3; f++) {
                asm volatile(
                    // p_x = &x[0]; 
                    "bnez %[j], None_Reset_PX\n\t" // jump if j != 0
                    "bnez %[f], None_Reset_PX\n\t" // jump if f != 0
                    "addi %[p_x], %[x], 0\n\t"
                    "None_Reset_PX:\n\t"
                    // p_h = &h[i * 3];
                    "bnez %[f], None_Reset_PH\n\t"  // if f != 0, jump to None_Reset_PH
                    "li t0, 12\n\t"                 // t0 = 3 * 4 = 12
                    "mul t1, %[i], t0\n\t"          // t1 = (i * 3) * 4
                    "add %[p_h], %[h], t1\n\t"    // p_h = &h[i * 3]
                    "None_Reset_PH:\n\t"

                    // For Loop內部：*p_y += (*p_h++) * (*p_x);
                    "lw t0, 0(%[p_h])\n\t"          // t0 = *p_h
                    "lw t1, 0(%[p_x])\n\t"          // t1 = *p_x
                    "mul t2, t0, t1\n\t"            // t2 = t0 * t1
                    "lw t1, 0(%[p_y])\n\t"          // t1 = *p_y
                    "add t3, t2, t1\n\t"            // t3 = t2 + t1 (*p_y = *p_y + (*p_h) * (*p_x))
                    "sw t3, 0(%[p_y])\n\t"          // *p_y = t3
                    "addi %[p_h], %[p_h], 4\n\t"    // p_h++

                    // For Loop內部：p_x += 2;
                    "addi %[p_x], %[p_x], 8\n\t"    // p_x += 2
                    // Next Element: p_x = &x[1];  p_y++;
                    "li t0, 2\n\t"
                    "bne %[f],t0 , None_Next_Element\n\t" // if f != 2, jump to None_Next_Element
                    "addi %[p_x], %[x], 4\n\t"
                    "addi %[p_y], %[p_y], 4\n\t"
                    "None_Next_Element:\n\t"
                    : [p_x] "+r" (p_x), [p_h] "+r" (p_h), [p_y] "+r" (p_y)
                    : [i] "r" (i), [j] "r" (j), [f] "r" (f), [x] "r" (x), [h] "r" (h)
                    : "t0", "t1", "t2","t3"
                );
            }
            // p_x = &x[1];  
            // p_y++;
        }
    }
    p_y = &y[0];
    for(i = 0; i < 6; i++) 
        printf("%d ", *p_y++);
    printf("\n");
    return 0;
}