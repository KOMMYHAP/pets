#pragma once
#include <variant>

namespace ApplicationEvents
{
	struct MouseMoved
	{
		int x;
		int y;
	};

	struct MouseClicked
	{
		enum class MouseButton
		{
			LeftButton
		};

		int			x;
		int			y;
		MouseButton	button;
	};

	struct CloseRequest
	{
	};

	struct WindowResized
	{
		int newSizeX;
		int newSizeY;
	};
}

struct ApplicationEvent
{
	using ContentType = std::variant
	<
		ApplicationEvents::MouseMoved,
		ApplicationEvents::MouseClicked,
		ApplicationEvents::CloseRequest,
		ApplicationEvents::WindowResized
	>;

	ApplicationEvent(ContentType content)
		: content(std::move(content))
	{}

	ContentType content;
};
