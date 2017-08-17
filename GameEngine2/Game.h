//
// Game.h
//

#pragma once

#include <DirectXMath.h>
#include <DirectXColors.h>

#include "MyLibrary.h"
#include "ParticleTest.h"

class Game : public MyLibrary::Framework
{
public:
	Game(HINSTANCE hInstance, int nCmdShow);

    void Initialize() override;

	void Finalize() override;

private:

    void Update(MyLibrary::StepTimer const& timer);
    void Render();

	std::unique_ptr<ParticleTest> m_ParticleTest;
};