#include "Engine.hpp"
#include "Timer.hpp"
#include "Window.hpp"

using namespace Mini;

void Engine::MainLoop() {

    Screen::MainScreen = &state->device.Screen;
    
    Window window;
    Timer timer;
    
    float elapsed = 1.0f / 60.0f;
    
    Window::MainLoopData data;
    data.Initialize =
    [&, this]() {
        state->device.Menu.InitializeMainMenu();
        state->Initialize();
    };
    
    data.Update =
    [&, this]() {
        elapsed = timer.Stop();
        timer.Start();
        window.inputDevice.StartFrame();
        window.inputDevice.UpdateInputManager(state->device.Input);
        state->Update(elapsed);
        window.inputDevice.EndFrame();
        window.PreRender();
        state->Render();
        window.PostRender();
        return state->device.exited;
    };
    
    data.ScreenSize =
    [&, this] (int width, int height) {
        state->device.Screen.Size = Vector2(width, height);
    };
    
    data.ScreenScalingFactor =
    [&, this] (float scalingFactor) {
        state->device.Screen.ScalingFactor = scalingFactor;
    };
    
    window.StartLoop(data);
}
