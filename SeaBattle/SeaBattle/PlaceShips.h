#pragma once

#include "Game.h"

void RandomlyPlaceShips(Game & game, Field & field);

void RandomlyPlaceFourDecked(Game & game, Field & field);

void RandomlyPlaceThreeDecked(Game & game, Field & field);

void RandomlyPlaceDoubleDecked(Game & game, Field & field);

void RandomlyPlaceSingleDecked(Game & game, Field & field);

bool CheckPlace(int col, int row, Field & field);

void ShowPlayerShips(Game & game);