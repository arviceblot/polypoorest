#ifndef __MATRIX4x4_H__
#define __MATRIX4x4_H__ 1

#include "Vector3D.h"

class Matrix3x3
{
public:
  double d[3][3];
  double det(void) const;
};

class Matrix4x4 
{
public:
  double d[4][4];

  Matrix4x4();
  Matrix4x4(const double a11, const double a12, const double a13, const double a14,
	    const double a21, const double a22, const double a23, const double a24,
	    const double a31, const double a32, const double a33, const double a34,
	    const double a41, const double a42, const double a43, const double a44);

  void set(const double, const double, const double, const double, 
	   const double, const double, const double, const double, 
	   const double, const double, const double, const double, 
	   const double, const double, const double, const double);

  void print();

  void setIdentity(void);

  const Matrix4x4 operator*(const Matrix4x4 &) const;
  const sivelab::Vector3D multVector(const sivelab::Vector3D &v, const double w) const;

  double det(void) const; 
  double cofactor(int i, int j) const;
  const Matrix4x4 inverse(void) const;
  const Matrix4x4 transpose(void) const;       // return the transpose of self

  void makeTranslation(const double x, const double y, const double z);
  void makeScale(const double x, const double y, const double z);

  void makeRotationX( const double angle_in_radians );
  void makeRotationY( const double angle_in_radians );
  void makeRotationZ( const double angle_in_radians );
};

inline const Matrix4x4 operator*(const Matrix4x4& m, const double c)
{
  return Matrix4x4(m.d[0][0]*c, m.d[0][1]*c, m.d[0][2]*c, m.d[0][3]*c,
		   m.d[1][0]*c, m.d[1][1]*c, m.d[1][2]*c, m.d[1][3]*c,
		   m.d[2][0]*c, m.d[2][1]*c, m.d[2][2]*c, m.d[2][3]*c,
		   m.d[3][0]*c, m.d[3][1]*c, m.d[3][2]*c, m.d[3][3]*c);
}

#endif

