#pragma once
#include "gameObject.h"

class Camera : public GameObject
{
private:
	Vector3 m_Target{ 0, 0, 0 };
public:
	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
};

