# include <stdio.h>
# include <stdlib.h>
# include <time.h>

# include "./ourMath.h"

int main(void) {

    
    // mat2 matrix = {
    //     {3.0f, 0.4f},
    //     {5.0f, 2.0f}
    // };

    // printf("%f \n", det2(matrix, NULL));

    // mat3 _3dMatrix = {
    //     {2.0f, 0.3f, 4.0f},
    //     {5.0f, 2.0f, 2.0f},
    //     {6.0f, 3.0f, 3.0f}
    // };
    // printf("%f \n", det3(_3dMatrix, NULL));

    // printMatrix3(_3dMatrix);

    // for(size_t i = 0; i < 100000; ++i) {
    //     size_t a, b;
        
    //     // Generate Random Matrix
    //     srand((time(NULL)+i )% 31);
    //     for(a = 0; a < 3; ++a) 
    //         for(b = 0; b < 3; ++b)
    //             _3dMatrix[a][b] =(float) ((float) rand() / (float) RAND_MAX) * 10.0f;
        
    //     printf("The Determinent Of : \n");
    //     printMatrix3(_3dMatrix);
    //     printf("\t\t Is %f \n", det3(_3dMatrix, NULL));
    // }

    mat4 _4dMatrix = {
        {3.0f, 2.0f, 4.0f, 1.0f},
        {9.0f, 8.0f, 3.0f, 2.0f},
        {3.0f, 5.0f, 6.0f, 4.0f},
        {5.0f, 2.0f, 3.0f, 5.0f}
    };
    // printf("%f \n", det4(_4dMatrix, NULL));

    // printMatrix4(_4dMatrix);
    // putchar('\n');
    // inverse4(_4dMatrix, NULL);
    // putchar('\n');
    // printMatrix4(_4dMatrix);

    // printf("%u \n", sizeof(mat3));
    mat4 Matrix4D = {
        {1.0f, 2.0f, 3.0f, 4.0f},
        {2.0f, 3.0f, 4.0f, 1.0f},
        {3.0f, 4.0f, 1.0f, 2.0f},
        {4.0f, 1.0f, 2.0f, 3.0f}
    };


    mat4 res1 = { .0f };
    mat4 res2 = { .0f };

    (*res2)[0] = 1.0f;
    printf("%.3f \n", res2[0][0]);

    omMultiply4m4(res1, _4dMatrix, Matrix4D);
    multiplyM(MAT4,  res2, _4dMatrix, Matrix4D);

    printMatrix(MAT4, res1);
    printf("\n\n\n ------------------------- \n\n\n");
    printMatrix(MAT4, res2);


}