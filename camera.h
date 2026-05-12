#pragma once
#include "gameObject.h"

class Camera : public GameObject
{
private:
	Vector3 m_Target{ 0, 0, 0 };
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
};

