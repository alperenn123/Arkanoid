#include <SFML/Graphics.hpp>
#include <stdint.h>
#include <time.h>
#include <sstream>
#define uint8 uint8_t
#define int8  int8_t 
#define int64 int64_t
#define SCREEN_WIDTH 672
#define SCREEN_HEIGHT 450

typedef enum
{
	Running = 0,
	Gameover
}GameState;
int main()
{
	srand(time(0));
	GameState eGameState = Running;
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Arkanoid V0.1");
	window.setFramerateLimit(60);
	int64 score = 0;
	std::ostringstream oScore_Stream;
	sf::Font fScoreFont;
	sf::Texture tBackground;
	sf::Texture tPaddle;
	sf::Texture tBall;
	sf::Texture tGreen;
	sf::Texture tRed;
	sf::Texture tGrey;
	sf::Texture tNavyBlue;
	sf::Texture tTurquoise;
	
	tBackground.loadFromFile("..\\..\\..\\textures\\background.jpg");
	tPaddle.loadFromFile("..\\..\\..\\textures\\paddle.png");
	tBall.loadFromFile("..\\..\\..\\textures\\ball.png");
	tGreen.loadFromFile("..\\..\\..\\textures\\block01.png");
	tRed.loadFromFile("..\\..\\..\\textures\\block02.png");
	tTurquoise.loadFromFile("..\\..\\..\\textures\\block03.png");
	tGrey.loadFromFile("..\\..\\..\\textures\\block04.png");
	tNavyBlue.loadFromFile("..\\..\\..\\textures\\block05.png");
	fScoreFont.loadFromFile("..\\..\\..\\fonts\\OpenSans-Regular.ttf");
	
	sf::Sprite sBall(tBall), sPaddle(tPaddle), sBackground(tBackground);
	sPaddle.setPosition(300, 440);
	sBall.setPosition(250, 250);
	sf::Text sScore_Display;
	sScore_Display.setFont(fScoreFont);
	sScore_Display.setCharacterSize(15);
	sScore_Display.setColor(sf::Color(0, 0, 0));
	sScore_Display.setPosition(0, 0);
	sf::Sprite sBlocks[80];

	for (int y = 0; y < 5; ++y)
	{
		for (int x = 0; x < 16; ++x)
		{
			switch (y)
			{
				case 0: { sBlocks[y * 16 + x].setTexture(tGreen); sBlocks[y * 16 + x].setPosition(x * 42 , y+1 * 20);  break; }
				case 1: { sBlocks[y * 16 + x].setTexture(tRed); sBlocks[y * 16 + x].setPosition(x * 42, y+2 * 20); break; }
				case 2: { sBlocks[y * 16 + x].setTexture(tTurquoise); sBlocks[y * 16 + x].setPosition(x * 42, y+3 * 20); break; }
				case 3: { sBlocks[y * 16 + x].setTexture(tGrey); sBlocks[y * 16 + x].setPosition(x * 42, y+4 * 20); break; }
				case 4: { sBlocks[y * 16 + x].setTexture(tNavyBlue); sBlocks[y * 16 + x].setPosition(x * 42, y+5 * 20); break; }
			}
			
		}
	}

	float dirX = 0.2f;
	float dirY = 0.2f;
	float x = 250.0f;
	float y = 250.0f;
	sf::Clock clock;
	float gameover = false;

	
	while (window.isOpen())
    {
		switch (eGameState)
		{
			case Running:

		}

		
		sf::Event event;
		sf::Int32 dTime = clock.restart().asMilliseconds();
		sf::Vector2f ball_pos = sBall.getPosition();
		if (ball_pos.y < 0)
			gameover = true;
		while (window.pollEvent(event))
		{
			if(event.type == sf::Event::Closed)
			window.close();
		}

		
		if (ball_pos.x < 0 || ball_pos.x > SCREEN_WIDTH)  dirX = -dirX;
		if (ball_pos.y < 0) dirY = -dirY;
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) sPaddle.move(6, 0);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) sPaddle.move(-6, 0);
		if (sf::FloatRect(x + 1, y + 1, 12, 12).intersects(sPaddle.getGlobalBounds()))
		{
			//float r =  0.3f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (0.5f - 0.3f)));
			dirY = -dirY;
		}


		x += dirX * dTime;
		for (int i = 0; i < 80; ++i)
		{
			if (sf::FloatRect(x + 1, y + 1, 12, 12).intersects(sBlocks[i].getGlobalBounds()))
			{
				sBlocks[i].setPosition(0, -100);
				dirX = -dirX;
				score += 10;
				
			}
		}
		y += dirY * dTime;
		
		for (int i = 0; i < 80; ++i)
		{
			if (sf::FloatRect(x + 1, y + 1, 12, 12).intersects(sBlocks[i].getGlobalBounds()))
			{
				sBlocks[i].setPosition(0, -100);
				dirY = -dirY;
				score += 10;
			}
		}

		if (y >= SCREEN_HEIGHT)
			gameover = true;
		sBall.setPosition(x, y);
		dirX += 0.0005f;
		dirY += 0.0005f;
		window.clear();
		window.draw(sBackground);
		window.draw(sPaddle);
		window.draw(sBall);
		
		for (int i = 0; i < 80; ++i)
		{
			window.draw(sBlocks[i]);
		}
		oScore_Stream << score;
		sScore_Display.setString("Score = " + oScore_Stream.str());
		oScore_Stream.str("");
		window.draw(sScore_Display);
		window.display();
    }
    return 0;
}
