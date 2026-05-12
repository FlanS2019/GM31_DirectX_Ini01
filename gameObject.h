#pragma once
#include "vector3.h"

class GameObject
{
protected://継承先でアクセスできるようにする
	Vector3 m_Position{ 0,0,0 };
	Vector3 m_Rotation{ 0,0,0 };
	Vector3 m_Scale{ 1,1,1 };

public:
	// 仮想デストラクタを追加（delete 時に派生クラスのデストラクタが呼ばれるように）
	virtual ~GameObject();

	// 派生クラスでオーバーライド可能にする
	virtual void Init();
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();
};
