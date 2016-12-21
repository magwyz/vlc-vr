#ifndef QUATERNION_H
#define QUATERNION_H

#include <iostream>


class Quaternion
{
public:
    Quaternion();
    Quaternion(float q[4]);
    Quaternion(float x, float y, float z, float w);

    Quaternion conjugate();
    Quaternion normalise();
    Quaternion operator*(const float s);
    Quaternion operator*(const Quaternion &q2);
    Quaternion operator+(const Quaternion &q2);
    friend std::ostream& operator<<(std::ostream& os, const Quaternion& q);
    float x, y, z, w;
};

#endif // QUATERNION_H
