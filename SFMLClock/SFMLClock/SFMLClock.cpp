////////////////////////////////////////////////////////////
// Headers:
// ctime for getting system time and
// cmath for sin and cos functions
////////////////////////////////////////////////////////////
#include "stdafx.h"

#define _USE_MATH_DEFINES

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <ctime>
#include <cmath>
#include <string>

const float CLOCK_CIRCLE_SIZE = 250;
const float CLOCK_CIRCLE_THICKNESS = 2;
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

struct ClockHands
{
	sf::RectangleShape hourHand;
	sf::RectangleShape minuteHand;
	sf::RectangleShape secondsHand;
};

struct Clock
{
	sf::CircleShape centerCircle;
	sf::CircleShape clockCircle; 
	sf::CircleShape dots[60];
};

int CreateDigits(sf::RenderWindow & window) {
	sf::Font font;
	if (!font.loadFromFile("resources/calibri.ttf"))
	{
		return EXIT_FAILURE;
	}

	float x, y;
	double angle = -(2 * M_PI) / 6;

	for (int i = 1; i < 13; i++)
	{
		x = static_cast<float>((CLOCK_CIRCLE_SIZE - 40) * cos(angle) - 2);
		y = static_cast<float>((CLOCK_CIRCLE_SIZE - 40) * sin(angle) - 12);

		sf::Text text;
		text.setFont(font);
		text.setString(std::to_string(i));
		text.setCharacterSize(36);
		text.setFillColor(sf::Color::Black);

		text.setOrigin(text.getGlobalBounds().width / 2, text.getGlobalBounds().height / 2);
		text.setPosition(x + window.getSize().x / 2, y + window.getSize().y / 2);

		angle = angle + ((2 * M_PI) / 12);

		window.draw(text);
	}
	
	return EXIT_SUCCESS;
}

// Create dots and place them to very right positions
void CreateDots(Clock &clock, sf::RenderWindow const& window)
{
	float x, y;
	double angle = 0.0;

	for (int i = 0; i < 60; i++)
	{
		x = static_cast<float>((CLOCK_CIRCLE_SIZE - 10) * cos(angle));
		y = static_cast<float>((CLOCK_CIRCLE_SIZE - 10) * sin(angle));

		if (i % 5 == 0)
			clock.dots[i] = sf::CircleShape(5);
		else
			clock.dots[i] = sf::CircleShape(2);
		clock.dots[i].setFillColor(sf::Color::Black);
		clock.dots[i].setOrigin(clock.dots[i].getGlobalBounds().width / 2, clock.dots[i].getGlobalBounds().height / 2);
		clock.dots[i].setPosition(x + window.getSize().x / 2, y + window.getSize().y / 2);

		angle = angle + ((2 * M_PI) / 60);
	}
}

void InitClockCircle(Clock &clock, sf::RenderWindow const& window)
{
	clock.clockCircle.setPointCount(100);
	clock.clockCircle.setOutlineThickness(CLOCK_CIRCLE_THICKNESS);
	clock.clockCircle.setOutlineColor(sf::Color::Black);
	clock.clockCircle.setOrigin(clock.clockCircle.getGlobalBounds().width / 2, clock.clockCircle.getGlobalBounds().height / 2);
	clock.clockCircle.setPosition(window.getSize().x / 2 + CLOCK_CIRCLE_THICKNESS, window.getSize().y / 2 + CLOCK_CIRCLE_THICKNESS);
}

void InitCenterCircle(Clock &clock, sf::Vector2f const& windowCenter)
{
	clock.centerCircle.setPointCount(100);
	clock.centerCircle.setFillColor(sf::Color::Black);
	clock.centerCircle.setOrigin(clock.centerCircle.getGlobalBounds().width / 2, clock.centerCircle.getGlobalBounds().height / 2);
	clock.centerCircle.setPosition(windowCenter);
}

void InitHands(ClockHands &clockHands, sf::Vector2f const& windowCenter)
{
	clockHands.hourHand.setFillColor(sf::Color::Black);
	clockHands.minuteHand.setFillColor(sf::Color::Black);
	clockHands.secondsHand.setFillColor(sf::Color::Red);

	clockHands.hourHand.setOrigin(clockHands.hourHand.getGlobalBounds().width / 2, clockHands.hourHand.getGlobalBounds().height - 25);
	clockHands.minuteHand.setOrigin(clockHands.minuteHand.getGlobalBounds().width / 2, clockHands.minuteHand.getGlobalBounds().height - 25);
	clockHands.secondsHand.setOrigin(clockHands.secondsHand.getGlobalBounds().width / 2, clockHands.secondsHand.getGlobalBounds().height - 25);

	clockHands.hourHand.setPosition(windowCenter);
	clockHands.minuteHand.setPosition(windowCenter);
	clockHands.secondsHand.setPosition(windowCenter);
}

int CreateSoundEffect(sf::Music & clockTick)
{
	if (!clockTick.openFromFile("resources/clock-1.wav"))
	{
		return EXIT_FAILURE;
	}
	clockTick.setLoop(true);
	clockTick.play();
	return EXIT_SUCCESS;
}

int CreateClockBrand(sf::Texture & clockBrand, sf::Sprite & clockBrandSprite, sf::RenderWindow & window)
{
	if (!clockBrand.loadFromFile("resources/clock-brand.png"))
	{
		return EXIT_FAILURE;
	}

	clockBrandSprite.setTexture(clockBrand);
	clockBrandSprite.setOrigin(clockBrandSprite.getTextureRect().left + clockBrandSprite.getTextureRect().width / 2.0f,
		clockBrandSprite.getTextureRect().top + clockBrandSprite.getTextureRect().height / 2.0f);

	clockBrandSprite.setPosition(window.getSize().x / 2.0f, window.getSize().y - 150.0f);
	return EXIT_SUCCESS;
}

int CreateClockBackground(sf::Texture & clockImage, Clock &clock)
{
	if (!clockImage.loadFromFile("resources/clock-image.jpg"))
	{
		return EXIT_FAILURE;
	}

	clock.clockCircle.setTexture(&clockImage);
	clock.clockCircle.setTextureRect(sf::IntRect(40, 0, 500, 500));
	return EXIT_SUCCESS;
}

void HandleEvents(sf::RenderWindow & window)
{
	sf::Event event;
	while (window.pollEvent(event))
	{
		// Window closed: exit
		if (event.type == sf::Event::Closed)
			window.close();
	}
}

void RotateHands(ClockHands &clockHands)
{
	// Get system time
	std::time_t currentTime = std::time(NULL);

	struct tm * ptm = new tm;
	localtime_s(ptm, &currentTime);

	clockHands.hourHand.setRotation(ptm->tm_hour * 30.0f + (ptm->tm_min / 2));
	clockHands.minuteHand.setRotation(ptm->tm_min * 6.0f + (ptm->tm_sec / 10));
	clockHands.secondsHand.setRotation(ptm->tm_sec * 6.0f);
}

void DrawClock(sf::RenderWindow & window, sf::Sprite & clockBrandSprite, ClockHands &clockHands, Clock &clock)
{
	// Clear the window
	window.clear(sf::Color::White);

	// Draw all parts of clock
	window.draw(clock.clockCircle);

	for (int i = 0; i<60; i++)
	{
		window.draw(clock.dots[i]);
	}

	window.draw(clockBrandSprite);
	window.draw(clockHands.hourHand);
	window.draw(clockHands.minuteHand);
	window.draw(clockHands.secondsHand);
	window.draw(clock.centerCircle);
}

int main()
{
	// Set multisampling level
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "SFML Analog Clock", sf::Style::Close, settings);
		
	// Define windowCenter which gets the center of the window here, right after creating window
	sf::Vector2f windowCenter = sf::Vector2f(window.getSize().x / 2.0f, window.getSize().y / 2.0f);
	
	Clock clock;
	clock.centerCircle = sf::CircleShape(10);
	clock.clockCircle = sf::CircleShape(CLOCK_CIRCLE_SIZE);

	// Create a list for clock's dots
	CreateDots(clock, window);

	// Create outline of the clock
	InitClockCircle(clock, window);
	
	// Crate another circle for center
	InitCenterCircle(clock, windowCenter);
	
	// Create hour, minute, and seconds hands
	ClockHands clockHands;
	clockHands.hourHand = sf::RectangleShape(sf::Vector2f(5, 180));
	clockHands.minuteHand = sf::RectangleShape(sf::Vector2f(3, 240));
	clockHands.secondsHand = sf::RectangleShape(sf::Vector2f(2, 250));
	InitHands(clockHands, windowCenter);

	sf::Music clockTick;
	if (CreateSoundEffect(clockTick) == EXIT_FAILURE)
	{
		return EXIT_FAILURE;
	}

	sf::Texture clockBrand;
	sf::Sprite clockBrandSprite;
	if (CreateClockBrand(clockBrand, clockBrandSprite, window) == EXIT_FAILURE)
	{
		return EXIT_FAILURE;
	}

	sf::Texture clockImage;
	if (CreateClockBackground(clockImage, clock) == EXIT_FAILURE)
	{
		return EXIT_FAILURE;
	}

	while (window.isOpen())
	{
		HandleEvents(window);

		RotateHands(clockHands);

		DrawClock(window, clockBrandSprite, clockHands, clock);

		CreateDigits(window);

		// Display things on screen
		window.display();
	}

	return EXIT_SUCCESS;
}