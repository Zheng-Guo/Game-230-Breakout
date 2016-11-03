#include <SFML\Graphics.hpp>

using namespace sf;

int main() {
	RenderWindow window(VideoMode(500, 500), "Working");
	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed)
				window.close();
		}
		window.clear(Color::Green);
		window.display();
	}
	return EXIT_SUCCESS;
}