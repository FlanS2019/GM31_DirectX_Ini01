#include "main.h"
#include "player.h"
#include "renderer.h"
#include "modelRenderer.h"
#include "Input.h"
#include "camera.h"
#include "manager.h"
#include "bullet.h"
#include <cmath>
#include <algorithm>
#include "tree.h"
#include "box.h"
#include "audio.h"

void Player::Init()
{
	m_Layer = 2;

	m_Position = { 0, 0, 0 };// 初期位置を設定

	ModelRenderer* modelRenderer = AddComponent<ModelRenderer>();
	modelRenderer->Load("model\\player.obj");

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\unlitTextureVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "shader\\unlitTexturePS.cso");

	m_JumpSE = AddComponent<Audio>();
	m_JumpSE->Load("audio\\wan.mp3");
}
void Player::Uninit()
{
	if (m_VertexLayout) { m_VertexLayout->Release(); m_VertexLayout = nullptr; }
	if (m_VertexShader) { m_VertexShader->Release(); m_VertexShader = nullptr; }
	if (m_PixelShader) { m_PixelShader->Release(); m_PixelShader = nullptr; }
	if (m_JumpSE) { m_JumpSE->Uninit(); }
}
void Player::Update()
{
	// フレーム固定の簡易 dt（実際は実時間差を使うのが望ましい）
	float dt = 1.0f / 60.0f;
	// パラメータ
	const float accel = 1.0f;    // 加速度 (units/s^2)
	const float maxSpeed = 5.0f;  // 最大速度 (units/s)
	const float friction = 15.0f; // 減速 (units/s^2)
	const float gravity = 60.0f;   // 重力 (units/s^2)
	const float jumpImpulse = 25.0f; // ジャンプ初速aaa (units/s)

	// --- Sprint (Shift) ---
	// Shiftキーでスプリントさせる: 倍率を変更することで調整可能
	const float sprintMultiplier = Input::GetKeyPress(VK_SHIFT) ? 2.0f : 1.0f;
	const float currentAccel = accel * sprintMultiplier;
	const float currentMaxSpeed = maxSpeed * sprintMultiplier;

	Vector3 forward = GetForward();
	Vector3 right = GetRight();

	// 移動入力（ワールド基準で速度を与える）
	bool moving = false;
	float inputX = 0.0f;
	float inputZ = 0.0f;

	if (Input::GetKeyPress('D')) { inputX += 0.5f; moving = true; } // 右
	if (Input::GetKeyPress('A')) { inputX -= 0.5f; moving = true; } // 左
	if (Input::GetKeyPress('W')) { inputZ += 0.5f; moving = true; } // 前
	if (Input::GetKeyPress('S')) { inputZ -= 0.5f; moving = true; } // 後ろ
	m_Rotation.y = atan2f(m_Velocity.x, m_Velocity.z); // 前方ベクトルから Yaw を計算

	// 地面判定（小さな許容誤差を使用）
	const float groundEpsilon = 0.001f;
	bool grounded = (m_Position.y <= groundEpsilon);
	bool oldGround = m_Grounded;
	m_Grounded = false;

	// 地面上でのジャンプトリガーはここで判定（GetKeyTrigger は Input::Update の後に呼ばれる必要あり）
	if (grounded)
	{
		// 地面にぴったりなら補正しておく
		m_Position.y = 0.0f;
		// ジャンプ可能状態でスペースがトリガーされたら上向きの速度を与える
		if (Input::GetKeyTrigger(VK_SPACE))
		{
			m_Velocity.y = jumpImpulse;

			m_Scale.x = 1.0f;
			m_Scale.y = 1.5f; // ジャンプしたときに一瞬伸びる
			m_Scale.z = 1.0f;
		}
		else
		{
			m_Scale.x = 1.0f;
			m_Scale.y = 1.0f;
			m_Scale.z = 1.0f;
			m_JumpSE->Play(); // ← ジャンプ音再生

		}

		if(!oldGround && m_Grounded)
		{
			m_Scale.x = 1.0f;
			m_Scale.y = 0.5f; // ジャンプから着地したときに一瞬潰れる
			m_Scale.z = 1.0f;
		}

	}

	//// 回転
	//if (Input::GetKeyPress('Q')) // 左回転
	//{
	//	m_Rotation.y -= 2.0f * dt; // 回転速度は適宜調整
	//}
	//else if (Input::GetKeyPress('E')) // 右回転
	//{
	//	m_Rotation.y += 2.0f * dt;
	//}

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
			// 入力方向に速度を与える（ワールド座標基準）
			m_Velocity.x += inputX * 15.0f * dt;
			m_Velocity.z += inputZ * 15.0f * dt;

			float currentSpeed = std::sqrt(m_Velocity.x * m_Velocity.x + m_Velocity.z * m_Velocity.z);
			if (currentSpeed > maxSpeed * sprintMultiplier)
			{
				float inv = maxSpeed / currentSpeed;
				m_Velocity.x *= inv;
				m_Velocity.z *= inv;
			}
			// 入力した方向にプレイヤーを向ける（速度ではなく入力から計算）
			m_Rotation.y = atan2f(inputX, inputZ);
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
		m_Grounded = true;
	}
	//木の当たり判定
	auto trees = Manager::GetGameObjects<Tree>();
	for (auto tree : trees)
	{
		Vector3 direction = tree->GetPosition() - m_Position;// プレイヤーと木の位置の差を計算
		float length = direction.length();// プレイヤーと木の距離を計算
		if (length < 1.0f) // 当たり判定の半径（例: 1.0f）
		{
			// プレイヤーを木から押し出す
			Vector3 pushDir = direction * (1.0f - length); // 押し出す距離を計算
			m_Position -= pushDir; // プレイヤーを押し出す
		}
	}
	//ボックスとの衝突判定
	auto boxes = Manager::GetGameObjects<Box>();
	for (auto box : boxes)
	{
		Vector3 boxPosition = box->GetPosition();
		Vector3 boxScale = box->GetScale();
		if(boxPosition.x - boxScale.x < m_Position.x && m_Position.x < boxPosition.x + boxScale.x &&
		   boxPosition.y - boxScale.y < m_Position.y && m_Position.y < boxPosition.y + boxScale.y &&
		   boxPosition.z - boxScale.z < m_Position.z && m_Position.z < boxPosition.z + boxScale.z)
		{
			if(boxPosition.y + boxScale.y - m_Position.y < m_Position.y - (boxPosition.y - boxScale.y))
			{
				// 上から衝突
				m_Position.y = boxPosition.y + boxScale.y;
				if (m_Velocity.y < 0.0f) m_Velocity.y = 0.0f;
				m_Grounded = true; // ジャンプしたので地面から離れる
			}
			else
			{
				// 横から衝突
				if(m_Position.x < boxPosition.x)
				{
					m_Position.x = boxPosition.x - boxScale.x;
				}
				else
				{
					m_Position.x = boxPosition.x + boxScale.x;
				}
			}
		}
	}
	if (Input::GetKeyTrigger('F'))//弾発射
	{
		OutputDebugStringA("Bullet Create\n");

		Bullet* bullet = Manager::AddGameObject<Bullet>();

		bullet->SetPosition(m_Position);

		bullet->SetVelocity(GetForward() * 25.0f);//弾の速度をプレイヤーの前方に設定（例: 1.0f）
	}
	if (m_Grounded)
	{
		m_MoveAnimetion += m_Velocity.length() * dt;
		m_Scale.y += sinf(m_MoveAnimetion * 3.0f) * 0.05f;
	}

	GameObject::Update();
}
void Player::Draw()
{
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	XMMATRIX world, scale, rot, trans;
	scale = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);//拡大率
	rot = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y + XM_PI, m_Rotation.z);//回転量
	trans = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);//平行移動量
	world = scale * rot * trans;

	Renderer::SetWorldMatrix(world);

	GameObject::Draw();
}