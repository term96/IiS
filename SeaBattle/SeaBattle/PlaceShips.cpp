#include "Game.h"
#include "PlaceShips.h"
#include <ctime>

void RandomlyPlaceFourDecked(Game & game, Field & field)
{
	int col;
	int row;
	Cell *cell;

	col = rand() % 10;
	do
		row = rand() % 10;
	while (row > 6);

	for (int i = row; i < row + 4; ++i)
	{
		if (!CheckPlace(col, i, field))
		{
			RandomlyPlaceFourDecked(game, field);
			return;
		}
	}

	Ship *ship = new Ship;
	ship->health = 4;
	ship->maxHealth = 4;
	ship->startCol = col;
	ship->startRow = row;

	for (int i = row; i < row + 4; ++i)
	{
		cell = &field[col][i];
		cell->state = CellState::SHIP;
		cell->ship = ship;
		ship->cells.push_back(cell);
	}
}

void RandomlyPlaceThreeDecked(Game & game, Field & field)
{
	int col;
	int row;
	Cell *cell;

	col = rand() % 10;
	do
		row = rand() % 10;
	while (row > 7);

	for (int i = row; i < row + 3; ++i)
	{
		if (!CheckPlace(col, i, field))
		{
			RandomlyPlaceThreeDecked(game, field);
			return;
		}
	}

	Ship *ship = new Ship;
	ship->health = 3;
	ship->maxHealth = 3;
	ship->startCol = col;
	ship->startRow = row;

	for (int i = row; i < row + 3; ++i)
	{
		cell = &field[col][i];
		cell->state = CellState::SHIP;
		cell->ship = ship;
		ship->cells.push_back(cell);
	}
}

void RandomlyPlaceDoubleDecked(Game & game, Field & field)
{
	int col;
	int row;
	Cell *cell;

	col = rand() % 10;
	do
		row = rand() % 10;
	while (row > 8);

	for (int i = row; i < row + 2; ++i)
	{
		if (!CheckPlace(col, i, field))
		{
			RandomlyPlaceDoubleDecked(game, field);
			return;
		}
	}

	Ship *ship = new Ship;
	ship->health = 2;
	ship->maxHealth = 2;
	ship->startCol = col;
	ship->startRow = row;

	for (int i = row; i < row + 2; ++i)
	{
		cell = &field[col][i];
		cell->state = CellState::SHIP;
		cell->ship = ship;
		ship->cells.push_back(cell);
	}
}

void RandomlyPlaceSingleDecked(Game & game, Field & field)
{
	int col;
	int row;
	Cell *cell;

	col = rand() % 10;
	row = rand() % 10;
	

	if (!CheckPlace(col, row, field))
	{
		RandomlyPlaceSingleDecked(game, field);
		return;
	}

	Ship *ship = new Ship;
	ship->health = 1;
	ship->maxHealth = 1;
	ship->startCol = col;
	ship->startRow = row;

	cell = &field[col][row];
	cell->state = CellState::SHIP;
	cell->ship = ship;
	ship->cells.push_back(cell);
}

void RandomlyPlaceShips(Game & game, Field & field)
{
	RandomlyPlaceSingleDecked(game, field);
	RandomlyPlaceSingleDecked(game, field);
	RandomlyPlaceSingleDecked(game, field);
	RandomlyPlaceSingleDecked(game, field);

	RandomlyPlaceDoubleDecked(game, field);
	RandomlyPlaceDoubleDecked(game, field);
	RandomlyPlaceDoubleDecked(game, field);

	RandomlyPlaceThreeDecked(game, field);
	RandomlyPlaceThreeDecked(game, field);

	RandomlyPlaceFourDecked(game, field);
}

bool CheckPlace(int col, int row, Field & field)
{
	Cell *cell = &field[col][row];
	if (cell->state == CellState::SHIP)
		return false;

	if (col > 0)
	{
		cell = &field[col - 1][row];
		if (cell->state == CellState::SHIP)
			return false;
	}

	if (col < 9)
	{
		cell = &field[col + 1][row];
		if (cell->state == CellState::SHIP)
			return false;
	}

	if (row > 0)
	{
		cell = &field[col][row - 1];
		if (cell->state == CellState::SHIP)
			return false;
	}

	if (row < 9)
	{
		cell = &field[col][row + 1];
		if (cell->state == CellState::SHIP)
			return false;
	}

	if ((col > 0) && (row > 0))
	{
		cell = &field[col - 1][row - 1];
		if (cell->state == CellState::SHIP)
		{
			return false;
		}
	}

	if (col > 0 && row < 9)
	{
		cell = &field[col - 1][row + 1];
		if (cell->state == CellState::SHIP)
			return false;
	}

	if (col < 9 && row > 0)
	{
		cell = &field[col + 1][row - 1];
		if (cell->state == CellState::SHIP)
			return false;
	}

	if (col < 9 && row < 9)
	{
		cell = &field[col + 1][row + 1];
		if (cell->state == CellState::SHIP)
			return false;
	}

	return true;
}

void ShowPlayerShips(Game & game)
{
	for (int i = 0; i < 10; ++i)
	{
		for (int j = 0; j < 10; ++j)
		{
			Cell *cell = &game.playerField[i][j];
			if (cell->state == CellState::SHIP)
				cell->shape.setFillColor(sf::Color(0, 0, 0, 200));
		}
	}
}