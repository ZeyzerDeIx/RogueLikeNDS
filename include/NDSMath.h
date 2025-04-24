#pragma once

#include <iostream>

#include <concepts>
#include <cmath>
#include <compare>

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

	// Addition with scalar (Vector2 + T)
	constexpr Vector2 operator+(const T other) const noexcept {
		return {x + other, y + other};
	}
	constexpr Vector2& operator+=(const T other) noexcept {
		x += other; y += other;
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
	// Subtraction with scalar (Vector2 - T)
	constexpr Vector2 operator-(const T other) const noexcept {
		return {x - other, y - other};
	}
	constexpr Vector2& operator-=(const T other) noexcept {
		x -= other; y -= other;
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
	// Return ceil of the abs value while conserving sign
	int roundAbsCeil(float x);
	Vector2i roundAbsCeil(Vector2f vec);

	// A hash function used to hash a Vector2i
	struct HashVector2i
	{
		size_t operator()(const Vector2i& vec) const
		{
			// Hash the first element
			size_t hash1 = std::hash<int>{}(vec.x);
			// Hash the second element
			size_t hash2 = std::hash<int>{}(vec.y);
			// Combine the two hash values
			return hash1
				   ^ (hash2 + 0x9e3779b9 + (hash1 << 6)
					  + (hash1 >> 2));
		}
	};
}