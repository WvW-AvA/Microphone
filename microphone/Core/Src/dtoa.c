#include "dtoa.h"
#include "math.h"
vector2 zero_point = {0};

inline matrix22 inv(matrix22 matrix)
{
    matrix22 ret;
    float abs = matrix.v00 * matrix.v11 - matrix.v01 * matrix.v10;
    if (abs == 0)
        abs = 1.0f;
    ret.v00 = matrix.v00 / abs;
    ret.v01 = matrix.v01 / abs;
    ret.v10 = matrix.v10 / abs;
    ret.v11 = matrix.v11 / abs;
    return ret;
}

inline vector2 matrix22_dot_vector2(matrix22 m, vector2 v)
{
    vector2 ret;
    ret.x = m.v00 * v.x + m.v01 * v.y;
    ret.y = m.v10 * v.x + m.v11 * v.y;
    return ret;
}

inline vector2 vector2_dot_matrix22(vector2 v, matrix22 matrix)
{
    vector2 ret;
    ret.x = v.x * matrix.v00 + v.y * matrix.v01;
    ret.y = v.x * matrix.v01 + v.y * matrix.v11;
    return ret;
}

inline matrix22 matrix_plus(matrix22 m, float value)
{
    matrix22 ret;
    ret.v00 = m.v00 * value;
    ret.v01 = m.v01 * value;
    ret.v10 = m.v10 * value;
    ret.v11 = m.v11 * value;
    return ret;
}

inline vector2 vector2_plus(vector2 v, float value)
{
    vector2 ret;
    ret.x = v.x * value;
    ret.y = v.y * value;
    return ret;
}

inline matrix22 matrix_transform(matrix22 m)
{
    matrix22 ret;
    ret.v00 = m.v00;
    ret.v01 = m.v10;
    ret.v10 = m.v01;
    ret.v11 = m.v11;
    return ret;
}
inline float vector2_dot_vector2(vector2 v1, vector2 v2)
{
    return v1.x * v2.x + v1.y * v2.y;
}
inline float squ_distance(vector2 p1, vector2 p2)
{
    return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
}
inline vector2 vector2_descent_vector2(vector2 v1, vector2 v2)
{
    vector2 ret;
    ret.x = v1.x - v2.x;
    ret.y = v1.y - v2.y;
    return ret;
}
inline vector2 vector2_add_vector2(vector2 v1, vector2 v2)
{
    vector2 ret;
    ret.x = v1.x + v2.x;
    ret.y = v1.y + v2.y;
    return ret;
}
vector2 chans_method(vector2 mp0, vector2 mp1, vector2 mp2, float d1, float d2)
{
    vector2 ret = {0};
    float x10 = mp1.x - mp0.x;
    float y10 = mp1.y - mp0.y;
    float x20 = mp2.x - mp0.x;
    float y20 = mp2.y - mp0.y;
    float k0 = squ_distance(zero_point, mp0);
    float k1 = squ_distance(zero_point, mp1);
    float k2 = squ_distance(zero_point, mp2);
    matrix22 p1;
    p1.v00 = x10;
    p1.v01 = y10;
    p1.v10 = x20;
    p1.v11 = y20;
    p1 = matrix_plus(inv(p1), -1.0f);
    vector2 p2;
    p2.x = d1;
    p2.y = d2;
    vector2 p3;
    p3.x = (k1 + d1 * d1 - k2) / 2;
    p3.y = (k1 + d2 * d2 - k2) / 2;
    vector2 a1;
    a1.x = mp0.x;
    a1.y = mp0.y;
    vector2 p1_dot_p2 = matrix22_dot_vector2(p1, p2);
    vector2 p1_dot_p3 = matrix22_dot_vector2(p1, p3);
    vector2 tem = vector2_descent_vector2(p1_dot_p3, a1);
    float a = vector2_dot_vector2(p1_dot_p2, p1_dot_p2) - 1;
    float b = vector2_dot_vector2(p1_dot_p2, tem) + vector2_dot_vector2(tem, matrix22_dot_vector2(p1, p2));
    float c = vector2_dot_vector2(tem, tem);
    float delta = b * b - 4 * a * c;
    if (delta > 0)
        return ret;
    float r1 = (-b - sqrtf(delta)) / (2 * a);

    ret = vector2_add_vector2(vector2_plus(p1_dot_p2, r1), p1_dot_p3);
    return ret;
}

void genfunc(float x1, float b_y1, float x2, float y2, float x3, float y3, float s1, float s2, float *x, float *y)
{
    float k1;
    float k2;
    float k3;
    float p1;
    float q1;
    float p2;
    float q2;
    float a;
    float b;
    float b_a;
    float c_a;
    float c;
    float r1;
    float r2;
    k1 = x1 * x1 + b_y1 * b_y1;

    /*  中间值 */
    k2 = x2 * x2 + y2 * y2;
    k3 = x3 * x3 + y3 * y3;
    p1 = ((((y2 - b_y1) * (s2 * s2) - (y3 - b_y1) * (s1 * s1)) + (y3 - b_y1) * (k2 - k1)) - (y2 - b_y1) * (k3 - k1)) / (((x2 - x1) * (y3 - b_y1) - (x3 -
                                                                                                                                                    x1) *
                                                                                                                                                       (y2 - b_y1)) *
                                                                                                                        2.0F);
    q1 = ((y2 - b_y1) * s2 - (y3 - b_y1) * s1) / ((x2 - x1) * (y3 - b_y1) - (x3 -
                                                                             x1) *
                                                                                (y2 - b_y1));
    p2 = ((((x2 - x1) * (s2 * s2) - (x3 - x1) * (s1 * s1)) + (x3 - x1) * (k2 - k1)) - (x2 - x1) * (k3 - k1)) / (((x3 - x1) * (y2 - b_y1) - (x2 - x1) * (y3 -
                                                                                                                                                        b_y1)) *
                                                                                                                2.0F);
    q2 = ((x2 - x1) * s2 - (x3 - x1) * s1) / ((x3 - x1) * (y2 - b_y1) - (x2 - x1) *
                                                                            (y3 - b_y1));

    /* % 求取方程 */
    a = (q1 * q1 + q2 * q2) - 1.0F;
    b = -2.0F * ((x1 - p1) * q1 + (b_y1 - p2) * q2);
    b_a = x1 - p1;
    c_a = b_y1 - p2;
    c = b_a * b_a + c_a * c_a;
    r1 = (-b + (float)sqrt(b * b - 4.0F * a * c)) / (2.0F * a);
    r2 = (-b - (float)sqrt(b * b - 4.0F * a * c)) / (2.0F * a);
    if (r1 > 0.0F)
    {
        *x = p1 + q1 * r1;
        *y = p2 + q2 * r1;
    }
    else
    {
        *x = p1 + q1 * r2;
        *y = p2 + q2 * r2;
    }
}
vector2 chans_meth(vector2 mp0, vector2 mp1, vector2 mp2, float d1, float d2)
{
    vector2 ret;
    genfunc(mp0.x, mp0.y, mp1.x, mp1.y, mp2.x, mp2.y, d1, d2, &(ret.x), &(ret.y));
    return ret;
}