#include "SceneGame.h"
#include "Geometory.h"
#include "Defines.h"

SceneGame::SceneGame()
{
	m_pModel = new Model();
	if (!m_pModel->Load(ASSET("Model/AkamiKarubi-Maken_MMD/AkamiKarubi.pmx"), 0.05f, Model::ZFlip))
	{
		MessageBox(NULL, "赤見かるび", "Error", MB_OK);
	}

	//===== レンダーターゲット、深度バッファの設定 =====//
	RenderTarget *pRTV = GetDefaultRTV();
	DepthStencil *pDSV = GetDefaultDSV();
	SetRenderTargets(1, &pRTV, pDSV);
	SetDepthTest(true);
}

SceneGame::~SceneGame()
{
	if (m_pModel)
	{
		delete m_pModel;
		m_pModel = nullptr;
	}
}

void SceneGame::Update()
{
}

void SceneGame::Draw()
{
	Geometory::DrawBox();

	{
		DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(0.f, -.1f, 0.6f);
		DirectX::XMMATRIX S = DirectX::XMMatrixScaling(8.f, .1f, 3.f);
		DirectX::XMMATRIX mat = S * T;
		mat = DirectX::XMMatrixTranspose(mat);
		DirectX::XMFLOAT4X4 fMat;
		DirectX::XMStoreFloat4x4(&fMat, mat);
		Geometory::SetWorld(fMat);
		Geometory::DrawBox();

		T = DirectX::XMMatrixTranslation(0.f, -.1f, 5.f);
		S = DirectX::XMMatrixScaling(3.f, .1f, 5.f);
		mat = S * T;
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
		DirectX::XMMATRIX S = DirectX::XMMatrixScaling(.5f, .5f, .5f);
		DirectX::XMMATRIX mat = S * Rx * Ry * Rz * T;
		mat = DirectX::XMMatrixTranspose(mat);
		DirectX::XMFLOAT4X4 fMat;
		DirectX::XMStoreFloat4x4(&fMat, mat);
		Geometory::SetWorld(fMat);
		rad += 0.1f;

		Geometory::DrawBox();
	}

	{
		if (m_pModel)
		{
			m_pModel->Draw();
		}
	}
}