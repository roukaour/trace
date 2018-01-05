#ifndef VECTOR_H
#define VECTOR_H

/**
 * A 3D vector.
 */
class Vector {
	public:
	
	// The x, y, and z components
	double x, y, z;
	
	// Constructors
	Vector();
	Vector(double x, double y, double z);
	
	// Methods
	Vector const cross(Vector const &v) const;
	double dot(Vector const &v) const;
	double norm() const;
	Vector const unit() const;
	Vector const abs() const;
	
	// Unary operators
	Vector const operator-() const;
	bool operator!() const;
	
	// Compound assignment operators
	Vector &operator+=(double k);
	Vector &operator+=(Vector const &v);
	Vector &operator-=(double k);
	Vector &operator-=(Vector const &v);
	Vector &operator*=(double k);
	Vector &operator*=(Vector const &v);
	Vector &operator/=(double k);
	Vector &operator/=(Vector const &v);
};

// Binary operators
Vector const operator+(Vector lhs, double rhs);
Vector const operator+(Vector lhs, Vector const &rhs);
Vector const operator-(Vector lhs, double rhs);
Vector const operator-(Vector lhs, Vector const &rhs);
Vector const operator*(Vector lhs, double rhs);
Vector const operator*(double lhs, Vector rhs);
Vector const operator*(Vector lhs, Vector const &rhs);
Vector const operator/(Vector lhs, double rhs);
Vector const operator/(Vector lhs, Vector const &rhs);

#endif
