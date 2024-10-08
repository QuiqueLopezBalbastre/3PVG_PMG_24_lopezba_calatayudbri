#ifndef __VEC4_H__
#define __VEC4_H__ 1

#include "vector_3.h"
#include "matrix_3.h"

class Vec4 {
 public:

	Vec4();
	Vec4(float x, float y, float z, float w);
	Vec4(Vec3 a, float w);
	Vec4(float a);
	Vec4(float* values_array);
	Vec4(const Vec4& other);
	~Vec4();
	
	Vec4 operator+(const Vec4& other) const;
	Vec4 operator+(float value) const;
	void operator+=(const Vec4& other);
	void operator+=(float value);
	Vec4 operator-(const Vec4& other) const;
	Vec4 operator-(float value) const;
	void operator -=(const Vec4& other);
	void operator -=(float value);

	Vec4 operator*(float value) const;
	void operator*=(float value);
	Vec4 operator/(float value) const;
	void operator/=(float value);
	bool operator==(const Vec4& other);
	bool operator!=(const Vec4& other);
	void operator=(const Vec4& other);

	float Magnitude() const;
	void Normalize();
	Vec4 Normalized() const;
	void Scale(Vec4 scale);
	float SqrMagnitude() const;

	static float Distance(const Vec4& a, const Vec4& b);
	static float DotProduct(Vec4 a, Vec4 b);
	static Vec4 Lerp(const Vec4& a, const Vec4& b, float index);	

	static const Vec4 one;
	static const Vec4 zero;

	float x;
	float y;
	float z;
	float w;

};


inline float Vec4::Magnitude() const{
	return std::sqrt(x * x + y * y + z * z + w * w);
}

inline void Vec4::Normalize() {
	float inverse_magnitude = 1.0f / Magnitude();
	*this *= inverse_magnitude;
}

inline Vec4 Vec4::Normalized() const {
	Vec4 ret;
	ret.x = (x) / (Magnitude());
	ret.y = (y) / (Magnitude());
	ret.z = (z) / (Magnitude());
	ret.w = (w) / (Magnitude());

	return ret;
}

inline void Vec4::Scale(Vec4 Scale) {
	x = x * Scale.x;
	y = y * Scale.y;
	z = z * Scale.z;
	w = w * Scale.w;
}

inline float Vec4::SqrMagnitude() const {
	return (x * x + y * y + z * z + w * w);
}

inline float Vec4::Distance(const Vec4& a, const Vec4& b) {
	Vec4 aux;
	aux.x = a.x - b.x;
	aux.y = a.y - b.y;
	aux.z = a.z - b.z;
	aux.w = a.w - b.w;
	float ret = std::sqrt((aux.x * aux.x) + (aux.y * aux.y) + 
												(aux.z * aux.z) + (aux.w *aux.w));
	return ret;
}

inline float Vec4::DotProduct(Vec4 a, Vec4 b) {
	float ret;
	ret = (a.x * b.x) + (a.y * b.y) + (a.z * b.z) + (a.w * b.w);
	return ret;
}

inline Vec4 Vec4::Lerp(const Vec4& a, const Vec4& b, float index) {
	if (index < 0.0f)
	{
		index = 0.0f;
	}
	else if (index > 1.0f)
	{
		index = 1.0f;
	}

	return Vec4(a.x + (b.x - a.x) * index,
							a.y + (b.y - a.y) * index,
							a.z + (b.z - a.z) * index,
							a.w + (b.w - a.w) * index);
}

inline Vec4 Vec4::operator+(const Vec4& other) const{
	Vec4 ret;
	ret.x = x + other.x;
	ret.y = y + other.y;
	ret.z = z + other.z;
	ret.w = w + other.w;
	return ret;
}

inline Vec4 Vec4::operator+(float value) const{
	Vec4 ret;

	ret.x = x + value;
	ret.y = y + value;
	ret.z = z + value;
	ret.w = w + value;
	return ret;
}

inline void Vec4::operator+=(const Vec4& other) {
	x += other.x;
	y += other.y;
	z += other.z;
	w += other.w;
}

inline void Vec4::operator+=(float value) {
	x += value;
	y += value;
	z += value;
	w += value;
}

inline Vec4 Vec4::operator-(const Vec4& other) const{
	Vec4 aux;
	aux.x = x - other.x;
	aux.y = y - other.y;
	aux.z = z - other.z;
	aux.w = w - other.w;
	return aux;
}

inline Vec4 Vec4::operator-(float value) const{
	Vec4 aux;
	aux.x = x - value;
	aux.y = y - value;
	aux.z = z - value;
	aux.w = w - value;
	return aux;
}

inline void Vec4::operator -=(const Vec4& other) {
	x -= other.x;
	y -= other.y;
	z -= other.z;
	w -= other.w;
}

inline void Vec4::operator -=(float value) {
	x -= value;
	y -= value;
	z -= value;
	w -= value;
}

inline Vec4 Vec4::operator*(float value) const{
	return Vec4(x * value, y * value, z * value, w * value);
}

inline void Vec4::operator*=(float value) {
	x *= value;
	y *= value;
	z *= value;
	w *= value;
}

inline Vec4 Vec4::operator/(float value) const{
	return Vec4(x / value, y / value, z / value, w / value);
}

inline void Vec4::operator/=(float value) {
	x /= value;
	y /= value;
	z /= value;
	w /= value;
}

inline bool Vec4::operator==(const Vec4& other) {
	return (x == other.x) && (y == other.y) && (z == other.z) && (w == other.w);
}
inline bool Vec4::operator!=(const Vec4& other) {
	return (x != other.x) || (y != other.y) || (z != other.z) || (w != other.w);
}
inline void Vec4::operator=(const Vec4& other) {
	x = other.x;
	y = other.y;
	z = other.z;
	w = other.w;
}

#endif 
