#ifndef __VEC3_H__
#define __VEC3_H__ 1

#include "math_utils.h"

#include <cmath>

class Vec3 {
 public:
	Vec3();
	Vec3(float value);
	Vec3(float x, float y, float z);
	Vec3(float* values_array);
	Vec3(const Vec3& other);
	~Vec3();

	Vec3 operator+(const Vec3& other) const;
	Vec3 operator+(float value) const;
	Vec3& operator+=(const Vec3& other);
	Vec3& operator+=(float value);
	Vec3 operator-(const Vec3& other) const;
	Vec3 operator-(float value) const;
	Vec3& operator-=(const Vec3& other);
	Vec3& operator-=(float value);
	bool operator==(const Vec3& other) const;
	bool operator!=(const Vec3& other) const;
	void operator=(const Vec3& other);
	void operator=(float value);
	Vec3 operator*(float value) const;
	Vec3& operator*=(float value);
	Vec3 operator/(float value) const;
	Vec3& operator/=(float value);

	float Magnitude() const;
	Vec3 Normalized() const;
	void Normalize();
	float SqrMagnitude() const;
	void Scale(const Vec3& other);

	static Vec3 Lerp(const Vec3& a, const Vec3& b, float t);
	static Vec3 LerpUnclamped(const Vec3& a, const Vec3& b, float t);
	static float DotProduct(const Vec3& a, const Vec3& b);
	static float Angle(const Vec3& a, const Vec3& b);
	static Vec3 CrossProduct(const Vec3& a,const Vec3& b);	
	static float Distance(const Vec3& a, const Vec3& b);
	static Vec3 Reflect(const Vec3& direction, const Vec3& normal);

	static const Vec3 up;
	static const Vec3 down;
	static const Vec3 right;
	static const Vec3 left;
	static const Vec3 forward;
	static const Vec3 back;
	static const Vec3 zero;
	static const Vec3 unit;

	float x;
	float y;
	float z;
};

inline float Vec3::Magnitude() const {
	return std::sqrt(x * x + y * y + z * z);
}

inline void Vec3::Normalize() {	

	float inverse_magnitude = 1.0f / Magnitude();
	*this *= inverse_magnitude;
}

inline Vec3 Vec3::Normalized() const {
	Vec3 ret;
	ret.x = (x) / (Magnitude());
	ret.y = (y) / (Magnitude());
	ret.z = (z) / (Magnitude());
	return ret;
}

inline float Vec3::DotProduct(const Vec3& a, const Vec3& other)  {
	float ret;
	ret = (a.x * other.x) + (a.y * other.y) + (a.z * other.z);
	return ret;
}

inline float Vec3::Angle(const Vec3& a, const Vec3& other)  {
	float dotProduct = DotProduct(a, other);
	if (a.Magnitude() > 0 && other.Magnitude() > 0)
	{
		float cosineTheta = dotProduct / (a.Magnitude() * other.Magnitude());
		float angleInRadians = std::acos(cosineTheta);
		return angleInRadians;
	}
	else
	{
		return 0.0f;
	}
}

inline Vec3 Vec3::CrossProduct(const Vec3& a, const Vec3& other)  {
	Vec3 ret;
		ret.x = a.y * other.z - a.z * other.y;
		ret.y = a.z * other.x - a.x * other.z;
		ret.z = a.x * other.y - a.y * other.x;

		return ret;

}

inline float Vec3::SqrMagnitude() const {
	return (x * x + y * y + z * z);
}

inline void Vec3::Scale(const Vec3& other) {
	x = x * other.x;
	y = y * other.y;
	z = z * other.z;
}

inline Vec3 Vec3::Lerp(const Vec3& a, const Vec3& b, float t) {
	if (t < 0.0f)
	{
		t = 0.0f;
	}
	else if (t > 1.0f)
	{
		t = 1.0f;
	}

	return LerpUnclamped(a, b, t);
}

inline Vec3 Vec3::LerpUnclamped(const Vec3& a, const Vec3& b, float t) {
	return Vec3(a.x + (b.x - a.x) * t, 
				a.y + (b.y - a.y) * t, 
				a.z + (b.z - a.z) * t);
}

inline float Vec3::Distance(const Vec3& a, const Vec3& b) {
	Vec3 aux;
	aux.x = a.x - b.x;
	aux.y = a.y - b.y;
	aux.z = a.z - b.z;
	float ret = std::sqrt((aux.x * aux.x) + (aux.y * aux.y) + (aux.z * aux.z));
	return ret;
}

inline Vec3 Vec3::Reflect(const Vec3& direction, const Vec3& normal) {
	float dotProduct = 2.0f * Vec3::DotProduct(direction, normal);
	Vec3 reflectionTerm = normal * dotProduct;
	Vec3 reflectedVector = direction - reflectionTerm;
	return reflectedVector;
}

inline Vec3 Vec3::operator+(const Vec3& other) const {
	Vec3 ret;
	ret.x = x + other.x;
	ret.y = y + other.y;
	ret.z = z + other.z;
	return ret;

}

inline Vec3 Vec3::operator+(float value) const {
	Vec3 ret;

	ret.x = x + value;
	ret.y = y + value;
	ret.z = z + value;
	return ret;

}

inline Vec3& Vec3::operator+=(const Vec3& other) {
	x += other.x;
	y += other.y;
	z += other.z;

	return *this;
}

inline Vec3& Vec3::operator+=(float value) {
	x += value;
	y += value;
	z += value;
	return *this;
}

inline Vec3 Vec3::operator-(const Vec3& other) const {
	Vec3 aux;
	aux.x = x - other.x;
	aux.y = y - other.y;
	aux.z = z - other.z;
	return aux;
}

inline Vec3 Vec3::operator-(float value) const {
	Vec3 aux;
	aux.x = x - value;
	aux.y = y - value;
	aux.z = z - value;
	return aux;
}

inline Vec3& Vec3::operator-=(const Vec3& other) {
	x -= other.x;
	y -= other.y;
	z -= other.z;
	return *this;	
}

inline Vec3& Vec3::operator-=(float value) {
	x -= value;
	y -= value;
	z -= value;
	return *this;	
}

inline bool Vec3::operator==(const Vec3& other) const {
	return (this->x == other.x) && (this->y == other.y) && (this->z == other.z);
}

inline bool Vec3::operator!=(const Vec3& other) const {
	return ((this->x != other.x) || (this->y != other.y) || (this->z != other.z));
}

inline void Vec3::operator=(const Vec3& other) {
	x = other.x;
	y = other.y;	
	z = other.z;	
}

inline void Vec3::operator=(float value) {	
	x = value;
	y = value;
	z = value;
}

inline Vec3 Vec3::operator*(float value) const {
	return Vec3(x * value, y * value, z * value);
}


inline Vec3& Vec3::operator*=(float value) {	
  x *= value;
  y *= value;
  z *= value;
  return *this;
}

inline Vec3 Vec3::operator/(float value) const {
	if(value != 0){
		return Vec3(x/value, y/value, z/value);
	}else{
		return Vec3();
	}
}

inline Vec3& Vec3::operator/=(float value) {
	if(value != 0){	
		x /= value;
		y /= value;
		z /= value;
		return *this;
	}else{
		return Vec3();
	}
}

#endif 
