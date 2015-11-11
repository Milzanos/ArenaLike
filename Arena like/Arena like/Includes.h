#pragma once

#include <iostream>
#include <sfml/Graphics.hpp>
#include <sfml/Audio.hpp>
#include <fstream>
#include <time.h>
#include <random>

using namespace std;

namespace hv
{
	class size
	{
	public:
		size(float x, float y) : x(x), y(y) {};

		float x, y;
	};

	class position
	{
	public:
		position(float x, float y) : x(x), y(y) {};
		void add(position& other)
		{
			x += other.x;
			y += other.y;
		}

		float x, y;
	};

	class bVec2
	{
	public:
		bVec2(bool x, bool y) : x(x), y(y) {};

		bool x, y;
	};

	class bVec3
	{
	public:
		bVec3(bool x, bool y, bool z) : x(x), y(y), z(z) {};

		bool x, y, z;
	};

	class bVec4
	{
	public:
		bVec4(bool x, bool y, bool z, bool w) : x(x), y(y), z(z), w(w) {};

		bool x, y, z, w;
	};

	class bVec5
	{
	public:
		bVec5(bool x, bool y, bool z, bool w, bool v) : x(x), y(y), z(z), w(w), v(v) {};

		bool x, y, z, w, v;
	};
}