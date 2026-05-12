#include "main.h"
#include "camera.h"
#include "renderer.h"
#include "manager.h"

void Camera::Init()
{
	m_Position = { 0, 5, -10 };
}

void Camera::Uninit()
{
}

void Camera::Update()
{
}

void Camera::Draw()
{
	//projectionçsóÒÇÃçÏê¨
	XMMATRIX projection = XMMatrixPerspectiveFovLH
	(1,(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 1000.0f);

	Renderer::SetProjectionMatrix(projection);

	//viewçsóÒÇÃçÏê¨
	XMFLOAT3 up = XMFLOAT3(0, 1, 0);
	XMMATRIX view = XMMatrixLookAtLH(XMLoadFloat3((XMFLOAT3*)&m_Position),
		XMLoadFloat3((XMFLOAT3*)&m_Target), XMLoadFloat3(&up));

	Renderer::SetViewMatrix(view);
}