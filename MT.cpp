#include "MT.h"

Matrix4x4 MakeIdentity4x4() {
	Matrix4x4 mat = {
		1.0f,0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};
	return mat;
}

Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 resultMatrix = {};

	resultMatrix.m[0][0] = m1.m[0][0] * m2.m[0][0] + m1.m[0][1] * m2.m[1][0] + m1.m[0][2] * m2.m[2][0] + m1.m[0][3] * m2.m[3][0];
	resultMatrix.m[0][1] = m1.m[0][0] * m2.m[0][1] + m1.m[0][1] * m2.m[1][1] + m1.m[0][2] * m2.m[2][1] + m1.m[0][3] * m2.m[3][1];
	resultMatrix.m[0][2] = m1.m[0][0] * m2.m[0][2] + m1.m[0][1] * m2.m[1][2] + m1.m[0][2] * m2.m[2][2] + m1.m[0][3] * m2.m[3][2];
	resultMatrix.m[0][3] = m1.m[0][0] * m2.m[0][3] + m1.m[0][1] * m2.m[1][3] + m1.m[0][2] * m2.m[2][3] + m1.m[0][3] * m2.m[3][3];

	resultMatrix.m[1][0] = m1.m[1][0] * m2.m[0][0] + m1.m[1][1] * m2.m[1][0] + m1.m[1][2] * m2.m[2][0] + m1.m[1][3] * m2.m[3][0];
	resultMatrix.m[1][1] = m1.m[1][0] * m2.m[0][1] + m1.m[1][1] * m2.m[1][1] + m1.m[1][2] * m2.m[2][1] + m1.m[1][3] * m2.m[3][1];
	resultMatrix.m[1][2] = m1.m[1][0] * m2.m[0][2] + m1.m[1][1] * m2.m[1][2] + m1.m[1][2] * m2.m[2][2] + m1.m[1][3] * m2.m[3][2];
	resultMatrix.m[1][3] = m1.m[1][0] * m2.m[0][3] + m1.m[1][1] * m2.m[1][3] + m1.m[1][2] * m2.m[2][3] + m1.m[1][3] * m2.m[3][3];

	resultMatrix.m[2][0] = m1.m[2][0] * m2.m[0][0] + m1.m[2][1] * m2.m[1][0] + m1.m[2][2] * m2.m[2][0] + m1.m[2][3] * m2.m[3][0];
	resultMatrix.m[2][1] = m1.m[2][0] * m2.m[0][1] + m1.m[2][1] * m2.m[1][1] + m1.m[2][2] * m2.m[2][1] + m1.m[2][3] * m2.m[3][1];
	resultMatrix.m[2][2] = m1.m[2][0] * m2.m[0][2] + m1.m[2][1] * m2.m[1][2] + m1.m[2][2] * m2.m[2][2] + m1.m[2][3] * m2.m[3][2];
	resultMatrix.m[2][3] = m1.m[2][0] * m2.m[0][3] + m1.m[2][1] * m2.m[1][3] + m1.m[2][2] * m2.m[2][3] + m1.m[2][3] * m2.m[3][3];

	resultMatrix.m[3][0] = m1.m[3][0] * m2.m[0][0] + m1.m[3][1] * m2.m[1][0] + m1.m[3][2] * m2.m[2][0] + m1.m[3][3] * m2.m[3][0];
	resultMatrix.m[3][1] = m1.m[3][0] * m2.m[0][1] + m1.m[3][1] * m2.m[1][1] + m1.m[3][2] * m2.m[2][1] + m1.m[3][3] * m2.m[3][1];
	resultMatrix.m[3][2] = m1.m[3][0] * m2.m[0][2] + m1.m[3][1] * m2.m[1][2] + m1.m[3][2] * m2.m[2][2] + m1.m[3][3] * m2.m[3][2];
	resultMatrix.m[3][3] = m1.m[3][0] * m2.m[0][3] + m1.m[3][1] * m2.m[1][3] + m1.m[3][2] * m2.m[2][3] + m1.m[3][3] * m2.m[3][3];

	return resultMatrix;
}

Matrix4x4 MakeRotateXMatrix(float radian) {
	Matrix4x4 mat = {
		1,0,0,0,
		0,std::cosf(radian),std::sinf(radian),0,
		0,-std::sinf(radian),std::cosf(radian),0,
		0,0,0,1
	};
	return mat;
}

Matrix4x4 MakeRotateYMatrix(float radian) {
	Matrix4x4 mat = {
		std::cosf(radian),0,-std::sinf(radian),0,
		0,1,0,0,
		std::sinf(radian),0,std::cosf(radian),0,
		0,0,0,1
	};
	return mat;
}

Matrix4x4 MakeRotateZMatrix(float radian) {
	Matrix4x4 mat = {
		std::cosf(radian),std::sinf(radian),0,0,
		-std::sinf(radian),std::cosf(radian),0,0,
		0,0,1,0,
		0,0,0,1
	};
	return mat;
}


Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
	Matrix4x4 rotateXMatrix = MakeRotateXMatrix(rotate.x);
	Matrix4x4 rotateYMatrix = MakeRotateYMatrix(rotate.y);
	Matrix4x4 rotateZMatrix = MakeRotateZMatrix(rotate.z);
	Matrix4x4 rotateMatrix = Multiply(rotateXMatrix, Multiply(rotateYMatrix, rotateZMatrix));

	Matrix4x4 mat = {
		scale.x * rotateMatrix.m[0][0],   scale.x * rotateMatrix.m[0][1],   scale.x * rotateMatrix.m[0][2],   0,
		scale.y * rotateMatrix.m[1][0],   scale.y * rotateMatrix.m[1][1],   scale.y * rotateMatrix.m[1][2],   0,
		scale.z * rotateMatrix.m[2][0],   scale.z * rotateMatrix.m[2][1],   scale.z * rotateMatrix.m[2][2],   0,
		translate.x,translate.y,translate.z,1
	};
	return mat;
}

float InverseNum(const Matrix4x4& m, int a, int b, int c, int d, int e, int f, int g, int h) {
	float resultNum;
	resultNum = m.m[a - 1][b - 1] * m.m[c - 1][d - 1] * m.m[e - 1][f - 1] * m.m[g - 1][h - 1];
	return resultNum;
}
float InverseNum2(const Matrix4x4& m, int a, int b, int c, int d, int e, int f) {
	float resultNum;
	resultNum = m.m[a - 1][b - 1] * m.m[c - 1][d - 1] * m.m[e - 1][f - 1];
	return resultNum;
}

Matrix4x4 Inverse(const Matrix4x4& m) {
	Matrix4x4 resultInverse = {};
	float resultNum;
	resultNum = InverseNum(m, 1, 1, 2, 2, 3, 3, 4, 4) + InverseNum(m, 1, 1, 2, 3, 3, 4, 4, 2) +
		InverseNum(m, 1, 1, 2, 4, 3, 2, 4, 3) - InverseNum(m, 1, 1, 2, 4, 3, 3, 4, 2) -
		InverseNum(m, 1, 1, 2, 3, 3, 2, 4, 4) - InverseNum(m, 1, 1, 2, 2, 3, 4, 4, 3) -
		InverseNum(m, 1, 2, 2, 1, 3, 3, 4, 4) - InverseNum(m, 1, 3, 2, 1, 3, 4, 4, 2) -
		InverseNum(m, 1, 4, 2, 1, 3, 2, 4, 3) + InverseNum(m, 1, 4, 2, 1, 3, 3, 4, 2) +
		InverseNum(m, 1, 3, 2, 1, 3, 2, 4, 4) + InverseNum(m, 1, 2, 2, 1, 3, 4, 4, 3) +
		InverseNum(m, 1, 2, 2, 3, 3, 1, 4, 4) + InverseNum(m, 1, 3, 2, 4, 3, 1, 4, 2) +
		InverseNum(m, 1, 4, 2, 2, 3, 1, 4, 3) - InverseNum(m, 1, 4, 2, 3, 3, 1, 4, 2) -
		InverseNum(m, 1, 3, 2, 2, 3, 1, 4, 4) - InverseNum(m, 1, 2, 2, 4, 3, 1, 4, 3) -
		InverseNum(m, 1, 2, 2, 3, 3, 2, 4, 1) - InverseNum(m, 1, 3, 2, 4, 3, 2, 4, 1) -
		InverseNum(m, 1, 4, 2, 2, 3, 3, 4, 1) + InverseNum(m, 1, 4, 2, 3, 3, 2, 4, 1) +
		InverseNum(m, 1, 3, 2, 2, 3, 4, 4, 1) + InverseNum(m, 1, 2, 2, 4, 3, 3, 4, 1);

	resultInverse.m[0][0] = (InverseNum2(m, 2, 2, 3, 3, 4, 4) + InverseNum2(m, 2, 3, 3, 4, 4, 2) +
		InverseNum2(m, 2, 4, 3, 2, 4, 3) - InverseNum2(m, 2, 4, 3, 3, 4, 2) -
		InverseNum2(m, 2, 3, 3, 2, 4, 4) - InverseNum2(m, 2, 2, 3, 4, 4, 3)) *
		(1 / resultNum);
	resultInverse.m[0][1] = (InverseNum2(m, 1, 4, 3, 3, 4, 2) + InverseNum2(m, 1, 3, 3, 2, 4, 4) +
		InverseNum2(m, 1, 2, 3, 4, 4, 3) - InverseNum2(m, 1, 2, 3, 3, 4, 4) -
		InverseNum2(m, 1, 3, 3, 4, 4, 2) - InverseNum2(m, 1, 4, 3, 2, 4, 3)) *
		(1 / resultNum);
	resultInverse.m[0][2] = (InverseNum2(m, 1, 2, 2, 3, 4, 4) + InverseNum2(m, 1, 3, 2, 4, 4, 2) +
		InverseNum2(m, 1, 4, 2, 2, 4, 3) - InverseNum2(m, 1, 4, 2, 3, 4, 2) -
		InverseNum2(m, 1, 3, 2, 2, 4, 4) - InverseNum2(m, 1, 2, 2, 4, 4, 3)) *
		(1 / resultNum);
	resultInverse.m[0][3] = (InverseNum2(m, 1, 4, 2, 3, 3, 2) + InverseNum2(m, 1, 3, 2, 2, 3, 4) +
		InverseNum2(m, 1, 2, 2, 4, 3, 3) - InverseNum2(m, 1, 2, 2, 3, 3, 4) -
		InverseNum2(m, 1, 3, 2, 4, 3, 2) - InverseNum2(m, 1, 4, 2, 2, 3, 3)) *
		(1 / resultNum);

	resultInverse.m[1][0] = (InverseNum2(m, 2, 4, 3, 3, 4, 1) + InverseNum2(m, 2, 3, 3, 1, 4, 4) +
		InverseNum2(m, 2, 1, 3, 4, 4, 3) - InverseNum2(m, 2, 1, 3, 3, 4, 4) -
		InverseNum2(m, 2, 3, 3, 4, 4, 1) - InverseNum2(m, 2, 4, 3, 1, 4, 3)) *
		(1 / resultNum);
	resultInverse.m[1][1] = (InverseNum2(m, 1, 1, 3, 3, 4, 4) + InverseNum2(m, 1, 3, 3, 4, 4, 1) +
		InverseNum2(m, 1, 4, 3, 1, 4, 3) - InverseNum2(m, 1, 4, 3, 3, 4, 1) -
		InverseNum2(m, 1, 3, 3, 1, 4, 4) - InverseNum2(m, 1, 1, 3, 4, 4, 3)) *
		(1 / resultNum);
	resultInverse.m[1][2] = (InverseNum2(m, 1, 4, 2, 3, 4, 1) + InverseNum2(m, 1, 3, 2, 1, 4, 4) +
		InverseNum2(m, 1, 1, 2, 4, 4, 3) - InverseNum2(m, 1, 1, 2, 3, 4, 4) -
		InverseNum2(m, 1, 3, 2, 4, 4, 1) - InverseNum2(m, 1, 4, 2, 1, 4, 3)) *
		(1 / resultNum);
	resultInverse.m[1][3] = (InverseNum2(m, 1, 1, 2, 3, 3, 4) + InverseNum2(m, 1, 3, 2, 4, 3, 1) +
		InverseNum2(m, 1, 4, 2, 1, 3, 3) - InverseNum2(m, 1, 4, 2, 3, 3, 1) -
		InverseNum2(m, 1, 3, 2, 1, 3, 4) - InverseNum2(m, 1, 1, 2, 4, 3, 3)) *
		(1 / resultNum);

	resultInverse.m[2][0] = (InverseNum2(m, 2, 1, 3, 2, 4, 4) + InverseNum2(m, 2, 2, 3, 4, 4, 1) +
		InverseNum2(m, 2, 4, 3, 1, 4, 2) - InverseNum2(m, 2, 4, 3, 2, 4, 1) -
		InverseNum2(m, 2, 2, 3, 1, 4, 4) - InverseNum2(m, 2, 1, 3, 4, 4, 2)) *
		(1 / resultNum);
	resultInverse.m[2][1] = (InverseNum2(m, 1, 4, 3, 2, 4, 1) + InverseNum2(m, 1, 2, 3, 1, 4, 4) +
		InverseNum2(m, 1, 1, 3, 4, 4, 2) - InverseNum2(m, 1, 1, 3, 2, 4, 4) -
		InverseNum2(m, 1, 2, 3, 4, 4, 1) - InverseNum2(m, 1, 4, 3, 1, 4, 2)) *
		(1 / resultNum);
	resultInverse.m[2][2] = (InverseNum2(m, 1, 1, 2, 2, 4, 4) + InverseNum2(m, 1, 2, 2, 4, 4, 1) +
		InverseNum2(m, 1, 4, 2, 1, 4, 2) - InverseNum2(m, 1, 4, 2, 2, 4, 1) -
		InverseNum2(m, 1, 2, 2, 1, 4, 4) - InverseNum2(m, 1, 1, 2, 4, 4, 2)) *
		(1 / resultNum);
	resultInverse.m[2][3] = (InverseNum2(m, 1, 4, 2, 2, 3, 1) + InverseNum2(m, 1, 2, 2, 1, 3, 4) +
		InverseNum2(m, 1, 1, 2, 4, 3, 2) - InverseNum2(m, 1, 1, 2, 2, 3, 4) -
		InverseNum2(m, 1, 2, 2, 4, 3, 1) - InverseNum2(m, 1, 4, 2, 1, 3, 2)) *
		(1 / resultNum);

	resultInverse.m[3][0] = (InverseNum2(m, 2, 3, 3, 2, 4, 1) + InverseNum2(m, 2, 2, 3, 1, 4, 1) +
		InverseNum2(m, 2, 1, 3, 3, 4, 2) - InverseNum2(m, 2, 1, 3, 2, 4, 3) -
		InverseNum2(m, 2, 2, 3, 3, 4, 1) - InverseNum2(m, 2, 3, 3, 1, 4, 2)) *
		(1 / resultNum);
	resultInverse.m[3][1] = (InverseNum2(m, 1, 1, 3, 2, 4, 3) + InverseNum2(m, 1, 2, 3, 3, 4, 1) +
		InverseNum2(m, 1, 3, 3, 1, 4, 2) - InverseNum2(m, 1, 3, 3, 2, 4, 1) -
		InverseNum2(m, 1, 2, 3, 1, 4, 3) - InverseNum2(m, 1, 1, 3, 3, 4, 2)) *
		(1 / resultNum);
	resultInverse.m[3][2] = (InverseNum2(m, 1, 3, 2, 2, 4, 1) + InverseNum2(m, 1, 2, 2, 1, 4, 3) +
		InverseNum2(m, 1, 1, 2, 3, 4, 2) - InverseNum2(m, 1, 1, 2, 2, 4, 3) -
		InverseNum2(m, 1, 2, 2, 3, 4, 1) - InverseNum2(m, 1, 3, 2, 1, 4, 2)) *
		(1 / resultNum);
	resultInverse.m[3][3] = (InverseNum2(m, 1, 1, 2, 2, 3, 3) + InverseNum2(m, 1, 2, 2, 3, 3, 1) +
		InverseNum2(m, 1, 3, 2, 1, 3, 2) - InverseNum2(m, 1, 3, 2, 2, 3, 1) -
		InverseNum2(m, 1, 2, 2, 1, 3, 3) - InverseNum2(m, 1, 1, 2, 3, 3, 2)) *
		(1 / resultNum);

	return resultInverse;
}

Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip) {
	Matrix4x4 mat = {
		(1 / aspectRatio) * (1 / std::tanf(fovY / 2)),	0,	0,	0,
		0,	1 / std::tanf(fovY / 2),	0,	0,
		0,	0,	farClip / (farClip - nearClip),	1,
		0,	0,	(-nearClip * farClip) / (farClip - nearClip),	0
	};
	return mat;
}

Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip) {
	Matrix4x4 mat = {
		2 / (right - left),	0,0,0,
		0,2 / (top - bottom),0,0,
		0,0,1 / (farClip - nearClip),0,
		(left + right) / (left - right),(top + bottom) / (bottom - top),nearClip / (nearClip - farClip),1
	};
	return mat;
}

Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth) {
	Matrix4x4 mat = {
		width / 2,0,0,0,
		0,-height / 2,0,0,
		0,0,maxDepth - minDepth,0,
		left + width / 2,top + height / 2,minDepth,1
	};
	return mat;
}

//void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
//	const uint32_t kSubdivision = 16;
//	const float kLonEvery = (2 * M_PI) / kSubdivision;
//	const float kLatEvery = (M_PI) / kSubdivision;
//	//緯度の方向に分割 -π/1 ～ π/2
//	for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex) {
//		float lat = -M_PI / 2.0f + kLatEvery * latIndex; //現在の緯度
//		//経度の方向に分割 0～2π
//		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {
//			float lon = lonIndex * kLonEvery;
//			//world座標系でのa, b, cを求める
//			Vector3 a, b, c;
//			a = { std::cos(lat) * std::cos(lon),
//				std::sin(lat),
//				std::cos(lat) * std::sin(lon) };
//
//			b = { std::cos(lat + kLatEvery) * std::cos(lon),
//				std::sin(lat + kLatEvery),
//				std::cos(lat + kLatEvery) * std::sin(lon) };
//
//			c = { std::cos(lat) * std::cos(lon + kLonEvery),
//				std::sin(lat),
//				std::cos(lat) * std::sin(lon + kLonEvery) };
//
//			a.x *= sphere.radius;
//			a.y *= sphere.radius;
//			a.z *= sphere.radius;
//
//			b.x *= sphere.radius;
//			b.y *= sphere.radius;
//			b.z *= sphere.radius;
//
//			c.x *= sphere.radius;
//			c.y *= sphere.radius;
//			c.z *= sphere.radius;
//
//			a.x += sphere.center.x;
//			a.y += sphere.center.y;
//			a.z += sphere.center.z;
//
//			b.x += sphere.center.x;
//			b.y += sphere.center.y;
//			b.z += sphere.center.z;
//
//			c.x += sphere.center.x;
//			c.y += sphere.center.y;
//			c.z += sphere.center.z;
//
//			Matrix4x4 worldMatrixA = MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f }, a);
//
//			Matrix4x4 worldViewProjectionMatrixA = Multiply(worldMatrixA, viewProjectionMatrix);
//
//			Matrix4x4 worldMatrixB = MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f }, b);
//
//			Matrix4x4 worldViewProjectionMatrixB = Multiply(worldMatrixB, viewProjectionMatrix);
//
//			Matrix4x4 worldMatrixC = MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f }, c);
//
//			Matrix4x4 worldViewProjectionMatrixC = Multiply(worldMatrixC, viewProjectionMatrix);
//
//			Vector3 screenVertices[3]{};
//
//			//NDCまで変換。Transformを使うと同時座標系->デカルト座標系の処理が行われ、結果的にZDvivideが行われることになる
//			Vector3 ndcVertexA = Transform({ 0, 0, 0 }, worldViewProjectionMatrixA);
//			//viewport変換を使ってScreen空間へ
//			screenVertices[0] = Transform(ndcVertexA, viewportMatrix);
//
//			//NDCまで変換。Transformを使うと同時座標系->デカルト座標系の処理が行われ、結果的にZDvivideが行われることになる
//			Vector3 ndcVertexB = Transform({ 0, 0, 0 }, worldViewProjectionMatrixB);
//			//viewport変換を使ってScreen空間へ
//			screenVertices[1] = Transform(ndcVertexB, viewportMatrix);
//
//			//NDCまで変換。Transformを使うと同時座標系->デカルト座標系の処理が行われ、結果的にZDvivideが行われることになる
//			Vector3 ndcVertexC = Transform({ 0, 0, 0 }, worldViewProjectionMatrixC);
//			//viewport変換を使ってScreen空間へ
//			screenVertices[2] = Transform(ndcVertexC, viewportMatrix);
//		}
//	}
//}