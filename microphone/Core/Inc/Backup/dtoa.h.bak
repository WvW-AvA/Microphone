#ifndef __DTOA_H__
#define __DTOA_H__

typedef struct
{
    float x;
    float y;
    /* data */
} vector2;

typedef struct
{
    float v00;
    float v01;
    float v10;
    float v11;
} matrix22;
extern vector2 zero_point;
matrix22 inv(const matrix22 *matrix);
vector2 vector2_dot_matrix22(const vector2 *v, const matrix22 *matrix);
matrix22 matrix_plus(const matrix22 *m, float value);
float squ_distance(const vector2 *p1, const vector2 *p2);
#endif