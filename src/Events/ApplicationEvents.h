#pragma once

#include "Event.h"


class WindowCloseEvent : public Event
{
public:
	WindowCloseEvent() {}

	static EventType GetStaticType() { return EventType::WindowClose; }
	EventType GetEventType() const { return EventType::WindowClose; }
	const char* GetName() const { return "WindowCloseEvent"; }
};
class WindowResizeEvent : public Event
{
private:
	int m_x, m_y;
public:
	WindowResizeEvent(int x, int y)
		:m_x(x), m_y(y)
	{}
	int GetX() { return m_x; }
	int GetY() { return m_y; }
	static EventType GetStaticType() { return EventType::WindowResize; }
	EventType GetEventType() const { return EventType::WindowResize; }
	const char* GetName() const { return "WindowResizeEvent"; }
};