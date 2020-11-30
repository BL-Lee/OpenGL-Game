#pragma once
#include "Event.h"
#include <sstream>
class MouseEvent : public Event
{
protected:
	MouseEvent(int keycode)
		:m_Button(keycode) {}
	int m_Button;
public:
	inline int GetKeyCode() const { return m_Button; }
	static EventType GetStaticType();
	virtual EventType GetEventType() const = 0;
	virtual const char* GetName() const = 0;
private:

};

class MousePressedEvent : public MouseEvent
{
public:
	MousePressedEvent(int button)
		: MouseEvent(button) {}

	static EventType GetStaticType() { return EventType::MousePressed; }
	EventType GetEventType() const { return EventType::MousePressed; }
	const char* GetName() const { return "MousePressedEvent"; }
	std::string ToString() const { 
		std::ostringstream oss;
		oss << "MousePressedEvent: Button(" << m_Button << ")";
		return oss.str();
	}
};

class MouseReleasedEvent : public MouseEvent
{
public:
	MouseReleasedEvent(int button)
		: MouseEvent(button) {}

	static EventType GetStaticType() { return EventType::MouseReleased; }
	EventType GetEventType() const { return EventType::MouseReleased; }
	const char* GetName() const { return "MouseReleasedEvent"; }
	std::string ToString() const {
		std::ostringstream oss;
		oss << "MouseReleasedEvent: Button(" << m_Button << ")";
		return oss.str();
	}
};

class MouseScrollEvent : public Event
{
private:
	double m_X;
	double m_Y;
public:
	MouseScrollEvent(double x, double y)
		: m_X(x), m_Y(y) {}

	static EventType GetStaticType() { return EventType::MouseScrolled; }
	EventType GetEventType() const { return EventType::MouseScrolled; }
	const char* GetName() const { return "MouseScrollEvent"; }
	std::string ToString() const {
		std::ostringstream oss;
		oss << "MouseScrollEvent: Button(" << m_X << ", " << m_Y << ")";
		return oss.str();
	}
};

class MouseMovedEvent : public Event
{
private:
	double m_X;
	double m_Y;
public:
	MouseMovedEvent(double x, double y)
		: m_X(x), m_Y(y) {}

	static EventType GetStaticType() { return EventType::MouseMoved; }
	EventType GetEventType() const { return EventType::MouseMoved; }
	const char* GetName() const { return "MouseMovedEvent"; }
	std::string ToString() const {
		std::ostringstream oss;
		oss << "MouseMovedEvent: Button(" << m_X << ", " << m_Y << ")";
		return oss.str();
	}
};


