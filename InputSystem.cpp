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
	// Retrieves the entire sequence of states of each key of the keyboard
	if (::GetKeyboardState(this->m_keys_state)) {
		for (unsigned int i = 0; i < 256; i++) {
			// KEY IS DOWN
			if (this->m_keys_state[i] & 0x80) {
				std::map<InputListener*, InputListener*>::iterator it = this->m_map_listeners.begin();

				while (it != this->m_map_listeners.end()) {
					it->second->onKeyDown(i);
					++it;
				}
			}

			else {
				if (this->m_keys_state[i] != this->m_old_keys_state[i]) {
					std::map<InputListener*, InputListener*>::iterator it = this->m_map_listeners.begin();

					while (it != this->m_map_listeners.end()) {
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
