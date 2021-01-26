#include "Windows.h"

#include <chrono>			// std::chrono::system_clock::now(), std::chrono::duration<>

#include "GameEngine.h"

GameObj::GameObj()
{
	nScreenWidth = 120;
	nScreenHeight = 30;

	screen = new wchar_t[  nScreenWidth * nScreenHeight  ];
	hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	dwBytesWritten = 0;

	tpLastLoopStart = std::chrono::system_clock::now();
	tpThisLoopStart = std::chrono::system_clock::now();
};

int GameObj::getScreenWidth()
{
	return nScreenWidth;
}
int GameObj::getScreenHeight()
{
	return nScreenHeight;
}

void GameObj::BufferWipe()
{
	for (int x = 0; x < nScreenWidth; x++)
	{
		for (int y = 0; y < nScreenHeight; y++)
		{
			screen[y * nScreenWidth + x] = ' ';
		}
	}
}
void GameObj::BufferAddCharacter(char symbol, int x, int y)
{
		screen[y * nScreenWidth + x] = symbol;
}
void GameObj::BufferAddStats(float energy)
{
	swprintf_s(screen, 40, L"Rabbit: Energy=%3.2f", energy);
}
void GameObj::BufferPaint()
{
	screen[nScreenWidth * nScreenHeight - 1] = '\0';
	WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0, 0 }, &dwBytesWritten);
}

float GameObj::LatestLoopTime()
{
	tpThisLoopStart = std::chrono::system_clock::now();
	std::chrono::duration<float> loopDuration = tpThisLoopStart - tpLastLoopStart;
	tpLastLoopStart = tpThisLoopStart;
	return loopDuration.count();
}