#include "main.h"
#include "player.h"
#include "renderer.h"
#include "modelRenderer.h"
#include "Input.h"
#include "camera.h"
#include "manager.h"
#include <cmath>
#include <algorithm>

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
	const float accel = 5.0f;    // 加速度 (units/s^2)
	const float maxSpeed = 5.0f;  // 最大速度 (units/s)
	const float friction = 150.0f; // 減速 (units/s^2)
	const float gravity = 20.0f;   // 重力 (units/s^2)
	const float jumpImpulse = 25.0f; // ジャンプ初速

	// --- Sprint (Shift) ---
	// Shiftキーでスプリントさせる: 倍率を変更することで調整可能
	const float sprintMultiplier = Input::GetKeyPress(VK_SHIFT) ? 2.0f : 1.0f;
	const float currentAccel = accel * sprintMultiplier;
	const float currentMaxSpeed = maxSpeed * sprintMultiplier;

	Vector3 forward = GetForward();
	Vector3 right = GetRight();

	// 移動入力（加算）
	bool moving = false;
	if(Input::GetKeyPress('D')) { m_Velocity += right * 50.0f * dt; moving = true; }
	if(Input::GetKeyPress('A')) { m_Velocity -= right * 50.0f * dt; moving = true; }
	if(Input::GetKeyPress('W')) { m_Velocity -= forward * 50.0f * dt; moving = true; }
	if(Input::GetKeyPress('S')) { m_Velocity += forward * 50.0f * dt; moving = true; }

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

	// 回転
	if (Input::GetKeyPress('Q')) // 左回転
	{
		m_Rotation.y -= 2.0f * dt; // 回転速度は適宜調整
	}
	else if (Input::GetKeyPress('E')) // 右回転
	{
		m_Rotation.y += 2.0f * dt;
	}

	// 重力を適用
	m_Velocity.y -= gravity * dt;

	// --- 摩擦（地面上の水平速度にのみ適用） ---
	// 水平速度ベクトル
	Vector3 horizontalVel(m_Velocity.x, 0.0f, m_Velocity.z);
	float hSpeed = std::sqrt(horizontalVel.x * horizontalVel.x + horizontalVel.z * horizontalVel.z);

	if (grounded)
	{
		if (moving)
		{
			// 移動中は最大速度でクランプ
			if (hSpeed > currentMaxSpeed && hSpeed > 0.0f)
			{
				float scale = currentMaxSpeed / hSpeed;
				m_Velocity.x *= scale;
				m_Velocity.z *= scale;
			}
		}
		else
		{
			// 移動入力がない場合は摩擦で減速する（friction は units/s^2）
			if (hSpeed > 0.0f)
			{
				float decel = friction * dt;
				if (decel >= hSpeed)
				{
					// 十分に減速するので止める
					m_Velocity.x = 0.0f;
					m_Velocity.z = 0.0f;
				}
				else
				{
					// 方向を保ったまま速度を減らす
					float inv = 1.0f / hSpeed;
					m_Velocity.x -= m_Velocity.x * inv * decel;
					m_Velocity.z -= m_Velocity.z * inv * decel;
				}
			}
		}
	}

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
	rot = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);//回転量
	trans = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);//平行移動量
	world = scale * rot * trans;

	Renderer::SetWorldMatrix(world);

	GameObject::Draw();
}