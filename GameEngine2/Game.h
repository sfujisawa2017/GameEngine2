//
// Game.h
//

#pragma once

#include <DirectXMath.h>
#include <DirectXColors.h>

#include "MyLibrary.h"

class Game : public MyLibrary::Framework
{
public:
	Game(HINSTANCE hInstance, int nCmdShow);

    void Initialize() override;

	void Finalize() override;

private:

    void Update(MyLibrary::StepTimer const& timer);
    void Render();

    // Rendering loop timer.
	MyLibrary::StepTimer                           m_timer;
};