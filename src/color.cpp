#include "Color.h"

/**
 * Restricts v to between 0 and 1. Used by Color::clamp.
 */
inline double clamp(double v) {
	return v < 0 ? 0 : v > 1 ? 1 : v;
}

/**
 * The default constructor; yields black.
 */
Color::Color() : r(0), g(0), b(0) {}

/**
 * The constructor with explicit members.
 */
Color::Color(double r, double g, double b) : r(r), g(g), b(b) {}

/**
 * Returns the sum of the components of this color.
 */
double Color::sum() const {
	return r + g + b;
}

/**
 * Restricts the color components to between 0 and 1.
 */
Color const &Color::clamp() {
	r = ::clamp(r);
	g = ::clamp(g);
	b = ::clamp(b);
	return *this;
}

/**
 * Returns the inverse of this color.
 */
Color const Color::operator-() const {
	return Color(1 - r, 1 - g, 1 - b);
}

/**
 * Returns whether this color is black.
 */
bool Color::operator!() const {
	return r == 0 && g == 0 && b == 0;
}

/**
 * Adds a constant to this color.
 */
Color &Color::operator+=(double k) {
	r += k;
	g += k;
	b += k;
	return *this;
}

/**
 * Adds a color to this one.
 */
Color &Color::operator+=(Color const &c) {
	r += c.r;
	g += c.g;
	b += c.b;
	return *this;
}

/**
 * Subtracts a constant from this color.
 */
Color &Color::operator-=(double k) {
	r -= k;
	g -= k;
	b -= k;
	return *this;
}

/**
 * Subtracts a color from this one.
 */
Color &Color::operator-=(Color const &c) {
	r -= c.r;
	g -= c.g;
	b -= c.b;
	return *this;
}

/**
 * Multiplies this color by a constant.
 */
Color &Color::operator*=(double k) {
	r *= k;
	g *= k;
	b *= k;
	return *this;
}

/**
 * Multiplies this color by another one.
 */
Color &Color::operator*=(Color const &c) {
	r *= c.r;
	g *= c.g;
	b *= c.b;
	return *this;
}

/**
 * Divides this color by a constant.
 */
Color &Color::operator/=(double k) {
	r /= k;
	g /= k;
	b /= k;
	return *this;
}

/**
 * Divides this color by another one.
 */
Color &Color::operator/=(Color const &c) {
	r /= c.r;
	g /= c.g;
	b /= c.b;
	return *this;
}

/**
 * Returns a color plus a constant.
 */
Color const operator+(Color lhs, double rhs) {
	lhs += rhs;
	return lhs;
}

/**
 * Returns a color plus another color.
 */
Color const operator+(Color lhs, Color const &rhs) {
	lhs += rhs;
	return lhs;
}

/**
 * Returns a color minus a constant.
 */
Color const operator-(Color lhs, double rhs) {
	lhs -= rhs;
	return lhs;
}

/**
 * Returns a color minus another color.
 */
Color const operator-(Color lhs, Color const &rhs) {
	lhs -= rhs;
	return lhs;
}

/**
 * Returns a color times a constant.
 */
Color const operator*(Color lhs, double rhs) {
	lhs *= rhs;
	return lhs;
}

/**
 * Returns a color times a constant.
 */
Color const operator*(double lhs, Color rhs) {
	rhs *= lhs;
	return rhs;
}

/**
 * Returns a color times another color.
 */
Color const operator*(Color lhs, Color const &rhs) {
	lhs *= rhs;
	return lhs;
}

/**
 * Returns a color divided by a constant.
 */
Color const operator/(Color lhs, double rhs) {
	lhs /= rhs;
	return lhs;
}

/**
 * Returns a color divided by another color.
 */
Color const operator/(Color lhs, Color const &rhs) {
	lhs /= rhs;
	return lhs;
}
