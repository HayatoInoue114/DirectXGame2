#pragma once
#include "../../math/MyMath.h"
#include <map>
//#include "../../object/Sphere/Sphere.h"
class Sphere;

struct EulerTransform
{
	Vector3 scale;
	Vector3 rotate; // Eulerでの回転
	Vector3 translate;
};

struct QuaternionTransform {
	Vector3 scale;
	Quaternion rotate;
	Vector3 translate;
};

struct Joint {
	QuaternionTransform transform; // Transform情報
	Matrix4x4 localMatrix; // localMatrix
	Matrix4x4 skeletonSpaceMatrix; // skeltonSpaceでの変換行列
	std::string name; // 名前
	std::vector<int32_t> children; // 子Jointのindexのリスト。いなければ空
	int32_t index; // 自身のIndex
	std::optional<int32_t> parent; // 親JointのIndex。いなければnull
	std::shared_ptr<Sphere> sphere;
};

struct Skeleton {
	int32_t root; // RootJointnoIndex
	std::map<std::string, int32_t> jointMap; // Joint名とIndexとの辞書
	std::vector<Joint> joints; // 所属しているジョイント
};