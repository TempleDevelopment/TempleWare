#pragma once
#include <Windows.h>

struct Vec2
{
	constexpr Vec2(
		const float x = 0.f,
		const float y = 0.f) noexcept :
		x(x), y(y) { }

	float x, y;
};
struct Vec3
{
	constexpr Vec3(
		const float x = 0.f,
		const float y = 0.f,
		const float z = 0.f) noexcept :
		x(x), y(y), z(z) { }

	const bool WorldToScreen(Vec2& out, float(*ViewMatrix)[4][4]);

	float x, y, z;
};
