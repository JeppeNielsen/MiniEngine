#pragma once
#include <string>
#include "Event.hpp"
#include "Vector2.hpp"
#include "Property.hpp"

namespace Mini {

    enum class ModifierKey {
        None,
        Shift,
        Alt,
        Command,
        Ctrl,
        Fn
    };
    
	struct TouchEvent {
		TouchEvent(int index, Vector2 position) :Index(index), Position(position) {}
		int Index;
		Vector2 Position;
	};
    
    struct ButtonEvent {
        std::string Id;
        ModifierKey modifierKey;
    };

	class InputDevice;
	class InputManager {
	public:
		InputManager();
		~InputManager() = default;

		Event<ButtonEvent> ButtonDown;
		Event<ButtonEvent> ButtonUp;

		Event<TouchEvent> TouchDown;
		Event<TouchEvent> TouchUp;

		Vector2 GetTouchPosition(int index);
        
        Property<bool> KeyboardActive;
        Property<std::string> KeyboardText;
        
        Event<float> ScrollChanged;
       
        void SwallowTouch(int index, int depth);
        bool IsTouchSwallowed(int index, int depth);
        
	private:
		InputDevice* device;
		friend class InputDevice;
	};
}
