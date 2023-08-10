#include "PerlinNoise.h"

#include <ctime>
#include <glm/gtc/constants.hpp>

int PerlinNoise::s_Primes[_PERLIN_MAX_PRIMES][3] = {
  { 995615039, 600173719, 701464987 },
  { 831731269, 162318869, 136250887 },
  { 174329291, 946737083, 245679977 },
  { 362489573, 795918041, 350777237 },
  { 457025711, 880830799, 909678923 },
  { 787070341, 177340217, 593320781 },
  { 405493717, 291031019, 391950901 },
  { 458904767, 676625681, 424452397 },
  { 531736441, 939683957, 810651871 },
  { 997169939, 842027887, 423882827 }
};

PerlinNoise::PerlinNoise()
{
	srand(time(NULL));
	m_PrimeIndex = rand() % 10;
}

PerlinNoise::PerlinNoise(int octaves, float persistence)
	: m_NumOctaves(octaves), m_Persistence(persistence)
{
	srand(time(NULL));
	m_PrimeIndex = rand() % 10;
}

float PerlinNoise::GetNoise(int x, int y)
{
	float total = 0;
	float frequency = glm::pow(2, m_NumOctaves);
	float amplitude = 1;

	for (int i = 0; i < m_NumOctaves; ++i) {
		frequency /= 2;
		amplitude *= m_Persistence;
		total += InterpolatedNoise((m_PrimeIndex + i) % _PERLIN_MAX_PRIMES, x / frequency, y / frequency) * amplitude;
	}

	return total / frequency;
}

float PerlinNoise::Noise(int x, int y, int i)
{
	int n = x + y * 57;
	n = (n << 13) ^ n;

	int a = s_Primes[i][0], b = s_Primes[i][1], c = s_Primes[i][2];
	int t = (n * (n * n * a + b) + c) & 0x7fffffff;

	return 1.0 - (float)(t) / 1073741824.0;
}

float PerlinNoise::SmoothedNoise(int x, int y, int i)
{
	float corners = (Noise(x - 1, y - 1, i) + Noise(x + 1, y - 1, i) + Noise(x - 1, y + 1, i) + Noise(x + 1, y + 1, i)) / 16;
	float sides = (Noise(x - 1, y, i) + Noise(x + 1, y, i) + Noise(x, y - 1, i) + Noise(x, y + 1, i)) / 8;
	float center = Noise(x, y, i) / 4;

	return corners + sides + center;
}

float PerlinNoise::Interpolate(float a, float b, float x)
{
	float ft = x * glm::pi<float>();
	float f = (1 - glm::cos(ft)) * 0.5;
	return  a * (1 - f) + b * f;
}

float PerlinNoise::InterpolatedNoise(int x, int y, int i)
{
	int iX = x;
	float fX = x - iX;
	int iY = y;
	float fY = y - iY;

	float v1 = SmoothedNoise(iX, iY, i);
	float v2 = SmoothedNoise(iX + 1, iY, i);
	float v3 = SmoothedNoise(iX, iY + 1, i);
	float v4 = SmoothedNoise(iX + 1, iY + 1, i);
	float i1 = Interpolate(v1, v2, fX);
	float i2 = Interpolate(v3, v4, fX);
	return Interpolate(i1, i2, fY);
}
