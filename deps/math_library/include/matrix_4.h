#ifndef __MATRIX4_H__
#define __MATRIX4_H__ 1

#include "vector_3.h"
#include "vector_4.h"
#include "matrix_3.h"

/*
[0]  [1]  [2]  [3]
[4]  [5]  [6]  [7]
[8]  [9]  [10] [11]
[12] [13] [14] [15]
*/
class Mat4 {
 public:

  Mat4();
  Mat4(float a[16]);
  Mat4(float value);
  Mat4(const Mat4& copy);
  ~Mat4();

  static Mat4 Identity();
  Mat4 Multiply(const Mat4& other) const;

  float Determinant() const;
  Mat4 Adjoint() const;

  Vec4 TransformVec4(const Vec4 &v);
  Vec3 TransformVec3(const Vec3 &r);

  bool GetInverse(Mat4* out) const;
  bool Inverse();

  Mat4 Transpose() const;

  static Mat4 Translate(const Vec3& distance);
  static Mat4 Translate(float x, float y, float z);

  static Mat4 Scale(const Vec3& scale);
  static Mat4 Scale(float x, float y, float z);

  static Mat4 RotateX(float radians);
  static Mat4 RotateY(float radians);
  static Mat4 RotateZ(float radians);

  static Mat4 GetTransform(const Vec3& translate, const Vec3& scale,
                      float rotateX, float rotateY, float rotateZ);

  static Mat4 GetTransform(float trans_x, float trans_y, float trans_z,
                      float scale_x, float scale_y, float scale_Z,
                      float rotateX, float rotateY, float rotateZ);

  // NOTE: Not necessary until OpenGL.
  Mat4 PerspectiveMatrix(float fov, float aspect,
	  float near, float far) const;

  // NOTE: Not necessary until OpenGL.
  Mat4 OrthoMatrix(float right, float left, float top, float valueottom,
	  float near, float far) const;

  Vec4 GetColumn(int colum) const;
  Vec4 GetLine(int line) const;

  Mat4 operator+(const Mat4& other) const;
  Mat4& operator+=(const Mat4& other);
  Mat4 operator+(float value) const;
  Mat4& operator+=(float value);
  Mat4 operator-(const Mat4& other) const;
  Mat4& operator-=(const Mat4& other);
  Mat4 operator-(float value) const;
  Mat4& operator-=(float value);
  Mat4& operator*=(float value);
  Mat4 operator*(float value) const;
  Mat4& operator/=(float value);
  Mat4 operator/(float value) const;
  bool operator==(const Mat4& other);
  bool operator!=(const Mat4& other);
  void operator=(const Mat4& other);

  float m[16];
};


inline Mat4 Mat4::Identity() {
	Mat4 ret_;
	for(int i = 0; i < 16; i++)
	{
    if (i % 5 == 0)
    {
      ret_.m[i] = 1;
    }
    else
    {
      ret_.m[i] = 0;
    }
  }
	
	return ret_;
}

inline Mat4 Mat4::Multiply(const Mat4& other)const  {
  Mat4 result;

  for (int col = 0; col < 4; ++col)
  {
    for (int row = 0; row < 4; ++row)
    {
      result.m[col * 4 + row] =
          m[row] * other.m[col * 4] +
          m[row + 4] * other.m[col * 4 + 1] +
          m[row + 8] * other.m[col * 4 + 2] +
          m[row + 12] * other.m[col * 4 + 3];
    }
  }

  return result;
}

inline float Mat4::Determinant() const {
	
float det = 0.0f;

det += m[0] * (m[5] * (m[10] * m[15] - m[14] * m[11]) -
                      m[6] * (m[9] * m[15] - m[13] * m[11]) +
                      m[7] * (m[9] * m[14] - m[13] * m[10]));

det -= m[1] * (m[4] * (m[10] * m[15] - m[14] * m[11]) -
                      m[6] * (m[8] * m[15] - m[12] * m[11]) +
                      m[7] * (m[8] * m[14] - m[12] * m[10]));

det += m[2] * (m[4] * (m[9] * m[15] - m[13] * m[11]) -
                      m[5] * (m[8] * m[15] - m[12] * m[11]) +
                      m[7] * (m[8] * m[13] - m[12] * m[9]));

det -= m[3] * (m[4] * (m[9] * m[14] - m[13] * m[10]) -
                      m[5] * (m[8] * m[14] - m[12] * m[10]) +
                      m[6] * (m[8] * m[13] - m[12] * m[9]));

return det;
}

inline Mat4 Mat4::Adjoint() const {
  Mat4 adjoint;

  adjoint.m[0] = m[5] * (m[10] * m[15] - m[11] * m[14]) - m[9] * (m[6] * m[15] - m[7] * m[14]) + m[13] * (m[6] * m[11] - m[7] * m[10]);
  adjoint.m[4] = -(m[4] * (m[10] * m[15] - m[11] * m[14]) - m[8] * (m[6] * m[15] - m[7] * m[14]) + m[12] * (m[6] * m[11] - m[7] * m[10]));
  adjoint.m[8] = m[4] * (m[9] * m[15] - m[11] * m[13]) - m[8] * (m[5] * m[15] - m[7] * m[13]) + m[12] * (m[5] * m[11] - m[7] * m[9]);
  adjoint.m[12] = -(m[4] * (m[9] * m[14] - m[10] * m[13]) - m[8] * (m[5] * m[14] - m[6] * m[13]) + m[12] * (m[5] * m[10] - m[6] * m[9]));

  adjoint.m[1] = -(m[1] * (m[10] * m[15] - m[11] * m[14]) - m[9] * (m[2] * m[15] - m[3] * m[14]) + m[13] * (m[2] * m[11] - m[3] * m[10]));
  adjoint.m[5] = m[0] * (m[10] * m[15] - m[11] * m[14]) - m[8] * (m[2] * m[15] - m[3] * m[14]) + m[12] * (m[2] * m[11] - m[3] * m[10]);
  adjoint.m[9] = -(m[0] * (m[9] * m[15] - m[11] * m[13]) - m[8] * (m[1] * m[15] - m[3] * m[13]) + m[12] * (m[1] * m[11] - m[3] * m[9]));
  adjoint.m[13] = m[0] * (m[9] * m[14] - m[10] * m[13]) - m[8] * (m[1] * m[14] - m[2] * m[13]) + m[12] * (m[1] * m[10] - m[2] * m[9]);

  adjoint.m[2] = m[1] * (m[6] * m[15] - m[7] * m[14]) - m[5] * (m[2] * m[15] - m[3] * m[14]) + m[13] * (m[2] * m[7] - m[3] * m[6]);
  adjoint.m[6] = -(m[0] * (m[6] * m[15] - m[7] * m[14]) - m[4] * (m[2] * m[15] - m[3] * m[14]) + m[12] * (m[2] * m[7] - m[3] * m[6]));
  adjoint.m[10] = m[0] * (m[5] * m[15] - m[7] * m[13]) - m[4] * (m[1] * m[15] - m[3] * m[13]) + m[12] * (m[1] * m[7] - m[3] * m[5]);
  adjoint.m[14] = -(m[0] * (m[5] * m[14] - m[6] * m[13]) - m[4] * (m[1] * m[14] - m[2] * m[13]) + m[12] * (m[1] * m[6] - m[2] * m[5]));

  adjoint.m[3] = -(m[1] * (m[6] * m[11] - m[7] * m[10]) - m[5] * (m[2] * m[11] - m[3] * m[10]) + m[9] * (m[2] * m[7] - m[3] * m[6]));
  adjoint.m[7] = m[0] * (m[6] * m[11] - m[7] * m[10]) - m[4] * (m[2] * m[11] - m[3] * m[10]) + m[8] * (m[2] * m[7] - m[3] * m[6]);
  adjoint.m[11] = -(m[0] * (m[5] * m[11] - m[7] * m[9]) - m[4] * (m[1] * m[11] - m[3] * m[9]) + m[8] * (m[1] * m[7] - m[3] * m[5]));
  adjoint.m[15] = m[0] * (m[5] * m[10] - m[6] * m[9]) - m[4] * (m[1] * m[10] - m[2] * m[9]) + m[8] * (m[1] * m[6] - m[2] * m[5]);

  return adjoint;
}

inline Vec4 Mat4::TransformVec4(const Vec4 &v) {
  Vec4 o;
  o.x = m[0] * v.x + m[4] * v.y + m[8]  * v.z + m[12] * v.w;
  o.y = m[1] * v.x + m[5] * v.y + m[9]  * v.z + m[13] * v.w;
  o.z = m[2] * v.x + m[6] * v.y + m[10] * v.z + m[14] * v.w;
  o.w = m[3] * v.x + m[7] * v.y + m[11] * v.z + m[15] * v.w;
  return o;
}

inline Vec3 Mat4::TransformVec3(const Vec3 &r) {
  Vec4 tmp;
  tmp.x = r.x;
  tmp.y = r.y;
  tmp.z = r.z;
  tmp.w = 1.0;
  Vec4 result = TransformVec4(tmp);
  result.x = result.x / result.w;
  result.y = result.y / result.w;
  result.z = result.z / result.w;
  Vec3 k;
  k.x = result.x;
  k.y = result.y;
  k.z = result.z;
  return k;
}

inline bool Mat4::Inverse()
{
  Mat4 inverse;
  if (GetInverse(&inverse))
  {
    for (int i = 0; i < 16; ++i)
    {
      m[i] = inverse.m[i];
    }
    return true;
  }
  else
  {
    return false;
  }
}

inline bool Mat4::GetInverse(Mat4* out) const {
  float det = Determinant();

  if (det == 0.0f)
  {

   return false;
  }
  Mat4 adjoint = Adjoint();
  for (int i = 0; i < 16; ++i)
  {
   out->m[i] = adjoint.m[i] / det;
  }

  return true;  
}

inline Mat4 Mat4::Transpose() const {

  Mat4 ret;

  for (int i = 0; i < 4; ++i)
  {
    for (int j = 0; j < 4; ++j)
    {
      ret.m[i * 4 + j] = m[j * 4 + i];
    }
  }

  return ret;
}

inline Mat4 Mat4::Translate(const Vec3& distance){
//corregir
  Mat4 ret = Identity();
  ret.m[3] = distance.x;
  ret.m[7] = distance.y;
  ret.m[11] = distance.z;
  return ret;
}

inline Mat4 Mat4::Translate(float x, float y, float z){
//corregir
Mat4 translationMatrix = Identity();

translationMatrix.m[0] = 1.0f;
translationMatrix.m[5] = 1.0f;
translationMatrix.m[10] = 1.0f;
translationMatrix.m[15] = 1.0f;

translationMatrix.m[3] = x;
translationMatrix.m[7] = y;
translationMatrix.m[11] = z;

return translationMatrix;
}

inline Mat4 Mat4::Scale(const Vec3& scale){

  Mat4 ret = Identity();
  ret.m[0] = scale.x;
  ret.m[5] = scale.y;
  ret.m[10] = scale.z;
  return ret;
}

inline Mat4 Mat4::Scale(float x, float y, float z){

  Mat4 ret = Identity();
  ret.m[0] = x;
  ret.m[5] = y;
  ret.m[10] = z;
  return ret;
}

inline Mat4 Mat4::RotateX(float radians){
  Mat4 result;

  float cosTheta = cosf(radians);
  float sinTheta = sinf(radians);

  result.m[0]  = 1.0f;
  result.m[1]  = 0.0f;
  result.m[2]  = 0.0f;
  result.m[3]  = 0.0f;

  result.m[4]  = 0.0f;
  result.m[5]  = cosTheta;
  result.m[6]  = sinTheta;
  result.m[7]  = 0.0f;

  result.m[8]  = 0.0f;
  result.m[9]  = -sinTheta;
  result.m[10] = cosTheta;
  result.m[11] = 0.0f;

  result.m[12] = 0.0f;
  result.m[13] = 0.0f;
  result.m[14] = 0.0f;
  result.m[15] = 1.0f;

  return result;

}

inline Mat4 Mat4::RotateY(float radians){
	  Mat4 result;

  float cosTheta = cosf(radians);
  float sinTheta = sinf(radians);

  result.m[0] = cosTheta;
  result.m[1] = 0.0f;
  result.m[2] = -sinTheta;
  result.m[3] = 0.0f;

  result.m[4] = 0.0f;
  result.m[5] = 1.0f;
  result.m[6] = 0.0f;
  result.m[7] = 0.0f;

  result.m[8] = sinTheta;
  result.m[9] = 0.0f;
  result.m[10] = cosTheta;
  result.m[11] = 0.0f;

  result.m[12] = 0.0f;
  result.m[13] = 0.0f;
  result.m[14] = 0.0f;
  result.m[15] = 1.0f;

  return result;

}

inline Mat4 Mat4::RotateZ(float radians) {
	  Mat4 result;

  float cosTheta = cosf(radians);
  float sinTheta = sinf(radians);

  result.m[0] = cosTheta;
  result.m[1] = sinTheta;
  result.m[2] = 0.0f;
  result.m[3] = 0.0f;

  result.m[4] = -sinTheta;
  result.m[5] = cosTheta;
  result.m[6] = 0.0f;
  result.m[7] = 0.0f;

  result.m[8] = 0.0f;
  result.m[9] = 0.0f;
  result.m[10] = 1.0f;
  result.m[11] = 0.0f;

  result.m[12] = 0.0f;
  result.m[13] = 0.0f;
  result.m[14] = 0.0f;
  result.m[15] = 1.0f;

  return result;
}

inline Mat4 Mat4::GetTransform(const Vec3& translate,
								const Vec3& scale,
								float rotateX, float rotateY,
								float rotateZ)   {
  Mat4 translationMatrix = Translate(translate.x, translate.y, translate.z);
  Mat4 scaleMatrix = Scale(scale.x, scale.y, scale.z);
  Mat4 rotationXMatrix = RotateX(rotateX);
  Mat4 rotationYMatrix = RotateY(rotateY);
  Mat4 rotationZMatrix = RotateZ(rotateZ);

  // Realiza las transformaciones en el orden deseado
  Mat4 transformationMatrix = translationMatrix.Multiply(scaleMatrix).Multiply(rotationXMatrix).Multiply(rotationYMatrix).Multiply(rotationZMatrix);

  return transformationMatrix;
}

inline Mat4 Mat4::GetTransform(float trans_x, float trans_y, float trans_z,
	float scale_x, float scale_y, float scale_Z,
	float rotateX, float rotateY, float rotateZ)  {
  Mat4 translationMatrix = Translate(trans_x, trans_y, trans_z);
  Mat4 scaleMatrix = Scale(scale_x, scale_y, scale_Z);
  Mat4 rotationXMatrix = RotateX(rotateX);
  Mat4 rotationYMatrix = RotateY(rotateY);
  Mat4 rotationZMatrix = RotateZ(rotateZ);

  // Realiza las transformaciones en el orden deseado
  Mat4 transformationMatrix = translationMatrix.Multiply(scaleMatrix).Multiply(rotationXMatrix).Multiply(rotationYMatrix).Multiply(rotationZMatrix);

  return transformationMatrix;
}

inline Vec4 Mat4::GetColumn(int colum) const {
  Vec4 ret;

 
  if (colum >= 0 && colum < 4)
  {
    ret.x = m[colum];
    ret.y = m[colum + 4];
    ret.z = m[colum + 8];
    ret.w = m[colum + 12];
  }
  else
  {
    
    ret.x = ret.y = ret.z = ret.w = 0.0f;
  }

  return ret;
}

inline Vec4 Mat4::GetLine(int line) const {
  Vec4 ret;

  if (line >= 0 && line < 4)
  {
    ret.x = m[line * 4];
    ret.y = m[line * 4 + 1];
    ret.z = m[line * 4 + 2];
    ret.w = m[line * 4 + 3];
  }
  else
  {
    ret.x = 0;
    ret.y = 0;
    ret.z = 0;
    ret.w = 0;
    }
  return ret;
}

inline Mat4 Mat4::PerspectiveMatrix(float fov, float aspect,
	float near, float far) const {
	
	return Mat4();
}

inline Mat4 Mat4::OrthoMatrix(float right, float left, float top, float valueottom,
	float near, float far) const {
  Mat4 result = Identity();

  result.m[0] = 2.0f / (right - left);
  result.m[5] = 2.0f / (top - valueottom);
  result.m[10] = -2.0f / (far - near);
  result.m[12] = -(right + left) / (right - left);
  result.m[13] = -(top + valueottom) / (top - valueottom);
  result.m[14] = -(far + near) / (far - near);

  return result;
}

inline Mat4 Mat4::operator+(const Mat4& other) const {
  Mat4 ret;
  for (int i = 0; i < 16; i++)
  {
    ret.m[i] = m[i] + other.m[i];
  }
  return ret;
}

inline Mat4& Mat4::operator+=(const Mat4& other) {
  for (int i = 0; i < 16; i++)
  {
    this->m[i] += other.m[i];
  }
  return (*this);
}

inline Mat4 Mat4::operator+(float value) const {
  Mat4 ret;
  for (int i = 0; i < 16; i++)
  {
    ret.m[i] = m[i] + value;
  }
  return ret;
}

inline Mat4& Mat4::operator+=(float value) {
  for (int i = 0; i < 16; i++)
  {
    this->m[i] += value;
  }
  return (*this);
}

inline Mat4 Mat4::operator-(const Mat4& other) const  {
  Mat4 ret;
  for (int i = 0; i < 16; i++)
  {
    ret.m[i] = m[i] - other.m[i];
  }
  return ret;
}

inline Mat4& Mat4::operator-=(const Mat4& other) {
  for (int i = 0; i < 16; i++)
  {
    m[i] -= other.m[i];
  }
  return (*this);
}

inline Mat4 Mat4::operator-(float value) const  {
  Mat4 ret;
  for (int i = 0; i < 16; i++)
  {
    ret.m[i] = m[i] - value;
  }
  return ret;
}

inline Mat4& Mat4::operator-=(float value) {
  for (int i = 0; i < 16; i++)
  {
    m[i] -= value;
  }
  return (*this);
}

inline Mat4& Mat4::operator*=(float value) {
  for (int i = 0; i < 16; i++)
  {
    m[i] *= value;
  }
  return (*this);
}

inline Mat4 Mat4::operator*(float value) const  {
  Mat4 ret;
  for (int i = 0; i < 16; i++)
  {
    ret.m[i] = m[i] * value;
  }
  return ret;
}

inline Mat4& Mat4::operator/=(float value) {
  for (int i = 0; i < 16; i++)
  {
    m[i] /= value;
  }
  return (*this);
}

inline Mat4 Mat4::operator/(float value) const {
  Mat4 ret;
  for (int i = 0; i < 16; i++)
  {
    ret.m[i] = m[i] / value;
  }
  return ret;
}

inline bool Mat4::operator==(const Mat4& other) {
  for (int i = 0; i < 16; i++)
  {
    if (this->m[i] != other.m[i])
    {
      return false;
    }
  }
  return true;
}

inline bool Mat4::operator!=(const Mat4& other) {
  for (int i = 0; i < 16; i++)
  {
    if (this->m[i] != other.m[i])
    {
      return true;
    }
  }
  return false;
}

inline void Mat4::operator=(const Mat4& other) {
  for (int i = 0; i < 16; i++)
  {
    m[i] = other.m[i];
  }
}

#endif
