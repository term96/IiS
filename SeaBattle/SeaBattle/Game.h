#pragma once

#include <SFML\Graphics.hpp>
#include <SFML\System.hpp>
#include <SFML\Window.hpp>
#include <string>
#include <vector>

const float CELL_SIZE = 40;
const float OUTLINE_SIZE = -1;

const int WINDOW_WIDTH = 1280; //1366
const int WINDOW_HEIGHT = 600; // 768
const wchar_t GAME_NAME[] = L"Морской бой";

const float FIELD_SIZE = 10 * CELL_SIZE;
const float VER_PADDING = (WINDOW_HEIGHT - FIELD_SIZE) / 2;
const float HOR_PADDING = (WINDOW_WIDTH - 2 * FIELD_SIZE) / 3;

enum CellState
{
	HIT, MISS, SHIP, EMPTY
};

enum WhichMove
{
	PLAYER, AI
};

struct Ship;

struct Cell
{
	sf::RectangleShape shape;
	CellState state;
	Ship *ship = nullptr;
};

typedef Cell Field[10][10];

enum GameState
{
	RUN, END
};

struct Game
{
	sf::RenderWindow window;

	Field enemyField;
	Field playerField;

	sf::Vector2i enemyPos[10][10];
	sf::Vector2i playerPos[10][10];

	int enemyAliveCells;
	int playerAliveCells;

	GameState state = GameState::RUN;

	sf::Font font;
};

struct Ship
{
	std::vector<Cell *> cells;
	int health;
	int maxHealth;
	int startCol;
	int startRow;
};

void InitiateCell(Cell & cell, sf::Vector2f position);

void InitiateGame(Game & game);

void AIMove(Game & game);

bool HitCell(Cell * cell, Field & field);

bool CellCanBeHit(Cell * cell);

void DestroyShip(Ship * ship, Field & field);