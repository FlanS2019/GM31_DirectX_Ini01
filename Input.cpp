#include "main.h"
#include "Input.h"

BYTE Input::m_OldKeyState[256] = {};
BYTE Input::m_PrevKeyState[256] = {};

void Input::Init()
{
	memset(m_OldKeyState, 0, 256);
	memset(m_PrevKeyState, 0, 256);
}
void Input::Uninit()
{
}
void Input::Update()
{
	memcpy(m_PrevKeyState, m_OldKeyState, 256);
	GetKeyboardState(m_OldKeyState);
}
bool Input::GetKeyPress(BYTE KeyCode)
{
	return (m_OldKeyState[KeyCode] & 0x80) != 0;
}
bool Input::GetKeyTrigger(BYTE KeyCode)
{
	return ((m_OldKeyState[KeyCode] & 0x80) != 0) && ((m_PrevKeyState[KeyCode] & 0x80) == 0);
}