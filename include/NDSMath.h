#pragma once

#include <iostream>
#include <concepts>
#include <cmath>

template <typename T>
struct Vector2
{
	T x, y;

	template <typename U>
	constexpr explicit operator Vector2<U>() const noexcept
	{
		return Vector2<U>{static_cast<U>(x), static_cast<U>(y)};
	}


	// Addition
	constexpr Vector2 operator+(const Vector2& other) const noexcept
	{ return {x + other.x, y + other.y}; }

	constexpr Vector2& operator+=(const Vector2& other) noexcept
	{
		x += other.x; y += other.y;
		return *this;
	}

	// Subtraction
	constexpr Vector2 operator-(const Vector2& other) const noexcept
	{ return {x - other.x, y - other.y}; }

	constexpr Vector2& operator-=(const Vector2& other) noexcept
	{
		x -= other.x; y -= other.y;
		return *this;
	}

	// Scalar multiplication
	constexpr Vector2 operator*(T scalar) const noexcept
	{ return {x * scalar, y * scalar}; }

	constexpr Vector2& operator*=(T scalar) noexcept
	{
		x *= scalar; y *= scalar;
		return *this;
	}

	// Scalar division (only for floating-point types)
	constexpr Vector2 operator/(T scalar) const noexcept
	{ return {x / scalar, y / scalar}; }

	constexpr Vector2& operator/=(T scalar)
	{
		x /= scalar; y /= scalar;
		return *this;
	}

	// Equality comparison
	constexpr bool operator==(const Vector2& other) const noexcept
	{ return x == other.x && y == other.y; }

	constexpr bool operator!=(const Vector2& other) const noexcept
	{ return !(*this == other); }

	// Output stream operator
	friend std::ostream& operator<<(std::ostream& os, const Vector2& v)
	{ return os << "(" << v.x << ", " << v.y << ")";  }
};

// Scalar * Vector multiplication
template <typename T>
constexpr Vector2<T> operator*(T scalar, const Vector2<T>& v) noexcept
{ return {scalar * v.x, scalar * v.y}; }


using Vector2b = Vector2<bool>;
using Vector2f = Vector2<float>;
using Vector2i = Vector2<int>;

template <typename T>
struct Rect { T x, y, w, h; };

namespace NDSMath
{
	int roundAbsCeil(float x);
}