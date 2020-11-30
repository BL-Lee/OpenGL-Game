#pragma once
#include <string>
#include <functional>

enum class EventType
{
	None = 0,

	KeyPressed,
	KeyReleased,

	MousePressed,
	MouseReleased,
	MouseMoved,
	MouseScrolled,

	WindowClose,
	WindowResize,
	WindowFocus,
	WindowLostFocus,
};

enum EventCategory
{
	None = 0,

	EventCategoryApplication =		1 << 1,
	EventCategoryInput =			1 << 2,
	EventCategoryKeyboardInput =	1 << 3,
	EventCategoryMouseInput =		1 << 4,
};	

class Event
{
	friend class EventDispatcher;
public:
	static EventType GetStaticType();
	virtual EventType GetEventType() const = 0;
	virtual const char* GetName() const = 0;
	virtual std::string ToString() const { return GetName(); }
	//virtual EventCategory GetCategoryFlags() const = 0;
	//inline bool IsInCategory(EventCategory e) { return GetCategoryFlags() & e; }
protected:
	bool m_Handled = false;
};

class EventDispatcher
{
	template<typename T>
	using EventFunc = std::function<bool(T&)>;
private:
	Event& m_Event;
public:
	EventDispatcher(Event& e)
		:m_Event(e)
	{
	}
	template<typename T>
	bool Dispatch(EventFunc<T> e)
	{
		if (m_Event.GetEventType() == T::GetStaticType())
		{
			m_Event.m_Handled = e(*(T*)&m_Event);
			return true;
		}
		return false;
	}
};