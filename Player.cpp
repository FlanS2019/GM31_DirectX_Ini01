#include "main.h"
#include "player.h"
#include "renderer.h"
#include "modelRenderer.h"
#include "Input.h"

void Player::Init()
{
	m_Position = { -5, 0, 0 };

	ModelRenderer* modelRenderer = AddComponent<ModelRenderer>();
	modelRenderer->Load("model\\player.obj");

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\unlitTextureVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "shader\\unlitTexturePS.cso");

}
void Player::Uninit()
{
	if (m_VertexLayout) { m_VertexLayout->Release(); m_VertexLayout = nullptr; }
	if (m_VertexShader) { m_VertexShader->Release(); m_VertexShader = nullptr; }
	if (m_PixelShader) { m_PixelShader->Release(); m_PixelShader = nullptr; }
}
void Player::Update()
{
	// フレーム固定の簡易 dt（実際は実時間差を使うのが望ましい）
	float dt = 1.0f / 60.0f;

	// パラメータ
	const float accel = 10.0f;    // 加速度 (units/s^2)
	const float maxSpeed = 5.0f;  // 最大速度 (units/s)
	const float friction = 15.0f; // 減速 (units/s^2)
	const float gravity = 119.8f;   // 重力 (units/s^2)
	const float jumpImpulse = 25.0f; // ジャンプ初速

	// --- X 軸（左右） ---
	if (Input::GetKeyPress('D')) // 右
	{
		m_Velocity.x += accel * dt;
		if (m_Velocity.x > maxSpeed) m_Velocity.x = maxSpeed;
	}
	else if (Input::GetKeyPress('A')) // 左
	{
		m_Velocity.x -= accel * dt;
		if (m_Velocity.x < -maxSpeed) m_Velocity.x = -maxSpeed;
	}
	else
	{
		// 摩擦で速度を 0 に近づける
		if (m_Velocity.x > 0.0f)
		{
			m_Velocity.x -= friction * dt;
			if (m_Velocity.x < 0.0f) m_Velocity.x = 0.0f;
		}
		else if (m_Velocity.x < 0.0f)
		{
			m_Velocity.x += friction * dt;
			if (m_Velocity.x > 0.0f) m_Velocity.x = 0.0f;
		}
	}

	// --- Z 軸（前後）: X 軸と同様の加速・摩擦を適用 ---
	if (Input::GetKeyPress('W')) // 前 (負方向)
	{
		m_Velocity.z -= accel * dt;
		if (m_Velocity.z < -maxSpeed) m_Velocity.z = -maxSpeed;
	}
	else if (Input::GetKeyPress('S')) // 後 (正方向)
	{
		m_Velocity.z += accel * dt;
		if (m_Velocity.z > maxSpeed) m_Velocity.z = maxSpeed;
	}
	else
	{
		// 摩擦で速度を 0 に近づける
		if (m_Velocity.z > 0.0f)
		{
			m_Velocity.z -= friction * dt;
			if (m_Velocity.z < 0.0f) m_Velocity.z = 0.0f;
		}
		else if (m_Velocity.z < 0.0f)
		{
			m_Velocity.z += friction * dt;
			if (m_Velocity.z > 0.0f) m_Velocity.z = 0.0f;
		}
	}

	// 地面判定（小さな許容誤差を使用）
	const float groundEpsilon = 0.001f;
	bool grounded = (m_Position.y <= groundEpsilon);

	// 地面上でのジャンプトリガーはここで判定（GetKeyTrigger は Input::Update の後に呼ばれる必要あり）
	if (grounded)
	{
		// 地面にぴったりなら補正しておく
		m_Position.y = 0.0f;
		// ジャンプ可能状態でスペースがトリガーされたら上向きの速度を与える
		if (Input::GetKeyTrigger(VK_SPACE))
		{
			m_Velocity.y = jumpImpulse;
		}
	}

	// 重力を適用
	m_Velocity.y -= gravity * dt;

	// 速度を位置に反映（加速が見えるようにする重要なステップ）
	m_Position.x += m_Velocity.x * dt;
	m_Position.y += m_Velocity.y * dt;
	m_Position.z += m_Velocity.z * dt;

	// 地面衝突判定（位置反映後に行う）
	if (m_Position.y < 0.0f)
	{
		m_Position.y = 0.0f;
		if (m_Velocity.y < 0.0f) m_Velocity.y = 0.0f;
	}
}
void Player::Draw()
{
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	XMMATRIX world, scale, rot, trans;
	scale = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);//拡大率
	rot = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);//
	trans = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);//平行移動量
	world = scale * rot * trans;

	Renderer::SetWorldMatrix(world);

	GameObject::Draw();
}