#include "noise_.h"
#include<accidentalNoise/anl.h>
#include<iostream>


Noise::Noise()
{
	fastNoise = FastNoise::NewFromEncodedNodeTree("EwAAAIA/EwCPwnU+HAABGwAhACEAGwAWAB0BAAATAK5H4T4HAACPwvU9FgBc////EwAfhes/BwAAcT0KPxsAIQAWAKf///8TAD0KV0AIABYAhQAAABMA7FFIQQcAAAAAAD8APQrXPgAfhes+ARMACtcjPRYAGAEAAAcAAI/CdT0=");
}

void Noise::setupNoiseAnl()
{
	groundGradient.setGradient(0.0, 0.0, 0.0, 1.0);
	lowlandShapeFractal.setNumOctaves(2);
	lowlandShapeFractal.setFrequency(0.25);
	lowlandAutocorrect.setSource(&lowlandShapeFractal);
	lowlandScale.setSource(&lowlandAutocorrect);
	lowlandTerrain.setSource(&groundGradient);
	lowlandTerrain.setYAxisSource(&lowlandYScale);
}

float Noise::GetNoiseAnl(int x, int y, int z)
{
	//std::cout << lowlandShapeFractal.get(x, y, z) << std::endl;
	return lowlandShapeFractal.get(x, y, z);
}


