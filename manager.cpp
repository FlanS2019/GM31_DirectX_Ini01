#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "polygon2d.h"

Polygon2D* g_Polygon2D;

void Manager::Init()
{
	Renderer::Init();
	g_Polygon2D = new Polygon2D();
	g_Polygon2D->Init();
}


void Manager::Uninit()
{
	g_Polygon2D->Uninit();
	delete g_Polygon2D;

	Renderer::Uninit();

}

void Manager::Update()
{
		g_Polygon2D->Update();
}

void Manager::Draw()
{
	Renderer::Begin();

	g_Polygon2D->Draw();

	Renderer::End();
}
