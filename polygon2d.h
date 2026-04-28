#pragma once
#include <d3d11.h>

class Polygon2D
{
private:

	ID3D11Buffer* m_VertexBuffer;

	ID3D11InputLayout* m_VertexLayout;
	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
};
