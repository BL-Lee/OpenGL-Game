#pragma once
#include "Event.h"
#include <sstream>
class KeyEvent : public Event
{
protected:
	KeyEvent(int keycode)
		:m_KeyCode(keycode) {}
	int m_KeyCode;
public:
	inline int GetKeyCode() const { return m_KeyCode; }
	static EventType GetStaticType();
	virtual EventType GetEventType() const = 0;
	virtual const char* GetName() const = 0;
private:

};

class KeyPressedEvent : public KeyEvent
{
public: 
	KeyPressedEvent(int keycode, int repeats)
		: KeyEvent(keycode), m_Repeats(repeats) {}

	static EventType GetStaticType() { return EventType::KeyPressed; }
	EventType GetEventType() const { return EventType::KeyPressed; }
	const char* GetName() const { return "KeyPressedEvent"; }
	std::string ToString() const {
		std::ostringstream oss;
		oss << "KeyPressedEvent: Key(" << m_KeyCode << ") Repeats:" << m_Repeats << "";
		return oss.str();
	}

	inline int GetRepeatCount() const { return m_Repeats; }

private:
	int m_Repeats;
};



class KeyReleasedEvent : public KeyEvent
{
public:
	KeyReleasedEvent(int keycode)
		: KeyEvent(keycode) {}

	static EventType GetStaticType() { return EventType::KeyReleased; }
	EventType GetEventType() const { return EventType::KeyReleased; }
	const char* GetName() const { return "KeyReleasedEvent"; }
	std::string ToString() const { 
		std::ostringstream oss;
		oss << "KeyReleasedEvent: Key(" << m_KeyCode << ")";
		return oss.str();
	}
};