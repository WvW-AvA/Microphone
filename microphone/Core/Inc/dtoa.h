#ifndef __DTOA_H__
#define __DTOA_H__

typedef struct
{
    float x;
    float y;
} vector2;

typedef struct
{
    float v00;
    float v01;
    float v10;
    float v11;
} matrix22;
extern vector2 zero_point;
matrix22 inv(matrix22 matrix);
vector2 matrix22_dot_vector2(matrix22 m, vector2 v);
vector2 vector2_dot_matrix22(vector2 v, matrix22 matrix);
matrix22 matrix_plus(matrix22 m, float value);
vector2 vector2_plus(vector2 v, float value);
matrix22 matrix_transform(matrix22 m);
float vector2_dot_vector2(vector2 v1, vector2 v2);
float squ_distance(vector2 p1, vector2 p2);
float vector2_dot_vector2(vector2 v1, vector2 v2);
vector2 vector2_add_vector2(vector2 v1, vector2 v2);
vector2 chans_method(vector2 mp0, vector2 mp1, vector2 mp2, float d1, float d2);

vector2 genfunc(float x1, float b_y1, float x2, float y2, float x3, float y3, float s1, float s2);
vector2 chans_meth(vector2 mp0, vector2 mp1, vector2 mp2, float d1, float d2);

void print_vector(vector2 v);
#endif