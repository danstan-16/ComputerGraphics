#pragma once

#include <cstdlib>

namespace nu {
	inline int RandomInt() {
		return rand();
	}
	/// <summary>
	/// Get random interger value [0, max)
	/// </summary>
	/// <param name="max">exclusive max</param>
	/// <returns>Random number between 0 and max (exclusive)</returns>
	inline int RandomInt(int max) {
		return rand() % max;
	}

	/// <summary>
	/// get random int value [min, max]
	/// </summary>
	/// <param name="min">inclusive min</param>
	/// <param name="max">inclusive max</param>
	/// <returns>random number between min and max (inclusive)</returns>
	inline int RandomInt(int min, int max) {
		return min + RandomInt((max - min) + 1);
	}

	inline float RandomFloat() {
		// rand() = 0 <- RAND_MAX
		return rand() / (float)RAND_MAX;
	}

	inline float RandomFloat(float max) {
		// (0 <-> 1) * max
		return RandomFloat() * max;
	}

	inline float RandomFloat(float min, float max) {
		// min + (0 <-> 1) * (max - min)
		return min + RandomFloat() * (max - min);
	}
}


