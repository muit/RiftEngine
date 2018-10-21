// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#include "EngineTypes.h"
#include "Math.h"

using namespace std;


constexpr float DegToRad(const float deg) {
	return (deg * Math::PI) / 180.0f;
}


// Añadir los operadores ==, !=, <, <=, >, >=, +, +=, -, -=
// Añadir también operador que permite sumar a izquierda y derecha con un valor
// entero.
// Añadir operador <<

class Vector2D {
public:
	float x, y;

	constexpr Vector2D() : Vector2D{0, 0} {}

	constexpr Vector2D(const float x, const float y) : x{ x }, y{ y } {}

	constexpr Vector2D(const Vector2D&) = default;
	constexpr Vector2D(Vector2D&&) = default;

	Vector2D& operator=(const Vector2D&) = default;
	Vector2D& operator=(Vector2D&&) = default;

	~Vector2D() = default;


	constexpr void Set(const float inX = 0, const float inY = 0) noexcept {
		x = inX;
		y = inY;
	}

	// Comparison Overloads

	constexpr bool operator== (const Vector2D& other) const {
		return x == other.x && y == other.y;
	}

	constexpr bool operator!= (const Vector2D& other) const {
		return !(*this == other);
	}

	// Operation Overloads
	constexpr Vector2D operator-() const {
		return { -x, -y };
	}

	constexpr Vector2D& operator+=(const Vector2D& rhs) {
		x += rhs.x;
		y += rhs.y;
		return *this;
	}

	constexpr Vector2D& operator-=(const Vector2D& rhs) {
		operator+=(-rhs);
		return *this;
	}

	constexpr Vector2D& operator++() {
		operator+=(Vector2D::One);
		return *this;
	}

	constexpr Vector2D& operator--() {
		operator-=(Vector2D::One);
		return *this;
	}

	constexpr Vector2D& operator*=(const Vector2D& rhs) {
		x *= rhs.x;
		y *= rhs.y;
		return *this;
	}

	constexpr Vector2D& operator/=(const Vector2D& rhs) {
		x /= rhs.x;
		y /= rhs.y;
		return *this;
	}

	static const Vector2D Zero;
	static const Vector2D One;

	static constexpr Vector2D FromAngle(float angle, float intensity = 1);
};

constexpr Vector2D operator+(Vector2D lhs, const Vector2D rhs);
constexpr Vector2D operator-(Vector2D lhs, const Vector2D rhs);
constexpr Vector2D operator*(Vector2D lhs, const Vector2D rhs);
constexpr Vector2D operator*(Vector2D lhs, const float rhs);
constexpr Vector2D operator/(Vector2D lhs, const Vector2D rhs);
ostream & operator<<(ostream &os, const Vector2D& p);
