#include "Particle.h"
#include <assert.h>
#include "../../manager/ModelManager/ModelManager.h"
#include <numbers>

ModelData Particle::LoadObjFile(const std::string& directoryPath, const std::string& filename) {
	ModelData modelData; // 構築するModelData
	std::vector<Vector4> positions; // 位置
	std::vector<Vector3> normals; // 法線
	std::vector<Vector2> texcoords; // テクスチャ座標
	std::string line; // ファイルから呼んだ1行を格納するもの

	std::ifstream file(directoryPath + "/" + filename + ".obj"); // ファイルを開く
	assert(file.is_open()); // とりあえず聞けなかったら止める

	while (std::getline(file, line))
	{
		std::string identifier;
		std::istringstream s(line);
		s >> identifier; // 先頭の識別子を読む

		//identifierに応じた報酬
		if (identifier == "v") {
			Vector4 position{};
			s >> position.x >> position.y >> position.z;
			position.z *= -1.0f;
			position.w = 1.0f;
			positions.push_back(position);
		}
		else if (identifier == "vt") {
			Vector2 texcoord{};
			s >> texcoord.x >> texcoord.y;
			texcoord.y = 1.0f - texcoord.y;
			texcoords.push_back(texcoord);
		}
		else if (identifier == "vn") {
			Vector3 normal{};
			s >> normal.x >> normal.y >> normal.z;
			normal.x *= -1.0f;
			normals.push_back(normal);
		}
		else if (identifier == "f") {
			VertexData triangle[3]{};
			//面は三角形限定。その他は未対応
			for (int32_t faceVertex = 0; faceVertex < 3; ++faceVertex) {
				std::string vertexDefinition;
				s >> vertexDefinition;
				//頂点の要素へのIndexは「位置/UV/法線」で格納されているので、分解してIndexを取得する
				std::istringstream v(vertexDefinition);
				uint32_t elementIndices[3]{};
				for (int32_t element = 0; element < 3; ++element) {
					std::string index;
					std::getline(v, index, '/');// /区切りでインデックスを読んでいく
					elementIndices[element] = std::stoi(index);
				}
				Vector4 position = positions[static_cast<std::vector<Vector4, std::allocator<Vector4>>::size_type>(elementIndices[0]) - 1];
				Vector2 texcoord = texcoords[static_cast<std::vector<Vector2, std::allocator<Vector2>>::size_type>(elementIndices[1]) - 1];
				Vector3 normal = normals[static_cast<std::vector<Vector3, std::allocator<Vector3>>::size_type>(elementIndices[2]) - 1];
				VertexData vertex = { position,texcoord,normal };
				modelData.vertices.push_back(vertex);
				triangle[faceVertex] = { position,texcoord,normal };
			}
			//頂点を逆順で登録することで、回り順を逆にする
			modelData.vertices.push_back(triangle[2]);
			modelData.vertices.push_back(triangle[1]);
			modelData.vertices.push_back(triangle[0]);
		}
		else if (identifier == "mtllib") {
			//materialTemplateLibraryファイルの名前を取得する
			std::string materialFilename;
			s >> materialFilename;
			//基本的にobjファイルと同一階層にmtlは存在させるので,ディレクトリ名とファイル名を渡す
			modelData.material = LoadMaterialTemplateFile(materialFilename);
		}
	}
	return modelData;
}

Microsoft::WRL::ComPtr<ID3D12Resource> Particle::CreateBufferResource(const Microsoft::WRL::ComPtr<ID3D12Device>& device, size_t sizeInBytes) {
	HRESULT hr;
	// 頂点リソース用のヒープの設定
	D3D12_HEAP_PROPERTIES uploadHeapProperties{};
	uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD; // UploadHeapを使う
	// 頂点リソースの設定
	D3D12_RESOURCE_DESC vertexResourceDesc{};
	// バッファソース。テクスチャの場合はまた別の設定をする
	vertexResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	vertexResourceDesc.Width = sizeInBytes; // リソースのサイズ。今回はVector4を3頂点分
	// バッファの場合はこれからは1にする決まり
	vertexResourceDesc.Height = 1;
	vertexResourceDesc.DepthOrArraySize = 1;
	vertexResourceDesc.MipLevels = 1;
	vertexResourceDesc.SampleDesc.Count = 1;
	// バッファの場合はこれにする決まり
	vertexResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource;
	// 実際に頂点リソースを作る
	hr = device.Get()->CreateCommittedResource(&uploadHeapProperties, D3D12_HEAP_FLAG_NONE,
		&vertexResourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertexResource));
	assert(SUCCEEDED(hr));

	return vertexResource;
}

Particle* Particle::CreateModelFromObj(int modelName) {
	Particle* model = new Particle();

	model->Initialize();
	// モデルの読み込み

	model->modelData_ = ModelManager::GetInstance()->GetModelData()[modelName];

	
	return model;
}

std::unique_ptr<Particle> Particle::CreateModelFromObjPtr(int modelName) {
	std::unique_ptr<Particle> model;
	model = std::make_unique<Particle>();
	model->Initialize();
	
	// モデルの読み込み
	model->modelData_ = ModelManager::GetInstance()->GetModelData()[modelName];

	
	return model;
}

ParticleData Particle::MakeNewParticle(std::mt19937& randomEngine) {
	std::uniform_real_distribution<float> distribution(-1.0f, 1.0f);
	ParticleData particle;
	particle.transform.Initialize();
	particle.transform.scale = { 1.0f,1.0f,1.0f };
	particle.transform.rotate = {};
	particle.transform.translate = { distribution(randomEngine),distribution(randomEngine), distribution(randomEngine) };
	particle.velocity = { distribution(randomEngine),distribution(randomEngine), distribution(randomEngine) };
	std::uniform_real_distribution<float> distTime(1.0f, 3.0f);
	particle.lifeTime = distTime(randomEngine);
	particle.currentTime = 0;
	return particle;
}

void Particle::Initialize() {

	//Transform変数を作る
	transform_.Initialize();

	//CreateVertexResource();
	CreateModel();
	CreateMaterialResource();
	CreateVertexBufferView();
	//CreateTransformationMatrixResource();
	CreateInstance();

	CreateSRV();

	WriteDataToResource();


	SetMaterialData();

	// エミッターの設定
	emitter_.count = 3;
	emitter_.frequency = 10;
	emitter_.frequencyTime = 0.0f;
	emitter_.transform.Initialize();

	// フィールド(疑似風を作成)
	accField_.acc = { 15,0,0 };
	accField_.area.min = { -10,-10,-10 };
	accField_.area.max = { 10,10,10 };
	accField_.isActive = true;
}

void Particle::CreateVertexResource() {
	//vertexResource_ = directX12_->CreateBufferResource(directX12_->GetDevice(), sizeof(VertexData) * vertexIndex_);
}

void Particle::CreateVertexBufferView() {
	vertexBufferView_ = {};
	//リソースの先頭のアドレスから使う
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点３つ分のサイズ
	vertexBufferView_.SizeInBytes = UINT(sizeof(VertexData) * modelData_.vertices.size());
	//1頂点当たりのサイズ
	vertexBufferView_.StrideInBytes = sizeof(VertexData);
}

void Particle::CreateMaterialResource() {
	materialResource_ = CreateBufferResource(DirectX12::GetInstance()->GetDevice().Get(), sizeof(Material));
	materialData_ = nullptr;
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
}

void Particle::CreateTransformationMatrixResource() {
	//WVP用のリソースを作る。Matrix4x4　1つ分のサイズを用意する
	wvpResource_ = DirectX12::GetInstance()->CreateBufferResource(DirectX12::GetInstance()->GetDevice().Get(), sizeof(TransformationMatrix));
	//データを書き込む
	wvpData_ = nullptr;
	//書き込むためのアドレスを取得
	wvpResource_->Map(0, nullptr, reinterpret_cast<void**>(&wvpData_));
	//単位行列を書き込んでおく
	wvpData_->WVP = MakeIdentity4x4();
	wvpData_->World = MakeIdentity4x4();
}

void Particle::WriteDataToResource() {
	//書き込むためのアドレスを取得
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
	//頂点データをリソースにコピー
	std::memcpy(vertexData_, modelData_.vertices.data(), sizeof(VertexData) * modelData_.vertices.size());
}

void Particle::CreateWVPMatrix() {
	//カメラ
	cameraTransform_.Initialize();
	cameraTransform_.translate = { 0.0f,0.0f,-10.0f };
	cameraMatrix_ = MakeAffineMatrix(cameraTransform_.scale, cameraTransform_.rotate, cameraTransform_.translate);

	viewMatrix_ = Inverse(cameraMatrix_);
	projectionMatrix_ = MakePerspectiveFovMatrix(0.45f, float(kCliantWidth) / float(kCliantHeight), 0.1f, 100.0f);
	
}

void Particle::SetMaterialData() {
	// 色の設定
	materialData_->color = { 1.0f,1.0f,1.0f,1.0f };
	// Lightingを有効にする
	materialData_->enableLighting = true;
	//UVTransformを単位行列で初期化
	materialData_->uvTransform = MakeIdentity4x4();
	//uvTransform用の変数
	uvTransform_.Initialize();
}

void Particle::CreateModel() {
	//モデル読み込み
	modelData_ = LoadObjFile("resources", "plane");
	//頂点リソースを作る
	vertexResource_ = DirectX12::GetInstance()->CreateBufferResource(DirectX12::GetInstance()->GetDevice().Get(), sizeof(VertexData) * modelData_.vertices.size());
}

void Particle::LoadModel(const std::string& filename) {
	//モデル読み込み
	modelData_ = LoadObjFile("resources", filename);
	//頂点リソースを作る
	vertexResource_ = DirectX12::GetInstance()->CreateBufferResource(DirectX12::GetInstance()->GetDevice().Get(), sizeof(VertexData) * modelData_.vertices.size());
}

void Particle::CreateInstance() {
	instancingResource_ = CreateBufferResource(DirectX12::GetInstance()->GetDevice(), sizeof(ParticleForGPU) * MAXINSTANCE);

	//書き込むためのアドレスを取得
	instancingResource_->Map(0, nullptr, reinterpret_cast<void**>(&instancingData_));
	//単位行列を書き込んでおく
	for (uint32_t index = 0; index < MAXINSTANCE; ++index) {
		instancingData_[index].WVP = MakeIdentity4x4();
		instancingData_[index].World = MakeIdentity4x4();
	}

	/*for (uint32_t index = 0; index < MAXINSTANCE; ++index) {
		particles_[index].transform.Initialize();
	}*/


}

void Particle::CreateSRV() {
	descriptorSizeSRV_ = DirectX12::GetInstance()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	D3D12_SHADER_RESOURCE_VIEW_DESC instancingSrvDesc{};
	instancingSrvDesc.Format = DXGI_FORMAT_UNKNOWN;
	instancingSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	instancingSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	instancingSrvDesc.Buffer.FirstElement = 0;
	instancingSrvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	instancingSrvDesc.Buffer.NumElements = MAXINSTANCE;
	instancingSrvDesc.Buffer.StructureByteStride = sizeof(ParticleForGPU);
	instancingSrvHandleCPU_ = TextureManager::GetInstance()->GetCPUDescriptorHandle(SrvManager::GetInstance()->Allocate());
	instancingSrvHandleGPU_ = TextureManager::GetInstance()->GetGPUDescriptorHandle(SrvManager::GetInstance()->Allocate());
	DirectX12::GetInstance()->GetDevice()->CreateShaderResourceView(instancingResource_.Get(), &instancingSrvDesc, instancingSrvHandleCPU_);
}


void Particle::Update() {
	for (std::list<ParticleData>::iterator particleIterator = particles_.begin(); particleIterator != particles_.end();) {
		// 生存時間が過ぎたら処理を行わない
		/*if ((*particleIterator).lifeTime <= (*particleIterator).currentTime) {
			particleIterator = particles_.erase(particleIterator);
			continue;
		}*/

		
		
		
		/*if (numInstance_ < MAXINSTANCE) {
			
			
			
			
			++numInstance_;
		}*/

		++particleIterator;
	}

	/*emitter_.frequencyTime += kDeltaTime;
	if (emitter_.frequency <= emitter_.frequencyTime) {
		std::random_device seedGenerator;
		std::mt19937 randomEngine(seedGenerator());
		particles_.splice(particles_.end(), Emission(emitter_, randomEngine));
		emitter_.frequencyTime -= emitter_.frequency;
	}*/
}

void Particle::Draw(Camera* camera, uint32_t textureNum) {
	CreateWVPMatrix();
	// カメラ行列
	Matrix4x4 cameraMatrix = MakeAffineMatrix(Vector3{ 1,1,1 }, camera->GetRotate(), camera->GetTranslate());
	// 板ポリを正面に向ける
	Matrix4x4 backToFrontMatrix = MakeRotateYMatrix(std::numbers::pi_v<float>);
	// billboardMatrixを作成
	Matrix4x4 billboardMatrix = cameraMatrix * backToFrontMatrix;
	billboardMatrix.m[3][0] = 0.0f;
	billboardMatrix.m[3][1] = 0.0f;
	billboardMatrix.m[3][2] = 0.0f;


	//for (uint32_t index = 0; index < MAXINSTANCE; ++index) {
	//	if (particles_[index].lifeTime <= particles_[index].currentTime) { // 生存時間を過ぎていたら更新せず描画対象にしない
	//		continue;
	//	}
	//	float alpha = 1.0f - (particles_[index].currentTime / particles_[index].lifeTime);
	//	Matrix4x4 worldMatrix =AffineMatrix(particles_[index].transform.scale, billboardMatrix, particles_[index].transform.translate);
	//	Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix_, projectionMatrix_));
	//	particles_[index].transform.translate += particles_[index].velocity * kDeltaTime;
	//	particles_[index].currentTime += kDeltaTime; // 経過時間を足す
	//	instancingData_[index].WVP = worldViewProjectionMatrix;
	//	instancingData_[index].World = worldMatrix;
	//	instancingData_[index].color = particles_[index].color;
	//	instancingData_[index].color.w = alpha;
	//	++numInstance_; //生きていればParticleの数を1つカウントする
	//}

	numInstance_ = 0;

	for (std::list<ParticleData>::iterator particleIterator = particles_.begin(); particleIterator != particles_.end();) {
		// 生存時間が過ぎたら処理を行わない
		if ((*particleIterator).lifeTime <= (*particleIterator).currentTime) {
			particleIterator = particles_.erase(particleIterator);
			continue;
		}

		

		if (numInstance_ < MAXINSTANCE) {
			
			// 移動処理
			(*particleIterator).transform.translate = Add((*particleIterator).transform.translate, (*particleIterator).velocity);
			// 指定した時間に透明になる

			float alpha = 1.0f - ((*particleIterator).currentTime / (*particleIterator).lifeTime);
			(*particleIterator).color.w = alpha;
			instancingData_[numInstance_].color = (*particleIterator).color;

			// WVPとworldMatrixの計算
			Matrix4x4 scaleMatrix = MakeScaleMatrix((*particleIterator).transform.scale);
			Matrix4x4 translateMatrix = MakeTranslateMatrix((*particleIterator).transform.translate);

			Matrix4x4 cameraMat = camera->GetViewMatrix() * camera->GetProjectionMatrix();


			//Matrix4x4 worldMatrix = AffineMatrix((*particleIterator).transform.scale, billboardMatrix, (*particleIterator).transform.translate);//MakeAffineMatrix(particles_[index].transform.scale, Vector3{ 1,1,1 }/*particles_[index].transform.rotate*/, particles_[index].transform.translate);
			Matrix4x4 worldMatrix = scaleMatrix * billboardMatrix * translateMatrix;
			instancingData_[numInstance_].WVP = worldMatrix * cameraMat;
			
			++numInstance_;
		}

		// 時間を進める
		(*particleIterator).currentTime += kDeltaTime;

		++particleIterator;

		
	}

	//パラメータからUVTransform用の行列を生成する
	uvTransformMatrix_ = MakeScaleMatrix(uvTransform_.scale);
	uvTransformMatrix_ = Multiply(uvTransformMatrix_, MakeRotateZMatrix(uvTransform_.rotate.z));
	uvTransformMatrix_ = Multiply(uvTransformMatrix_, MakeTranslateMatrix(uvTransform_.translate));
	materialData_->uvTransform = uvTransformMatrix_;

	DirectX12::GetInstance()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);	//VBVを設定
	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えておけばよい
	DirectX12::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DirectX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	//wvp用のCBufferの場所を設定
	DirectX12::GetInstance()->GetCommandList()->SetGraphicsRootDescriptorTable(1, instancingSrvHandleGPU_);
	//SRV用のDescriptorTableの先頭を設定。2は:rootParameter[2]である。
	DirectX12::GetInstance()->GetCommandList()->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetGPUDescriptorHandle(textureNum));
	DirectX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(3, Light::Getinstance()->GetDirectionalLightResource()->GetGPUVirtualAddress());

	//描画！　（DrawCall/ドローコール)。3頂点で1つのインスタンス。
	DirectX12::GetInstance()->GetCommandList()->DrawInstanced(UINT(modelData_.vertices.size()), numInstance_, 0, 0);
}

ParticleData Particle::MakeNewParticle(const Vector3& translate) {
	//// 座標
	//std::uniform_real_distribution<float> distribution(-1.0f, 1.0f);
	//// 速度
	//std::uniform_real_distribution<float> distVel(-1.0f, 1.0f);
	//// 色
	//std::uniform_real_distribution<float> distColor(0.0f, 1.0f);
	//// 生存可能時間
	//std::uniform_real_distribution<float> distTime(1.0f, 3.0f);

	ParticleData particle;
	particle.transform.scale = { 1.0f,1.0f,1.0f };
	particle.transform.rotate = { 0,0,0 };
	Vector3 randomTranslate{};

	Scope trans = { -1.0f,1.0f };
	ScopeVec3 translateVec3 = { trans,trans,trans };
	randomTranslate = RandomGenerator::getRandom(translateVec3);
	//randomTranslate = { distribution(randomEngine),distribution(randomEngine) ,distribution(randomEngine) };
	particle.transform.translate = Add(translate, randomTranslate);
	//if()
	Scope vel = { -0.01f,0.01f };
	ScopeVec3 randomVec3 = { vel,vel,vel };
	particle.velocity = RandomGenerator::getRandom(randomVec3);

	Scope color = { 0,255 };
	ScopeVec4 colorVec4 = { color,color,color,{255,255} };
	particle.color = RandomGenerator::getColorRandom(colorVec4);

	Scope time = { 1.0f,3.0f };
	particle.lifeTime = RandomGenerator::getRandom(time);
	particle.currentTime = 0;
	return particle;
}

std::list<ParticleData> Particle::Emission(const Emitter& emitter)
{
	std::list<ParticleData> particles;
	for (uint32_t count = 0; count < emitter.count; ++count) {
		particles.push_back(MakeNewParticle(emitter_.transform.translate));
	}
	return particles;
}

void Particle::ImGuiAdjustParameter() {
	/*std::random_device seedGenerator;
	std::mt19937 randomEngine(seedGenerator());*/

	ImGui::Begin("Particles");
	if (ImGui::Button("Add Particle")) {
		particles_.splice(particles_.end(), Emission(emitter_));
	}
	ImGui::Text("Emitter.frequencyTime:%f", emitter_.frequencyTime);
	ImGui::DragFloat3("Emitter.Translate", &emitter_.transform.translate.x, 0.01f, -100.0f, 100.0f);
	ImGui::Checkbox("isFieldAcceleration", &accField_.isActive);
	ImGui::End();
}

Matrix4x4 Particle::AffineMatrix(const Vector3& scale, const Matrix4x4& rotateMatrix, const Vector3& translate) {
	// 計算結果
	Matrix4x4 result{};

	// アフィン変換行列の計算
	result.m[0][0] = scale.x * rotateMatrix.m[0][0];
	result.m[0][1] = scale.x * rotateMatrix.m[0][1];
	result.m[0][2] = scale.x * rotateMatrix.m[0][2];
	result.m[0][3] = 0.0f;

	result.m[1][0] = scale.y * rotateMatrix.m[1][0];
	result.m[1][1] = scale.y * rotateMatrix.m[1][1];
	result.m[1][2] = scale.y * rotateMatrix.m[1][2];
	result.m[1][3] = 0.0f;

	result.m[2][0] = scale.z * rotateMatrix.m[2][0];
	result.m[2][1] = scale.z * rotateMatrix.m[2][1];
	result.m[2][2] = scale.z * rotateMatrix.m[2][2];
	result.m[2][3] = 0.0f;

	result.m[3][0] = translate.x;
	result.m[3][1] = translate.y;
	result.m[3][2] = translate.z;
	result.m[3][3] = 1.0f;

	return result;
}



//void Particle::ImGuiAdjustParameter() {
//	//ImGui::Begin("model");
//	////ImGui::Text("Model");
//	//ImGui::CheckboxFlags("isLighting", &materialData_->enableLighting, 1);
//	//ImGui::SliderFloat3("Translate", &transform_.translate.x, -5, 5);
//	//ImGui::SliderFloat3("Scale", &transform_.scale.x, -5, 5);
//	//ImGui::SliderFloat3("Rotate", &transform_.rotate.x, -5, 5);
//	//ImGui::Text("UVTransform");
//	//ImGui::DragFloat2("UVTranslate", &uvTransform_.translate.x, 0.01f, -10.0f, 10.0f);
//	//ImGui::DragFloat2("UVScale", &uvTransform_.scale.x, 0.01f, -10.0f, 10.0f);
//	//ImGui::SliderAngle("UVRotate.z", &uvTransform_.rotate.z);
//	//ImGui::ColorEdit4("ModelColor", &materialData_->color.x, 1);
//	//ImGui::End();
//}

