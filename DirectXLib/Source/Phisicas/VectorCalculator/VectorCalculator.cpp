#include "VectorCalculator.h"

namespace phy {
	float VectorCalculator::I[3][3] = {
			{1, 0, 0},
			{0, 1, 0},
			{0, 0, 1}
	};
	float (*VectorCalculator::getMatrixI())[3]{
		return I;
	}
	void VectorCalculator::copy3_3(float r[3], const float a[3]){
		r[0] = a[0];
		r[1] = a[1];
		r[2] = a[2];
	}
	float VectorCalculator::fb(float length, float L) {
		float x = length / L;
		float xx = x * x;
		float xxx = xx * x;
		float xxxx = xxx * x;
		return (-11.541f * xxxx + 34.193f * xxx - 39.083f * xx + 23.116f * x - 9.713f);
	}
	float VectorCalculator::dot3_3(const float a[3], const float b[3]) {
		return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
	}
	float VectorCalculator::fbstar(float length, float L, float kb, float cb) {
		float tempfb_fl = kb * fb(length, L);
		float fbstar_fl = cb * (length - L);

		if (tempfb_fl < fbstar_fl) {
			return fbstar_fl;
		}
		return tempfb_fl;
	}
	float VectorCalculator::fbderiv(float length, float L) {
		float x = length / L;
		float xx = x * x;
		float xxx = xx * x;
		return (-46.164f * xxx + 102.579f * xx - 78.166f * x + 23.116f);
	}
	float VectorCalculator::fbstarJacobi(float length, float L, float kb, float cb) {
		float tempfb_fl = kb * fb(length, L);
		float fbstar_fl = cb * (length - L);

		if (tempfb_fl < fbstar_fl)return -cb;

		return -kb * fbderiv(length, L);
	}
	void VectorCalculator::mul3_3fl(float r[3], const float a[3], float f) {
		r[0] = a[0] * f;
		r[1] = a[1] * f;
		r[2] = a[2] * f;
	}
	void VectorCalculator::mul33_fl(float R[3][3], float f) {
		int i, j;
		for (i = 0; i < 3; i++) {
			for (j = 0; j < 3; j++) {
				R[i][j] *= f;
			}
		}
	}
	void VectorCalculator::mul3_fl(float r[3], float f) {
		r[0] *= f;
		r[1] *= f;
		r[2] *= f;
	}
	void VectorCalculator::mul3_m33(float r[3], const float M[3][3], const float a[3]){
		float t[3];
		copy3_3(t, a);
		r[0] = M[0][0] * t[0] + M[1][0] * t[1] + M[2][0] * t[2];
		r[1] = M[0][1] * t[0] + M[1][1] * t[1] + M[2][1] * t[2];
		r[2] = M[0][2] * t[0] + M[1][2] * t[1] + M[2][2] * t[2];
	}
	void VectorCalculator::add3_3(float r[3], const float a[3]) {
		r[0] += a[0];
		r[1] += a[1];
		r[2] += a[2];
	}
	void VectorCalculator::add3_3v3(float r[3], const float a[3], const float b[3]) {
		r[0] = a[0] + b[0];
		r[1] = a[1] + b[1];
		r[2] = a[2] + b[2];
	}
	void VectorCalculator::madd3_3fl(float r[3], const float a[3], float f) {
		r[0] += a[0] * f;
		r[1] += a[1] * f;
		r[2] += a[2] * f;
	}
	void VectorCalculator::sub3_3(float r[3], const float a[3]) {
		r[0] -= a[0];
		r[1] -= a[1];
		r[2] -= a[2];
	}
	void VectorCalculator::sub3_3m3(float R[3][3], const float A[3][3], const float B[3][3]) {
		int i, j;
		for (i = 0; i < 3; i++) {
			for (j = 0; j < 3; j++) {
				R[i][j] = A[i][j] - B[i][j];
			}
		}
	}
	void VectorCalculator::interp3_v33(float r[3], const float a[3], const float b[3], const float t){
		const float s = 1.0f - t;
		r[0] = s * a[0] + t * b[0];
		r[1] = s * a[1] + t * b[1];
		r[2] = s * a[2] + t * b[2];
	}
	void VectorCalculator::zero3(float m[3][3]) {
		for (int ite1 = 0; ite1 < 3; ite1++) {
			for (int ite2 = 0; ite2 < 3; ite2++)m[ite1][ite2] = 0;
		}
	}
	void VectorCalculator::outerProduct(float r[3][3], const float a[3], const float b[3]) {
		mul3_3fl(r[0], a, b[0]);
		mul3_3fl(r[1], a, b[1]);
		mul3_3fl(r[2], a, b[2]);
	}
	void VectorCalculator::dfdxSpring(float to[3][3], const float dir[3], float length, float L, float k) {
		outerProduct(to, dir, dir);
		sub3_3m3(to, I, to);
		mul33_fl(to, (L / length));
		sub3_3m3(to, to, I);
		mul33_fl(to, k);
	}
	void VectorCalculator::dfdvDamp(float to[3][3], const float dir[3], float damping) {
		/* derivative of force wrt velocity */
		outerProduct(to, dir, dir);
		mul33_fl(to, -damping);
	}
}