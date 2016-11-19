// Animation.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <iostream>

using namespace std;
using namespace sf;

const float RECT_SIZE_X = 40.f;
const float RECT_SIZE_Y = 30.f;
const int WINDOW_SIZE_X = 600;
const int WINDOW_SIZE_Y = 600;
const int FRAMERATE = 60;
const int RECTS_COUNT = 8;

enum Direction
{
	leftUp, rightDown
};

enum Side
{
	left, right, notSide
};

void CreateRects(RectangleShape (&rects)[RECTS_COUNT])
{
	for (int i = 0; i < RECTS_COUNT; ++i)
	{
		rects[i] = RectangleShape(Vector2f(RECT_SIZE_X, RECT_SIZE_Y));
		rects[i].setOrigin(rects[i].getGlobalBounds().width / 2, rects[i].getGlobalBounds().height / 2);
		rects[i].setFillColor(Color(255, 0, 0, 128));
		rects[i].setPosition(RECT_SIZE_X * i, RECT_SIZE_Y * i);
	}
}

void ScaleRects(RectangleShape (&rects)[RECTS_COUNT])
{
	// от минимального размера до максимального 2 секунды при FPS = 60
	static float delta = 0.5f / 60.f; 

	float scale = rects[0].getScale().x;
	scale += delta;

	for (int i = 0; i < RECTS_COUNT; ++i)
	{
		rects[i].setScale(scale, scale);
	}

	if (scale >= 1.5f || scale <= 0.5f)
	{
		delta = -delta;
	}
}

void ChangeColors(RectangleShape (&rects)[RECTS_COUNT])
{
	static const Color RED = Color(255, 0, 0, 128);
	static const Color GREEN = Color(0, 255, 0, 128);
	static const Color BLUE = Color(0, 0, 255, 128);
	static Color current = RED;

	if (current == RED)
		current = GREEN;
	else if (current == GREEN)
		current = BLUE;
	else if (current == BLUE)
		current = RED;

	for (int i = 0; i < RECTS_COUNT; ++i)
	{
		rects[i].setFillColor(current);
	}
}

Side GetSide(RectangleShape (&rects)[RECTS_COUNT])
{
	float leftBound = rects[0].getGlobalBounds().left;

	if (leftBound >= WINDOW_SIZE_X - RECT_SIZE_X)
		return Side::right;
	if (leftBound <= 0)
		return Side::left;

	return Side::notSide;
}

void ChangeDirection(Side side, Direction &direction)
{
	switch (side)
	{
	case Side::left:
		direction = Direction::rightDown;
		break;
	case Side::right:
		direction = Direction::leftUp;
	}
}

void MoveRects(RectangleShape (&rects)[RECTS_COUNT])
{
	// время перехода из стороны в сторону 2 секунды
	static float verticalDistance = WINDOW_SIZE_Y - RECT_SIZE_Y - rects[RECTS_COUNT - 1].getGlobalBounds().top;
	static float x = (WINDOW_SIZE_X - RECT_SIZE_X) / FRAMERATE / RECTS_COUNT / 2;
	static float y = verticalDistance / FRAMERATE / RECTS_COUNT / 2;
	static Direction direction = Direction::rightDown;

	for (int i = 0; i < RECTS_COUNT; ++i)
	{
		switch (direction)
		{
		case Direction::rightDown:
			rects[i].move(x * (RECTS_COUNT - i), y * RECTS_COUNT);
			break;
		case Direction::leftUp:
			rects[i].move(-x * (RECTS_COUNT - i), -y * RECTS_COUNT);
			break;
		}
	}

	Side side = GetSide(rects);
	ChangeDirection(side, direction);
}

void RotateRects(RectangleShape (&rects)[RECTS_COUNT])
{
	static const float ANGLE = 45.f / FRAMERATE;
	cout << rects[0].getRotation() << " ";
	for (int i = 0; i < RECTS_COUNT; ++i)
	{
		rects[i].rotate(ANGLE);
	}
}

void HandleEvents(RenderWindow &window)
{
	sf::Event event;
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			window.close();
	}
}

void DrawRects(RectangleShape (&rects)[RECTS_COUNT], RenderWindow &window)
{
	for (int i = 0; i < RECTS_COUNT; ++i)
	{
		window.draw(rects[i]);
	}
}

void StartAnimationLoop(RectangleShape (&rects)[RECTS_COUNT], RenderWindow &window) 
{
	Side side;
	while (window.isOpen())
	{
		HandleEvents(window);
		window.clear(Color::White);

		// синхронное изменение размера
		ScaleRects(rects); 
		
		// синхронное перемещение вверх/вниз + асинхронное перемещение влево/вправо
		MoveRects(rects);

		//RotateRects(rects);

		// синхронное изменение цвета при попадании в верхний левый угол
		side = GetSide(rects);
		if (side == Side::left)
			ChangeColors(rects);

		DrawRects(rects, window);
		window.display();
	}
}

int main()
{
	RectangleShape rects[RECTS_COUNT];
	CreateRects(rects);

	RenderWindow window(VideoMode(WINDOW_SIZE_X, WINDOW_SIZE_Y), "SFML Animation", Style::Close);
	window.setFramerateLimit(FRAMERATE);

	StartAnimationLoop(rects, window);
    return 0;
}

