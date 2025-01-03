#include "Star.h"
#include "config.h"
#include "Game.h"
#include "Texture.h"
#include "SFML/Graphics.hpp"
#include <Player.h>
#include "Log.h"

std::vector<Star> Star::stars;
sf::VertexArray Star::verticles;
Texture* Star::texture;
sf::RenderStates* Star::state;

const sf::Vector2f bl = { -0.5,-0.5 };
const sf::Vector2f tl = { 0.5, -0.5 };
const sf::Vector2f br = { 0.5,0.5 };
const sf::Vector2f tr = { -0.5,0.5 };

void Star::init() {
	texture = new Texture("img/star.png");
	state = new sf::RenderStates();
	state->transform.translate({ Config::SCREEN_WIDTH * 0.5,Config::SCREEN_HEIGHT * 0.5 });
	state->texture = texture;
}

void Star::create(const int& count)
{
    stars.resize(count);
    for (int i = 0; i < stars.size(); i++) {
        stars[i] = Star();
    }

	verticles = sf::VertexArray(sf::PrimitiveType::Quads, Config::STAR_COUNT * 4);
	int s;
	auto texSize = texture->getSize();
	for (int i = 0; i < stars.size(); i++) {
		s = i * 4;
		verticles[s].texCoords = { 0,0 };
		verticles[s+1].texCoords = { (float)texSize.x,0 };
		verticles[s+2].texCoords = { (float)texSize.x,(float)texSize.y };
		verticles[s+3].texCoords = { 0,(float)texSize.y };
	}
}

void Star::draw() {
	glm::vec3 locPos, scrPos;
	sf::Vector2f center;
	int i = 0;
	for (int s = 0; s < stars.size(); s++) {
		locPos = glm::inverse(Player::main->rotation) * stars[s].rot * glm::vec3(0, 0, 10000);
		scrPos = Player::main->local2ScreenPos(locPos);
		center = { scrPos.x, scrPos.y };
		i = s * 4;
		verticles[i].position = center + bl * stars[s].size;
		verticles[i + 1].position = center + tl * stars[s].size;
		verticles[i + 2].position = center + br * stars[s].size;
		verticles[i + 3].position = center + tr * stars[s].size;

		verticles[i].color = stars[s].color;
		verticles[i+1].color = stars[s].color;
		verticles[i+2].color = stars[s].color;
		verticles[i+3].color = stars[s].color;
	}

	Game::window->draw(verticles, *state);	
}
