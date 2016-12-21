#include <math.h>

#include "quaternion.h"

Quaternion::Quaternion()
    : x(x), y(y), z(z), w(w)
{ }


Quaternion::Quaternion(float q[4])
    : x(q[0]), y(q[1]), z(q[2]), w(q[3])
{ }


Quaternion::Quaternion(float x, float y, float z, float w)
    : x(x), y(y), z(z), w(w)
{ }


Quaternion Quaternion::conjugate() {
    Quaternion ret;
    ret.x = -x;
    ret.y = -y;
    ret.z = -z;
    ret.w = w;
    return ret;
}


Quaternion Quaternion::normalise() {
    Quaternion ret;
    double n = sqrt(x*x + y*y + z*z + w*w);
    ret.x = x / n;
    ret.y = y / n;
    ret.z = z / n;
    ret.w = w / n;
    return ret;
}


Quaternion Quaternion::operator*(const float s) {
    Quaternion ret;
    ret.x = x * s;
    ret.y = y * s;
    ret.z = z * s;
    ret.w = w * s;
    return ret;
}


Quaternion Quaternion::operator*(const Quaternion &q2) {
    Quaternion ret;
    ret.x =  x * q2.w + y * q2.z - z * q2.y + w * q2.x;
    ret.y = -x * q2.z + y * q2.w + z * q2.x + w * q2.y;
    ret.z =  x * q2.y - y * q2.x + z * q2.w + w * q2.z;
    ret.w = -x * q2.x - y * q2.y - z * q2.z + w * q2.w;
    return ret;
}


Quaternion Quaternion::operator+(const Quaternion &q2) {
    Quaternion ret;
    ret.x = x + q2.x;
    ret.y = y + q2.y;
    ret.z = z + q2.z;
    ret.w = w + q2.w;
    return ret;
}


std::ostream& operator<<(std::ostream& os, const Quaternion& q)
{
    os << "[" << q.x << ", " << q.y << ", " << q.z << ", " << q.w << "]";
    return os;
}
