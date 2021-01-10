#pragma once

namespace Math
{
	float dot(vec2 a, vec2 b);
	float dot(vec3 a, vec3 b);
	float dot(vec4 a, vec4 b);
}

struct vec2
{
	float x;
	float y;

  inline vec2 &operator+=(vec2& b);
  inline vec2 &operator-=(vec2& b);
  inline vec2 &operator*=(float b);
  inline vec2 &operator/=(float b);
};
inline vec2
operator+(vec2& a, vec2& b)
{
	vec2 result;
	result.x = a.x + b.x;
	result.y = a.y + b.y;
	return result;
}
inline vec2
operator-(vec2& a, vec2& b)
{
	vec2 result;
	result.x = a.x - b.x;
	result.y = a.y - b.y;
	return result;
}
inline vec2
&vec2::operator+=(vec2& b)
{
  *this = *this + b;
  return *this;
}
inline vec2
&vec2::operator-=(vec2& b)
{
  *this = *this - b;
  return *this;
}
inline vec2
operator*(vec2& a, float b)
{
  vec2 result;
  result.x = a.x * b;
  result.y = a.y * b;
  return result;
}
inline vec2
operator/(vec2& a, float b)
{
  vec2 result;
  result.x = a.x / b;
  result.y = a.y / b;
  return result;
}
inline vec2
&vec2::operator*=(float b)
{
  *this = *this * b;
  return *this;
}
inline vec2
&vec2::operator/=(float b)
{
  *this = *this / b;
  return *this;
}

struct vec3
{
  float x;
  float y;
  float z;

  inline vec3 &operator+=(vec3& b);
  inline vec3 &operator-=(vec3& b);
  inline vec3 &operator*=(float b);
  inline vec3 &operator/=(float b);
};
inline vec3
operator+(vec3& a, vec3& b)
{
	vec3 result;
	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;
	return result;
}
inline vec3
operator-(vec3& a, vec3& b)
{
	vec3 result;
	result.x = a.x - b.x;
	result.y = a.y - b.y;
	result.z = a.z - b.z;
	return result;
}
inline vec3
&vec3::operator+=(vec3& b)
{
  *this = *this + b;
  return *this;
}
inline vec3
&vec3::operator-=(vec3& b)
{
  *this = *this - b;
  return *this;
}
inline vec3
operator*(vec3& a, float b)
{
  vec3 result;
  result.x = a.x * b;
  result.y = a.y * b;
  result.z = a.z * b;
  return result;
}
inline vec3
operator/(vec3& a, float b)
{
  vec3 result;
  result.x = a.x / b;
  result.y = a.y / b;
  result.z = a.z / b;
  return result;
}
inline vec3
&vec3::operator*=(float b)
{
  *this = *this * b;
  return *this;
}
inline vec3
&vec3::operator/=(float b)
{
  *this = *this / b;
  return *this;
}

 
struct vec4
{
  float x;
  float y;
  float z;
  float w;
  
  inline vec4 &operator+=(vec4& b);
  inline vec4 &operator-=(vec4& b);
  inline vec4 &operator*=(float b);
  inline vec4 &operator/=(float b);
};
inline vec4
operator+(vec4& a, vec4& b)
{
	vec4 result;
	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;
	result.w = a.w + b.w;
	return result;
}
inline vec4
operator-(vec4& a, vec4& b)
{
	vec4 result;
	result.x = a.x - b.x;
	result.y = a.y - b.y;
	result.z = a.z - b.z;
	result.w = a.w - b.w;
	return result;
}
inline vec4
&vec4::operator+=(vec4& b)
{
  *this = *this + b;
  return *this;
}
inline vec4
&vec4::operator-=(vec4& b)
{
  *this = *this - b;
  return *this;
}
inline vec4
operator*(vec4& a, float b)
{
  vec4 result;
  result.x = a.x * b;
  result.y = a.y * b;
  result.z = a.z * b;
  result.w = a.w * b;
  return result;
}
inline vec4
operator/(vec4& a, float b)
{
  vec4 result;
  result.x = a.x / b;
  result.y = a.y / b;
  result.z = a.z / b;
  result.w = a.w / b;
  return result;
}
inline vec4
&vec4::operator*=(float b)
{
  *this = *this * b;
  return *this;
}
inline vec4
&vec4::operator/=(float b)
{
  *this = *this / b;
  return *this;
}


struct mat2
{
	vec2 a;
	vec2 b;
};
inline mat2
operator*(mat2& f, mat2& s)
{
  mat2 result;
  result.a.x = f.a.x * s.a.x + f.a.y * s.b.x;
  result.a.y = f.a.x * s.a.y + f.a.y * s.b.y;
  result.b.x = f.b.x * s.a.x + f.b.y * s.b.y;
  result.b.y = f.b.x * s.a.y + f.b.y * s.b.y;
  return result;
}

struct mat3
{
	vec3 a;
	vec3 b;
	vec3 c;
};
inline mat3
operator*(mat3& f, mat3& s)
{
  mat3 result;
  result.a.x = f.a.x * s.a.x + f.a.y * s.b.x + f.a.z * s.c.x;
  result.a.y = f.a.x * s.a.y + f.a.y * s.b.y + f.a.z * s.c.y;
  result.a.z = f.a.x * s.a.z + f.a.y * s.b.z + f.a.z * s.c.z;
  
  result.b.x = f.b.x * s.a.x + f.b.y * s.b.x + f.b.z * s.c.x;
  result.b.y = f.b.x * s.a.y + f.b.y * s.b.y + f.b.z * s.c.y;
  result.b.z = f.b.x * s.a.z + f.b.y * s.b.z + f.b.z * s.c.z;

  result.c.x = f.c.x * s.a.x + f.c.y * s.b.x + f.c.z * s.c.x;
  result.c.y = f.c.x * s.a.y + f.c.y * s.b.y + f.c.z * s.c.y;
  result.c.z = f.c.x * s.a.z + f.c.y * s.b.z + f.c.z * s.c.z;

  return result;
}

struct mat4
{
	vec4 a; //Written out as a.x a.y a.z a.w
	vec4 b; //				 b.x b.y b.z b.w
	vec4 c; //			     ...
	vec4 d;
};
inline mat4
operator*(mat4& f, mat4& s)
{
  mat4 result;
  result.a.x = f.a.x * s.a.x + f.a.y * s.b.x + f.a.z * s.c.x + f.a.w * s.d.x;
  result.a.y = f.a.x * s.a.y + f.a.y * s.b.y + f.a.z * s.c.y + f.a.w * s.d.y;
  result.a.z = f.a.x * s.a.z + f.a.y * s.b.z + f.a.z * s.c.z + f.a.w * s.d.z;
  result.a.w = f.a.x * s.a.w + f.a.y * s.b.w + f.a.z * s.c.w + f.a.w * s.d.w;
  
  result.b.x = f.b.x * s.a.x + f.b.y * s.b.x + f.b.z * s.c.x + f.b.w * s.d.x;
  result.b.y = f.b.x * s.a.y + f.b.y * s.b.y + f.b.z * s.c.y + f.b.w * s.d.y;
  result.b.z = f.b.x * s.a.z + f.b.y * s.b.z + f.b.z * s.c.z + f.b.w * s.d.z;
  result.b.w = f.b.x * s.a.w + f.b.y * s.b.w + f.b.z * s.c.w + f.b.w * s.d.w;

  result.c.x = f.c.x * s.a.x + f.c.y * s.b.x + f.c.z * s.c.x + f.c.w * s.d.x;
  result.c.y = f.c.x * s.a.y + f.c.y * s.b.y + f.c.z * s.c.y + f.c.w * s.d.y;
  result.c.z = f.c.x * s.a.z + f.c.y * s.b.z + f.c.z * s.c.z + f.c.w * s.d.z;
  result.c.w = f.c.x * s.a.w + f.c.y * s.b.w + f.c.z * s.c.w + f.c.w * s.d.w;

  result.d.x = f.d.x * s.a.x + f.d.y * s.b.x + f.d.z * s.c.x + f.d.w * s.d.x;
  result.d.y = f.d.x * s.a.y + f.d.y * s.b.y + f.d.z * s.c.y + f.d.w * s.d.y;
  result.d.z = f.d.x * s.a.z + f.d.y * s.b.z + f.d.z * s.c.z + f.d.w * s.d.z;
  result.d.w = f.d.x * s.a.w + f.d.y * s.b.w + f.d.z * s.c.w + f.d.w * s.d.w;
  
  return result;
}

