//#include "RailCamera.h"
//
//void RailCamera::Initialize(Vector3 translation, Vector3 rotate) {
//	worldTransform_.rotate = rotate;
//	worldTransform_.translate = translation;
//	//viewProjection_.Initialize();
//}
//
//void RailCamera::Update() {
//	Vector3 move = { 0, 0, -0.0f };
//	worldTransform_.translate = Add(worldTransform_.translate, move);
//
//	Vector3 rotate = { 0.0f, 0.0f, 0.0f };
//	worldTransform_.rotate = Add(worldTransform_.rotate, rotate);
//
//	worldTransform_.matWorld_ = MakeAffineMatrix(
//		worldTransform_.scale, worldTransform_.rotate, worldTransform_.translate);
//
//	// カメラオブジェクトのワールド行列からビュー行列を計算する
//	camera_->SetViewMatrix(Inverse(worldTransform_.matWorld_));
//
//
//	// カメラの座標を画面表示する処理
//	/*ImGui::Begin("Camera");
//	ImGui::SliderFloat3("translation", &viewProjection_.translate.x, 30.0f, 100.0f);
//	ImGui::SliderFloat3("rotation", &viewProjection_.rotate.x, -30, 120);
//	ImGui::End();*/
//}

#include "RailCamera.h"
#include "../../Input/Input.h"


// 初期化処理
void RailCamera::Initialize(Vector3 rotate, Vector3 translate)
{
	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	worldTransform_.rotate = rotate;
	worldTransform_.translate = translate;

	// ビュープロジェクションの初期化
	camera_ = std::make_unique<Camera>();
	camera_->Initialize();
	camera_->SetFarClip(100.0f);

	// スプライン曲線制御点(通過点)
	controlPoints_ = {
		translate,
		{0, 0, 4 },
		{0, 0, 8 },
		{0, 5, 16 },
		{0, 0,  32},
		{0, 5, 64 },
		{0, 10, 96},
		{-0,5,128},
		{0,0,0}
	};

	t_ = 0.0f;
	nowFrame_ = 0.0f;


	eye_ = translate;
	eye_T_ = 0.0f;
	eyeIndex_ = 0;

	target_ = {};
	target_T_ = 0.1f;
	targetIndex_ = 0;

	Vector3 i = { 100.0f, 100.0f, 50.0f };
	Vector3 j = { 60.0f, 120.0f, 50.0f };
	Vector3 k = i - j;

	supurain_ = false;

	
}


// 更新処理
void RailCamera::Update()
{
	// ワールド座標の更新
	worldTransform_.UpdateMatrix();

	// ビューの更新
	camera_->Update();

	if (Input::GetInstance()->PushKeyTrigger(DIK_F)) {
		supurain_ = true;
	}

	if (supurain_) {
		const uint32_t kIndex = uint32_t(controlPoints_.size() - 1);

		if (targetIndex_ >= kIndex) {
			eye_T_ = 0.0f;
			eyeIndex_ = 0;
			target_T_ = 0.1f;
			targetIndex_ = 0;
			return;
		}

		UpdateFrame(eye_T_, eyeIndex_);
		UpdateFrame(target_T_, targetIndex_);

		eye_ = CatmullRomInt(eyeIndex_, eye_T_);
		target_ = CatmullRomInt(targetIndex_, target_T_);

		worldTransform_.translate = eye_;


		// 視点の回転
		Vector3 diff = target_ - eye_;
		Vector3 diffRotate = Normalize(target_ - eye_);
		worldTransform_.rotate.y = std::atan2(diffRotate.x, diffRotate.z);
		float velZ = std::sqrt((diffRotate.x * diffRotate.x) + (diffRotate.z * diffRotate.z));
		float height = -diffRotate.y;
		worldTransform_.rotate.x = std::atan2(height, velZ);
	}



	camera_->SetRotate(worldTransform_.rotate);
	camera_->SetTranslate(worldTransform_.translate);
	camera_->SetViewMatrix(Inverse(worldTransform_.matWorld_));

#ifdef _DEBUG

	if (ImGui::TreeNode("RailCamera"))
	{
		ImGui::Text("eye");
		ImGui::Text("eye_t = %.2f", eye_T_);
		ImGui::Text("eye_Index = %d", eyeIndex_);
		ImGui::DragFloat3("eye_Translate", &eye_.x, 0.01f);
		ImGui::Text("target");
		ImGui::Text("target_t = %.2f", target_T_);
		ImGui::Text("target_Index = %d", targetIndex_);
		ImGui::DragFloat3("target_Translate", &target_.x, 0.01f);
		//ImGui::Text("diff = { %.2f, %.2f, %.2f }", diff.x, diff.y, diff.z);
		ImGui::TreePop();
	}

#endif // _DEBUG
}


// フレームの更新処理
void RailCamera::UpdateFrame(float& frame, uint32_t& index)
{
	const uint32_t division = uint32_t(controlPoints_.size() - 1);

	if (index != division) {

		frame += 1.0f / kEndFrame_;

		if (frame >= 1.0f) {
			index++;
			frame = 0.0f;
		}
	}
}

Vector3 RailCamera::CatmullRomInt(uint32_t index, float t)
{
	const uint32_t kIndex = uint32_t(controlPoints_.size() - 1);

	int index0 = ((index - 1) + kIndex) % kIndex;
	int index1 = index;
	int index2 = (index + 1) % kIndex;
	int index3 = (index + 2) % kIndex;

	Vector3 p0 = controlPoints_[index0];
	Vector3 p1 = controlPoints_[index1];
	Vector3 p2 = controlPoints_[index2];
	Vector3 p3 = controlPoints_[index3];

	return CatmullRomInterpolation(p0, p1, p2, p3, t);
}
