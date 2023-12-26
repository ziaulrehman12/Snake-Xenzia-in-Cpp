// SNAKE XENZIA IN C++ 
#include <iostream>
#include <list>
#include <chrono>
#include <Windows.h>
using namespace std;

   //defining screen size
   int ScreenWidth = 120;
   int ScreenHeight = 30;

   //snake body
   struct SnakeSegment
   {
	int x;
	int y;
   };

int main() 
{
	//creating screen buffer
	wchar_t* screen = new wchar_t[ScreenWidth * ScreenHeight];
	for (int i = 0; i < ScreenWidth * ScreenHeight; i++) screen[i] = L' ';
	HANDLE Console = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(Console);
	DWORD dwBytesWritten = 0;

	bool GameStarted = false;

	while (1) 
	{

		//checking if user wants to start the game
		if ((0x8000 & GetAsyncKeyState((VK_SPACE))) != 0) {
			GameStarted = true;

		}

		if (GameStarted) 
		{
			//game variables
			std::list<SnakeSegment> snake = { {60,15},{61,15},{62,15},{63,15},
				{64,15},{65,15},{66,15},{67,15},{68,15},{69,15}};
			int FoodX = 30;
			int FoodY = 15;
			int Score = 0;
			int SnakeDirection = 3;
			bool Dead = false;
			bool KeyLeft = false, KeyRight = false, KeyLeftOld = false, KeyRightOld = false;
			bool KeyUp = false, KeyDown = false, KeyUpOld = false, KeyDownOld = false;

			while (!Dead) 
			{
				//slowing down the snake for y axis
				auto t1 = chrono::system_clock::now();
				while ((chrono::system_clock::now() - t1) < ((SnakeDirection % 2 == 1) ? 120ms : 200ms)) 
				{
					//moving the snake with arrow keys
					KeyRight = (0x8000 & GetAsyncKeyState(VK_RIGHT)) != 0;
					KeyLeft = (0x8000 & GetAsyncKeyState(VK_LEFT)) != 0;
					KeyUp = (0x8000 & GetAsyncKeyState(VK_UP)) != 0;
					KeyDown = (0x8000 & GetAsyncKeyState(VK_DOWN)) != 0;

					if (KeyRight && !KeyRightOld && SnakeDirection != 3) {
						SnakeDirection = 1;
					}

					if (KeyLeft && !KeyLeftOld && SnakeDirection != 1) {
						SnakeDirection = 3;
					}

					if (KeyUp && !KeyUpOld && SnakeDirection != 2) {
						SnakeDirection = 0;
					}

					if (KeyDown && !KeyDownOld && SnakeDirection != 0) {
						SnakeDirection = 2;
					}

					KeyRightOld = KeyRight;
					KeyLeftOld = KeyLeft;
					KeyUpOld = KeyUp;
					KeyDownOld = KeyDown;

				}

				switch (SnakeDirection) //moving the snake
				{
				case 0:
					snake.push_front({ snake.front().x, snake.front().y - 1 });
					break;
				case 1:
					snake.push_front({ snake.front().x + 1, snake.front().y });
					break;
				case 2:
					snake.push_front({ snake.front().x, snake.front().y + 1 });
					break;
				case 3:
					snake.push_front({ snake.front().x - 1, snake.front().y });
					break;
				}

				//collision with walls
				if (snake.front().x <= 0 || snake.front().x >= ScreenWidth)
					Dead = true;
				if (snake.front().y < 3 || snake.front().y >= ScreenHeight)
					Dead = true;

				//collision with food
				if (snake.front().x == FoodX && snake.front().y == FoodY) {
					Score++;
					do {
						FoodX = rand() % ScreenWidth;
						FoodY = (rand() % (ScreenHeight - 3)) + 3;
					} while (screen[FoodY * ScreenWidth + FoodX] != L' ' || FoodY >= ScreenHeight - 3 || FoodY < 3 || FoodX >= ScreenWidth || FoodX < 0);

					for (int i = 0; i < 5; i++)
						snake.push_back({ snake.back().x, snake.back().y });
				}

				//collision with snake
				for (list<SnakeSegment>::iterator i = snake.begin(); i != snake.end(); i++)
					if (i != snake.begin() && i->x == snake.front().x && i->y == snake.front().y)
						Dead = true;

				//remove snake tail
				snake.pop_back();

				//display
				for (int i = 0; i < ScreenWidth * ScreenHeight; i++) screen[i] = L' ';

				//displaying the top decoration
				for (int i = 0; i < ScreenWidth; i++) {
					screen[i] = L'=';
					if (2 * ScreenWidth + i < ScreenWidth * ScreenHeight) {
						screen[2 * ScreenWidth + i] = L'=';
					}
				}
				//print score & project name
				wsprintf(&screen[ScreenWidth + 5], L"      Computer Programming Project - SNAKE XENZIA !!!!                    SCORE: %d", Score);

				//update snake body after collision
				for (auto s : snake)
					screen[s.y * ScreenWidth + s.x] = Dead ? L'+' : L'O';

				screen[snake.front().y * ScreenWidth + snake.front().x] = Dead ? L'X' : L'@';

				//generate new food on the screen
				if (FoodY * ScreenWidth + FoodX < ScreenWidth * ScreenHeight) {
					screen[FoodY * ScreenWidth + FoodX] = L'&';
				}

				//displaying message to restart the game
				if (Dead)
					wsprintf(&screen[15 * ScreenWidth + 40], L"    Press Space to Play Again       ");

				
				//writing to console
				WriteConsoleOutputCharacter(Console, screen, ScreenWidth * ScreenHeight, { 0,0 }, &dwBytesWritten);
			}

			//check if user wants to restart the game
			while ((GetAsyncKeyState(VK_SPACE) & 0x8000) == 0);
			GameStarted = false; // Reset GameStarted flag for restarting the game
		}
		else 
		{
			// Display a message to prompt the user to press Space key to start the game
			for (int i = 0; i < ScreenWidth * ScreenHeight; i++) screen[i] = L' ';
			wsprintf(&screen[15 * ScreenWidth + 40], L"    Press Space to Play Snake Xenzia        ");
			WriteConsoleOutputCharacter(Console, screen, ScreenWidth * ScreenHeight, { 0,0 }, &dwBytesWritten);
		}

	}
	return 0;
}
