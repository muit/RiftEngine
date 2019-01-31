// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreEngine.h"


enum class EKeyPressState : u8 {
	None,
	Down,
	Pressed,
	Up
};

enum class EKeyModifier : u8 {
	LShift = 1 << 0,
	RShift = 1 << 1,
	LCtrl  = 1 << 2,
	RCtrl  = 1 << 3,
	LAlt   = 1 << 4,
	RAlt   = 1 << 5,
	Shift  = LShift | RShift,
	Ctrl   = LCtrl  | RCtrl,
	Alt    = LAlt   | RAlt
};

enum class EKey : u64 {
	MouseLeft   = 1 << 0,
	MouseRight  = 1 << 1,
	MouseCenter = 1 << 2,
	Space       = 1 << 3,
	ArrowUp     = 1 << 4,
	ArrowDown   = 1 << 5,
	ArrowLeft   = 1 << 6,
	ArrowRight  = 1 << 7,
	A  = 1 << 8,
	B  = 1 << 9,
	C  = 1 << 10,
	D  = 1 << 11,
	E  = 1 << 12,
	F  = 1 << 13,
	G  = 1 << 14,
	H  = 1 << 15,
	I  = 1 << 16,
	J  = 1 << 17,
	K  = 1 << 18,
	L  = 1 << 19,
	M  = 1 << 20,
	N  = 1 << 21,
	O  = 1 << 22,
	P  = 1 << 23,
	Q  = 1 << 24,
	R  = 1 << 25,
	S  = 1 << 26,
	T  = 1 << 27,
	W  = 1 << 28,
	X  = 1 << 29,
	Y  = 1 << 30,
	Z  = 1 << 31,
	/*N0 = 1 << 32,
	N1 = 1 << 33,
	N2 = 1 << 34,
	N3 = 1 << 35,
	N4 = 1 << 36,
	N5 = 1 << 37,
	N6 = 1 << 38,
	N7 = 1 << 39,
	N8 = 1 << 40,
	N9 = 1 << 41,
	BackSlash = 1 << 42,
	Tab       = 1 << 43,
	Enter     = 1 << 44,
	Intro     = 1 << 45*/
};

enum class EAxis : u8 {
	MouseX,
	MouseY
};


/** Key Modifier operations */
constexpr u8 operator*(EKeyModifier f)
{
	return static_cast<u8>(f);
}

constexpr EKeyModifier operator|(EKeyModifier lhs, EKeyModifier rhs)
{
	return static_cast<EKeyModifier>((*lhs) | (*rhs));
}

constexpr EKeyModifier operator&(EKeyModifier lhs, EKeyModifier rhs)
{
	return static_cast<EKeyModifier>((*lhs) & (*rhs));
}

constexpr bool operator!(EKeyModifier lhs)
{
	return *lhs <= 0;
}

constexpr bool operator>(EKeyModifier lhs, u8 rhs)
{
	return (*lhs) > rhs;
}

constexpr bool operator<(EKeyModifier lhs, u8 rhs)
{
	return (*lhs) < rhs;
}

constexpr bool operator>(u8 lhs, EKeyModifier rhs)
{
	return lhs > (*rhs);
}

constexpr bool operator<(u8 lhs, EKeyModifier rhs)
{
	return lhs < (*rhs);
}


/** Key operations */
constexpr u64 operator*(EKey f)
{
	return static_cast<u64>(f);
}

constexpr EKey operator|(EKey lhs, EKey rhs)
{
	return static_cast<EKey>((*lhs) | (*rhs));
}

constexpr EKey operator&(EKey lhs, EKey rhs)
{
	return static_cast<EKey>((*lhs) & (*rhs));
}

constexpr bool operator!(EKey lhs)
{
	return *lhs <= 0;
}

constexpr bool operator>(EKey lhs, u64 rhs)
{
	return (*lhs) > rhs;
}

constexpr bool operator<(EKey lhs, u64 rhs)
{
	return (*lhs) < rhs;
}

constexpr bool operator>(u64 lhs, EKey rhs)
{
	return lhs > (*rhs);
}

constexpr bool operator<(u64 lhs, EKey rhs)
{
	return lhs < (*rhs);
}

