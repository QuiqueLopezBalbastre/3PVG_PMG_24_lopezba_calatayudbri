//--------------------------------------------------------------//
//  Math Library
//  Matrix 3 Class Definition.
//--------------------------------------------------------------//
//
//   m0 m3 m6
//   m1 m4 m7
//   m2 m5 m8
//
//--------------------------------------------------------------//
#ifndef __MATRIX3_H__
#define __MATRIX3_H__ 1

#include "vector_2.h"
#include "vector_3.h"

class Mat3 {
public:

	Mat3();
	Mat3(float *values_array);
	Mat3(float value);
	Mat3(Vec3 a, Vec3 b, Vec3 c);

	Mat3(const Mat3& copy);
	~Mat3();

	static Mat3 Identity();

	Mat3 Multiply(const Mat3& other) const;

	float Determinant() const;

	Mat3 Adjoint() const;
	bool GetInverse(Mat3& out) const;
	bool Inverse();

	Mat3 Transpose() const;

	static Mat3 Translate(const Vec2& position);
	static Mat3 Translate(float x, float y);
  static Mat3 Rotate(float rotation);
  static Mat3 Scale(const Vec2& scale);
  static Mat3 Scale(float x, float y);

	Vec3 GetColumn(int colum) const;
	Vec3 GetLine(int line) const;

	inline Mat3 operator+(const Mat3& other) const;
	inline Mat3& operator+=(const Mat3& other);
	inline Mat3 operator+(float value) const;
	inline Mat3& operator+=(float value);
	inline Mat3 operator-(const Mat3& other) const;
	inline Mat3& operator-=(const Mat3& other);
	inline Mat3 operator-(float value) const;
	inline Mat3& operator-=(float value);
	inline Mat3 operator*(float value) const;
	inline Mat3& operator*=(float value);
	inline Mat3 operator/(float value) const;
	inline Mat3& operator/=(float value);
	bool operator==(const Mat3& other) const;
	bool operator!=(const Mat3& other) const;
	inline void operator=(const Mat3& other);
	
  Vec3 TransformVec3(const Vec3& vec);
  Vec2 TransformVec2(const Vec2& vec);

	float m[9];
};


inline Mat3 Mat3::operator+(const Mat3& other) const {
	Mat3 ret;
	for(int i = 0; i < 9; i++)
	{
		ret.m[i] = m[i] + other.m[i];
	}
	return ret;
}

inline Mat3& Mat3::operator+=(const Mat3& other) {
	for (int i = 0; i < 9; i++)
	{
		m[i] += other.m[i];
	}
	return (*this);
}

inline Mat3 Mat3::operator+(float value) const {
	Mat3 ret;
	for (int i = 0; i < 9; i++)
	{
		ret.m[i] = m[i] + value;
	}
	return ret;
}

inline Mat3& Mat3::operator+=(float value) {
	for (int i = 0; i < 9; i++)
	{
		m[i] += value;
	}
	return (*this);
}

inline Mat3 Mat3::operator-(const Mat3& other) const {
	Mat3 ret;
	for (int i = 0; i < 9; i++)
	{
		ret.m[i] = m[i] - other.m[i];
	}
	return ret;
}

inline Mat3& Mat3::operator-=(const Mat3& other) {
	for (int i = 0; i < 9; i++)
	{
		m[i] -= other.m[i];
	}
	return (*this);
}

inline Mat3 Mat3::operator-(float value) const {
	Mat3 ret;
	for (int i = 0; i < 9; i++)
	{
		ret.m[i] = m[i] - value;
	}
	return ret;
}

inline Mat3& Mat3::operator-=(float value) {
	for (int i = 0; i < 9; i++)
	{
		m[i] -= value;
	}
	return (*this);
}

inline Mat3 Mat3::operator*(float value) const {
	Mat3 ret;
	for (int i = 0; i < 9; i++)
	{
		ret.m[i] = m[i] * value;
	}
	return ret;
}

inline Mat3& Mat3::operator*=(float value) {
	for (int i = 0; i < 9; i++)
	{
		m[i] *= value;
	}
	return (*this);
}

inline Mat3 Mat3::operator/(float value) const {
	Mat3 ret;
	if(value != 0){
		for (int i = 0; i < 9; i++)
		{
			ret.m[i] = m[i] / value;
		}
		return ret;
	}else{
		return Mat3();
	}
}

inline Mat3& Mat3::operator/=(float value) {
	if(value != 0){
		for (int i = 0; i < 9; i++)
		{
			m[i] /= value;
		}
		return (*this);
	}else{
		return Mat3();
	}
}

inline bool Mat3::operator==(const Mat3& other) const {
	for(int i = 0; i < 9; i++){
		if(this->m[i] != other.m[i]){
			return false;
		}
	}
	return true;
}

inline bool Mat3::operator!=(const Mat3& other) const {
	for (int i = 0; i < 9; i++)
	{
		if (this->m[i] != other.m[i])
		{
			return true;
		}
	}
	return false;
}

inline void Mat3::operator=(const Mat3& other) {
	for(int i = 0; i < 9; i++){
		m[i] = other.m[i];
	}
}

inline Mat3 Mat3::Identity() {
	Mat3 ret_;
	for(int i = 0; i < 9; i++)
	{
		if(i == 0 || i == 4 || i == 8){
			ret_.m[i] = 1;
		}else{
			ret_.m[i] = 0;
		}
	}
	
	return ret_;
}

inline float Mat3::Determinant() const {
	float ret_;
	ret_ = ((m[0] * m[4] * m[8]) + (m[1] * m[5] * m[6]) + (m[2] * m[7] * m[3])) -
			((m[6] * m[4] * m[2]) + (m[3] * m[1] * m[8]) + (m[7] * m[5] * m[0]));
	
	return ret_;
}

inline bool Mat3::GetInverse(Mat3& out) const {
	if(out.Determinant() != 0){
		out = (this->Adjoint().Transpose()) / this->Determinant();
		return true;
	}
	return false;
}

inline bool Mat3::Inverse(){
	if(this->Determinant() != 0){
		
		*this = (this->Adjoint().Transpose() / this->Determinant());
		return true;
	}
	return false;
		
}

inline Mat3 Mat3::Translate(const Vec2& mov_vector) {
	Mat3 ret = ret.Identity();

	ret.m[6] = mov_vector.x;
	ret.m[7] = mov_vector.y;

	return ret;
}

inline Mat3 Mat3::Translate(float x, float y) {
	Mat3 ret = ret.Identity();

	ret.m[6] = x;
	ret.m[7] = y;

	return ret;
}
 inline Mat3 Mat3::Rotate(float rotation){
	Mat3 result;
	result = result.Identity();
	float angle_cos = cosf(rotation);
	float angle_sin = sinf(rotation);

	result.m[0] = angle_cos;
	result.m[3] = -angle_sin;
	result.m[1] = angle_sin;
	result.m[4] = angle_cos;

	return result;
}
 
inline Mat3 Mat3::Scale(const Vec2 &scale){
	Mat3 result;
	result = result.Identity();
	result.m[0] *= scale.x;
	result.m[4] *= scale.y;
	result.m[8] = 1.0f;
	return result;
}
inline Mat3 Mat3::Scale(float x, float y){
	Mat3 result;
	result = result.Identity();
	result.m[0] *= x;
	result.m[4] *= y;
	result.m[8] = 1.0f;

	return result;
}
 
inline Mat3 Mat3::Multiply(const Mat3& other) const {
	Mat3 ret_;
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			ret_.m[i * 3 + j] = m[i * 3] * other.m[j] +
								  m[i * 3 + 1] * other.m[3 + j] +
								  m[i * 3 + 2] * other.m[6 + j];
		}
	}

	return ret_;
}

inline Mat3 Mat3::Adjoint() const {
	Mat3 adjoint;

    adjoint.m[0] = (m[4] * m[8]) - (m[5] * m[7]);
    adjoint.m[1] = ((m[3] * m[8]) - (m[5] * m[6])) * -1;
    adjoint.m[2] = (m[3] * m[7]) - (m[6] * m[4]);

    adjoint.m[3] = ((m[1] * m[8]) - (m[2] * m[7])) * -1;
    adjoint.m[4] = (m[0] * m[8]) - (m[2] * m[6]);
    adjoint.m[5] = ((m[0] * m[7]) - (m[1] * m[6])) * -1;

    adjoint.m[6] = (m[1] * m[5]) - (m[4] * m[2]);
    adjoint.m[7] = ((m[0] * m[5]) - (m[2] * m[3])) * -1;
    adjoint.m[8] = (m[0] * m[4]) - (m[1] * m[3]);

    return adjoint;
}

inline Mat3 Mat3::Transpose() const {
	Mat3 traspose;
	traspose.m[0] = m[0];
	traspose.m[3] = m[1];
	traspose.m[6] = m[2];

	traspose.m[1] = m[3];
	traspose.m[4] = m[4];
	traspose.m[7] = m[5];
	
	traspose.m[2] = m[6];
	traspose.m[5] = m[7];
	traspose.m[8] = m[8];

	return traspose;
}

inline Vec3 Mat3::GetColumn(int colum) const {

	Vec3 ret;

	if (colum >= 0 && colum < 3)
	{
		ret.x = m[colum];
		ret.y = m[colum + 3];
		ret.z = m[colum + 6];
	}
	else
	{
		ret.x = 0;
		ret.y = 0;
		ret.z = 0;
	}
	return ret;

}

inline Vec3 Mat3::GetLine(int line) const {
	
	Vec3 ret;

	if (line >= 0 && line < 3)
	{
		ret.x = m[line * 3];
		ret.y = m[line * 3 + 1];
		ret.z = m[line * 3 + 2];

	}else{
		ret.x = 0;
		ret.y = 0;
		ret.z = 0;
		
	}
		return ret;
}

inline Vec3 Mat3::TransformVec3(const Vec3& vec) {
  float c[3] = { vec.x, vec.y, vec.z };
  float r[3] = {
    m[0] * c[0] + m[3] * c[1] + m[6] * c[2],
    m[1] * c[0] + m[4] * c[1] + m[7] * c[2],
    m[2] * c[0] + m[5] * c[1] + m[8] * c[2]};
  Vec3 k;
  k.x = r[0];
  k.y = r[1];
  k.z = r[2];
  return k;
}

inline Vec2 Mat3::TransformVec2(const Vec2& vec) {
  Vec3 tmp = { vec.x, vec.y, 1.0 };
  Vec3 result = TransformVec3(tmp);
  result.x = result.x/result.z;
  result.y = result.y/result.z;
  Vec2 k;
  k.x = result.x;
  k.y = result.y;
  return k;
}
#endif 
