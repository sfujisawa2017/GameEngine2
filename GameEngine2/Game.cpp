//
// Game.cpp
//

#include "Game.h"

extern void ExitGame();

using namespace DirectX;
using Microsoft::WRL::ComPtr;
using namespace MyLibrary;

Game::Game(HINSTANCE hInstance, int nCmdShow)
	: Framework(hInstance, nCmdShow)
{
}

void Game::Initialize()
{
	// �}�E�X���C�u�����ɃE�B���h�E�n���h����n��
	MouseUtil::GetInstance()->SetWindow(DeviceResources::GetInstance()->GetWindow());
}

void Game::Finalize()
{

}

#pragma region Frame Update
/// <summary>
/// ���t���[���X�V
/// </summary>
/// <param name="timer">���ԏ��</param>
void Game::Update(StepTimer const& timer)
{
    
}
#pragma endregion

#pragma region Frame Render
/// <summary>
/// ���t���[���`��
/// </summary>
void Game::Render()
{
    
}
#pragma endregion
