#include "InputSystem.h"
#include <Windows.h>

InputSystem::InputSystem()
{
}

InputSystem::~InputSystem()
{
}

void InputSystem::update()
{
	POINT current_mouse_pos = {};
	::GetCursorPos(&current_mouse_pos);

	if (this->m_first_time) {
		this->m_old_mouse_pos = Point(current_mouse_pos.x, current_mouse_pos.y);
		this->m_first_time = false;
	}

	if (current_mouse_pos.x != this->m_old_mouse_pos.m_x || current_mouse_pos.y != this->m_old_mouse_pos.m_y)
	{
		//Old position was changed, mouse move event detected
		std::map<InputListener*, InputListener*>::iterator it = this->m_map_listeners.begin();

		while (it != this->m_map_listeners.end()) {
			it->second->onMouseMove(Point(current_mouse_pos.x - this->m_old_mouse_pos.m_x, current_mouse_pos.y - this->m_old_mouse_pos.m_y));
			++it;
		}
	}

	this->m_old_mouse_pos = Point(current_mouse_pos.x, current_mouse_pos.y);

	// Retrieves the entire sequence of states of each key of the keyboard
	if (::GetKeyboardState(this->m_keys_state)) {
		for (unsigned int i = 0; i < 256; i++) {
			// KEY IS DOWN
			if (this->m_keys_state[i] & 0x80) {
				std::map<InputListener*, InputListener*>::iterator it = this->m_map_listeners.begin();

				while (it != this->m_map_listeners.end()) {

					if (i == VK_LBUTTON) {
						if (this->m_keys_state[i] != this->m_old_keys_state[i])
							it->second->onLeftMouseDown(Point(current_mouse_pos.x, current_mouse_pos.y));
					}
					
					if (i == VK_RBUTTON) {
						if (this->m_keys_state[i] != this->m_old_keys_state[i])
							it->second->onRightMouseDown(Point(current_mouse_pos.x, current_mouse_pos.y));
					}

					else
						it->second->onKeyDown(i);

					++it;
				}
			}

			else {
				if (this->m_keys_state[i] != this->m_old_keys_state[i]) {
					std::map<InputListener*, InputListener*>::iterator it = this->m_map_listeners.begin();

					while (it != this->m_map_listeners.end()) {
						if (i == VK_LBUTTON) 
							it->second->onLeftMouseUp(Point(current_mouse_pos.x, current_mouse_pos.y));
						
						else if (i == VK_RBUTTON)
							it->second->onRightMouseUp(Point(current_mouse_pos.x, current_mouse_pos.y));

						else
							it->second->onKeyUp(i);

						++it;
					}
				}
			}
		}

		// Stores the current keys state to old keys state buffer
		::memcpy(this->m_old_keys_state, this->m_keys_state, sizeof(unsigned char) * 256);
	}
}

void InputSystem::addListener(InputListener* listener)
{
	this->m_map_listeners.insert(std::make_pair<InputListener*, InputListener*>(std::forward<InputListener*>(listener), std::forward<InputListener*>(listener)));
}

void InputSystem::removeListener(InputListener* listener)
{
	std::map<InputListener*, InputListener*>::iterator it = this->m_map_listeners.find(listener);

	if (it != this->m_map_listeners.end())
		this->m_map_listeners.erase(it);
}

InputSystem* InputSystem::get()
{
	static InputSystem system;
	return &system;
}
