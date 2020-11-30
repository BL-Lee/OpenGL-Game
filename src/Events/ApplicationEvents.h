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