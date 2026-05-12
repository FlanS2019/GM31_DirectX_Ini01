#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "polygon2d.h"
#include "field.h"
#include "camera.h"

GameObject* g_GameObject[3];

void Manager::Init()
{
	Renderer::Init();
	g_GameObject[0] = new Camera();
	g_GameObject[0]->Init();

	g_GameObject[1] = new Field();
	g_GameObject[1]->Init();	

	g_GameObject[2] = new Polygon2D();
	g_GameObject[2]->Init();

}

void Manager::Uninit()
{
	for(GameObject* gameObject : g_GameObject)
	{
		gameObject->Uninit();
		delete gameObject;
	}
	Renderer::Uninit();
}

void Manager::Update()
{
	for (GameObject* gameObject : g_GameObject)
	{
		gameObject->Update();
	}
}

void Manager::Draw()
{
	Renderer::Begin();

	for (GameObject* gameObject : g_GameObject)
	{
		gameObject->Draw();
	}

	Renderer::End();
}
