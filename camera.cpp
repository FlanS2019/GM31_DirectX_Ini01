#include "main.h"
#include "camera.h"
#include "renderer.h"
#include "manager.h"
#include "player.h"
#include "Input.h"

void Camera::Init()
{
	m_Position = { 0, 5, -20 };
	//m_Yaw = 0.0f;    // í«â¡
	//m_Pitch = 0.3f;  // í«â¡
	//m_Distance = 8.0f; // í«â¡
}

void Camera::Uninit()
{
}

void Camera::Update()
{
	Player* player = Manager::GetGameObject<Player>();
	Vector3 playerPos = player->GetPosition();

	float dt = 1.0f / 60.0f;

	if (Input::GetKeyPress(VK_RIGHT))
		m_Rotation.y -= 2.0f * dt;
	else if (Input::GetKeyPress(VK_LEFT))
		m_Rotation.y += 2.0f * dt;

	float t = 0.1f;
	m_Target = m_Target * (1.0f - t) + (playerPos + Vector3(0.0f, 5.0f, 0.0f)) * t;
	m_Position = m_Target + Vector3(sinf(m_Rotation.y) * 10.0f, 2.5f, -cosf(m_Rotation.y) * 10.0f);
}

void Camera::Draw()
{
	//projectionçsóÒÇÃçÏê¨
	XMMATRIX projection = XMMatrixPerspectiveFovLH
	(1, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 1000.0f);

	Renderer::SetProjectionMatrix(projection);

	XMFLOAT3 up = XMFLOAT3(0, 1, 0);
	XMFLOAT3 pos = { m_Position.x, m_Position.y, m_Position.z };
	XMFLOAT3 target = { m_Target.x, m_Target.y, m_Target.z };
	m_ViewMatrix = XMMatrixLookAtLH(XMLoadFloat3(&pos),
		XMLoadFloat3(&target), XMLoadFloat3(&up));

	Renderer::SetViewMatrix(m_ViewMatrix);
}