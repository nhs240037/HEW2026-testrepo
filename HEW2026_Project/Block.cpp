#include "Block.h"
#include"Geometory.h"
#include"ShaderList.h"
#include"Sprite.h"

Block::Block()
	: m_move{}
	, m_playerPos{}
	, m_pCamera(nullptr)
	, csv(CsvData::get_instance())
	, m_state(Block_Drop)
	, m_fStepSizeY(0.2f)
	, m_nStep(0)
	, m_CollisionSize{}
	, m_bColor(Block_Color::None)
	, m_pModel(nullptr)
	, m_dxpos{}
	, wvp{}
{
	m_pos.x = csv.GetBlockState().blo.pos.x;
	m_pos.y = csv.GetBlockState().height;
	m_pos.z = csv.GetBlockState().blo.pos.y;

	int count=0;
	fileName[count] = "Assets/Model/Prototype/MD_Buns_Bottom.fbx";	count++;
	fileName[count] = "Assets/Model/Prototype/MD_Buns_Top.fbx";		count++;
	fileName[count] = "Assets/Model/Prototype/MD_Patty.fbx";		count++;
	fileName[count] = "Assets/Model/Prototype/MD_Lettuce.fbx";		count++;
	fileName[count] = "Assets/Model/Prototype/MD_Egg.fbx";			count++;
	fileName[count] = "Assets/Model/Prototype/MD_Bacon.fbx";		count++;
	fileName[count] = "Assets/Model/Prototype/MD_Cheese.fbx";		count++;
	fileName[count] = "Assets/Model/Prototype/MD_Tomato.fbx";		count++;
	m_pModel = new Model();
	switch (m_bColor)
	{
	case Block::Buns_up:
		if (!m_pModel->Load(fileName[1].c_str(), 0.5f, Model::ZFlip)) { // 倍率と反転は省略可
			MessageBox(NULL, "Top", "Error", MB_OK); // エラーメッセージの表示
		}
		break;
	case Block::Buns_Button:
		if (!m_pModel->Load(fileName[0].c_str(), 0.5f, Model::ZFlip)) { // 倍率と反転は省略可
			MessageBox(NULL, "Buttom", "Error", MB_OK); // エラーメッセージの表示
		}
		break;
	case Block::Bacon:
		if (!m_pModel->Load(fileName[5].c_str(), 0.5f, Model::ZFlip)) { // 倍率と反転は省略可
			MessageBox(NULL, "Bacon", "Error", MB_OK); // エラーメッセージの表示
		}
		break;
	case Block::Cheese:
		if (!m_pModel->Load(fileName[6].c_str(), 0.5f, Model::ZFlip)) { // 倍率と反転は省略可
			MessageBox(NULL, "Cheese", "Error", MB_OK); // エラーメッセージの表示
		}
		break;
	case Block::Fried_egg:
		if (!m_pModel->Load(fileName[4].c_str(), 0.5f, Model::ZFlip)) { // 倍率と反転は省略可
			MessageBox(NULL, "Egg", "Error", MB_OK); // エラーメッセージの表示
		}
		break;
	case Block::Patty:
		if (!m_pModel->Load(fileName[2].c_str(), 0.5f, Model::ZFlip)) { // 倍率と反転は省略可
			MessageBox(NULL, "Patty", "Error", MB_OK); // エラーメッセージの表示
		}
		break;
	case Block::Lettuce:
		if (!m_pModel->Load(fileName[3].c_str(), 0.5f, Model::ZFlip)) { // 倍率と反転は省略可
			MessageBox(NULL, "Lettuce", "Error", MB_OK); // エラーメッセージの表示
		}
		break;
	case Block::Tomato:
		if (!m_pModel->Load(fileName[7].c_str(), 0.5f, Model::ZFlip)) { // 倍率と反転は省略可
			MessageBox(NULL, "Tomato", "Error", MB_OK); // エラーメッセージの表示
		}
		break;
	case Block::None:
		m_pModel = nullptr;
		break;
	default:
		break;
	}
	DirectX::XMMATRIX view, proj;
	m_dxpos = DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);
	m_dxpos *= DirectX::XMMatrixScaling(
		csv.GetPlayerState().size.x,
		csv.GetBlockState().height,
		csv.GetPlayerState().size.y);

	view = DirectX::XMMatrixLookAtLH(
		DirectX::XMVectorSet(5.0f, 5.0f, 0.0f, 0.0f),
		DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
		DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
	);
	proj = DirectX::XMMatrixPerspectiveFovLH
	(
		(1.0f / 6.0f) * 3.1415f * 2.0f,	// FovAngleY
		16.0f / 9,	// AspectRatio
		0.001f,	// NearZ
		10.0f	// FarZ
	);
	DirectX::XMStoreFloat4x4(&wvp[0], DirectX::XMMatrixTranspose(m_dxpos));
	DirectX::XMStoreFloat4x4(&wvp[1], DirectX::XMMatrixTranspose(view));
	DirectX::XMStoreFloat4x4(&wvp[2], DirectX::XMMatrixTranspose(proj));
	m_pModel = nullptr;
}

Block::~Block()
{
	if(m_pCamera)
	{
		//delete m_pCamera;
		m_pCamera = nullptr;
	}
}

void Block::Update()
{
	switch (m_state)
	{
	case Block::Block_Idle:
		break;
	case Block::Block_Drop:
		m_pos.y -= csv.GetBlockState().blo.posY;

		// 最新のプレイヤー位置で当たり判定する
		if (m_playerPos.x > m_pos.x - (csv.GetBlockState().blo.size.x / 2.0f) &&
			m_playerPos.x < m_pos.x + (csv.GetBlockState().blo.size.x / 2.0f))
		{
			if (m_playerPos.z > m_pos.z - (csv.GetBlockState().blo.size.y / 2.0f) &&
				m_playerPos.z < m_pos.z + (csv.GetBlockState().blo.size.y / 2.0f))
			{
				float playerTopY = csv.GetPlayerState().size.y + (m_nStep * m_fStepSizeY);
				if (playerTopY > m_pos.y)
				{
					m_state = BlockState::Block_Catch;
				}
			}
		}
		else if (m_pos.y <= 0.0f)
			m_state = BlockState::Block_Idle;
		break;
	case Block::Block_Catch:
	case BlockState::Block_Catched:
		m_pos = m_playerPos;
		m_pos.y = csv.GetPlayerState().size.y + (m_nStep * m_fStepSizeY);
		break;
	case Block::Block_Draw:
		break;
	default:
		break;
	}

	m_dxpos = DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);
}

void Block::Draw()
{
	DirectX::XMMATRIX T; // 天面がグリッドよりも下に来るように移動
	DirectX::XMMATRIX S; // 地面となるように、前後左右に広く、上下に狭くする
	DirectX::XMMATRIX mat;
	DirectX::XMFLOAT4X4 fMat;

	T = DirectX::XMMatrixTranslation(m_pos.x,m_pos.y,m_pos.z);
	S = DirectX::XMMatrixScaling(csv.GetBlockState().blo.size.x, m_fStepSizeY, csv.GetBlockState().blo.size.y);
	mat = S * T;
	mat = DirectX::XMMatrixTranspose(mat);

	DirectX::XMStoreFloat4x4(&fMat,mat);

	Geometory::SetWorld(fMat); // ボックスに変換行列を設定
	if(m_pModel == nullptr)
		Geometory::DrawCylinder();// 影の大きさを計算
	if(m_pModel != nullptr)
	{
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
		m_pModel->SetVertexShader(ShaderList::GetVS(ShaderList::VS_WORLD));
		m_pModel->SetPixelShader(ShaderList::GetPS(ShaderList::PS_LAMBERT));

		//　複数のメッシュで構成されている場合、ある部分は金属的な表現、ある部分は非金属的な表現と
		// 分ける場合がある。前回の表示は同じマテリアルで一括表示していたため、メッシュごとにマテリアルを
		// 切り替える。
		for (int i = 0; i < m_pModel->GetMeshNum(); ++i)
		{
			// モデルのメッシュを取得
			Model::Mesh mesh = *m_pModel->GetMesh(i);
			// メッシュに割り当てられているマテリアルを取得
			Model::Material	material = *m_pModel->GetMaterial(mesh.materialID);
			// シェーダーへマテリアルを設定
			ShaderList::SetMaterial(material);
			// モデルの描画
			m_pModel->Draw(i);
		}
	}
}

void Block::OnCollision(Collision::Result collision)
{
	// 計算に必要な情報を事前に計算
	DirectX::XMVECTOR vHitPos = DirectX::XMLoadFloat3(&collision.point);
	DirectX::XMVECTOR vNormal = DirectX::XMLoadFloat3(&collision.normal);
	DirectX::XMVECTOR vMove = DirectX::XMLoadFloat3(&m_move);
	vNormal = DirectX::XMVector3Normalize(vNormal);

	// 反射の計算
	DirectX::XMVECTOR vDot = DirectX::XMVector3Dot(vNormal, vMove);
	vDot = DirectX::XMVectorScale(vDot, 2.0f);
	vDot = DirectX::XMVectorMultiply(vNormal, DirectX::XMVectorAbs(vDot));
	vMove = DirectX::XMVectorAdd(vMove, vDot);
	DirectX::XMStoreFloat3(&m_move, vMove);

	// 反射後の補正
	if (collision.other.type == Collision::eBox) {
		// ボックスに衝突した場合、衝突位置の補正
		Collision::Box other = collision.other.box;
		if (collision.normal.x != 0.0f)
			m_pos.x =
			other.center.x + collision.normal.x * (other.size.x + collision.other.box.size.x) * 0.5f;
		else if (collision.normal.y != 0.0f)
			m_pos.y =
			other.center.y + collision.normal.y * (other.size.y + collision.other.box.size.y) * 0.5f;
		else
			m_pos.z =
			other.center.z + collision.normal.z * (other.size.z + collision.other.box.size.z) * 0.5f;
		// 反射後の移動速度の補正
		m_move.x *= 0.8f;
		m_move.y *= 0.6f;
		m_move.z *= 0.8f;
	}
	else {
		// 斜面に衝突した場合の位置の補正
		m_pos.x = collision.point.x + collision.normal.x * collision.other.box.size.x * 0.5f;
		m_pos.y = collision.point.y + collision.normal.y * collision.other.box.size.y * 0.5f;
		m_pos.z = collision.point.z + collision.normal.z * collision.other.box.size.z * 0.5f;
		// 反射後の移動速度の補正
		m_move.x *= 0.2f;
		m_move.y *= 0.5f;
		m_move.z *= 0.2f;
	}
}

void Block::GetCamera(Camera *camera)
{
	m_pCamera = camera;
}

void Block::SetCollision(f2col size)
{
	m_CollisionSize = size;
}

void Block::SetPlayerPos(DirectX::XMFLOAT3 set)
{
	m_playerPos = set;
}

Block::BlockState Block::GetState()
{
	return m_state;
}

void Block::SetState(BlockState set)
{
	m_state = set;
}

void Block::SetStep(int set)
{
	m_nStep = set;
}

int Block::GetStep()
{
	return m_nStep;
}

Block::Block(Block_Color set)
	: m_move{}
	, m_playerPos{}
	, m_pCamera(nullptr)
	, csv(CsvData::get_instance())
	, m_state(Block_Drop)
	, m_fStepSizeY(0.2f)
	, m_nStep(0)
	, m_CollisionSize{}
	, m_bColor(set)
	, m_pModel(nullptr)
	, m_dxpos{}
	, wvp{}
{
	m_pos.x = csv.GetBlockState().blo.pos.x;
	m_pos.y = csv.GetBlockState().height;
	m_pos.z = csv.GetBlockState().blo.pos.y;

	int count = 0;
	fileName[count] = "Assets/Model/Prototype/MD_Buns_Bottom.fbx";	count++;
	fileName[count] = "Assets/Model/Prototype/MD_Buns_Top.fbx";		count++;
	fileName[count] = "Assets/Model/Prototype/MD_Patty.fbx";		count++;
	fileName[count] = "Assets/Model/Prototype/MD_Lettuce.fbx";		count++;
	fileName[count] = "Assets/Model/Prototype/MD_Egg.fbx";			count++;
	fileName[count] = "Assets/Model/Prototype/MD_Bacon.fbx";		count++;
	fileName[count] = "Assets/Model/Prototype/MD_Cheese.fbx";		count++;
	fileName[count] = "Assets/Model/Prototype/MD_Tomato.fbx";		count++;
	// まだないのでPattyでも出そうかな
	//fileName[count] = "Assets/Model/Prototype/MD_Bacon.fbx";		count++;

	m_pModel = new Model();
	switch (m_bColor)
	{
	case Block::Buns_up:
		if (!m_pModel->Load(fileName[1].c_str(), 0.5f, Model::ZFlip)) { // 倍率と反転は省略可
			MessageBox(NULL, "Top", "Error", MB_OK); // エラーメッセージの表示
		}
		break;
	case Block::Buns_Button:
		if (!m_pModel->Load(fileName[0].c_str(), 0.5f, Model::ZFlip)) { // 倍率と反転は省略可
			MessageBox(NULL, "Buttom", "Error", MB_OK); // エラーメッセージの表示
		}
		break;
	case Block::Bacon:
		if (!m_pModel->Load(fileName[5].c_str(), 0.5f, Model::ZFlip)) { // 倍率と反転は省略可
			MessageBox(NULL, "Bacon", "Error", MB_OK); // エラーメッセージの表示
		}
		break;
	case Block::Cheese:
		if (!m_pModel->Load(fileName[6].c_str(), 0.5f, Model::ZFlip)) { // 倍率と反転は省略可
			MessageBox(NULL, "Cheese", "Error", MB_OK); // エラーメッセージの表示
		}
		break;
	case Block::Fried_egg:
		if (!m_pModel->Load(fileName[4].c_str(), 0.5f, Model::ZFlip)) { // 倍率と反転は省略可
			MessageBox(NULL, "Egg", "Error", MB_OK); // エラーメッセージの表示
		}
		break;
	case Block::Patty:
		if (!m_pModel->Load(fileName[2].c_str(), 0.5f, Model::ZFlip)) { // 倍率と反転は省略可
			MessageBox(NULL, "Patty", "Error", MB_OK); // エラーメッセージの表示
		}
		break;
	case Block::Lettuce:
		if (!m_pModel->Load(fileName[3].c_str(), 0.5f, Model::ZFlip)) { // 倍率と反転は省略可
			MessageBox(NULL, "Lettuce", "Error", MB_OK); // エラーメッセージの表示
		}
		break;
	case Block::Tomato:
		if (!m_pModel->Load(fileName[7].c_str(), 0.5f, Model::ZFlip)) { // 倍率と反転は省略可
			MessageBox(NULL, "Tomato","Error", MB_OK); // エラーメッセージの表示
		}
		break;
	case Block::None:
		m_pModel = nullptr;
		break;
	default:
		break;
	}
	DirectX::XMMATRIX view, proj;
	m_dxpos = DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);
	m_dxpos *= DirectX::XMMatrixScaling(
		csv.GetPlayerState().size.x,
		csv.GetBlockState().height,
		csv.GetPlayerState().size.y);

	view = DirectX::XMMatrixLookAtLH(
		DirectX::XMVectorSet(5.0f, 5.0f, 0.0f, 0.0f),
		DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
		DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
	);
	proj = DirectX::XMMatrixPerspectiveFovLH
	(
		(1.0f / 6.0f) * 3.1415f * 2.0f,	// FovAngleY
		16.0f / 9,	// AspectRatio
		0.001f,	// NearZ
		10.0f	// FarZ
	);
	DirectX::XMStoreFloat4x4(&wvp[0], DirectX::XMMatrixTranspose(m_dxpos));
	DirectX::XMStoreFloat4x4(&wvp[1], DirectX::XMMatrixTranspose(view));
	DirectX::XMStoreFloat4x4(&wvp[2], DirectX::XMMatrixTranspose(proj));

	// エラーが出るのでnullptrに仮置き
	m_pModel = nullptr;
	//
	// m_pBlock[i]にカメラが設定されていないためnullptrエラーが発生。
	// かなり面倒になってしまうが、3Dオブジェクトの共通クラス化をしたほうがいいかも。。。(気が向いたらやります) 
	// by 若林
}

Block::Block(Block_Color set, float setX, float setY)
	: m_move{}
	, m_playerPos{}
	, m_pCamera(nullptr)
	, csv(CsvData::get_instance())
	, m_state(Block_Drop)
	, m_fStepSizeY(0.2f)
	, m_nStep(0)
	, m_CollisionSize{}
	, m_bColor(set)
	, m_pModel(nullptr)
	, m_dxpos{}
	, wvp{}
{
	m_pos.x = csv.GetBlockState().blo.pos.x;
	m_pos.y = csv.GetBlockState().height;
	m_pos.z = csv.GetBlockState().blo.pos.y;

	int count = 0;
	fileName[count] = "Assets/Model/Prototype/MD_Buns_Bottom.fbx";		count++;
	fileName[count] = "Assets/Model/Prototype/MD_Buns_Top.fbx";				count++;
	fileName[count] = "Assets/Model/Prototype/MD_Patty.fbx";					count++;
	fileName[count] = "Assets/Model/Prototype/MD_Lettuce.fbx";				count++;
	fileName[count] = "Assets/Model/Prototype/MD_Egg.fbx";						count++;
	fileName[count] = "Assets/Model/Prototype/MD_Bacon.fbx";					count++;
	fileName[count] = "Assets/Model/Prototype/MD_Cheese.fbx";					count++;
	fileName[count] = "Assets/Model/Prototype/MD_Tomato.fbx";					count++;
	m_pModel = new Model();
	switch (m_bColor)
	{
	case Block::Buns_up:
		if (!m_pModel->Load(fileName[1].c_str(), 0.5f, Model::ZFlip)) { // 倍率と反転は省略可
			MessageBox(NULL, "Branch_01", "Error", MB_OK); // エラーメッセージの表示
		}
		break;
	case Block::Buns_Button:
		if (!m_pModel->Load(fileName[0].c_str(), 0.5f, Model::ZFlip)) { // 倍率と反転は省略可
			MessageBox(NULL, "Branch_01", "Error", MB_OK); // エラーメッセージの表示
		}
		break;
	case Block::Bacon:
		if (!m_pModel->Load(fileName[5].c_str(), 0.5f, Model::ZFlip)) { // 倍率と反転は省略可
			MessageBox(NULL, "Branch_01", "Error", MB_OK); // エラーメッセージの表示
		}
		break;
	case Block::Cheese:
		if (!m_pModel->Load(fileName[6].c_str(), 0.5f, Model::ZFlip)) { // 倍率と反転は省略可
			MessageBox(NULL, "Branch_01", "Error", MB_OK); // エラーメッセージの表示
		}
		break;
	case Block::Fried_egg:
		if (!m_pModel->Load(fileName[4].c_str(), 0.5f, Model::ZFlip)) { // 倍率と反転は省略可
			MessageBox(NULL, "Branch_01", "Error", MB_OK); // エラーメッセージの表示
		}
		break;
	case Block::Patty:
		if (!m_pModel->Load(fileName[2].c_str(), 0.5f, Model::ZFlip)) { // 倍率と反転は省略可
			MessageBox(NULL, "Branch_01", "Error", MB_OK); // エラーメッセージの表示
		}
		break;
	case Block::Lettuce:
		if (!m_pModel->Load(fileName[3].c_str(), 0.5f, Model::ZFlip)) { // 倍率と反転は省略可
			MessageBox(NULL, "Branch_01", "Error", MB_OK); // エラーメッセージの表示
		}
		break;
	case Block::Tomato:
		if (!m_pModel->Load(fileName[7].c_str(), 0.5f, Model::ZFlip)) { // 倍率と反転は省略可
			MessageBox(NULL, "Branch_01", "Error", MB_OK); // エラーメッセージの表示
		}
		break;
	case Block::None:
		m_pModel = nullptr;
		break;
	default:
		break;
	}
	DirectX::XMMATRIX view, proj;
	m_dxpos = DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);
	m_dxpos *= DirectX::XMMatrixScaling(
		csv.GetPlayerState().size.x,
		csv.GetBlockState().height,
		csv.GetPlayerState().size.y);

	view = DirectX::XMMatrixLookAtLH(
		DirectX::XMVectorSet(5.0f, 5.0f, 0.0f, 0.0f),
		DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
		DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
	);
	proj = DirectX::XMMatrixPerspectiveFovLH
	(
		(1.0f / 6.0f) * 3.1415f * 2.0f,	// FovAngleY
		16.0f / 9,	// AspectRatio
		0.001f,	// NearZ
		10.0f	// FarZ
	);
	DirectX::XMStoreFloat4x4(&wvp[0], DirectX::XMMatrixTranspose(m_dxpos));
	DirectX::XMStoreFloat4x4(&wvp[1], DirectX::XMMatrixTranspose(view));
	DirectX::XMStoreFloat4x4(&wvp[2], DirectX::XMMatrixTranspose(proj));
}
