//result.cpp
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "Input.h"
#include "result.h"
#include "polygon2d.h"
#include "title.h"
#include "Game.h"
void result::Init()
{
	//結果画面の背景画像を表示するためのPolygon2Dオブジェクトを作成
	Manager::AddGameObject<Polygon2D>()->Init(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, L"texture\\image1.png");
}

void result::Uninit()
{
}

void result::Update()
{	
	Manager::ChangeScene<result>();//タイトル画面に遷移
}

void result::Draw()
{
}
