#pragma once

#include "Edge.h"

double Det3x3(double* col_0, double* col_1, double* col_2)
{
    double a = col_0[0];
    double b = col_1[0];
    double c = col_2[0];
    double d = col_0[1];
    double e = col_1[1];
    double f = col_2[1];
    double g = col_0[2];
    double h = col_1[2];
    double i = col_2[2];

    double det = a * (e * i - f * h) - b * (d * i - f * g) + c * (d * h - e * g);
    return det;
}

double Det4x4(double* col_0, double* col_1, double* col_2, double* col_3)
{
    double a = col_0[0];
    double b = col_1[0];
    double c = col_2[0];
    double d = col_3[0];
    double e = col_0[1];
    double f = col_1[1];
    double g = col_2[1];
    double h = col_3[1];
    double i = col_0[2];
    double j = col_1[2];
    double k = col_2[2];
    double l = col_3[2];
    double m = col_0[3];
    double n = col_1[3];
    double o = col_2[3];
    double p = col_3[3];

    double adet
        = a * ((f * k * p) - (f * l * o) - (g * j * p) + (g * l * n) + (h * j * o) - (h * k * n));
    double bdet
        = b * ((e * k * p) - (e * l * o) - (g * i * p) + (g * l * m) + (h * i * o) - (h * k * m));
    double cdet
        = c * ((e * j * p) - (e * l * n) - (f * i * p) + (f * l * m) + (h * i * n) - (h * j * m));
    double ddet
        = d * ((e * j * o) - (e * k * n) - (f * i * o) + (f * k * m) + (g * i * n) - (g * j * m));

    double det = adet - bdet + cdet - ddet;
    return det;
}

bool InCircle(Vert* a, Vert* b, Vert* c, Vert* d)
{
    double m[4][4] = { { a->x(), b->x(), c->x(), d->x() },{ a->y(), b->y(), c->y(), d->y() },
    { a->lengthsquared(), b->lengthsquared(), c->lengthsquared(), d->lengthsquared() },
    { 1, 1, 1, 1 } };

    return Det4x4(m[0], m[1], m[2], m[3]) > 0;
}

bool CCW(Vert* a, Vert* b, Vert* c)
{
    float a_x = a->x();
    float a_y = a->y();
    float b_x = b->x();
    float b_y = b->y();
    float c_x = c->x();
    float c_y = c->y();

    double m[3][3] = { { a_x, b_x, c_x },{ a_y, b_y, c_y },{ 1, 1, 1 } };

    return Det3x3(m[0], m[1], m[2]) > 0;
}

bool LeftOf(Edge* e, Vert* z)
{
    return CCW(z, e->origin(), e->destination());
}

bool RightOf(Edge* e, Vert* z)
{
    return CCW(z, e->destination(), e->origin());
}

bool Valid(Edge* e, Edge* base_edge)
{
    return RightOf(base_edge, e->destination());
}