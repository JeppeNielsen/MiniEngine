#include "Engine.hpp"
#include "Timer.hpp"
#include "Window.hpp"

using namespace Mini;

void Engine::MainLoop() {

    Window window;
    Timer timer;
    
    float elapsed = 1.0f / 60.0f;
    
    window.StartLoop(
    [&, this]() {
        state->Initialize();
    },
    [&, this]() {
        timer.Start();
        state->Update(elapsed);
        window.PreRender();
        state->Render();
        window.PostRender();
        elapsed = timer.Stop();
        return state->device.exited;
    });
}
