#include <SFML/Graphics.hpp>
#include <stdint.h>
#include <time.h>
#include <sstream>
#include <iostream>
#include <chrono>
#include <thread>
#define uint8 uint8_t
#define int8  int8_t 
#define int64 int64_t
#define SCREEN_WIDTH 672
#define SCREEN_HEIGHT 450

typedef enum
{
	Init = 0,
	Running,
	GameOver,
	Exit
}GameState;
typedef struct 
{
	GameState eGameState;
	void ChangeState(GameState state)
	{
		eGameState = state;
	}
	GameState GetState()
	{
		return this->eGameState;
	}
}StateMachine;
 
static void InitializeBlocks
(
	sf::Sprite* blocks,
	const sf::Texture* const t1,
	const sf::Texture* const t2,
	const sf::Texture* const t3,
	const sf::Texture* const t4,
	const sf::Texture* const t5
);
static void InitializeBallPosition(sf::Sprite* ball);
static void InitializePaddlePosition(sf::Sprite* paddle);
int main()
{
	srand(time(0));
	StateMachine stateMachine;
	stateMachine.eGameState = Init;
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
	
	sf::Text sScore_Display;
	sf::Text sGameover_Text;
	sScore_Display.setFont(fScoreFont);
	sScore_Display.setCharacterSize(15);
	sScore_Display.setColor(sf::Color(0, 0, 0));
	sScore_Display.setPosition(0, 0);
	sf::Sprite sBlocks[80];


	float dirX = 0.2f;
	float dirY = 0.2f;
	float x = 250.0f;
	float y = 250.0f;
	sf::Clock clock;

	
	while (window.isOpen())
    {
		
		switch (stateMachine.GetState())
		{
			case Init:
			{
				InitializeBlocks
				(
					&sBlocks[0],
					&tGreen,
					&tRed,
					&tTurquoise,
					&tGrey,
					&tNavyBlue
				);
				InitializeBallPosition(&sBall);
				InitializePaddlePosition(&sPaddle);
				dirX = 0.2f;
				dirY = 0.2f;
				x = 150.0f;
				y = 150.0f;
				score = 0;
				stateMachine.ChangeState(Running);
				
			}break;
			case Running:
			{
				window.setKeyRepeatEnabled(true);
				sf::Int32 dTime = clock.restart().asMilliseconds();
				sf::Vector2f ball_pos = sBall.getPosition();
				sf::Event event;
				while (window.pollEvent(event))
				{
					
					if (event.type == sf::Event::Closed)
						stateMachine.ChangeState(Exit);
				}
				if (ball_pos.y > SCREEN_HEIGHT)
				{
					stateMachine.ChangeState(GameOver);
				}	
				else
				{
					if (ball_pos.x < 0 || ball_pos.x > SCREEN_WIDTH)  dirX = -dirX;
					if (ball_pos.y < 0) dirY = -dirY;
					

					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) sPaddle.move(5, 0);
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) sPaddle.move(-5, 0);
					sf::Vector2f paddle_position = sPaddle.getPosition();
					if (paddle_position.x + 90  > SCREEN_WIDTH)
					{
						sPaddle.move(-5, 0);
					}
					if (paddle_position.x < 0)
					{
						sPaddle.move(5, 0);
					}
					if (sf::FloatRect(x, y, 12, 12).intersects(sPaddle.getGlobalBounds()))
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
					 case sf::Event::Closed: {stateMachine.ChangeState(Exit); }break;
			
					}
				}
				sGameover_Text.setFont(fScoreFont);
				sGameover_Text.setCharacterSize(30);
				sGameover_Text.setColor(sf::Color(0, 0, 0));
				sGameover_Text.setPosition((SCREEN_HEIGHT-150)/2, (SCREEN_WIDTH-150)/2);
				sGameover_Text.setString("GAMEOVER!!Restarting game");
				window.draw(sGameover_Text);
				window.display();
				std::this_thread::sleep_for(std::chrono::seconds(1));
				stateMachine.ChangeState(Init);
				
			}break;

			case Exit:
			{
				window.close(); 
			}break;
		}
		


	}

		
		
    return 0;
}
static void InitializeBlocks
(
	sf::Sprite* blocks,
	const sf::Texture* const t1,
	const sf::Texture* const t2,
	const sf::Texture* const t3,
	const sf::Texture* const t4,
	const sf::Texture* const t5
)
{
	for (int y = 0; y < 5; ++y)
	{
		for (int x = 0; x < 16; ++x)
		{
			switch (y)
			{
			case 0: { blocks[y * 16 + x].setTexture(*t1); blocks[y * 16 + x].setPosition(x * 42, y + 1 * 20);  break; }
			case 1: { blocks[y * 16 + x].setTexture(*t2); blocks[y * 16 + x].setPosition(x * 42, y + 2 * 20); break; }
			case 2: { blocks[y * 16 + x].setTexture(*t3); blocks[y * 16 + x].setPosition(x * 42, y + 3 * 20); break; }
			case 3: { blocks[y * 16 + x].setTexture(*t4); blocks[y * 16 + x].setPosition(x * 42, y + 4 * 20); break; }
			case 4: { blocks[y * 16 + x].setTexture(*t5); blocks[y * 16 + x].setPosition(x * 42, y + 5 * 20); break; }
			}

		}
	}
}
static void InitializeBallPosition(sf::Sprite* ball)
{
	ball->setPosition(250, 250);
}
static void InitializePaddlePosition(sf::Sprite* paddle)
{
	paddle->setPosition(300, 440);
}