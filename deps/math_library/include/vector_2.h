//--------------------------------------------------------------//
//  Math Library
//  Vector 2 Class.
//--------------------------------------------------------------//

#ifndef __VEC2_H__
#define __VEC2_H__ 1

#include <cmath>


/// @class Vec2 vector2.h "vector2.h"
/// @brief Basic float vector 2.
class Vec2 {
 public:

  Vec2();
  Vec2(float x, float y);
  Vec2(const Vec2& copy);
  ~Vec2();

  Vec2 operator+(const Vec2& other) const;
  Vec2 operator+(float value);
  Vec2& operator+=(const Vec2& other);
  Vec2& operator+=(float value);
  Vec2 operator-(const Vec2& other) const;  // Vec2 v = a - b;
  Vec2 operator-(float value) const;        // Vec2 v = a - 3.0f;
  Vec2 operator-() const;                   // Vec2 v = -a;
  Vec2& operator-=(const Vec2& other);
  Vec2& operator-=(float value);
  bool operator==(const Vec2& other) const;
  bool operator!=(const Vec2& other) const;
  void operator=(const Vec2& other);
  void operator=(float value);
  Vec2 operator*(float value) const;
  Vec2& operator*=(float value);
  Vec2 operator/(float value) const;
  Vec2& operator/=(float value);

  float Magnitude() const;

  /// @brief Normalizes the Vec2.
  void Normalize();

  /// @brief Returns a normalized version of this Vec2.
  /// @return Normalized version of this Vec2.
  Vec2 Normalized() const;

  void Scale(const Vec2 scale);

  float SqrMagnitude() const;
  static float Distance(const Vec2 a, const Vec2 b);

  static float DotProduct(Vec2 a, Vec2 b);

  static Vec2 Lerp(const Vec2 a, const Vec2 b, float t);
  static Vec2 LerpUnclamped(const Vec2 a, const Vec2 b, float t);

  static const Vec2 up;
  static const Vec2 down;
  static const Vec2 right;
  static const Vec2 left;
  static const Vec2 zero;
  static const Vec2 one;

  float x;
  float y;
};


// Vec2 v = {0.0f, 1.0f};
// Vec2 u = {2.0f, 3.0f};
// Vec2 t = u + v;
inline Vec2 Vec2::operator+(const Vec2& other) const {
  Vec2 ret;

  ret.x = x + other.x;
  ret.y = y + other.y;

  return ret;
}

inline Vec2 Vec2::operator+(float value)
{
	Vec2 aux;

	aux.x = x + value;
	aux.y = y + value;

	return aux;
}

inline Vec2& Vec2::operator+=(const Vec2& other)
{
	x += other.x;
	y += other.y;

	return *this;
}

inline Vec2& Vec2::operator+=(float value)
{
  x += value;
  y += value;
  return *this;
}

inline Vec2 Vec2::operator-(const Vec2 &other) const
{
		Vec2 aux;
		
		aux.x = x - other.x;
		aux.y = y - other.y;

		return aux;
}

inline Vec2 Vec2::operator-(float value) const {
  Vec2 ret;
  ret.x = x - value;
  ret.y = y - value;
  return ret;
}

inline Vec2 Vec2::operator-() const {
  Vec2 ret;
  ret.x = -x;
  ret.y = -y;
	return ret;
}

inline Vec2 &Vec2::operator-=(const Vec2 &other)
{
	x -= other.x;
	y -= other.y;

	return *this;	
}

inline Vec2 &Vec2::operator-=(float value)
{

	x -= value;
	y -= value;

	return *this;	

}


inline bool Vec2::operator==(const Vec2& value) const {
  return ((x == value.x) && (y == value.y));
}

inline bool Vec2::operator!=(const Vec2& value) const {
  return ((x != value.x) && (y != value.y));;
}

// Vec2 v = { 1.0f, 2.0f };
// Vec2 u = v;
inline void Vec2::operator=(const Vec2& other) {
  x = other.x;
  y = other.y;
}

// Vec2 v = 1.0f;
inline void Vec2::operator=(float value) {
  x = value;
  y = value;
}

inline Vec2 Vec2::operator*(float value) const {
	return Vec2(x * value, y * value);
}

// Vec2 v = { 1.0f, 2.0f };
// v *= (-1.0f);  // Expected v: { -1.0f, -2.0f };
inline Vec2& Vec2::operator*=(float value) {  
  x *= value;
  y *= value;
  return *this;
}

inline Vec2 Vec2::operator/(float value) const {
  if(value != 0){
	  return Vec2(x / value, y / value);
  }else{
    return Vec2();
  }
}

inline Vec2& Vec2::operator/=(float value)
{
  if(value != 0){
	  x /= value;
	  y /= value;
	  return *this;
  }else{
    return Vec2();
  }
}

inline float Vec2::Magnitude() const {

  return std::sqrt(x * x + y * y);
}

inline void Vec2::Normalize() {  
  float inverse_magnitude = 1.0f / Magnitude();
	*this *= inverse_magnitude;
}

inline Vec2 Vec2::Normalized() const {
  Vec2 ret;
  ret.x = (x)/(Magnitude());
  ret.y = (y)/(Magnitude());
	return ret;
}

inline void Vec2::Scale(const Vec2 scale){

  x = x * scale.x;
  y = y * scale.y;

}

inline float Vec2::SqrMagnitude() const {
  return (x * x + y * y);
}

inline float Vec2::Distance(const Vec2 a, const Vec2 b) {
  Vec2 aux;
  aux.x = a.x - b.x;
  aux.y = a.y - b.y;
  float ret = std::sqrt((aux.x * aux.x) + (aux.y * aux.y));
  return ret;
}

inline Vec2 Vec2::Lerp(const Vec2 a, const Vec2 b, float t) {
  if (t > 1.0f || t < -1.0f) { t = 1.0f / t; }
  if (t < 0.0f) { t *= -1.0f; }

  return LerpUnclamped(a, b, t);
}

inline Vec2 Vec2::LerpUnclamped(const Vec2 a, const Vec2 b, float t) {
  return Vec2(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t);
}

inline float Vec2::DotProduct(Vec2 a, Vec2 b) {
  float ret;
  ret = (a.x * b.x) + (a.y * b.y);
	return ret;
}

#endif 
