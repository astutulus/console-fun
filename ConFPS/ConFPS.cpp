
/*
* -------------------------------
* First Person Shooter in Console 
* --- OneLoneCoder -- YouTube ---
* -------------------------------
* 
* To do:- 
* Divisions on wall blocks, from about 28 mins in video
*/


#include <iostream>
#include <chrono>

#include "Windows.h"

int nScreenWidth = 120;
int nScreenHeight = 30;

float fPlayerX = 8.0f;
float fPlayerY = 8.0f;
float fPlayerA = 0.0f;

int nMapHeight = 16;
int nMapWidth = 16;

float fFOV = 3.14159 / 4.0f;
float fDepthOfView = 16.0f;

float fRotSpeed = 0.8f;
float fFwdSpeed = 5.0f;
float fBckSpeed = fFwdSpeed * 0.6f;

int main()
{
    // Create Screen Buffer
    wchar_t* screen = new wchar_t[nScreenWidth * nScreenHeight];
    HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleActiveScreenBuffer(hConsole);
    DWORD dwBytesWritten = 0;        
        
    std::wstring map;

    map += L"################";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#......####....#";
    map += L"#.........#....#";
    map += L"#.........#....#";
    map += L"#...#.....#....#";
    map += L"#...#..........#";
    map += L"#...#..........#";
    map += L"#...##.........#";
    map += L"#....###.......#";
    map += L"#......####....#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"################";

    auto tp1 = std::chrono::system_clock::now();
    auto tp2 = std::chrono::system_clock::now();


    while (1)
    {
        // TIMING
        tp2 = std::chrono::system_clock::now();
        std::chrono::duration<float> elapsedTime = tp2 - tp1;
        tp1 = tp2;
        float fElapsedTime = elapsedTime.count();


        // CONTROL
        // Handle CCW rotation
        if (GetAsyncKeyState((unsigned short)'A') & 0x8000)
        {
            fPlayerA -= fRotSpeed * fElapsedTime;
        }
        // Handle CW rotation
        if (GetAsyncKeyState((unsigned short)'D') & 0x8000)
        {
            fPlayerA += fRotSpeed * fElapsedTime;
        }
        // Handle walking forward
        if (GetAsyncKeyState((unsigned short)'W') & 0x8000)
        {
            float proposedX = fPlayerX + sinf(fPlayerA) * fFwdSpeed * fElapsedTime;
            float proposedY = fPlayerY + cosf(fPlayerA) * fFwdSpeed * fElapsedTime;
            if (map[proposedY * nMapWidth + proposedX] != '#')
            {
                fPlayerX = proposedX;
                fPlayerY = proposedY;
            }
        }
        // Handle walking backward
        if (GetAsyncKeyState((unsigned short)'S') & 0x8000)
        {
            float proposedX = fPlayerX - sinf(fPlayerA) * fFwdSpeed * fElapsedTime;
            float proposedY = fPlayerY - cosf(fPlayerA) * fFwdSpeed * fElapsedTime;
            if (map[proposedY * nMapWidth + proposedX] != '#')
            {
                fPlayerX = proposedX;
                fPlayerY = proposedY;
            }
        }


        // SHADER
        for (int x = 0; x < nScreenWidth; x++)
        {
            float fDistanceToWall = 0;

            float fRayAngle = (fPlayerA - fFOV / 2.0f) + ((float)x / (float)nScreenWidth) * fFOV;
            float fEyeX = sinf(fRayAngle);
            float fEyeY = cosf(fRayAngle);

            bool bHitWall = false;
            while (!bHitWall && fDistanceToWall < fDepthOfView)
            {
                fDistanceToWall += 0.01f;

                int nTestX = (int)(fPlayerX + fDistanceToWall * fEyeX);
                int nTestY = (int)(fPlayerY + fDistanceToWall * fEyeY);

                // Test if ray is out of bounds
                if (nTestX < 0 || nTestY < 0 || nTestX >= nMapWidth || nTestY >= nMapWidth)
                {
                    bHitWall = true;
                    fDistanceToWall = fDepthOfView; // to cause 'while' loop to end
                }
                else
                {
                    int mapCell = nTestY * nMapWidth + nTestX;
                    if (map[mapCell] == '#')
                    {
                        bHitWall = true;
                    }
                }
            }

            int nCeilingEdge = (float)(nScreenHeight / 2) - nScreenHeight / ((float)fDistanceToWall);
            int nFloorEdge = nScreenHeight - nCeilingEdge;

            short nShade = ' ';                                                 // empty black
            if (fDistanceToWall > fDepthOfView / 2.0f)        nShade = 0x2591;    // dark
            else if (fDistanceToWall > fDepthOfView / 3.0f)   nShade = 0x2592;    // medium
            else if (fDistanceToWall > fDepthOfView / 4.0f)   nShade = 0x2593;    // light
            else                                              nShade = 0x2588;    // solid white 


            for (int y = 0; y < nScreenHeight; y++)
            {
                if (y < nCeilingEdge)
                    screen[y * nScreenWidth + x] = ' ';
                else if (y < nFloorEdge)
                    screen[y * nScreenWidth + x] = nShade;
                else if (y < nScreenHeight * 0.8)
                    screen[y * nScreenWidth + x] = ':';
                else
                    screen[y * nScreenWidth + x] = '.';
            }
        }


        // OUTPUT
        screen[nScreenWidth * nScreenHeight - 1] = '\0';
        WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
    }


    std::cout << "Hello World!\n";

    return 0;
}

