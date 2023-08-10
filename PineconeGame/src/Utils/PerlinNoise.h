#pragma once

#include <glm/glm.hpp>

#define _PERLIN_MAX_PRIMES 10

class PerlinNoise
{
public:
	PerlinNoise();
	PerlinNoise(int octaves, float persistence = 0.5f);
	
	float GetNoise(int x, int y);
private:
	float Noise(int x, int y, int i);
	float SmoothedNoise(int x, int y, int i);

	float Interpolate(float a, float b, float x);
	float InterpolatedNoise(int x, int y, int i);
private:
	int m_PrimeIndex;
	int m_NumOctaves = 7;
	float m_Persistence = 0.5f;
private:
	static int s_Primes[_PERLIN_MAX_PRIMES][3];
};