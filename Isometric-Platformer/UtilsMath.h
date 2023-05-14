//Math utility functions

#pragma once

constexpr float PI = 3.14159265359f;
constexpr float RAD2DEG = 57.2957795056f;
constexpr float DEG2RAD = 0.01745329252f;

//Get the XY part of Vec3f
static inline Vec2f U_GetXY(const Vec3f& v)
{
    return Vec2f(v.x, v.y);
}

//x^2
static inline float U_Squared(const float x)
{
    return x * x;
}

//Dot product of two vectors
static inline float U_Dot(const Vec2f& a, const Vec2f& b)
{
    return a.x * b.x + a.y * b.y;
}

//Squared of the euclidean norm of vector v
static inline float U_SquaredMagnitude(const Vec2f& v)
{
    return U_Dot(v, v);
}

//The euclidean norm of vector v
static inline float U_Magnitude(const Vec2f& v)
{
    return sqrt(U_SquaredMagnitude(v));
}

//Euclidean distance between two points
static inline float U_Distance(const Vec2f& x, const Vec2f& y)
{
    return U_Magnitude(x - y);
}

//Linear interpolation between a & b using t
static inline float U_Lerp(float a, float b, float t)
{
    if(t < 0)
        return a;
    if(t > 1)
        return b;
    return a + t * (b - a);
}

//Generate a random number between 0 & 1
static inline float U_Rand01()
{
    return float(rand()) / float(RAND_MAX);
}