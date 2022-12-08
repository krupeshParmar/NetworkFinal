#pragma once

#include <glm/glm.hpp>
#include <glm/vec4.hpp>
#include "../System/Components/iComponent.h"

class Light : public iComponent
{
public:
	Light();
	glm::vec4 position;
	glm::vec4 diffuse;
	glm::vec4 specular;		// rgb = highlight color, w = power
	glm::vec4 atten;		// x = constant, y = linear, z = quadratic, w = distance cut off
	glm::vec4 direction;	// spot or directional light
	glm::vec4 param1;		// x = lightType, y = inner angle, z = outer angle, w = TBD
							// 0 = point light
							// 1 = spot light
							// 2 = directional light
	glm::vec4 param2;		// x = 0 for OFF, 1 for ON

	int MyIndex = -1;

	// uniform locations from the shader
	int position_uniformLocation = -1;
	int diffuse_uniformLocation = -1;
	int specular_uniformLocation = -1;
	int atten_uniformLocation = -1;
	int direction_uniformLocation = -1;
	int param1_uniformLocation = -1;
	int param2_uniformLocation = -1;

	void SetConstantAttenuation(float newConstAtten);
	void SetLinearAttenuation(float newConstAtten);
	void SetQuadraticAttenuation(float newConstAtten);

	void TurnON(void);
	void TurnOFF(void);
	void SetToWhite(void);
	void SetToBlack(void);
	virtual void Reset();
	virtual void CopyComponent();
	virtual void GetParentValues(); 
	virtual void Update(PoRoSc porosc);

	enum LIGHT_TYPE {
		POINT_LIGHT,
		DIRECTIONAL_LIGHT,
		SPOT_LIGHT
	};

	LIGHT_TYPE lightType;
	float Range;
	float Intensity;
	int Mode;			// change it to an enum

	void SetLightType(LIGHT_TYPE newLightType);
	void MakeItAPointLight(void);
	void MakeItASpotLight(void);
};

