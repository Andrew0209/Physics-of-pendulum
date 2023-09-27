#pragma once
#include <iostream>

class Vec {
public:
	double x, y, z;
	Vec(double xPos = 0, double yPos = 0, double zPos = 0) {
		x = xPos;
		y = yPos;
		z = zPos;
	}
	//operator +
	Vec operator+ (const Vec& vec)const {
		return Vec{ x + vec.x, y + vec.y, z + vec.z };
	}

	//operator -
	Vec operator- (const Vec& vec)const {
		return Vec{ x - vec.x, y - vec.y, z - vec.z };
	}

	// operator * vector ans scalar
	Vec operator* (const Vec& vec)const {
		return Vec{ y * vec.z - z * vec.y, z * vec.x - x * vec.z, x * vec.y - y * vec.x };
	}	
	Vec operator* (const double& n)const {
		return Vec{ x * n, y * n, z * n };
	}

	// operator / scalar
	Vec operator/ (const double& n)const {
		return Vec{ x / n, y / n, z / n };
	}
	void print() {
		std::cout << '{' << x << ' ' << y << ' ' << z << '}' << '\n';
	}
	double module() {
		return sqrt(x * x + y * y + z * z);
	}
	double module2() {
		return (x * x + y * y + z * z);
	}
private:
	
};

// declaration function on Vec class
double dotProduct(Vec& v1, Vec& v2);
Vec Normalize(Vec& v1);
double angleVec(Vec& v1, Vec& v2);
double angleVecCos(Vec& v1, Vec& v2);
double angleVecSin(Vec& v1, Vec& v2);

