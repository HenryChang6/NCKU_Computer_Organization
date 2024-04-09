#include<stdio.h>
int main()
{ 
    int i = 0;
    int h[9] = {0}, x[6] = {0}, y[6] = {0}; 
    FILE *input = fopen("../input/4.txt", "r");
    for(i = 0; i < 9; i++) fscanf(input, "%d", &h[i]);
    for(i = 0; i < 6; i++) fscanf(input, "%d", &x[i]);
    for(i = 0; i < 6; i++) fscanf(input, "%d", &y[i]);
    fclose(input);
    
    int *p_x = &x[0];
    int *p_h = &h[0];
    int *p_y = &y[0];
    
    asm volatile(
        // Initialization
        "li t0, -1\n\t"
        "li t1, 2\n\t"               // t1 = 2 for i
        "li t2, 1\n\t"               // t2 = 1 for j
        "li t3, 2\n\t"               // t3 = 2 for f
        // Start ForI Loop
        "ForI:\n\t" "beq t1, t0, ForIEND\n\t"    // If i < 0, END ForI
            // Start ForJ Loop
            "ForJ:\n\t" "beq t2, t0, ForJEND\n\t"    // If j < 0, END ForJ
                // Start ForF Loop
                "ForF:\n\t" "beq t3, t0, ForFEND\n\t"    // If f < 0, END ForF

                    // p_x = &x[2 * f + j];
                    "li t4, 0\n\t"              // hold's p_x offset
                    "slli t4, t3, 1\n\t"        // t4 = f << 1 (same as f * 2)
                    "add t4, t4, t2\n\t"        // t4 = 2 * f + j   
                    "slli t4, t4, 2\n\t"        // t4 = 4 * (2 * f + j)
                    "add t4, t4, %[p_x]\n\t"    // t4 = x[2 * f + j]  

                    // p_h = &h[3 * i + f];
                    "li t5, 0\n\t"              // hold's p_h offset
                    "slli t5, t1, 1\n\t"        // t5 = 2 * i
                    "add t5, t5, t1\n\t"        // t5 = 3 * i 
                    "add t5, t5, t3\n\t"        // t5 = 3 * i + f
                    "slli t5, t5, 2\n\t"        // t5 = (3 * i + f) * 4
                    "add t5, t5, %[p_h]\n\t"    // t5 = h[3 * i + f]

                    // p_y = &y[2 * i + j];
                    "li t6, 0\n\t"              // hold's p_y offset
                    "slli t6, t1, 1\n\t"        // t6 = 2 * i
                    "add t6, t6, t2\n\t"        // t6 = 2 * i + j
                    "slli t6, t6, 2\n\t"        // t6 = (2 * i + j) * 4 
                    "add t6, t6, %[p_y]\n\t"    // t6 = y[2 * i + j] 
                    // *p_y = (*p_y) + (*p_h) * (*p_x);
                    "lw t4, 0(t4)\n\t"          // load (*p_x) into t4
                    "lw t5, 0(t5)\n\t"          // load (*p_h) into t5
                    "mul t4, t4, t5\n\t"        // t4 = (*p_h) * (*p_x)
                    "lw t5, 0(t6)\n\t"          // load (*p_y) into t5
                    "add t4, t4, t5\n\t"        // t4 = (*p_h) * (*p_x) + (*p_y)
                    "sw t4, 0(t6)\n\t"          // *p_y = t4

                    "addi t3, t3, -1\n\t"       // update's f's value
                    "j ForF\n\t"                // jump back to ForF

                "ForFEND:\n\t"              // ForF end
                "addi t2, t2, -1\n\t"       // update j's value
                "j ForJ\n\t"                // jump back to ForJ

            "ForJEND:\n\t"
            "addi t1, t1, -1\n\t"         // update i's value
            "j ForI\n\t"                 // jump back to ForI
            
        "ForIEND:\n\t"               // ForI End
        :[p_x] "+r" (p_x), [p_h] "+r" (p_h), [p_y] "+r" (p_y)
        :[x] "r" (x), [h] "r" (h), [y] "r" (y)
        :"t0", "t1", "t2", "t3", "t4", "t5", "t6"
    );

    p_y = &y[0];
    for(i = 0; i < 6; i++)
        printf("%d ", *p_y++);
    printf("\n");
    return 0; 
 
}
