#include <SFML\Graphics.hpp>
#include <SFML\System.hpp>
#include <SFML\Window.hpp>

#include <string>
#include <iostream>

#include "Game.h"
using namespace std;
using namespace sf;

Cell* GetCell(Game &game, Vector2i mousePos)
{
	for (int i = 0; i < 10; ++i)
	{
		for (int j = 0; j < 10; ++j)
		{
			if (mousePos.x >= game.enemyPos[i][j].x && mousePos.x <= game.enemyPos[i][j].x + CELL_SIZE &&
				mousePos.y >= game.enemyPos[i][j].y && mousePos.y <= game.enemyPos[i][j].y + CELL_SIZE)
			{
				return &game.enemyField[i][j];
			}
		}
	}

	return nullptr;
}

void HandleCellClick(Game &game, Vector2i position)
{
	Cell *cell = GetCell(game, position);
	if (cell == nullptr)
		return;

	if (CellCanBeHit(cell))
	{
		if (!HitCell(cell, game.enemyField))
		{
			AIMove(game);
		}
		else
		{
			game.enemyAliveCells--;
		}
	}
}

void DrawFields(Game &game)
{
	for (int i = 0; i < 10; ++i)
	{
		for (int j = 0; j < 10; ++j)
		{
			game.window.draw(game.enemyField[i][j].shape);
			game.window.draw(game.playerField[i][j].shape);
		}
	}
}

void DrawText(Game &game)
{
	float x;
	float y = VER_PADDING - 48;

	sf::Text enemyFieldText;
	enemyFieldText.setFont(game.font);
	enemyFieldText.setCharacterSize(24);
	enemyFieldText.setString(L"Вражеское поле");
	x = (CELL_SIZE * 10 - enemyFieldText.getGlobalBounds().width) / 2 + HOR_PADDING;
	enemyFieldText.setPosition(x, y);
	enemyFieldText.setFillColor(Color::Black);

	sf::Text playerFieldText;
	playerFieldText.setFont(game.font);
	playerFieldText.setCharacterSize(24);
	playerFieldText.setString(L"Ваше поле");
	x = (CELL_SIZE * 10 - playerFieldText.getGlobalBounds().width) / 2 + (2 * HOR_PADDING + FIELD_SIZE);
	playerFieldText.setPosition(x, y);
	playerFieldText.setFillColor(Color::Black);

	game.window.draw(enemyFieldText);
	game.window.draw(playerFieldText);

	
}

void CheckGameEnd(Game &game)
{
	wchar_t *gameResult;
	if (game.enemyAliveCells == 0)
	{
		gameResult = L"Вы выиграли";
	}
	else if (game.playerAliveCells == 0)
	{
		gameResult = L"Вы проиграли";
	}
	else
	{
		return;
	}

	game.state = GameState::END;

	sf::Text gameEndText;

	gameEndText.setFont(game.font);
	gameEndText.setCharacterSize(72);
	gameEndText.setString(gameResult);
	
	float x = game.window.getSize().x / 2 - gameEndText.getGlobalBounds().width / 2;
	float y = game.window.getSize().y / 2.f;

	gameEndText.setPosition(x, y);
	gameEndText.setFillColor(Color::Black);

	game.window.draw(gameEndText);
}

void HandleEvents(Game &game)
{
	Event event;
	while (game.window.pollEvent(event))
	{
		switch (event.type)
		{
		case Event::Closed:
			game.window.close();
		case Event::MouseButtonPressed:
			if (event.mouseButton.button == sf::Mouse::Left && game.state == GameState::RUN)
			{
				Vector2i mousePos = Mouse::getPosition(game.window);
				HandleCellClick(game, mousePos);
			}
		}
	}
}

int main()
{
	Game game;
	InitiateGame(game);
	game.window.setFramerateLimit(60);

	while (game.window.isOpen())
	{
		HandleEvents(game);
		game.window.clear(Color::White);

		DrawFields(game);
		DrawText(game);

		CheckGameEnd(game);
		game.window.display();
	}
	
	return 0;
}