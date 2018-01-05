#ifndef COLOR_H
#define COLOR_H

#include "Color.h"

/**
 * A color with RGB values from 0 to 1.
 */
class Color {
	public:
	
	// The red, green, and blue components
	double r, g, b;
	
	// Constructors
	Color();
	Color(double r, double g, double b);
	
	// Methods
	double sum() const;
	Color const &clamp();
	
	// Unary operators
	Color const operator-() const;
	bool operator!() const;
	
	// Compound assignment operators
	Color &operator+=(double k);
	Color &operator+=(Color const &c);
	Color &operator-=(double k);
	Color &operator-=(Color const &c);
	Color &operator*=(double k);
	Color &operator*=(Color const &c);
	Color &operator/=(double k);
	Color &operator/=(Color const &c);
};

// Binary operators
Color const operator+(Color lhs, double rhs);
Color const operator+(Color lhs, Color const &rhs);
Color const operator-(Color lhs, double rhs);
Color const operator-(Color lhs, Color const &rhs);
Color const operator*(Color lhs, double rhs);
Color const operator*(double lhs, Color rhs);
Color const operator*(Color lhs, Color const &rhs);
Color const operator/(Color lhs, double rhs);
Color const operator/(Color lhs, Color const &rhs);

#endif
