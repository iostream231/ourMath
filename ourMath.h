# include <stdio.h>


typedef float mat3[3][3]; // Matrices Type
typedef float mat4[4][4];
typedef float mat2[2][2];


/**
 * @name Translate Matrix
 * @brief Generates A Translation Matrix And Stores It In `data`, the generated matrix is in row-major order, therefor `transpose` in glUniformMatrix...() must be GL_TRUE
 * 
 * @param data where the matrix is to be stored. needs to be filled with .0f
 * @param x translation value on the x-axis
 * @param y translation valur on the y-axis
 * @param z translation value on the z-axis
 * 
 * @example translate(dat, .4, -.3f, .0f) ; Moves each vertex a little bit into the right and some into the bottom
 */
void translate4(mat4 data, float x, float y, float z) {

    // Init Coords
    data[0][0] = 1.0f;
    data[1][1] = 1.0f;
    data[2][2] = 1.0f;
    data[3][3] = 1.0f;


    // The Translation
    data[0][3] = x;
    data[1][3] = y;
    data[2][3] = z;

};

/**
 * @name Multiplication Matrix
 * @brief Multiplies Matrix A by Matrix B and stores the result in Matrix C, basically : [ C = BA ]. The Outputed Matrix `res` is in Column-Major order, therfor transpose must be GL_DALSE for glUniformMatrix..()
 * 
 * @param res The Resultin Matrix
 * @param a Matrix A as In res = BA
 * @param b Matrix B as In res = BA
 * 
 * @note If you want to scale a geometry linearly, set x=y=z=f With f is the factor of scaling
 */
void multiply4(mat4 res, mat4 b, mat4 a) {
    for(size_t i = 0; i < 4; ++i) 
        for(size_t j = 0; j < 4; ++j) 
            res[i][j] = b[0][i]*a[0][j] + b[1][i] * a[1][j] + b[2][i]*a[2][i] + b[3][i] * a[3][j];

    
}

/**
 * @name Scale Matrix
 * @brief Generates A Scaling Matrix And Stores It In `data`, the generated matrix is in row-major order, therefor `transpose` in glUniformMatrix...() must be GL_TRUE.
 * 
 * @param data where the generated matrix is to be stored, needs to be new.
 * @param x scaling on the X-axis
 * @param y scaling on the Y-axis
 * @param z scaling on the Z-axis
 * 
 * @note If you want to scale a geometry linearly, set x=y=z=f With f is the factor of scaling
 */
void scale4(mat4 data, float x, float y, float z) {

    if(x*y*z == 0.0f) 
        printf("Warning: One Of Your Scaling Factors Is 0 .\n");    
    // One Of The Is Null

    // The Scaling
    data[0][0] = x;
    data[1][1] = y;
    data[2][2] = z;
    data[3][3] = 1.0f;
}



/**
 * @brief Calculates The Determinant of a 2D Column-Majored Matrix.
 * 
 * @param data The Matrix that its determinant is to be calculated
 * @param det2 A Pointer to a float value where the determinent is to be saved. Can be NULL if not necessary.
 * @return float Returns The Determinant
 */
float det2(mat2 data, float * det2) {

     return det2 != NULL ? (*det2 = data[0][0] * data[1][1] - data[0][1] * data[1][0]) : data[0][0] * data[1][1] - data[0][1] * data[1][0];
    
}

float det3(mat3 data, float * det3) {
    mat2 matrix1 = {
        {data[1][1], data[1][2]},
        {data[2][1], data[2][2]}
    };
    mat2 matrix2 = {
        {data[1][0], data[1][2]}, 
        {data[2][0], data[2][2]},
    };
    mat2 matrix3 = {
        {data[1][0], data[1][1]},
        {data[2][0], data[2][1]}
    };

    return (det3 != NULL) ? (*det3 = det2(matrix1, NULL) + det2(matrix2, NULL) + det2(matrix3, NULL)) \
    :  det2(matrix1, NULL) + det2(matrix2, NULL) + det2(matrix3, NULL);
}


// float det3(mat3 data, float * det3) {
    
//     return (data[1][1] * data[2][2] - data[1][2] * data[2][1]) + \
//             (data[1][0] * data[2][2] - data[1][2] * data[2][0]) + \
//             (data[1][0])
// }

