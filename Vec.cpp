#include "Vec.h"

// Basic functions on vector
double dotProduct(Vec& v1, Vec& v2) {
	return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}
Vec Normalize(Vec& v1) {
	return v1 / v1.module();
}
double angleVec(Vec& v1, Vec& v2) {
	Vec v3 = v1 + v2;
	return acos((dotProduct(v1, v2)) / (v1.module() * v2.module()));
}
double angleVecCos(Vec& v1, Vec& v2) {
	return ((dotProduct(v1, v2)) / (v1.module() * v2.module()));
}
double angleVecSin(Vec& v1, Vec& v2) {
	double angleCos = angleVecCos(v1, v2);
	return sqrt(1 - angleCos * angleCos);

}