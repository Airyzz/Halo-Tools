#include "Math.h"
#include <math.h>

#define PI 3.141592653589

float Math::radians(float degrees)
{
    return degrees * (PI / 180.0f);
}

float Math::degrees(float radians)
{
    return (180.0 / PI) * radians;
}

float Math::LinearInterpolate(float y1, float y2, float alpha)
{
    return(y1 * (1 - alpha) + y2 * alpha);
}

float Math::CosineInterpolate(float y1, float y2, float alpha)
{
    float mu2 = (1 - cos(alpha * PI)) / 2;
    return(y1 * (1 - mu2) + y2 * mu2);
}

float Math::CubicInterpolate(float y0, float y1, float y2, float y3, float alpha)
{
    float a0, a1, a2, a3, mu2;

    mu2 = alpha * alpha;
    a0 = y3 - y2 - y0 + y1;
    a1 = y0 - y1 - a0;
    a2 = y2 - y0;
    a3 = y1;

    return(a0 * alpha * mu2 + a1 * mu2 + a2 * alpha + a3);
}

float Math::HermiteInterpolate(float y0, float y1, float y2, float y3, float alpha, float tension, float bias)
{
    float m0, m1, mu2, mu3;
    float a0, a1, a2, a3;

    mu2 = alpha * alpha;
    mu3 = mu2 * alpha;
    m0 = (y1 - y0) * (1 + bias) * (1 - tension) / 2;
    m0 += (y2 - y1) * (1 - bias) * (1 - tension) / 2;
    m1 = (y2 - y1) * (1 + bias) * (1 - tension) / 2;
    m1 += (y3 - y2) * (1 - bias) * (1 - tension) / 2;
    a0 = 2 * mu3 - 3 * mu2 + 1;
    a1 = mu3 - 2 * mu2 + alpha;
    a2 = mu3 - mu2;
    a3 = -2 * mu3 + 3 * mu2;

    return(a0 * y1 + a1 * m0 + a2 * m1 + a3 * y2);
}

float Math::distance(Vector3 a, Vector3 b)
{
    return (float)sqrt(pow((b.x - a.x), 2.0) + pow((b.y, a.y), 2) + pow((b.z, a.z), 2));
}

//https://qroph.github.io/2018/07/30/smooth-paths-using-catmull-rom-splines.html
Vector3 Math::CatmullRomInterpolate(Vector3 p0, Vector3 p1, Vector3 p2, Vector3 p3, float t, float tension)
{
    float t0 = 0.0f;
    float t1 = t0 + pow(distance(p0, p1), t);
    float t2 = t1 + pow(distance(p1, p2), t);
    float t3 = t2 + pow(distance(p2, p3), t);

    Vector3 m1 = ((p1 - p0) / (t1 - t0) - (p2 - p0) / (t2 - t0) + (p2 - p1) / (t2 - t1)) * ((1.0f - tension) * (t2 - t1));
    Vector3 m2 = ((p2 - p1) / (t2 - t1) - (p3 - p1) / (t3 - t1) + (p3 - p2) / (t3 - t2)) * ((1.0f - tension) * (t2 - t1));

    CatmullRomSegment segment;
    segment.a = (p1 - p2) * 2.0f + m1 + m2;
    segment.b = (p1 - p2) * -3.0f - m1 - m1 - m2;
    segment.c = m1;
    segment.d = p1;

    return (segment.a * t * t * t) + (segment.b * t * t) + (segment.c * t) + segment.d;
}


//Vector3 Math::GetForwardPosition(float Distance, Vector3 pos, float pitch, float yaw){
Vector3 Math::GetForwardPosition(float Distance, Vector3 pos, Vector3 rotation)
{
    Vector3 vec = Vector3();

    float rx = (-rotation.x) + radians(90.0);
    float ry = rotation.y;

    double Px = Distance * sin(rx) * cos(ry);
    double Py = Distance * sin(ry);
    double Pz = Distance * cos(rx) * cos(ry);

    vec.x = pos.x + (float)Px;
    vec.y = pos.y + (float)Pz;
    vec.z = pos.z + (float)Py;
    return vec;
}

Vector3 Math::LookAt(Vector3 current, Vector3 target)
{
    float xdis = target.x - current.x;
    float ydis = target.y - current.y;
    float zdis = target.z - current.z;

    float xydis = (float)sqrt((ydis * ydis + xdis * xdis));

    float angle1 = (float)atan2(ydis, xdis);
    float angle2 = (float)atan2(zdis, xydis);
    Vector3 final = Vector3();

    final.x = angle1;

    final.y = angle2;
    return final;
}