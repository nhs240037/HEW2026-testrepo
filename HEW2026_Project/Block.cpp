#include "Block.h"
#include"Geometory.h"

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
{
	m_pos.x = csv.GetBlockState().blo.pos.x;
	m_pos.y = csv.GetBlockState().height;
	m_pos.z = csv.GetBlockState().blo.pos.y;

	int count=0;
	fileName[count] = "Assets/Model/Prototype/MD_Buns_Bottom.fbx";	count++;
	fileName[count] = "Assets/Model/Prototype/MD_Buns_Top.fbx";		count++;
	fileName[count] = "Assets/Model/Prototype/MD_Cheese.fbx";		count++;
	fileName[count] = "Assets/Model/Prototype/MD_Egg.fbx";			count++;
	fileName[count] = "Assets/Model/Prototype/MD_Patty.fbx";		count++;
	fileName[count] = "Assets/Model/Prototype/MD_Tomato.fbx";		count++;
	fileName[count] = "Assets/Model/Prototype/MD_Lettuce.fbx";		count++;
}

Block::~Block()
{
	delete m_pCamera;
	m_pCamera = nullptr;
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
	Geometory::DrawCylinder();// 影の大きさを計算
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
{
	m_pos.x = csv.GetBlockState().blo.pos.x;
	m_pos.y = csv.GetBlockState().height;
	m_pos.z = csv.GetBlockState().blo.pos.y;

	int count = 0;
	fileName[count] = "Assets/Model/Prototype/MD_Buns_Bottom.fbx";	count++;
	fileName[count] = "Assets/Model/Prototype/MD_Buns_Top.fbx";		count++;
	fileName[count] = "Assets/Model/Prototype/MD_Cheese.fbx";		count++;
	fileName[count] = "Assets/Model/Prototype/MD_Egg.fbx";			count++;
	fileName[count] = "Assets/Model/Prototype/MD_Patty.fbx";		count++;
	fileName[count] = "Assets/Model/Prototype/MD_Tomato.fbx";		count++;
	fileName[count] = "Assets/Model/Prototype/MD_Lettuce.fbx";		count++;
	switch (m_bColor)
	{
	case Block::None:
		break;
	case Block::Bacon:
		break;
	case Block::Buns_up:
		break;
	case Block::Buns_Button:
		break;
	case Block::Cheese:
		break;
	case Block::Fried_egg:
		break;
	case Block::Patty:
		break;
	case Block::Tomato:
		break;
	default:
		break;
	}
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
{
	m_pos.x = csv.GetBlockState().blo.pos.x;
	m_pos.y = csv.GetBlockState().height;
	m_pos.z = csv.GetBlockState().blo.pos.y;

	int count = 0;
	fileName[count] = "Assets/Model/Prototype/MD_Buns_Bottom.fbx";	count++;
	fileName[count] = "Assets/Model/Prototype/MD_Buns_Top.fbx";		count++;
	fileName[count] = "Assets/Model/Prototype/MD_Cheese.fbx";		count++;
	fileName[count] = "Assets/Model/Prototype/MD_Egg.fbx";			count++;
	fileName[count] = "Assets/Model/Prototype/MD_Patty.fbx";		count++;
	fileName[count] = "Assets/Model/Prototype/MD_Tomato.fbx";		count++;
	fileName[count] = "Assets/Model/Prototype/MD_Lettuce.fbx";		count++;
}
