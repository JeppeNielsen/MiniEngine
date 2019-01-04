#pragma once
#include <memory>
#include "IState.hpp"

namespace Mini {
    class Engine {
    public:
        template<typename State>
        void Start() {
            state = std::make_unique<State>();
            MainLoop();
        }
        
    private:
        void MainLoop();
        std::unique_ptr<IState> state;
    };
}
