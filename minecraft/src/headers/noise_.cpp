#include "noise_.h"
#include<accidentalNoise/anl.h>



Noise::Noise()
{
	fastNoise = FastNoise::NewFromEncodedNodeTree("EwAAAIA/EwCPwnU+HAABGwAhACEAGwAWAB0BAAATAK5H4T4HAACPwvU9FgBc////EwAfhes/BwAAcT0KPxsAIQAWAKf///8TAD0KV0AIABYAhQAAABMA7FFIQQcAAAAAAD8APQrXPgAfhes+ARMACtcjPRYAGAEAAAcAAI/CdT0=");
}

float Noise::GetNoiseAnl(int x, int y, int z)
{
	anl::CImplicitGradient groundGradient;
	groundGradient.setGradient(0.0, 0.0, 0.0, 1.0);

	anl::CImplicitFractal lowlandShapeFractal(anl::BILLOW,anl::GRADIENT, anl::QUINTIC);
	lowlandShapeFractal.setNumOctaves(2);
	lowlandShapeFractal.setFrequency(0.25);

	anl::CImplicitAutoCorrect lowlandAutocorrect(0.0,1.0);
	lowlandAutocorrect.setSource(&lowlandShapeFractal);
	anl::CImplicitScaleOffset lowlandScale(0.125, -0.45);
	lowlandScale.setSource(&lowlandAutocorrect);
	anl::CImplicitScaleDomain lowlandYScale(1.0,0.0);
	anl::CImplicitTranslateDomain lowlandTerrain;
	lowlandTerrain.setSource(&groundGradient);
	lowlandTerrain.setYAxisSource(&lowlandYScale);

	return lowlandTerrain.get(x, y, z);

}


