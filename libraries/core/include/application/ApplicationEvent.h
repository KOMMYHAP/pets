#pragma once
#include <variant>

struct MouseMoveEvent
{
	int x;
	int y;
};

struct MouseClickEvent
{
	enum class MouseButton
	{
		LeftButton
	};

	int			x;
	int			y;
	MouseButton	button;
};

struct ApplicationClose
{
};

struct ApplicationEvent
{
	std::variant<
		MouseMoveEvent,
		MouseClickEvent,
		ApplicationClose>		content;
};
