#include "Mat3.h"
#include <cmath>

Mat3::Mat3(const float* pArr)
{
	for (int i = 0; i < 9; ++i)
		m[i] = pArr[i];
}

Mat3::Mat3(float _00, float _01, float _02, float _10, float _11, float _12, float _20, float _21, float _22)
{
	m[0] = _00;
	m[1] = _01;
	m[2] = _02;
	m[3] = _10;
	m[4] = _11;
	m[5] = _12;
	m[6] = _20;
	m[7] = _21;
	m[8] = _22;
}

Mat3& Mat3::operator=(const Mat3& rhs)
{
	m00 = rhs.m00;
	m01 = rhs.m01;
	m02 = rhs.m02;
	m10 = rhs.m10;
	m11 = rhs.m11;
	m12 = rhs.m12;
	m20 = rhs.m20;
	m21 = rhs.m21;
	m22 = rhs.m22;

	return *this;
}

Mat3& Mat3::operator*=(const Mat3& rhs)
{
	float m[3][3]{};

	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			for (int k = 0; k < 3; k++)
				m[i][j] += this->m2[i][k] * rhs.m2[k][j];

	for (int i = 0; i < 9; i++)
		this->m2[i / 3][i % 3] = m[i / 3][i % 3];

	return *this;
}

glm::mat3 Mat3::ToGlmMat3()
{
	return glm::mat3(m00, m01, m02, m10, m11, m12, m20, m21, m22);
}

Mat3 operator*(const Mat3& lhs, const Mat3& rhs)
{
	float m[3][3]{};

	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			for (int k = 0; k < 3; k++)
				m[i][j] += lhs.m2[i][k] * rhs.m2[k][j];

	float m2[9]{};

	for (int i = 0; i < 9; i++)
		m2[i] = m[i / 3][i % 3];

	return Mat3(m2);
}

Vec2 operator*(const Mat3& pMtx, const Vec2& rhs)
{
	return Vec2(pMtx.m00 * rhs.x + pMtx.m01 * rhs.y + pMtx.m02, pMtx.m10 * rhs.x + pMtx.m11 * rhs.y + pMtx.m12);
}

Mat3 Mat3Identity()
{
	return Mat3({1.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 1.f});
}

Mat3 Mat3Translate(float x, float y)
{
	return Mat3({ 1.f, 0.f, x, 0.f, 1.f, y, 0.f, 0.f, 1.f });
}

Mat3 Mat3Scale(float x, float y)
{
	return Mat3({ x, 0.f, 0.f, 0.f, y, 0.f, 0.f, 0.f, 1.f });
}

Mat3 Mat3RotRad(float angle)
{
	return Mat3({ float(cos(angle)), float(-sin(angle)), 0.f, float(sin(angle)), float(cos(angle)), 0.f, 0.f, 0.f, 1.f });
}

Mat3 Mat3RotDeg(float angle)
{
	return Mat3RotRad(angle * 3.14159265358f / 180.f);
}

Mat3 Mat3Transpose(const Mat3& pMtx)
{
	return Mat3({ pMtx.m00 , pMtx.m10, pMtx.m20, pMtx.m01, pMtx.m11, pMtx.m21, pMtx.m02, pMtx.m12, pMtx.m22 });
}

Mat3 Mat3Inverse(float* determinant, const Mat3& pMtx)
{
	*determinant = pMtx.m00 * ((pMtx.m11 * pMtx.m22) - (pMtx.m12 * pMtx.m21))
		- pMtx.m01 * ((pMtx.m10 * pMtx.m22) - (pMtx.m12 * pMtx.m20))
		+ pMtx.m02 * ((pMtx.m10 * pMtx.m21) - (pMtx.m11 * pMtx.m20));

	if (!*determinant) {
		return NULL;
	}
	else {
		Mat3 pResult, tmp = Mat3Transpose(pMtx);
		pResult.m00 = ((tmp.m11 * tmp.m22) - (tmp.m12 * tmp.m21)) / *determinant;
		pResult.m01 = -((tmp.m10 * tmp.m22) - (tmp.m12 * tmp.m20)) / *determinant;
		pResult.m02 = ((tmp.m10 * tmp.m21) - (tmp.m11 * tmp.m20)) / *determinant;
		pResult.m10 = -((tmp.m01 * tmp.m22) - (tmp.m02 * tmp.m21)) / *determinant;
		pResult.m11 = ((tmp.m00 * tmp.m22) - (tmp.m02 * tmp.m20)) / *determinant;
		pResult.m12 = -((tmp.m00 * tmp.m21) - (tmp.m01 * tmp.m20)) / *determinant;
		pResult.m20 = ((tmp.m01 * tmp.m12) - (tmp.m02 * tmp.m11)) / *determinant;
		pResult.m21 = -((tmp.m00 * tmp.m12) - (tmp.m02 * tmp.m10)) / *determinant;
		pResult.m22 = ((tmp.m00 * tmp.m11) - (tmp.m01 * tmp.m10)) / *determinant;
		return pResult;
	}
}