#include "main.h"
#include "camera.h"
#include "renderer.h"
#include "manager.h"
#include "player.h"
#include "Input.h"

void Camera::Init()
{
	m_Position = { 0, 5, -10 };
	//m_Yaw = 0.0f;    // 追加
	//m_Pitch = 0.3f;  // 追加
	//m_Distance = 8.0f; // 追加
}

void Camera::Uninit()
{
}

void Camera::Update()
{
	Player* player = Manager::GetGameObject<Player>();
	if (player == nullptr) return;

	Vector3 playerPos = player->GetPosition();
	float playerYaw = player->GetRotation().y + XM_PI;// プレイヤーの向きに合わせてカメラも回転させるため、プレイヤーの回転からYawを取得して180度（PIラジアン）加算する
	// プレイヤーの真後ろにカメラを置く
	const float distance = 8.0f;//プレイヤーからカメラまでの距離
	const float height = 4.0f;

	m_Position.x = playerPos.x - sinf(playerYaw) * distance;
	m_Position.y = playerPos.y + height;
	m_Position.z = playerPos.z - cosf(playerYaw) * distance;

	m_Target = playerPos;
}

void Camera::Draw()
{
	//projection行列の作成
	XMMATRIX projection = XMMatrixPerspectiveFovLH
	(1, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 1000.0f);

	Renderer::SetProjectionMatrix(projection);

	//view行列の作成
	XMFLOAT3 up = XMFLOAT3(0, 1, 0);
	XMMATRIX view = XMMatrixLookAtLH(XMLoadFloat3((XMFLOAT3*)&m_Position),
		XMLoadFloat3((XMFLOAT3*)&m_Target), XMLoadFloat3(&up));

	Renderer::SetViewMatrix(view);
}