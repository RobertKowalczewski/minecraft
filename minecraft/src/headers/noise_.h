#pragma once
#include <FastNoise/FastNoise.h>
#include <perlinNoise.h>
#include<libnoise/noise.h>
#include<accidentalNoise/anl.h>


class Noise {
	anl::CImplicitGradient groundGradient;
	anl::CImplicitFractal lowlandShapeFractal = { anl::BILLOW, anl::GRADIENT, anl::QUINTIC };
	anl::CImplicitAutoCorrect lowlandAutocorrect = { 0.0, 1.0 };
	anl::CImplicitScaleOffset lowlandScale = { 0.125, -0.45 };
	anl::CImplicitScaleDomain lowlandYScale = { 1.0, 0.0 };
	anl::CImplicitTranslateDomain lowlandTerrain;
public:

	//perlinNoise.h
	const siv::BasicPerlinNoise<float> perlinNoise{ 0 };
	//FastNoise/FastNoise.h
	FastNoise::SmartNode<> fastNoise;
	//libnoise
	noise::module::Perlin libNoise;
	//anl

	Noise();
	void setupNoiseAnl();
	float GetNoiseAnl(int x, int y, int z);
};