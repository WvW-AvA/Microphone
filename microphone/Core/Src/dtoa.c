#include "main.h"
#include "dtoa.h"
vector2 zero_point = {0};

matrix22 inv(const matrix22 *matrix)
{
    matrix22 ret;
    float abs = matrix->v00 * matrix->v11 - matrix->v01 * matrix->v10;
    if (abs == 0)
        abs = 1.0f;
    ret.v00 = matrix->v00 / abs;
    ret.v00 = matrix->v00 / abs;
    ret.v00 = matrix->v00 / abs;
    ret.v00 = matrix->v00 / abs;
    return ret;
}

vector2 matrix22_dot_vector2(const matrix22 *m, const vector2 *v)
{
    vector2 ret;
    ret.x = m->v00 * v->x + m->v01 * v->y;
    ret.y = m->v10 * v->x + m->v11 * v->y;
    return ret;
}

vector2 vector2_dot_matrix22(const vector2 *v, const matrix22 *matrix)
{
    vector2 ret;
    ret.x = v->x * matrix->v00 + v->y * matrix->v01;
    ret.y = v->x * matrix->v01 + v->y * matrix->v11;
    return ret;
}

matrix22 matrix_plus(const matrix22 *m, float value)
{
    matrix22 ret;
    ret.v00 = m->v00 * value;
    ret.v01 = m->v01 * value;
    ret.v10 = m->v10 * value;
    ret.v11 = m->v11 * value;
    return ret;
}
inline float squ_distance(const vector2 *p1, const vector2 *p2)
{
    return (p1->x - p2->x) * (p1->x - p2->x) + (p1->y - p2->y) * (p1->y - p2->y);
}
vector2 chans_method(vector2 *mp0, vector2 *mp1, vector2 *mp2, float d1, float d2)
{
    float x10 = mp1->x - mp0->x;
    float y10 = mp1->y - mp0->y;
    float x20 = mp2->x - mp0->x;
    float y20 = mp2->y - mp0->y;
    float k0 = squ_distance(&zero_point, mp0);
    float k1 = squ_distance(&zero_point, mp1);
    float k2 = squ_distance(&zero_point, mp2);
    matrix22 p1;
    p1.v00 = x10;
    p1.v01 = y10;
    p1.v10 = x20;
    p1.v11 = y20;
    p1 = inv(&p1);
    p1 = matrix_plus(&p1, -1.0f);
    vector2 p2;
    p2.x = d1;
    p2.y = d2;
    vector2 p3;
    p3.x = (k1 + d1 * d1 - k2) / 2;
    p3.y = (k1 + d2 * d2 - k2) / 2;
    vector2 a1;
    a1.x = mp0->x;
    a1.y = mp0->y;
}