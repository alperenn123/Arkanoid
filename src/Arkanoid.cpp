#include <SFML/Graphics.hpp>
#include <stdint.h>
#include <time.h>
#include <sstream>
#include <iostream>
#define uint8 uint8_t
#define int8  int8_t 
#define int64 int64_t
#define SCREEN_WIDTH 672
#define SCREEN_HEIGHT 450

typedef enum
{
	Running = 0,
	GameOver,
	Exit
}GameState;
typedef struct 
{
	GameState eGameState;
	void Change_State(GameState state)
	{
		eGameState = state;
	}
	GameState Get_State()
	{
		return this->eGameState;
	}
}StateMachine;
 

 
int main()
{
	srand(time(0));
	StateMachine stateMachine;
	stateMachine.eGameState = Running;
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
	sf::Text sGameover_Text;
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

	
	while (window.isOpen())
    {
		
		switch (stateMachine.Get_State())
		{
			case Running:
			{
				window.setKeyRepeatEnabled(true);
					
				
				sf::Event event;
				
				sf::Int32 dTime = clock.restart().asMilliseconds();
				sf::Vector2f ball_pos = sBall.getPosition();
				while (window.pollEvent(event))
				{
					
					if (event.type == sf::Event::Closed)
						stateMachine.Change_State(Exit);
				}

				if (ball_pos.y > SCREEN_HEIGHT)
					stateMachine.Change_State(GameOver);
				else
				{
					if (ball_pos.x < 0 || ball_pos.x > SCREEN_WIDTH)  dirX = -dirX;
					if (ball_pos.y < 0) dirY = -dirY;

					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) sPaddle.move(6, 0);
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) sPaddle.move(-6, 0);
					if (sf::FloatRect(x + 1, y + 1, 12, 12).intersects(sPaddle.getGlobalBounds()))
					{

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

					sBall.setPosition(x, y);
					dirX += 0.0005f;
					dirY += 0.0005f;
					window.clear(sf::Color::Black);
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
				
				
			}break;
			case GameOver:
			{
				window.setKeyRepeatEnabled(false);
				sf::Event event;
				while (window.pollEvent(event))
				{
					switch (event.type)
					{
					case sf::Event::Closed: {stateMachine.Change_State(Exit); }break;
						case sf::Event::KeyPressed: 
						{
							if (event.key.code == sf::Keyboard::Enter)
							{
								stateMachine.Change_State(Running);
								sBall.setPosition(150, 150);
								dirX = 0.2f;
								dirY = 0.2f;
								 x = 150.0f;
								 y = 150.0f;
							}
							
						}break;
					}
				}
				sGameover_Text.setFont(fScoreFont);
				sGameover_Text.setCharacterSize(30);
				sGameover_Text.setColor(sf::Color(0, 0, 0));
				sGameover_Text.setPosition((SCREEN_HEIGHT-150)/2, (SCREEN_WIDTH-150)/2);
				sGameover_Text.setString("GAMEOVER!Press Enter to restart...");
				window.draw(sGameover_Text);
				window.display();

				
			}break;

			case Exit:
			{
				window.close(); 
			}break;
		
		}
		


	}

		
		
    return 0;
}
