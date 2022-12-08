#pragma once
#include <glm/glm.hpp>
#include <glm/vec3.hpp>

class LightHelper
{
public:

	static const float DEFAULT_ATTEN_CONST;					// 0.1f
	static const float DEFAULT_ATTEN_LINEAR;				// 0.1f
	static const float DEFAULT_ATTEN_QUADRATIC;				// 0.2f
	static const float DEFAULT_AMBIENT_TO_DIFFUSE_RATIO;	// 0.2f

	static const unsigned int DEFAULT_MAX_ITERATIONS = 50;
	static const float DEFAULT_INFINITE_DISTANCE;			// 10,000.0f
	static const float DEFAULT_DIFFUSE_ACCURACY_THRESHOLD;	// 0.001f
	float CalcApproxDistFromAtten(float targetLightLevel);					// default
	float CalcApproxDistFromAtten(float targetLightLevel, float accuracy);	// default
	float CalcApproxDistFromAtten(float targetLightLevel, float accuracy, 
									float constAttenuation, float linearAttenuation, 
									float quadraticAttenuation,	float infiniteDistance,
									unsigned int maxIterations = DEFAULT_MAX_ITERATIONS);
	static const float DEFAULT_ZERO_THRESHOLD;

	float CalcDiffuseFromAttenByDistance(float distance,
		float constAttenuation,
		float linearAttenuation,
		float quadraticAttenuation,
		float zeroThreshold = DEFAULT_ZERO_THRESHOLD);
};

