#include "Math.h"
#include "Math.h"

#define PI 3.1415926

float Math::radians(float degrees)
{
    return degrees * (PI / 180.0f);
}

float Math::degrees(float radians)
{
    return (180.0 / PI) * radians;
}
