#pragma once

template <class T>
struct Vector2
{
	T x;
	T y;

	Vector2<T> operator+(const Vector2<T>& other)
	{ return {x+other.x, y+other.y}; }
	Vector2<T> operator-(const Vector2<T>& other)
	{ return {x-other.x, y-other.y}; }

	void operator+=(const Vector2<T>& other)
	{ x+=other.x; y+=other.y; }
	void operator-=(const Vector2<T>& other)
	{ x-=other.x; y-=other.y; }
};

using Vector2f = Vector2<float>;
using Vector2i = Vector2<int>;