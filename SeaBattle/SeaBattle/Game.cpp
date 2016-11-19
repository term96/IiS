#include "Game.h"
#include "PlaceShips.h"

#include <string>
#include <iostream>
#include <algorithm>

using namespace std;
using namespace sf;


void InitiateCell(Cell &cell, Vector2f position)
{
	cell.shape = RectangleShape(Vector2f(CELL_SIZE, CELL_SIZE));
	cell.shape.setFillColor(Color::White);
	cell.shape.setOutlineThickness(OUTLINE_SIZE);
	cell.shape.setOutlineColor(Color(0, 0, 0, 128));
	cell.shape.setPosition(position);
	cell.state = CellState::EMPTY;
}

void InitiateGame(Game &game)
{
	game.window.create(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), GAME_NAME, Style::Close);
	game.font.loadFromFile("resources/comic.ttf");

	float enemyX;
	float playerX;
	float y;
	float horPadding = 2 * HOR_PADDING + FIELD_SIZE;

	for (int i = 0; i < 10; ++i)
	{
		for (int j = 0; j < 10; ++j)
		{
			enemyX = HOR_PADDING + j * CELL_SIZE;
			playerX = horPadding + j * CELL_SIZE;
			y = VER_PADDING + i * CELL_SIZE;
			Vector2f enemyPos(enemyX, y);
			Vector2f playerPos(playerX, y);

			InitiateCell(game.enemyField[i][j], enemyPos);
			InitiateCell(game.playerField[i][j], playerPos);

			game.enemyPos[i][j] = Vector2i(static_cast<int>(enemyX), static_cast<int>(y));
			game.playerPos[i][j] = Vector2i(static_cast<int>(playerX), static_cast<int>(y));
		}
	}

	RandomlyPlaceShips(game, game.enemyField);
	RandomlyPlaceShips(game, game.playerField);
	ShowPlayerShips(game);

	game.enemyAliveCells = 4 + (2 * 3) + (3 * 2) + 4;
	game.playerAliveCells = 4 + (2 * 3) + (3 * 2) + 4;
}

void AIMove(Game & game)
{
	static vector<Cell *> *cells = nullptr;
	if (cells == nullptr)
	{
		cells = new vector<Cell *>;
		for (int i = 0; i < 10; ++i)
		{
			for (int j = 0; j < 10; ++j)
			{
				cells->push_back(&game.playerField[i][j]);
			}
		}
	}
	random_shuffle(cells->begin(), cells->end());

	static vector<Cell *> hitQueue;
	static bool randomly = true;

	Cell *cell = nullptr;
	
	if (!randomly)
	{
		for (auto nextCell : hitQueue)
		{
			if (nextCell->state != CellState::MISS && nextCell->state != CellState::HIT)
			{
				cell = nextCell;
				break;
			}
		}
	}

	if (cell == nullptr)
	{
		do
		{
			cell = cells->front();
			cells->erase(cells->begin());
		} while (!CellCanBeHit(cell));
	}

	if (HitCell(cell, game.playerField))
	{
		randomly = !randomly;
		hitQueue.insert(hitQueue.end(), cell->ship->cells.begin(), cell->ship->cells.end());
		AIMove(game);
		game.playerAliveCells--;
	}
}

bool CellCanBeHit(Cell * cell)
{
	switch (cell->state)
	{
	case CellState::EMPTY:
	case CellState::SHIP:
		return true;
	default:
		return false;
	}
}

bool HitCell(Cell * cell, Field &field)
{
	switch (cell->state)
	{
	case CellState::EMPTY:
		cell->shape.setFillColor(Color(0, 0, 255, 200));
		cell->state = CellState::MISS;
		return false;
	case CellState::SHIP:
		cell->shape.setFillColor(Color(255, 79, 0, 200));
		cell->state = CellState::HIT;
		cell->ship->health--;
		if (cell->ship->health == 0)
		{
			DestroyShip(cell->ship, field);
		}
		return true;
	default:
		return false;
	}
}

void DestroyShip(Ship *ship, Field &field)
{
	for (auto cell : ship->cells)
	{
		cell->shape.setFillColor(Color(255, 0, 0, 200));
	}

	if (ship->startCol > 0)
	{
		for (int i = ship->startRow; i < ship->startRow + ship->maxHealth; ++i)
		{
			field[ship->startCol - 1][i].shape.setFillColor(Color(0, 0, 255, 200));
			field[ship->startCol - 1][i].state = CellState::MISS;
		}
		if (ship->startRow > 0)
		{
			field[ship->startCol - 1][ship->startRow - 1].shape.setFillColor(Color(0, 0, 255, 200));
			field[ship->startCol - 1][ship->startRow - 1].state = CellState::MISS;
		}
		if (ship->startRow + ship->maxHealth < 10)
		{
			field[ship->startCol - 1][ship->startRow + ship->maxHealth].shape.setFillColor(Color(0, 0, 255, 200));
			field[ship->startCol - 1][ship->startRow + ship->maxHealth].state = CellState::MISS;
		}
	}

	if (ship->startCol < 9)
	{
		for (int i = ship->startRow; i < ship->startRow + ship->maxHealth; ++i)
		{
			field[ship->startCol + 1][i].shape.setFillColor(Color(0, 0, 255, 200));
			field[ship->startCol + 1][i].state = CellState::MISS;
		}
		if (ship->startRow > 0)
		{
			field[ship->startCol + 1][ship->startRow - 1].shape.setFillColor(Color(0, 0, 255, 200));
			field[ship->startCol + 1][ship->startRow - 1].state = CellState::MISS;
		}
		if (ship->startRow + ship->maxHealth < 10)
		{
			field[ship->startCol + 1][ship->startRow + ship->maxHealth].shape.setFillColor(Color(0, 0, 255, 200));
			field[ship->startCol + 1][ship->startRow + ship->maxHealth].state = CellState::MISS;
		}
	}

	if (ship->startRow > 0)
	{
		field[ship->startCol][ship->startRow - 1].shape.setFillColor(Color(0, 0, 255, 200));
		field[ship->startCol][ship->startRow - 1].state = CellState::MISS;
	}
	if (ship->startRow + ship->maxHealth < 10)
	{
		field[ship->startCol][ship->startRow + ship->maxHealth].shape.setFillColor(Color(0, 0, 255, 200));
		field[ship->startCol][ship->startRow + ship->maxHealth].state = CellState::MISS;
	}
}