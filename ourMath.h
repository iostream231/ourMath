# include <stdio.h>
# include <stdarg.h>
# include <math.h>

typedef float mat3[3][3]; // Matrices Type
typedef float mat4[4][4];
typedef float mat2[2][2];

typedef float vec2[2];
typedef float vec3[3];
typedef float vec4[4];

enum type {MAT2, MAT3, MAT4, VEC2, VEC3, VEC4};
enum params {ROW_MAJOR=0x1, COUNTER_CLOCKE_WISE=2};

void omGenTranslate4(mat4 data, float x, float y, float z, char rowMajor);
void omGenScale4(mat4 data, float x, float y, float z, char rowMajor);
void omGenRotation4(mat4 data, float x, float y, float z, char);

void transpose4(mat4 data, mat4);
void multiply4m4(mat4 , mat4, mat4);
void multiply4f(mat4 , float , mat4);
void adjugate4(mat4 , mat4 );
void copy4t4(mat4 , mat4);
void inverse4(mat4, mat4);


float det2(mat2, float *);
float det3(mat3, float *);


void printMatrix(enum type , ...);


/**
 * @name Translate Matrix
 * @brief Generates A Translation Matrix And Stores It In `data`,
 * 
 * @param data where the matrix is to be stored. needs to be filled with .0f
 * @param x translation value on the x-axis
 * @param y translation valur on the y-axis
 * @param z translation value on the z-axis
 * @param rowMajor Specifies How The Outputed Matrix Should Be Presented. Influences parameter `transpose` in glUniformMatrix...()
 * 
 * @example translate(dat, .4, -.3f, .0f) ; Moves each vertex a little bit into the right and some into the bottom
 */
void omGenTranslate4(mat4 data, float x, float y, float z, char rowMajor) {

    // Init Coords
    data[0][0] = 1.0f;
    data[1][1] = 1.0f;
    data[2][2] = 1.0f;
    data[3][3] = 1.0f;


    // The Translation
    data[0][3] = x;
    data[1][3] = y;
    data[2][3] = z;

    if(!rowMajor)
        transpose4(data, NULL);
};

/**
 * @brief Generates A Rotation Matrix In CounterClockWise;
 * 
 * @param data Wheee The Matrix Is To Be Stored
 * @param x Degrees Around The X-Axis (In Rad)
 * @param y Degrees Around The Y-Axis (In Rad)
 * @param z Degrees Around The Z-Axis (In Rad)
 * @param Parameters Supports Two Bitfields, As Of Now : + COUNTER_CLOCK_WISE if set the rotation will be counter clock wise\
 *                    + ROW_MAJOR if the returned value should be row major or column major
 */

void omGenRotation4(mat4 data, float x, float y, float z, char Parameters) {
    if(!(Parameters & COUNTER_CLOCKE_WISE))
        z = 2*M_PI - z, x = 2*M_PI - x, y = 2*M_PI - y;
    
    // Around Z Axis 
    mat4 zData = { .0f };
    zData[0][0] = cos(z);
    zData[0][1] = -sin(z);
    zData[1][0] = cos(z);
    zData[1][1] = sin(z);

    zData[2][2] = 1.0f;
    zData[3][3] = 1.0f;


    // Around Y Axis
    mat4 yData = { .0f };
    yData[0][0] = cos(y);
    yData[0][2] = sin(y);
    yData[2][0] = -sin(y);
    yData[2][2] = cos(y);

    yData[1][1] = 1.0f;
    yData[3][3] = 1.0f;


    // Around X Axis
    mat4 xData = { .0f };
    xData[1][1] = cos(x);
    xData[1][2] = -sin(x);
    xData[2][1] = sin(x);
    xData[2][2] = cos(x);

    xData[0][0] = 1.0f;
    xData[3][3] = 1.0f;


    // Multiplication R = Rz*Ry*Rx
    mat4 y_x = { .0f };
    multiply4m4(y_x, yData, xData);
    multiply4m4(data, zData, y_x);

    if(!(Parameters & ROW_MAJOR))
        transpose4(data, NULL);
}



/**
 * @name Multiplication Matrix
 * @brief Multiplies Matrix A by Matrix B and stores the result in Matrix C, basically : [ C = BA ]. The Outputed Matrix `res` is in Column-Major order, therfor transpose must be GL_FALSE for glUniformMatrix..()
 * 
 * @param res The Resultin Matrix
 * @param a Matrix A as In res = BA
 * @param b Matrix B as In res = BA
 * 
 * @note If you want to scale a geometry linearly, set x=y=z=f With f is the factor of scaling
 */
void multiply4m4(mat4 res, mat4 b, mat4 a) {
    for(size_t i = 0; i < 4; ++i) 
        for(size_t j = 0; j < 4; ++j) 
            res[i][j] = a[0][i] * b[0][j] + a[1][i] * b[1][j] + a[2][i] * b[2][j] + a[3][i] * b[3][j];

    
}

/**
 * @name Scale Matrix
 * @brief Generates A Scaling Matrix And Stores It In `data`,.
 * 
 * @param data where the generated matrix is to be stored, needs to be new.
 * @param x scaling on the X-axis
 * @param y scaling on the Y-axis
 * @param z scaling on the Z-axis
 * @param rowMajor Specifies How The Outputed Matrix Should Be Presented. Influences parameter `transpose` in glUniformMatrix...()
 * 
 * @note If you want to scale a geometry linearly, set x=y=z=f With f is the factor of scaling
 */
void omGenScale4(mat4 data, float x, float y, float z, char rowMajor) {
    if(x*y*z == 0.0f) 
        printf("Warning: One Of Your Scaling Factors Is 0 .\n");    
    // One Of The Is Null

    // The Scaling
    data[0][0] = x;
    data[1][1] = y;
    data[2][2] = z;
    data[3][3] = 1.0f;

    if(rowMajor != 1)
        transpose4(data, NULL);
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

/**
 * @brief Calculates the Determinenet of a 3D Column-Majored Matrix;
 * 
 * @param data The 3x3 Column-Majored Matrix that its data is to be calculated
 * @param det3 A Pointer to a float value where the determinent is to be saved. Can be NULL if not necessary.
 * @return float Returns The Determinant
 */
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

    return (det3 != NULL) ? (*det3 = data[0][0]*det2(matrix1, NULL) - data[0][1]*  det2(matrix2, NULL) + data[0][2]*  det2(matrix3, NULL)) \
    :  data[0][0]*det2(matrix1, NULL) - data[0][1] * det2(matrix2, NULL) + data[0][2] * det2(matrix3, NULL);
}



// * Supposed to be a mpre optimized function, but clearly has no effect.
// float det3Opt(mat3 data, float * det3) {
//     return (det3 != NULL) ? (*det3 = (data[0][0] * (data[1][1] * data[2][2] - data[1][2] * data[2][1])) \
//         - (data[0][1] * (data[1][1] * data[2][2] - data[1][2] * data[2][0])\
//         + (data[0][2] * (data[1][0] * data[2][1] - data[1][1] * data[2][0])))) : \
//         (data[0][0] * (data[1][1] * data[2][2] - data[1][2] * data[2][1])) \
//         - (data[0][1] * (data[1][1] * data[2][2] - data[1][2] * data[2][0])\
//         + (data[0][2] * (data[1][0] * data[2][1] - data[1][1] * data[2][0])));

// } 

/**
 * @brief Calculates The Determinent of a 4x4 Matrix. (In A Column-Major order)
 * 
 * @param data The Matrix that's data is to be calculated.
 * @param det4 A Pointer If You want to store the data. Can Be NULL.
 * @return float The determinent.
 */

float det4(mat4 data, float * det4){ 
    mat3 matrix1 = {
        {data[1][1], data[1][2], data[1][3]},
        {data[2][1], data[2][2], data[2][3]},
        {data[3][1], data[3][2], data[3][3]}
    };

    mat3 matrix2 = {
        {data[1][0], data[1][2], data[1][3]},
        {data[2][0], data[2][2], data[2][3]},
        {data[3][0], data[3][2], data[3][3]}
    };
    
    mat3 matrix3 = {
        {data[1][0], data[1][1], data[1][3]},
        {data[2][0], data[2][1], data[2][3]},
        {data[3][0], data[3][1], data[3][3]}
    };

    mat3 matrix4 = {
        {data[1][0], data[1][1], data[1][2]},
        {data[2][0], data[2][1], data[2][2]},
        {data[3][0], data[3][1], data[3][2]}
    };

    return (det4 != NULL) ? (*det4 = data[0][0]*det3(matrix1, NULL) - data[0][1]*det3(matrix2, NULL) + data[0][2]*det3(matrix3, NULL) - data[0][3]*det3(matrix4, NULL)) \
    : (data[0][0]*det3(matrix1, NULL) - data[0][1]*det3(matrix2, NULL) + data[0][2]*det3(matrix3, NULL) - data[0][3]*det3(matrix4, NULL));
}

/**
 * @brief Removes An Index From A List of Indices. Used Specifically to calculate Adjunts of a matrix (Remove i-th row / j-th column)
 * 
 * @param lst The List Of Indices To Remove The Index From
 * @param ind The Index Of The Index To Be Removed.
 * @param sz the Size of the original List
 * @return char* A List With The Index Specified Removed.
 */
char * remIndFromLst(char lst[], uint ind, uint sz) {
    char * res = malloc((sz - 1) * sizeof(char));
    for(size_t i = 0, j = 0; j < sz-1 && sz; ++i)
        if(i == ind) 
            continue;
        else 
            res[j++] = lst[i];
        
    return res;

}


/**
 * @brief Calculates The Adjunct of A matrix and stores it in Res.
 * 
 * @param res The Matrix Where The Adjunct is to be stored.
 * @param data The Matrix That's Adjunct is to be calculated.
 */
void adjugate(mat4 res, mat4 data) {

    char indices[4] = {0, 1, 2, 3};

    for(size_t i = 0; i < 4; ++i) {
        char * Cdices = remIndFromLst(indices, i, 4);

        for(size_t j = 0; j < 4; ++j) {
            char * Rdices = remIndFromLst(indices,j, 4);
            mat3 matrix = {
                { data[Cdices[0]][Rdices[0]], data[Cdices[0]][Rdices[1]], data[Cdices[0]][Rdices[2]] },
                { data[Cdices[1]][Rdices[0]], data[Cdices[1]][Rdices[1]], data[Cdices[1]][Rdices[2]] },
                { data[Cdices[2]][Rdices[0]], data[Cdices[2]][Rdices[1]], data[Cdices[2]][Rdices[2]] }
            };

            res[i][j] = (i + j) % 2 == 0 ? det3(matrix, NULL) : -det3(matrix, NULL);

        }
    }
    transpose4(res, NULL);
}

/**
 * @brief Multiplies A 4x4 Matrix By A Value f, and Optionally Stores The Result in res.
 * 
 * @param data The Matrix to be multiplied, if you want it to be conserved pass another Matrix As Res.
 * @param f the value to multiply the matrix by.
 * @param res Optional (Can Be NULL), if you want data to not change you can store the results in another independent matrix by passing it here.
 */
void multiply4f(mat4 data, float f, mat4 res) {
    if(res == NULL) 
        for(size_t i = 0; i < 4; ++i) 
            for(size_t j = 0; j < 4; ++j)
                data[i][j] *= f;

    else 
        for(size_t i = 0; i < 4; ++i) 
            for(size_t j = 0; j < 4; ++j)
                res[i][j] = data[i][j] * f;
}


/**
 * @brief Copies Data From One 4x4 Matrix Into Another 4x4 Matrix
 * 
 * @param from The 4D Matrix to be copied from.
 * @param to The 4D Matrix To be copied to.
 */
void copy4t4(mat4 from, mat4 to) {
    for(size_t i = 0; i < 4; ++i) 
        for(size_t j = 0; j < 4; ++j)
            to[i][j] = from[i][j];
    
}


/**
 * @brief Calculates The Inverse of a matrix, and (Optionally) stores it in another one.
 * 
 * @param data The Matrix That's Inverse Is To Be Calculated
 * @param res Where To Store the Inversed matrix, if not specified `data` will be changed to store the inversed matrix
 */
void inverse4(mat4 data, mat4 res) {
    mat4 adj = {.0f};
    adjugate(adj, data);

    
    multiply4f(adj, (1.0f / det4(data, NULL)), NULL);

    if(res != NULL) 
        copy4t4(adj, res);
    else 
        copy4t4(adj, data);
}

/**
 * @brief Transposes A 4x4 Matrix : Switches Its Order. And Strores The Results (Optionally) In a Result Matrix.
 * 
 * @param data The Matrix To Be Transposed, if you dont want to Modify it you need to specify a `res` matrix where transposed data is to be stored.
 * @param res The Result Matrix Where The Transposed Matrix is to be stored. If specified as NULL, the main matrix is modified
 */
void transpose4(mat4 data, mat4 res) {
    if(res == NULL) {
        mat4 matx = {.0f};
        for(size_t i = 0; i < 4; ++i) 
            for(size_t j = 0; j < 4; ++j)
                matx[j][i] = data[i][j];
        copy4t4(matx, data);
        return ;
    } else 
        for(size_t i = 0; i < 4; ++i) 
            for(size_t j = 0; j < 4; ++j)
                res[j][i] = data[i][j];
}

/**
 * @brief Prints A User-Specified Matrix.
 * 
 * @param tp The Type Of The Matrix. Current Supported Types Are MAT2, MAT3 and MAT4.
 * @param ... the matrix to be printed.
 */
void printMatrix(enum type tp, ...) {
    va_list arg;

    va_start(arg, tp);

    mat2 * matrix2;
    mat3 * matrix3;
    mat4 * matrix4;

    switch(tp) {
        case MAT2 :
            matrix2 = va_arg(arg, float*);
            for(size_t i = 0; i < 2; ++i) {
                printf("[ ");
                for(size_t j = 0; j < 2; ++j) {
                    printf("%.4f, ", (*matrix2)[i][j]);
                }
                printf(" ]");
                putchar('\n');
            }
            break;
        case MAT3 :
            matrix3 = va_arg(arg, float*);
            for(size_t i = 0; i < 3; ++i) {
                printf("[ ");
                for(size_t j = 0; j < 3; ++j) {
                    printf("%.4f, ", (*matrix3)[i][j]);
                }
                printf(" ]");
                putchar('\n');
            }
            break;
        case MAT4 :
            matrix4 = va_arg(arg, float*);
            for(size_t i = 0; i < 4; ++i) {
                printf("[ ");
                for(size_t j = 0; j < 4; ++j) {
                    printf("%.4f, ", (*matrix4)[i][j]);
                }
                printf(" ]");
                putchar('\n');
            }
            break;
        default :
            perror("Unsupported Type :( \n");
            exit(EXIT_FAILURE);
    }



    va_end(arg);
}




/**
 * @brief Generates An Orthographic Projection Matrix. 
 * 
 * @param data The Matrix Where Result Is To Be Stored
 * @param position The Position of The Bottom-Right-Near Corner Of The Cube. (X, Y, Z) order
 * @param size The Size Of The Cube (X:W, Y:H, Z:Z)
 * @param isRowMajor Specify If The Cube Needs To Be Trasposed.
 */
void omGenOrthographicProjection(mat4 data, vec3 position, vec3 size, char isRowMajor) {
    mat4 translateToCenter = { .0f };
    translateToCenter[0][0] = translateToCenter[1][1] = translateToCenter[2][2] = translateToCenter[3][3] = 1.0f;
    translateToCenter[0][3] = -(2 * position[0] + size[0]) / 2.0f;
    translateToCenter[1][3] = -(2 * position[1] + size[1]) / 2.0f;
    translateToCenter[2][3] = -(2 * position[2] + size[2]) / 2.0f;

    mat4 scaleToStd = { .0f };
    scaleToStd[0][0] = 2.0f / size[0];
    scaleToStd[1][1] = 2.0f / size[1];
    scaleToStd[2][2] = 2.0f / size[2];
    scaleToStd[3][3] = 1.0f;   // NOTE: This One May Cause Problems

    multiply4m4(data, scaleToStd, translateToCenter);
    if(!isRowMajor)
        transpose4(data, NULL);
}

