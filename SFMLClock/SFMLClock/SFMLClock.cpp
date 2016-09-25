////////////////////////////////////////////////////////////
// Headers:
// ctime for getting system time and
// cmath for sin and cos functions
////////////////////////////////////////////////////////////
#include "stdafx.h"

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <ctime>
#include <cmath>


////////////////////////////////////////////////////////////
/// Entry point of application
///
/// \return Application exit code
///
////////////////////////////////////////////////////////////
int main()
{
	// Define some variables and constants
	static const int SCREEN_WIDTH = 800;
	static const int SCREEN_HEIGHT = 600;
	static const float PI = 3.1415927f;
	static const float CLOCK_CIRCLE_SIZE = 250;
	static const float CLOCK_CIRCLE_THICKNESS = 2;
	float x, y;
	float angle = 0.0;

	// Set multisampling level
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	// Create the window of the application
	sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "SFML Analog Clock", sf::Style::Close, settings);

	// Define windowCenter which gets the center of the window here, right after creating window
	sf::Vector2f windowCenter = sf::Vector2f(window.getSize().x / 2.0f, window.getSize().y / 2.0f);

	// Create a list for clock's dots
	sf::CircleShape dot[60];

	// Create dots and place them to very right positions
	for (int i = 0; i < 60; i++)
	{
		x = (CLOCK_CIRCLE_SIZE - 10) * cos(angle);
		y = (CLOCK_CIRCLE_SIZE - 10) * sin(angle);

		if (i % 5 == 0)
			dot[i] = sf::CircleShape(3);
		else
			dot[i] = sf::CircleShape(1);
		dot[i].setFillColor(sf::Color::Black);
		dot[i].setOrigin(dot[i].getGlobalBounds().width / 2, dot[i].getGlobalBounds().height / 2);
		dot[i].setPosition(x + window.getSize().x / 2, y + window.getSize().y / 2);

		angle = angle + ((2 * PI) / 60);
	}

	// Create outline of the clock
	sf::CircleShape clockCircle(CLOCK_CIRCLE_SIZE);

	clockCircle.setPointCount(100);
	clockCircle.setOutlineThickness(CLOCK_CIRCLE_THICKNESS);
	clockCircle.setOutlineColor(sf::Color::Black);
	clockCircle.setOrigin(clockCircle.getGlobalBounds().width / 2, clockCircle.getGlobalBounds().height / 2);
	clockCircle.setPosition(window.getSize().x / 2 + CLOCK_CIRCLE_THICKNESS, window.getSize().y / 2 + CLOCK_CIRCLE_THICKNESS);

	// Crate another circle for center
	sf::CircleShape centerCircle(10);

	centerCircle.setPointCount(100);
	centerCircle.setFillColor(sf::Color::Red);
	centerCircle.setOrigin(centerCircle.getGlobalBounds().width / 2, centerCircle.getGlobalBounds().height / 2);
	centerCircle.setPosition(windowCenter);

	// Create hour, minute, and seconds hands
	sf::RectangleShape hourHand(sf::Vector2f(5, 180));
	sf::RectangleShape minuteHand(sf::Vector2f(3, 240));
	sf::RectangleShape secondsHand(sf::Vector2f(2, 250));

	hourHand.setFillColor(sf::Color::Black);
	minuteHand.setFillColor(sf::Color::Black);
	secondsHand.setFillColor(sf::Color::Red);

	hourHand.setOrigin(hourHand.getGlobalBounds().width / 2, hourHand.getGlobalBounds().height - 25);
	minuteHand.setOrigin(minuteHand.getGlobalBounds().width / 2, minuteHand.getGlobalBounds().height - 25);
	secondsHand.setOrigin(secondsHand.getGlobalBounds().width / 2, secondsHand.getGlobalBounds().height - 25);

	hourHand.setPosition(windowCenter);
	minuteHand.setPosition(windowCenter);
	secondsHand.setPosition(windowCenter);

	// Create sound effect
	sf::Music clockTick;
	if (!clockTick.openFromFile("resources/clock-1.wav"))
		return EXIT_FAILURE;
	clockTick.setLoop(true);
	clockTick.play();

	// Use a part of SFML logo as clock brand
	sf::Texture clockBrand;
	if (!clockBrand.loadFromFile("resources/clock-brand.png"))
	{
		return EXIT_FAILURE;
	}

	sf::Sprite clockBrandSprite;
	clockBrandSprite.setTexture(clockBrand);
	clockBrandSprite.setOrigin(clockBrandSprite.getTextureRect().left + clockBrandSprite.getTextureRect().width / 2.0f,
		clockBrandSprite.getTextureRect().top + clockBrandSprite.getTextureRect().height / 2.0f);

	clockBrandSprite.setPosition(window.getSize().x / 2.0f, window.getSize().y - 100.0f);


	// Create clock background
	sf::Texture clockImage;
	if (!clockImage.loadFromFile("resources/clock-image.png"))
	{
		return EXIT_FAILURE;
	}

	clockCircle.setTexture(&clockImage);
	clockCircle.setTextureRect(sf::IntRect(40, 0, 500, 500));

	while (window.isOpen())
	{
		// Handle events
		sf::Event event;
		while (window.pollEvent(event))
		{
			// Window closed: exit
			if (event.type == sf::Event::Closed)
				window.close();
		}

		// Get system time
		std::time_t currentTime = std::time(NULL);

		struct tm * ptm = new tm;
		localtime_s(ptm, &currentTime);

		hourHand.setRotation(ptm->tm_hour * 30.0f + (ptm->tm_min / 2));
		minuteHand.setRotation(ptm->tm_min * 6.0f + (ptm->tm_sec / 10));
		secondsHand.setRotation(ptm->tm_sec * 6.0f);

		// Clear the window
		window.clear(sf::Color::White);

		// Draw all parts of clock
		window.draw(clockCircle);

		for (int i = 0; i<60; i++)
		{
			window.draw(dot[i]);
		}

		window.draw(clockBrandSprite);
		window.draw(hourHand);
		window.draw(minuteHand);
		window.draw(secondsHand);
		window.draw(centerCircle);

		// Display things on screen
		window.display();
	}

	return EXIT_SUCCESS;
}