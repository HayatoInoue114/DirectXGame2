#include "Model.h"
#include <assert.h>

ModelData Model::LoadObjFile(const std::string& directoryPath, const std::string& filename) {
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
				Vector4 position = positions[elementIndices[0] - 1];
				Vector2 texcoord = texcoords[elementIndices[1] - 1];
				Vector3 normal = normals[elementIndices[2] - 1];
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

Microsoft::WRL::ComPtr<ID3D12Resource> Model::CreateBufferResource(const Microsoft::WRL::ComPtr<ID3D12Device>& device, size_t sizeInBytes) {
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

void Model::Initialize() {

	//Transform変数を作る
	transform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };

	//CreatevertexResource();
	CreateModel();
	CreateMaterialResource();
	CreateVertexBufferView();
	//CreateTransformationMatrixResource();
	CreateInstance();

	CreateSRV();

	WriteDataToResource();


	SetMaterialData();
}

void Model::CreatevertexResource() {
	//vertexResource_ = directX12_->CreateBufferResource(directX12_->GetDevice(), sizeof(VertexData) * vertexIndex_);
}

void Model::CreateVertexBufferView() {
	vertexBufferView_ = {};
	//リソースの先頭のアドレスから使う
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点３つ分のサイズ
	vertexBufferView_.SizeInBytes = UINT(sizeof(VertexData) * modelData_.vertices.size());
	//1頂点当たりのサイズ
	vertexBufferView_.StrideInBytes = sizeof(VertexData);
}

void Model::CreateMaterialResource() {
	materialResource_ = CreateBufferResource(DirectX12::GetInstance()->GetDevice().Get(), sizeof(Material));
	materialData_ = nullptr;
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
}

void Model::CreateTransformationMatrixResource() {
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

void Model::WriteDataToResource() {
	//書き込むためのアドレスを取得
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
	//頂点データをリソースにコピー
	std::memcpy(vertexData_, modelData_.vertices.data(), sizeof(VertexData) * modelData_.vertices.size());
}

void Model::CreateWVPMatrix() {
	//カメラ
	cameraTransform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,-10.0f,} };
	cameramatrix_ = MakeAffineMatrix(cameraTransform_.scale, cameraTransform_.rotate, cameraTransform_.translate);


	//
	/*worldMatrix_ = MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
	viewMatrix_ = Inverse(cameramatrix_);
	projectionMatix_ = MakePerspectiveFovMatrix(0.45f, float(kCliantWidth) / float(kCliantHeight), 0.1f, 100.0f);
	worldViewProjectionMatrix_ = Multiply(worldMatrix_, Multiply(viewMatrix_, projectionMatix_));
	wvpData_->WVP = worldViewProjectionMatrix_;
	wvpData_->World = worldMatrix_;*/

	viewMatrix_ = Inverse(cameramatrix_);
	projectionMatix_ = MakePerspectiveFovMatrix(0.45f, float(kCliantWidth) / float(kCliantHeight), 0.1f, 100.0f);

	for (uint32_t index = 0; index < MAXINSTANCE; ++index) {
		Matrix4x4 worldMatrix = MakeAffineMatrix(transforms_[index].scale, transforms_[index].rotate, transforms_[index].translate);
		Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix_, projectionMatix_));
		instancingData_[index].WVP = worldViewProjectionMatrix;
		instancingData_[index].World = worldMatrix;
	}
}

void Model::SetMaterialData() {
	// 色の設定
	materialData_->color = { 1.0f,1.0f,1.0f,1.0f };
	// Lightingを有効にする
	materialData_->enableLighting = true;
	//UVTransformを単位行列で初期化
	materialData_->uvTransform = MakeIdentity4x4();
	//uvTransform用の変数
	uvTransform_ = {
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f}
	};
}

void Model::CreateModel() {
	//モデル読み込み
	modelData_ = LoadObjFile("resources", "plane");
	//頂点リソースを作る
	vertexResource_ = DirectX12::GetInstance()->CreateBufferResource(DirectX12::GetInstance()->GetDevice().Get(), sizeof(VertexData) * modelData_.vertices.size());
}

void Model::CreateInstance() {
	instancingResource_ = CreateBufferResource(DirectX12::GetInstance()->GetDevice(), sizeof(TransformationMatrix) * MAXINSTANCE);

	//書き込むためのアドレスを取得
	instancingData_ = nullptr;
	instancingResource_->Map(0, nullptr, reinterpret_cast<void**>(&instancingData_));
	//単位行列を書き込んでおく
	for (uint32_t index = 0; index < MAXINSTANCE; ++index) {
		instancingData_[index].WVP = MakeIdentity4x4();
		instancingData_[index].World = MakeIdentity4x4();
	}

	for (uint32_t index = 0; index < MAXINSTANCE; ++index) {
		transforms_[index] = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f}, {index * 0.1f,index * 0.1f,index * 0.1f} };
	}


}

void Model::CreateSRV() {
	descriptorSizeSRV_ = DirectX12::GetInstance()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	D3D12_SHADER_RESOURCE_VIEW_DESC instancingSrvDesc{};
	instancingSrvDesc.Format = DXGI_FORMAT_UNKNOWN;
	instancingSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	instancingSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	instancingSrvDesc.Buffer.FirstElement = 0;
	instancingSrvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	instancingSrvDesc.Buffer.NumElements = MAXINSTANCE;
	instancingSrvDesc.Buffer.StructureByteStride = sizeof(TransformationMatrix);
	instancingSrvHandleCPU_ = TextureManager::GetInstance()->GetCPUDescriptorHandle(DirectX12::GetInstance()->GetSrvDescriptorHeap(), descriptorSizeSRV_, 3);
	instancingSrvHandleGPU_ = TextureManager::GetInstance()->GetGPUDescriptorHandle(DirectX12::GetInstance()->GetSrvDescriptorHeap(), descriptorSizeSRV_, 3);
	DirectX12::GetInstance()->GetDevice()->CreateShaderResourceView(instancingResource_.Get(), &instancingSrvDesc, instancingSrvHandleCPU_);
}


void Model::Update(Transform& transform, Vector4& color) {
	transform_ = transform;
	CreateWVPMatrix();
	//色の指定
	//materialData_->color = color;
	ImGuiAdjustParameter();
}

void Model::Draw(uint32_t textureNum) {


	uvTransformMatrix_ = MakeScaleMatrix(uvTransform_.scale);
	uvTransformMatrix_ = Multiply(uvTransformMatrix_, MakeRotateZMatrix(uvTransform_.rotate.z));
	uvTransformMatrix_ = Multiply(uvTransformMatrix_, MakeTranslateMatrix(uvTransform_.translate));
	materialData_->uvTransform = uvTransformMatrix_;

	DirectX12::GetInstance()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);	//VBVを設定
	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えておけばよい
	
	DirectX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	//wvp用のCBufferの場所を設定
	DirectX12::GetInstance()->GetCommandList()->SetGraphicsRootDescriptorTable(1, instancingSrvHandleGPU_);
	//SRV用のDescriptorTableの先頭を設定。2はrootParameter[2]である。
	DirectX12::GetInstance()->GetCommandList()->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetTextureSrvHandleGPU()[textureNum]);
	DirectX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(3, Light::Getinstance()->GetDirectionalLightResource()->GetGPUVirtualAddress());

	//描画！　（DrawCall/ドローコール)。3頂点で1つのインスタンス。
	DirectX12::GetInstance()->GetCommandList()->DrawInstanced(UINT(modelData_.vertices.size()), MAXINSTANCE, 0, 0);
}

void Model::ImGuiAdjustParameter() {
	ImGui::Text("Sphere");
	ImGui::CheckboxFlags("isLighting", &materialData_->enableLighting, 1);
	ImGui::SliderFloat3("Translate", &transform_.translate.x, -5, 5);
	ImGui::SliderFloat3("Scale", &transform_.scale.x, -5, 5);
	ImGui::SliderFloat3("Rotate", &transform_.rotate.x, -5, 5);
	ImGui::Text("UVTransform");
	ImGui::DragFloat2("UVTranslate", &uvTransform_.translate.x, 0.01f, -10.0f, 10.0f);
	ImGui::DragFloat2("UVScale", &uvTransform_.scale.x, 0.01f, -10.0f, 10.0f);
	ImGui::SliderAngle("UVRotate.z", &uvTransform_.rotate.z);
	ImGui::ColorEdit4("ModelColor", &materialData_->color.x, 1);
}

