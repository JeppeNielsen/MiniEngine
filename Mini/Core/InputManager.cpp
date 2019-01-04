#include "InputManager.hpp"
#include "InputDevice.hpp"

using namespace Mini;

InputManager::InputManager() : device(0) {
    auto keyBoardChanged = [this]() {
        device->SetKeyboard(KeyboardText, KeyboardActive);
    };

    KeyboardActive.Changed.Bind(keyBoardChanged);
    KeyboardText.Changed.Bind(keyBoardChanged);
}

Vector2 InputManager::GetTouchPosition(int index) {
	return device->GetTouchPosition(index);
}

void InputManager::SwallowTouch(int index, int depth) {
    device->SwallowTouch(index, depth);
}

bool InputManager::IsTouchSwallowed(int index, int depth) {
    return device->IsTouchSwallowed(index, depth);
}
