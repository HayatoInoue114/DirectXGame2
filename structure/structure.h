#pragma once
#include "../math/MyMath.h"
#include "MaterialData/MaterialData.h"
#include <wrl.h>

struct AccField {
	Vector3 acc;  // 加速度
	AABB area;	  // 範囲
	bool isActive;// Fieldの活性化
};

struct Emitter {
	TransformS transform;
	uint32_t count; //!< 発生数
	float frequency; //!< 発生頻度
	float frequencyTime; //!< 頻度用時刻
};

struct Material
{
	Vector4 color;
	int32_t enableLighting;
	float padding[3];
	Matrix4x4 uvTransform;
};

struct ModelData
{
	std::vector<VertexData> vertices;
	MaterialData material;
	Node rootNode;
};

struct Node
{
	Matrix4x4 localMatrix{};
	std::string name;
	std::vector<Node> children;
};

struct ParticleData
{
	TransformS transform = {};
	Vector3 velocity = {};
	Vector4 color = {};
	float lifeTime = {};
	float currentTime = {};
};

struct ParticleForGPU
{
	Matrix4x4 WVP;
	Matrix4x4 World;
	Vector4 color;
};

struct VertexData
{
	Vector4 position;
	Vector2 texcoord;
	Vector3 normal;
};

struct KeyframeVector3
{
	Vector3 value; //<! キーフレームの値
	float time; //<! キーフレームの時刻（単位は秒）
};

struct KeyframeQuaternion
{
	Quaternion value; //<! キーフレームの値
	float time; //<! キーフレームの時刻（単位は秒）
};

struct Animation
{
	float duration;
};

typedef struct XAUDIO2_BUFFER {
	UINT32     Flags;
	UINT32     AudioBytes;
	const BYTE* pAudioData;
	UINT32     PlayBegin;
	UINT32     PlayLength;
	UINT32     LoopBegin;
	UINT32     LoopLength;
	UINT32     LoopCount;
	void* pContext;
} XAUDIO2_BUFFER;

//#include "Material/Material.h"
//#include "MaterialData/MaterialData.h"
//#include "ModelData/ModelData.h"
//#include "VertexData/VertexData.h"
//#include "XAudio2_Buffer/XAudio2_Buffer.h"
//#include "ParticleData/ParticleData.h"
//#include "ParticleForGPU/ParticleForGPU.h"
//#include "Emitter/Emitter.h"
//#include "AccField/AccField.h"
//#include "Node/Node.h"