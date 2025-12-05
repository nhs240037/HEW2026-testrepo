#include "Field.h"
#include "ShaderList.h"

Field::Field()
	: csv(CsvData::get_instance())
	, m_pModel{}
	, m_pos()
	, wvp{}
	, m_dxpos()
	, m_field_model(Field_Model::get_instance())
{
	if(false)
	// フィールドモデルの取得
	for (int i = 0; i < 6; i++)
	{
		auto set = static_cast<Field_Model::Field_Model_No>(i);
		m_pModel[i] = m_field_model.GetFieldModel(i);
	}

	std::string fileName[] =
	{
		"Assets/Model/Map/MD/モデル/MD_Tray_Green.fbx",
		"Assets/Model/Map/MD/モデル/MD_Tray_Green_Corner.fbx",
		"Assets/Model/Map/MD/モデル/MD_Tray_Green_Side.fbx",
		"Assets/Model/Map/MD/モデル/MD_Tray_White.fbx",
		"Assets/Model/Map/MD/モデル/MD_Tray_White_Corner.fbx",
		"Assets/Model/Map/MD/モデル/MD_Tray_White_Side.fbx"
	};
	std::string modelName[] = {
		"Green_Top",
		"Green_Corner",
		"Green_Side",
		"White_Top",
		"White_Corner",
		"White_Side"
	};
	// モデルの読み込み
	for(int i = 0;i < 6;i++)
	{
		m_pModel[i] = new Model();
		if (!m_pModel[i]->Load(fileName[i].c_str(), 0.57f, Model::ZFlip))  // 倍率と反転は省略可
		{
			MessageBox(NULL, modelName[i].c_str(), "Error", MB_OK); // エラーメッセージの表示
		}
	}
}

Field::~Field()
{
}

void Field::Update()
{

}

void Field::Draw()
{
	int max = 10;
	int maxZ = 6;
	int set = 0;
	float sabun = 0.25f; // 微調整用の変数
	for(int k = 0; k < maxZ;k++)
	for(int j = 0;j < max;j++)
	{
		m_dxpos = DirectX::XMMatrixRotationY(
			DirectX::XMConvertToRadians(0));

		// 描画するモデルを設定
		set = ((j + (k % 2)) % 2) * 3;

		if (j == 0 && k == 0)
		{
			set = 1; // Green_Corner
			m_dxpos = DirectX::XMMatrixRotationY(
				DirectX::XMConvertToRadians(-90));
			m_dxpos *= DirectX::XMMatrixTranslation(
				j * 2.0f - 9.0f - sabun, -0.20f, k * 2.0f - 05.0f - sabun);
		}
		else if (j == max - 1 && k == 0)
		{
			m_dxpos = DirectX::XMMatrixRotationY(
				DirectX::XMConvertToRadians(-180));
			m_dxpos *= DirectX::XMMatrixTranslation(
				j * 2.0f - 9.0f + sabun, -0.20f, k * 2.0f - 05.0f - sabun);
			set = 4; // White_Corner
		}
		else if (j == 0 && k == maxZ - 1)
		{
			set = 4; // White_Corner
			m_dxpos = DirectX::XMMatrixRotationY(
				DirectX::XMConvertToRadians(0));
			m_dxpos *= DirectX::XMMatrixTranslation(
				j * 2.0f - 9.0f - sabun, -0.20f, k * 2.0f - 05.0f + sabun);
		}
		else if (j == max - 1 && k == maxZ - 1)
		{
			set = 1; // Green_Corner
			m_dxpos = DirectX::XMMatrixRotationY(
				DirectX::XMConvertToRadians(90));
			m_dxpos *= DirectX::XMMatrixTranslation(
				j * 2.0f - 9.0f + sabun, -0.20f, k * 2.0f - 05.0f + sabun);
		}
		else if (k == 0)
		{
			set += 2;// Side
			m_dxpos = DirectX::XMMatrixRotationY(
				DirectX::XMConvertToRadians(-90));
			m_dxpos *= DirectX::XMMatrixTranslation(
				j * 2.0f - 9.0f, -0.20f, k * 2.0f - 05.0f - sabun);
		}
		else if (j == 0)
		{
			set += 2;// Side
			m_dxpos = DirectX::XMMatrixRotationY(
				DirectX::XMConvertToRadians(-0));
			m_dxpos *= DirectX::XMMatrixTranslation(
				j * 2.0f - 9.0f - sabun, -0.20f, k * 2.0f - 05.0f);
		}
		else if (k == maxZ - 1)
		{
			set += 2;// Side
			m_dxpos = DirectX::XMMatrixRotationY(
				DirectX::XMConvertToRadians(90));
			m_dxpos *= DirectX::XMMatrixTranslation(
				j * 2.0f - 9.0f, -0.20f, k * 2.0f - 05.0f + sabun);
		}
		else if(j == max - 1)
		{
			set += 2;// Side
			m_dxpos = DirectX::XMMatrixRotationY(
				DirectX::XMConvertToRadians(180));
			m_dxpos *= DirectX::XMMatrixTranslation(
				j * 2.0f - 9.0f + sabun, -0.20f, k * 2.0f - 05.0f);
		}
		else
		{
			m_dxpos *= DirectX::XMMatrixTranslation(
				j * 2.0f - 9.0f, -0.20f, k * 2.0f - 05.0f);
		}
		//　計算用のデータから読み取り用のデータに変換
		DirectX::XMStoreFloat4x4(&wvp[0], DirectX::XMMatrixTranspose(m_dxpos));


		// モデルに変換行列を設定
		wvp[1] = m_pCamera->GetViewMatrix();
		wvp[2] = m_pCamera->GetProjectionMatrix();

		//　シェーダーへ変換行列を設定
		ShaderList::SetWVP(wvp);	//　引数にはXMFLOAT4X4型の、要素数３の配列のアドレスを渡すこと

		Geometory::SetView(m_pCamera->GetViewMatrix(true));
		Geometory::SetProjection(m_pCamera->GetProjectionMatrix(true));
		// Spriteへの設定
		Sprite::SetView(m_pCamera->GetViewMatrix(true));
		Sprite::SetProjection(m_pCamera->GetProjectionMatrix(true));


		//　モデルに使用する頂点シェーダー、ピクセルシェーダーを設定
		m_pModel[set]->SetVertexShader(ShaderList::GetVS(ShaderList::VS_WORLD));
		m_pModel[set]->SetPixelShader(ShaderList::GetPS(ShaderList::PS_LAMBERT));

		//　複数のメッシュで構成されている場合、ある部分は金属的な表現、ある部分は非金属的な表現と
		// 分ける場合がある。前回の表示は同じマテリアルで一括表示していたため、メッシュごとにマテリアルを
		// 切り替える。
		for (int i = 0; i < m_pModel[set]->GetMeshNum(); ++i)
		{
			// モデルのメッシュを取得
			Model::Mesh mesh = *m_pModel[set]->GetMesh(i);
			// メッシュに割り当てられているマテリアルを取得
			Model::Material	material = *m_pModel[set]->GetMaterial(mesh.materialID);
			// マテリアルを編集する場合、SetMaterial関数へ設定する前に変更 
			material.ambient.x = 1.0f; // xは赤(r)を示す
			material.ambient.y = 1.0f; // yは緑(g)を示す
			material.ambient.z = 1.0f; // zは青(b)を示す
			// シェーダーへマテリアルを設定
			ShaderList::SetMaterial(material);
			// モデルの描画
			m_pModel[set]->Draw(i);
		}
	}
}

void Field::SetCamera(Camera* set)
{
	m_pCamera = set;
}

Field_Model::Field_Model()
{
	CsvData& csv = CsvData::get_instance();
	m_field_s.widhei.width		= csv.GetFieldState().width;
	m_field_s.widhei.height	= csv.GetFieldState().height;
	m_field_s.wall.back		= { 0.0f, 1.0f};
	m_field_s.wall.front	= { 0.0f,-1.0f};
	m_field_s.wall.left		= {-1.0f, 0.0f};
	m_field_s.wall.right	= { 1.0f, 0.0f};

	std::string fileName[] =
	{
		"Assets/Model/Map/MD/モデル/MD_Tray_Green.fbx",
		"Assets/Model/Map/MD/モデル/MD_Tray_Green_Corner.fbx",
		"Assets/Model/Map/MD/モデル/MD_Tray_Green_Side.fbx",
		"Assets/Model/Map/MD/モデル/MD_Tray_White.fbx",
		"Assets/Model/Map/MD/モデル/MD_Tray_White_Corner.fbx",
		"Assets/Model/Map/MD/モデル/MD_Tray_White_Side.fbx"
	};
	std::string modelName[] = {
		"Green_Top",
		"Green_Corner",
		"Green_Side",
		"White_Top",
		"White_Corner",
		"White_Side"
	};
	m_pModel = new Model[6];
	for(int i = 0;i < 6;i++)
	{
		if (!m_pModel[i].Load(fileName[i].c_str(), 0.5f, Model::ZFlip)) { // 倍率と反転は省略可
			MessageBox(NULL, modelName[i].c_str(), "Error", MB_OK); // エラーメッセージの表示
		}
	}
	m_f_m_n.Green_Top = new Model();
	if (!m_f_m_n.Green_Top->Load(fileName[0].c_str(), 0.5f, Model::ZFlip)) { // 倍率と反転は省略可
		MessageBox(NULL, modelName[0].c_str(), "Error", MB_OK); // エラーメッセージの表示
	}
	m_f_m_n.Green_Corner = new Model();
	if (!m_f_m_n.Green_Corner->Load(fileName[1].c_str(), 0.5f, Model::ZFlip)) { // 倍率と反転は省略可
		MessageBox(NULL, modelName[1].c_str(), "Error", MB_OK); // エラーメッセージの表示
	}
	m_f_m_n.Green_Side = new Model();
	if (!m_f_m_n.Green_Side->Load(fileName[2].c_str(), 0.5f, Model::ZFlip)) { // 倍率と反転は省略可
		MessageBox(NULL, modelName[2].c_str(), "Error", MB_OK); // エラーメッセージの表示
	}
	m_f_m_n.White_Top = new Model();
	if (!m_f_m_n.White_Top->Load(fileName[3].c_str(), 0.5f, Model::ZFlip)) { // 倍率と反転は省略可
		MessageBox(NULL, modelName[3].c_str(), "Error", MB_OK); // エラーメッセージの表示
	}
	m_f_m_n.White_Corner = new Model();
	if (!m_f_m_n.White_Corner->Load(fileName[4].c_str(), 0.5f, Model::ZFlip)) { // 倍率と反転は省略可
		MessageBox(NULL, modelName[4].c_str(), "Error", MB_OK); // エラーメッセージの表示
	}
	m_f_m_n.White_Side = new Model();
	if (!m_f_m_n.White_Side->Load(fileName[5].c_str(), 0.5f, Model::ZFlip)) { // 倍率と反転は省略可
		MessageBox(NULL, modelName[5].c_str(), "Error", MB_OK); // エラーメッセージの表示
	}
}

Field_Model::~Field_Model()
{
	delete[] m_pModel;
	m_pModel = nullptr;
}

Field_Model &Field_Model::get_instance()
{
	static Field_Model instance;
	return instance;
}

Field_Model::Field_State Field_Model::GetFieldState()
{
	return m_field_s;
}

Model* Field_Model::GetFieldModel()
{
	return m_pModel;
}

Model* Field_Model::GetFieldModel(Field_Model_No get)
{
	switch (get)
	{
	case Field_Model::GT:
		return m_f_m_n.Green_Top;
	case Field_Model::GC:
		return m_f_m_n.Green_Corner;
	case Field_Model::GS:
		return m_f_m_n.Green_Side;
	case Field_Model::WT:
		return m_f_m_n.White_Top;
	case Field_Model::WC:
		return m_f_m_n.White_Corner;
	case Field_Model::WS:
		return m_f_m_n.White_Side;
	}
	return GetFieldModel();
}

Model* Field_Model::GetFieldModel(int get)
{
	switch (get)
	{
	case Field_Model::GT:
		return m_f_m_n.Green_Top;
	case Field_Model::GC:
		return m_f_m_n.Green_Corner;
	case Field_Model::GS:
		return m_f_m_n.Green_Side;
	case Field_Model::WT:
		return m_f_m_n.White_Top;
	case Field_Model::WC:
		return m_f_m_n.White_Corner;
	case Field_Model::WS:
		return m_f_m_n.White_Side;
	}
	return nullptr;
}
