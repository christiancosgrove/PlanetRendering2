//
// Atmospheric scattering vertex shader
//
// Author: Sean O'Neil
//
// Copyright (c) 2004 Sean O'Neil
//

attribute vec4 vertex;
attribute vec2 tcoord;
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;

uniform vec3 v3CameraPos = vec3(1,0,0);		// The camera's current position
uniform vec3 v3LightPos = vec3(10,0,0);		// The direction vector to the light source
uniform vec3 v3InvWavelength 1/pow(600,4);	// 1 / pow(wavelength, 4) for the red, green, and blue channels
uniform float fCameraHeight = 1.;	// The camera's current height
uniform float fCameraHeight2 = 1.;	// fCameraHeight^2
uniform float fOuterRadius = 1.1;		// The outer (atmosphere) radius
uniform float fOuterRadius2 = 1.1*1.1;	// fOuterRadius^2
uniform float fInnerRadius = 1.;		// The inner (planetary) radius
uniform float fInnerRadius2 = 1.;	// fInnerRadius^2
uniform float fKrESun = 1.;			// Kr * ESun
uniform float fKmESun = 1.;			// Km * ESun
uniform float fKr4PI = 3.14*4;			// Kr * 4 * PI
uniform float fKm4PI = 3.14*4;			// Km * 4 * PI
uniform float fScale 1./(0.1);			// 1 / (fOuterRadius - fInnerRadius)
uniform float fScaleDepth = 1.05;		// The scale depth (i.e. the altitude at which the atmosphere's average density is found)
uniform float fScaleOverScaleDepth = 10./1.05;	// fScale / fScaleDepth
uniform vec3	lightDir = vec3(-1,0,0);
uniform vec3	earthCenter = vec3(0,0,0);

const int iSamples = 4;
const float fInvSamples = 0.25;

varying vec2 texCoord;
varying vec3 color;
varying vec3 secondaryColor;


float scale(float fCos)
{
	float x = 1.0 - fCos;
	return fScaleDepth * exp(-0.00287 + x*(0.459 + x*(3.83 + x*(-6.80 + x*5.25))));
}

void main(void)
{
	// Get the ray from the camera to the vertex and its length (which is the far point of the ray passing through the atmosphere)
	vec3 v3Pos = (modelViewMatrix * vertex).xyz;
	vec3 v3Ray = v3Pos;
	float fFar = length(v3Ray);
	v3Ray /= fFar;
    
	// Calculate the ray's starting position, then calculate its scattering offset
	float fDepth = exp((fInnerRadius - fCameraHeight) / fScaleDepth);
	vec3 normal = normalize(v3Pos - earthCenter);
	float fCameraAngle = dot(-v3Ray, normal );
	float fLightAngle = dot(lightDir, normal );
	float fCameraScale = scale(fCameraAngle);
	float fLightScale = scale(fLightAngle);
	float fCameraOffset = fDepth*fCameraScale;
	float fTemp = (fLightScale + fCameraScale);
    
	// Initialize the scattering loop variables
	float fSampleLength = fFar * fInvSamples;
	float fScaledLength = fSampleLength * fScale;
	vec3 v3SampleRay = v3Ray * fSampleLength;
	vec3 v3SamplePoint = v3SampleRay * 0.5;
    
	// Now loop through the sample rays
	vec3 v3FrontColor = vec3(0.0, 0.0, 0.0);
	vec3 v3Attenuate;
	for(int i=0; i<iSamples; i++)
	{
		float fHeight = length(v3SamplePoint-earthCenter);
		float fDepth = exp(fScaleOverScaleDepth * (fInnerRadius - fHeight));
		float fScatter = fDepth*fTemp - fCameraOffset;
		v3Attenuate = exp(-fScatter * (v3InvWavelength * fKr4PI + fKm4PI));
		v3FrontColor += v3Attenuate * (fDepth * fScaledLength);
		v3SamplePoint += v3SampleRay;
	}
    
	color.rgb = v3FrontColor * (v3InvWavelength * fKrESun + fKmESun);
    
	// Calculate the attenuation factor for the ground
	secondaryColor.rgb = v3Attenuate;
    
	gl_Position = projectionMatrix * modelViewMatrix * vertex;
	texCoord = tcoord;	
}