#pragma once

#define MAX_LIGHT_COUNT 256
#define MAX_DEFERRED_POINT_LIGHT 1000
#define MAX_WIREFRAME_DEBUG_DRAW 800
#define MAX_LINE_DEBUG_DRAW 2000
#define MAX_DIRECTIONAL_LIGHT_COUNT 8

#define MAT_ID_COLOR 0
#define MAT_ID_DIFFUSE_TEXTURE 1
#define MAT_ID_DIFFUSE_NORMAL_TEXTURE 2
#define MAT_ID_DIFFUSE_NORMAL_PARALLAX_TEXTURE 3

static const float PI = 3.14159f;


struct CameraUniformData
{
	float4x4 ViewMat;
	float4x4 InvViewMat;
	float4x4 ProjectionMat;
	float4x4 InvProjectionMat;
	float4x4 ViewProjectionMat;
	float4x4 InvViewProjectionMat;
	float4 CameraPosition;
	float2 WindowSize;
};

struct ObjectUniformData
{
	float4x4 ModelViewProjectionMat;
	float4x4 ModelMat;
	float4x4 InvModelMat;
	float4x4 NormalMat;
};

struct MaterialUniformData
{
	float4 DiffuseColor;
	float4 SpecularColor;

	//the W component determine the material type
	float4 MaterialMiscData;
};


struct DirectionalLightUniformData
{
	float4 LightDirection[MAX_DIRECTIONAL_LIGHT_COUNT];
	float4 DirectionalLightColor[MAX_DIRECTIONAL_LIGHT_COUNT];
	float4 DirectionalLightMiscData[MAX_DIRECTIONAL_LIGHT_COUNT];
	//for now W is the total number of light
	float4 DirectionalLightUniformMiscData;
};


struct ConstantPointLightData
{
	float4 LightPosition;
	float4 LightColor;
	//for now X is the radius/scale of the light
	float4 LightMiscData;
};


//for drawing AABB debug drawing & bounding sphere debug drawing
struct DebugInstanceUniformData
{
	float4x4 ModelViewProjectionMat;
	float4 Color;
};

struct LineVertexUniformData
{
	float4 Color;
};