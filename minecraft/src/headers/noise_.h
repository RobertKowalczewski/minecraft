#pragma once
#include <FastNoise/FastNoise.h>
#include <perlinNoise.h>
#include<libnoise/noise.h>
#include<accidentalNoise/anl.h>


class Noise {
public:
	//perlinNoise.h
	const siv::BasicPerlinNoise<float> perlinNoise{ 0 };
	//FastNoise/FastNoise.h
	FastNoise::SmartNode<> fastNoise;
	//libnoise
	noise::module::Perlin libNoise;
	//anl

	Noise();
	float GetNoiseAnl(int x, int y, int z);
};