#pragma once
#include "../include.h"

struct Vector {
	Vector() noexcept : x(), y(), z() {}

	Vector(float x, float y, float z) noexcept : x(x), y(y), z(z) {}

	Vector& operator+=(const Vector& v) noexcept {
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	};

	Vector& operator-=(const Vector& v) noexcept {
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	};

	float x, y, z;
};

struct ViewMatrix {
	ViewMatrix() noexcept : data() {}

	float* operator[](int index) noexcept {
		return data[index];
	};

	const float* operator[](int index) const noexcept {
		return data[index];
	};

	float data[4][4];
};

static bool w2s(const Vector& world, ImVec2& screen, const ViewMatrix& vm) noexcept {
	float View = 0.f;
	float SightX = ImGui::GetIO().DisplaySize.x / 2, SightY = ImGui::GetIO().DisplaySize.y / 2;

	View = vm[3][0] * world.x + vm[3][1] * world.y + vm[3][2] * world.z + vm[3][3];

	if (View <= 0.01)
		return false;

	screen.x = SightX + (vm[0][0] * world.x + vm[0][1] * world.y + vm[0][2] * world.z + vm[0][3]) / View * SightX;
	screen.y = SightY - (vm[1][0] * world.x + vm[1][1] * world.y + vm[1][2] * world.z + vm[1][3]) / View * SightY;

	return true;
}