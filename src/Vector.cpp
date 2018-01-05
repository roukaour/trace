#include <cmath>

#include "Vector.h"

/**
 * The default constructor; yields a null vector.
 */
Vector::Vector() : x(0), y(0), z(0) {}

/**
 * The constructor with explicit members.
 */
Vector::Vector(double x, double y, double z) : x(x), y(y), z(z) {}

/**
 * Returns the cross product of this vector and another one.
 */
Vector const Vector::cross(Vector const &v) const {
	return Vector(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
}

/**
 * Returns the dot product of this vector and another one.
 */
double Vector::dot(Vector const &v) const {
	return x * v.x + y * v.y + z * v.z;
}

/**
 * Returns the Euclidean norm of this vector.
 */
double Vector::norm() const {
	return sqrt(x * x + y * y + z * z);
}

/**
 * Returns this vector scaled to have Euclidean norm 1.
 */
Vector const Vector::unit() const {
	double d = norm();
	if (d != 0) {
		return *this / d;
	}
	return *this;
}

/**
 * Returns this vector with all components made non-negative.
 */
Vector const Vector::abs() const {
	return Vector(fabs(x), fabs(y), fabs(z));
}

/**
 * Returns the opposite of this vector.
 */
Vector const Vector::operator-() const {
	return Vector(-x, -y, -z);
}

/**
 * Returns whether this vector is null.
 */
bool Vector::operator!() const {
	return x == 0 && y == 0 && z == 0;
}

/**
 * Adds a constant to this vector.
 */
Vector &Vector::operator+=(double k) {
	x += k;
	y += k;
	z += k;
	return *this;
}

/**
 * Adds a vector to this one.
 */
Vector &Vector::operator+=(Vector const &v) {
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

/**
 * Subtracts a constant from this vector.
 */
Vector &Vector::operator-=(double k) {
	x -= k;
	y -= k;
	z -= k;
	return *this;
}

/**
 * Subtracts a vector from this one.
 */
Vector &Vector::operator-=(Vector const &v) {
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

/**
 * Multiplies this vector by a constant.
 */
Vector &Vector::operator*=(double k) {
	x *= k;
	y *= k;
	z *= k;
	return *this;
}

/**
 * Multiplies this vector by another one.
 */
Vector &Vector::operator*=(Vector const &v) {
	x *= v.x;
	y *= v.y;
	z *= v.z;
	return *this;
}

/**
 * Divides this vector by a constant.
 */
Vector &Vector::operator/=(double k) {
	x /= k;
	y /= k;
	z /= k;
	return *this;
}

/**
 * Divides this vector by another one.
 */
Vector &Vector::operator/=(Vector const &v) {
	x /= v.x;
	y /= v.y;
	z /= v.z;
	return *this;
}

/**
 * Returns a vector plus a constant.
 */
Vector const operator+(Vector lhs, double rhs) {
	lhs += rhs;
	return lhs;
}

/**
 * Returns a vector plus another vector.
 */
Vector const operator+(Vector lhs, Vector const &rhs) {
	lhs += rhs;
	return lhs;
}

/**
 * Returns a vector minus a constant.
 */
Vector const operator-(Vector lhs, double rhs) {
	lhs -= rhs;
	return lhs;
}

/**
 * Returns a vector minus another vector.
 */
Vector const operator-(Vector lhs, Vector const &rhs) {
	lhs -= rhs;
	return lhs;
}

/**
 * Returns a vector times a constant.
 */
Vector const operator*(Vector lhs, double rhs) {
	lhs *= rhs;
	return lhs;
}

/**
 * Returns a vector times a constant.
 */
Vector const operator*(double lhs, Vector rhs) {
	rhs *= lhs;
	return rhs;
}

/**
 * Returns a vector times another vector.
 */
Vector const operator*(Vector lhs, Vector const &rhs) {
	lhs *= rhs;
	return lhs;
}

/**
 * Returns a vector divided by a constant.
 */
Vector const operator/(Vector lhs, double rhs) {
	lhs /= rhs;
	return lhs;
}

/**
 * Returns a vector divided by another vector.
 */
Vector const operator/(Vector lhs, Vector const &rhs) {
	lhs /= rhs;
	return lhs;
}
