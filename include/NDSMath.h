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
	constexpr int FIXED_POINT_SHIFT = 8;
	constexpr int TIME_FIXED_POINT_SHIFT = 16; // More precision for delta time
	constexpr int TO_STANDARD_FIXED_POINT_SHIFT = TIME_FIXED_POINT_SHIFT - FIXED_POINT_SHIFT;
}

struct FixedPoint
{
	int m_Raw = 0;

	constexpr FixedPoint() noexcept = default;
	constexpr explicit FixedPoint(int rawVal) noexcept : m_Raw(rawVal) {}

	[[nodiscard]] static constexpr FixedPoint FromInt(int v) noexcept
	{
		return FixedPoint(v << NDSMath::FIXED_POINT_SHIFT);
	}
	[[nodiscard]] constexpr int ToInt() const noexcept
	{
		return m_Raw >> NDSMath::FIXED_POINT_SHIFT;
	}

	constexpr FixedPoint operator*(FixedPoint other) const noexcept
	{
		return FixedPoint(static_cast<int>((static_cast<int64_t>(m_Raw) * other.m_Raw) >> NDSMath::FIXED_POINT_SHIFT));
	}

	constexpr FixedPoint operator*(int scalar) const noexcept
	{
		return FixedPoint(m_Raw * scalar);
	}

	template <typename T>
	constexpr Vector2<FixedPoint> operator*(const Vector2<T>& v) noexcept
	{ return {*this * v.x, *this * v.y}; }

	FixedPoint operator+(const FixedPoint& fixed_point) const
	{
		return static_cast<FixedPoint>(m_Raw + fixed_point.m_Raw);
	}

	void operator+=(const FixedPoint& fixed_point)
	{
		*this = *this + fixed_point;
	}
};
using Vector2fp = Vector2<FixedPoint>;

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