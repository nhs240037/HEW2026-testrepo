#include "SceneGame.h"
#include "Geometory.h"
#include "ShaderList.h"
#include "CameraDebug.h"


SceneGame::SceneGame()
{
	m_pModel = new Model();
	if (!m_pModel->Load("Assets/Model/AkamiKarubi-Maken_MMD/AkamiKarubi.pmx", 0.07f, Model::ZFlip))
	{
		MessageBox(NULL, "赤見かるび", "Error", MB_OK);
	}

	//===== レンダーターゲット、深度バッファの設定 =====//
	RenderTarget* pRTV = GetDefaultRTV();
	DepthStencil* pDSV = GetDefaultDSV();
	SetRenderTargets(1, &pRTV, pDSV);
	SetDepthTest(true);

	m_pCamera = new CameraDebug();
	m_pPlayer = new Player();
	m_pPlayer->SetCamera(m_pCamera);
	
	m_pNext = new NextItem();
}

SceneGame::~SceneGame()
{
	if (m_pModel)
	{
		delete m_pModel;
		m_pModel = nullptr;
	}
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pPlayer);
	SAFE_DELETE(m_pNext);
}

void SceneGame::Update()
{
	m_pCamera->Update();
	m_pPlayer->Update();
	m_pCamera->SetLook(m_pPlayer->GetPos());
}

void SceneGame::Draw()
{
	RenderTarget *pRTV = GetDefaultRTV();
	DepthStencil *pDSV = GetDefaultDSV();
	SetRenderTargets(1, &pRTV, pDSV);
	SetDepthTest(true);
	//===== 3Dモデル表示 =====//
		//----- 変換行列宣言 -----//
	DirectX::XMFLOAT4X4 fWVP[3];
	DirectX::XMMATRIX world, view, projection;
	DirectX::XMVECTOR eyePos, fcsPos, upDrct;
	{
		static float rad = 0.0f;
		eyePos = DirectX::XMVectorSet(5.f, 5.f, 5.f, .0f);
		fcsPos = DirectX::XMVectorSet(0.f, .5f, 0.f, .0f);
		upDrct = DirectX::XMVectorSet(.0f, 1.f, .0f, .0f);

		//----- set local coord -----//
		DirectX::XMMATRIX Rx = DirectX::XMMatrixRotationX(0.f);
		DirectX::XMMATRIX Ry = DirectX::XMMatrixRotationY(sinf(rad));
		DirectX::XMMATRIX Rz = DirectX::XMMatrixRotationZ(0.f);
		DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(0.f, 0.f, 0.f);
		DirectX::XMMATRIX S = DirectX::XMMatrixScaling(2.f, 2.f, 2.f);
		DirectX::XMMATRIX localMatrix = S * Rx * Ry * Rz * T;

		world = localMatrix;
		view = DirectX::XMMatrixLookAtLH(eyePos, fcsPos, upDrct);
		projection = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(80.f), 16 / 9.f, 0.001f, 1000.f);

		DirectX::XMStoreFloat4x4(&fWVP[0], DirectX::XMMatrixTranspose(world));
		DirectX::XMStoreFloat4x4(&fWVP[1], DirectX::XMMatrixTranspose(view));
		DirectX::XMStoreFloat4x4(&fWVP[2], DirectX::XMMatrixTranspose(projection));

		fWVP[1] = m_pCamera->GetViewMatrix();
		fWVP[2] = m_pCamera->GetProjectionMatrix();

		ShaderList::SetWVP(fWVP);

		m_pModel->SetVertexShader(ShaderList::GetVS(ShaderList::VS_WORLD));
		m_pModel->SetPixelShader(ShaderList::GetPS(ShaderList::PS_TOON));

		for (unsigned int i = 0; i < m_pModel->GetMeshNum(); ++i)
		{
			const Model::Mesh *mesh = m_pModel->GetMesh(i);
			Model::Material material = *m_pModel->GetMaterial(mesh->materialID);
			ShaderList::SetMaterial(material);
			m_pModel->Draw(i);
		}

		rad += 0.08f;

	}
	//Geometory::SetView(fWVP[1]);
	//Geometory::SetProjection(fWVP[2]);
	Geometory::SetView(m_pCamera->GetViewMatrix());
	Geometory::SetProjection(m_pCamera->GetProjectionMatrix());

	//Geometory::DrawBox();

	{
		DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(0.f, -.1f, 0.f);
		DirectX::XMMATRIX S = DirectX::XMMatrixScaling(1.f, .1f, 3.f);
		DirectX::XMMATRIX mat = S*T;
		mat = DirectX::XMMatrixTranspose(mat);
		DirectX::XMFLOAT4X4 fMat;
		DirectX::XMStoreFloat4x4(&fMat, mat);
		Geometory::SetWorld(fMat);
		Geometory::DrawBox();

		T = DirectX::XMMatrixTranslation(0.f, -.1f, 5.f);
		S = DirectX::XMMatrixScaling(1.f, .1f, 5.f);
		mat = S*T;
		mat = DirectX::XMMatrixTranspose(mat);
		DirectX::XMStoreFloat4x4(&fMat, mat);
		Geometory::SetWorld(fMat);
		Geometory::DrawBox();
	}

	{
		static float rad = 0.0f;
		DirectX::XMMATRIX Rx = DirectX::XMMatrixRotationX(0.f);
		DirectX::XMMATRIX Ry = DirectX::XMMatrixRotationY(rad);
		DirectX::XMMATRIX Rz = DirectX::XMMatrixRotationZ(0.f);
		DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(0.f, 1.f, 6.f);
		DirectX::XMMATRIX S = DirectX::XMMatrixScaling(.5f,.5f,.5f);
		DirectX::XMMATRIX mat = S * Rx * Ry * Rz * T;
		mat = DirectX::XMMatrixTranspose(mat);
		DirectX::XMFLOAT4X4 fMat;
		DirectX::XMStoreFloat4x4(&fMat, mat);
		Geometory::SetWorld(fMat);
		rad += 0.1f;

		Geometory::DrawBox();
		}

		if(m_pPlayer)
			m_pPlayer->Draw();

	//{
	//	if (m_pModel)
	//	{
	//		m_pModel->Draw();
	//	}
	
		SetRenderTargets(1, &pRTV, nullptr);
	m_pNext->Draw();
}