#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>

int paused = 0;
char map[50][79];
int snake[7][2]; // coordinates of each part of snake
int score = 0;
int minusScore = 0;
int i = 11; // Starting position
int j = 11;
char point = 'O';
char head = 'X';
char tail = 219;
int headx = 1, heady = 8;
int foodX, foodY;
int start = 0;
int skipWelcome = 0;

void welcomeScreen();
void gameMap();
void generateFood();
void input();
void draw();
void logic();
void reset();

void main(void)
{
	// Do your initialization before the infinite loop starts.
	int fps, sleepTime;
	fps = 60;				// 30 to 60 fps is common for rendering graphics.
	sleepTime = 1000 / fps;	// To get 60 fps, we have to redraw graphics every 1000/60=17 miliseconds

	if (skipWelcome == 0) 
		welcomeScreen();
	gameMap();  // This is our map where we play
	generateFood();  // It will randomly generate a food 

	// This is the structure used in programming most games.
	// An infinite loop is run with the game logic and graphics being updated at each iteration.
	while (1)
	{
		// Check if there is any input, update your variables accordingly.
		input();

		// Do not refresh graphics when paused, it causes unnecessary flicker.
		if (paused == 0)
		{
			// Refresh graphics.
			draw();

			// Update variables accordingly.
			logic();

			// At each iteration, we sleep for a predetermined amount of time.
			// There are better ways of doing this (multi-threading).
			Sleep(sleepTime);
		}
	}
}

void welcomeScreen()
{
	paused = 1;

	printf("	The game has been created by Serkan Kavak as an ASCII game.\n");
	Sleep(2000);
	printf("Before the snake catches you, eat '@' on the field\n");
	printf("Eat and make score as much as you can.\n\n");
	Sleep(2000);
	printf("Be aware that the snake does not only try to catch you,\nbut also try to catch your food!!!\n\n");
	Sleep(1000);
	printf("Try not to loose your food! You will loose 1 point for each lost food!!!\n\n");
	Sleep(1000);
	printf("Use W, A, S, D to control.\n\n");
	Sleep(1000);
	printf("To Exit the game - Press X.\nTo Pause the game - Press P\n\n");
	Sleep(2000);
	printf("Press Enter if you are ready to start : ");

	getchar();
	paused = 0;
}

void gameMap()
{
	int x, y;

	// Border of map
	for (x = 0; x < 22; x++) map[x][0] = 178;
	for (x = 0; x < 22; x++) map[x][22] = '\n';
	for (x = 0; x < 22; x++) map[0][x] = 178;
	for (x = 0; x < 22; x++) map[21][x] = 178;
	for (x = 0; x < 22; x++) map[x][21] = 178;

	// Inside of map must be empty. So, all the characters on map must be ''space ''
	for (x = 1; x < 21; x++){
		for (y = 1; y < 21; y++){
			map[x][y] = ' '; 
		}
	}

	map[i][j] = point;		// Player's location

	for (x = 0; x < 7; x++){
		snake[x][0] = 1;
		snake[x][1] = x + 1;
		map[headx][x + 1] = tail;
	}

	map[headx][8] = head;
}

void generateFood()
{
	// Food must be on the map. To avoid from bugs, foods location value must under 21
	srand(time(NULL));
	foodX = rand() % 21;
	foodY = rand() % 21;

	// Food must be generated on empty plane.
	while (map[foodX][foodY] != ' ')
	{
		foodX = rand() % 21;
		foodY = rand() % 21;
	}

	map[foodX][foodY] = '@';		// Character of food
}



void input()
{
	char key;

	// kbhit() checks if there is a character in the buffer, i.e. if you hit any buttons during Sleep().
	if (_kbhit())
	{
		// Since a character is already in the buffer, it is loaded to the variable instantly.
		// In other words, this getch() does not stall the program and wait for your response.
		key = _getch();

		start = 1;

		if (key == 'p')	 // Game will pause when pressed p
		{
			if (paused == 0)
				paused = 1;
			else
				paused = 0;
		}
		else if (key == 'x')  // To exit game, press x
			exit(0);


		/*  The other buttons to play
			W : upward direction
			S : downward direction
			A : left direction
			D : right direction
		*/
		if (paused == 0)
		{
			if (key == 'w')
			{
				if (i - 1 != 0)
				{
					map[i - 1][j] = map[i][j];
					map[i][j] = ' ';
					i--;
				}
			}

			if (key == 'a')
			{
				if (j - 1 != 0)
				{
					map[i][j - 1] = map[i][j];
					map[i][j] = ' ';
					j--;
				}
			}

			if (key == 's')
			{
				if (i + 1 != 21)
				{
					map[i + 1][j] = map[i][j];
					map[i][j] = ' ';
					i++;
				}
			}

			if (key == 'd')
			{
				if (j + 1 != 21)
				{
					map[i][j + 1] = map[i][j];
					map[i][j] = ' ';
					j++;
				}
			}
		}

	}
}


// Note that we have to redraw the entire image, even though only one character changes at a time.
// This is the case for all practical graphics engines.
void draw()
{
	int x, y;

	// Clears the command line, note that this is platform specific, i.e. will not work on UNIX etc.
	system("cls");

	// system("cls");
	printf("SCORE: %d\n", score);  // Scoreboard

	// Entire map
	for (x = 0; x < 22; x++){
		for (y = 0; y < 23; y++){ 
			printf("%c", map[x][y]); 
		}
	}
}

void logic()
{
	static int speed = 0;
	int x, coorx, coory, varx, vary;
	int getPlayerx, getPlayery;

	getPlayerx = foodX - i;
	getPlayery = foodY - j;
	if (getPlayerx < 0) getPlayerx = -getPlayerx;
	if (getPlayery < 0) getPlayery = -getPlayery;

	if (foodX == i && foodY == j)  //When players location and food location is same, score will increment and generate another food.
	{
		score++;
		generateFood();
	}
	else map[foodX][foodY] = '@';

	if (start != 0)
	{
		speed++;

		if (speed % 4 == 0)  // Speed of snake. Higher numbers will slow the snake
		{
			varx = snake[0][0];
			vary = snake[0][1];
			map[varx][vary] = ' ';

			for (x = 0; x < 7; x++){

				snake[x][0] = snake[x + 1][0];
				snake[x][1] = snake[x + 1][1];
				coorx = snake[x][0];
				coory = snake[x][1];
				map[coorx][coory] = tail;

				if (x == 6)
				{
					snake[6][0] = headx;
					snake[6][1] = heady;
					map[headx][heady] = tail;
				}
			}

			if ((foodX > headx - 7) && (foodX < headx + 7) && (foodY > heady - 7) && (foodY < heady + 7))
			{
				if ((i > headx - 4) && (i < headx + 4) && (j > heady - 4) && (j < heady + 4))
				{
					if (headx < i) headx++;
					if (headx > i) headx--;
					if (heady < j) heady++;
					if (heady > j) heady--;
				}
				else
				{
					if (foodX > headx) headx++;
					if (foodX < headx) headx--;
					if (foodY > heady) heady++;
					if (foodY < heady) heady--;
				}
			}
			else
			{
				if (headx < i) headx++;
				if (headx > i) headx--;
				if (heady < j) heady++;
				if (heady > j) heady--;
			}

			map[headx][heady] = head;
		}


		if (map[headx][heady] == map[foodX][foodY])
		{
			score--;
			minusScore++;
			generateFood();
		}


		if (map[headx][heady] == map[i][j])
		{
			char button;
			draw();
			Sleep(300);
			system("cls");
			Sleep(300);

			draw();
			Sleep(300);
			system("cls");
			Sleep(300);

			draw();
			Sleep(300);
			system("cls");
			Sleep(300);

			printf("\n\n	   Game Over");
			printf("\n\n\n\nSnake ate %d of your food\n", minusScore);
			printf("\nTotal Score : %d\n\n", score);
			printf("\nPress any key to restart again");
			printf("\nPress 'X' to exit the game\n\n");
			Sleep(1000);
			button = _getch();

			if (button == 'x' || button == 'X') 
				exit(0);
			else
			{
				reset();
				system("cls");
				skipWelcome++;
				return main();
			}

		}

	}

}

void reset()
{
	score = 0;
	minusScore = 0;
	i = 11, j = 11;
	headx = 1, heady = 8;
	start = 0;
}
