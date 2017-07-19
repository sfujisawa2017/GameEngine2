//
// Game.h
//

#pragma once

#include <DirectXMath.h>
#include <DirectXColors.h>

#include "MyLibrary.h"

// A basic game implementation that creates a D3D11 device and
// provides a game loop.
class Game : public MyLibrary::IDeviceNotify
{
public:

    Game();

    // Initialization and management
    void Initialize(HWND window, int width, int height);

    // Basic game loop
    void Tick();

    // IDeviceNotify
    virtual void OnDeviceLost() override;
    virtual void OnDeviceRestored() override;

    // Messages
    void OnActivated();
    void OnDeactivated();
    void OnSuspending();
    void OnResuming();
    void OnWindowSizeChanged(int width, int height);

    // Properties
    void GetDefaultSize( int& width, int& height ) const;

private:

    void Update(MyLibrary::StepTimer const& timer);
    void Render();

    void Clear();

    void CreateDeviceDependentResources();
    void CreateWindowSizeDependentResources();

    // Device resources.
    MyLibrary::DeviceResources*    m_deviceResources;

    // Rendering loop timer.
	MyLibrary::StepTimer                           m_timer;
};