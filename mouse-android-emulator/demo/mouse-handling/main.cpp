#include "imgui.h"
#include "imgui-SFML.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

sf::Vector2i g_mouseSpeed(0, 0);

static void HandleArrowKey(sf::Keyboard::Key key, bool pressed)
{
	int speed = pressed ? 5 : 0;
	g_mouseSpeed.x =
		key == sf::Keyboard::Left ? -speed : key == sf::Keyboard::Right ? +speed : g_mouseSpeed.x;

	g_mouseSpeed.y =
		key == sf::Keyboard::Up ? -speed : key == sf::Keyboard::Down ? +speed : g_mouseSpeed.y;
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(640, 480), "ImGui + SFML = <3");
	window.setFramerateLimit(60);
	ImGui::SFML::Init(window);

	sf::Clock deltaClock;
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			ImGui::SFML::ProcessEvent(event);

			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
			if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased)
			{
				switch (event.key.code)
				{
				case sf::Keyboard::Left:
				case sf::Keyboard::Right:
				case sf::Keyboard::Up:
				case sf::Keyboard::Down:
					HandleArrowKey(event.key.code, event.type == sf::Event::KeyPressed);
					break;
				default:
					break;
				}
			}
		}

		ImGui::SFML::Update(window, deltaClock.restart());

		{
			// game update
			if (g_mouseSpeed.x != 0 || g_mouseSpeed.y != 0)
			{
				sf::Vector2i pos = sf::Mouse::getPosition();
				pos.x += g_mouseSpeed.x;
				pos.y += g_mouseSpeed.y;
				sf::Mouse::setPosition(pos);
			}
		}
		{
			// imgui update
			ImGui::Begin("Hello, world!");
			ImGui::Button("Look at this pretty button");
			ImGui::End();
		}

		window.clear();
		{
			// game render
		}
		{
			// imgui render
			ImGui::SFML::Render(window);
		}
		window.display();
	}

	ImGui::SFML::Shutdown();

	return 0;
}
