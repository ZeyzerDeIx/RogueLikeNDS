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

	[[nodiscard]] constexpr T SquaredNorm() const noexcept
	{
		return x * x + y * y; // Squared norm for comparison
	}

	constexpr auto operator<=>(const Vector2& other) const noexcept
	{
		return SquaredNorm() <=> other.SquaredNorm();
	}
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
	int ToFixedPointInt(int x);
	int ToNormalInt(int x);


	constexpr int FIXED_POINT_SHIFT = 8;
	constexpr int TIME_FIXED_POINT_SHIFT = 16; // More precision for delta time
	constexpr int TO_STANDARD_FIXED_POINT_SHIFT = TIME_FIXED_POINT_SHIFT - FIXED_POINT_SHIFT;

	constexpr int FixedPointMult(int a, int b) noexcept
	{
		// Casting to int64_t prevents overflow during multiplication
		return static_cast<int>((static_cast<int64_t>(a) * b) >> FIXED_POINT_SHIFT);
	}

	constexpr Vector2i FixedPointMult(const Vector2i& vec, int scalar) noexcept
	{
		return { FixedPointMult(vec.x, scalar), FixedPointMult(vec.y, scalar) };
	}
}

struct FastRNG {
	unsigned int state;
	unsigned int Next() {
		state = state * 1103515245 + 12345;
		return (state >> 16) & 0x7FFF;
	}
	int Range(int min, int max) {
		if (max < min) return min;
		return min + (Next() % (max - min + 1));
	}
};