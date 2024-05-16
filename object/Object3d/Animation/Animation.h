#pragma once
#include "../../../structure/structure.h"
#include <string>
#include<map>
#include "../../Sphere/Sphere.h"

template <typename tValue>
struct Keyframe {
	float time;
	tValue value;
};

using KeyframeVector3 = Keyframe<Vector3>;
using KeyframeQuaternion = Keyframe<Quaternion>;

template<typename tValue>

struct AnimationCurve {
	std::vector<Keyframe<tValue>> keyFrames;
};

struct NodeAnimation {
	AnimationCurve<Vector3> translate;
	AnimationCurve<Quaternion> rotate;
	AnimationCurve<Vector3> scale;
};

struct Animation {
	float duration{}; //アニメーション全体の尺(単位は秒)
	//NodeAnimationの集合。Node名でひけるようにしておく
	std::map<std::string, NodeAnimation> nodeAnimations;
};

//Animation
Animation LoadAnimationFile(const std::string& fileName);

Vector3 CalculateValue(const std::vector<KeyframeVector3>& keyframes, float time);

Quaternion CalculateValue(const std::vector<KeyframeQuaternion>& keyframes, float time);


//Skeleton
Skeleton CreateSkeleton(const Node& rootNode);

int32_t CreateJoint(const Node& node, const std::optional<int32_t>& parent, std::vector<Joint>& joints);

void UpdateSkeleton(Skeleton& skeleton);

void DrawJoint(Skeleton& skeleton,Camera* camera);

void ApplyAnimation(Skeleton& skeleton, const Animation& animation, float animationTime);