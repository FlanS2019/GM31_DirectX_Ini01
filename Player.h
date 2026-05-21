#pragma once

#include "gameObject.h"

class Player : public GameObject
{
private:
	Vector3 m_Velocity{ 0,0,0 };

	ID3D11InputLayout* m_VertexLayout = nullptr;
	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11PixelShader* m_PixelShader = nullptr;

	// コンポーネント参照（Init で AddComponent して保持）
	class Transform* m_Transform = nullptr;
public:
	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
};