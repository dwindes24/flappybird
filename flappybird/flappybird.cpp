
#include <iostream>
#include <string>

#include "olcConsoleGameEngine.h"

using namespace std;

class FlappyBird : public olcConsoleGameEngine
{
public:
	FlappyBird()
	{
		m_sAppName = L"Flappy Bird";
	}
private:
	float birdPosition = 0.0f;
	float birdVel = 0.0f;
	float birdAccel = 0.0f;

	float Gravity = 100.0f;
	float sectionWidth;
	list<int> sectionList;
	float levelPosition = 0.0f;
	
	bool hasCollided = false;
	bool resetGame = false;

	int attemptNum = 0;
	int flapNum = 0;
	int maxFlapNum = 0;

protected:

	virtual bool OnUserCreate()
	{
		sectionList = { 0,0,0,0 };
		sectionWidth = (float)ScreenWidth() / (float)(sectionList.size() - 1);
		resetGame = true;
		return true;
	}

	virtual bool OnUserUpdate(float elapsedTime)
	{
		if (resetGame)
		{
			hasCollided = false;
			resetGame = false;
			sectionList = { 0, 0, 0, 0 };
			birdAccel = 0.0f;
			birdVel = 0.0f;
			birdPosition = ScreenHeight() / 2.0f;
			flapNum = 0;
			attemptNum++;

		}
		if (hasCollided)
		{
			if (m_keys[VK_SPACE].bReleased)
				resetGame = true;

		}
		else
		{
			//need to be falling down to flap again
			if (m_keys[VK_SPACE].bPressed && birdVel >= Gravity / 10.0f)
			{
				birdAccel = 0.0f;
				birdVel = -Gravity / 4.0f;
				flapNum++;
				if (flapNum > maxFlapNum)
					maxFlapNum = flapNum;
			}
			else
				birdAccel += Gravity * elapsedTime;

			if (birdAccel > Gravity)
				birdAccel = Gravity;


			birdVel += birdAccel * elapsedTime;
			birdPosition += birdVel * elapsedTime;
			levelPosition += 14.0f * elapsedTime;

			if (levelPosition > sectionWidth)
			{
				levelPosition -= sectionWidth;
				sectionList.pop_front();
				int i = rand() % (ScreenHeight() - 20);
				if (i <= 10)
					i = 0;
				sectionList.push_back(i);
			}

			Fill(0, 0, ScreenWidth(), ScreenHeight(), L' ');
			int sectionNum = 0;
			for (auto s : sectionList)
			{
				if (s != 0)
				{	//bottom rectangle
					Fill(sectionNum * sectionWidth + 10 - levelPosition, ScreenHeight() - s, sectionNum * sectionWidth + 15 - levelPosition,
						ScreenHeight(), PIXEL_SOLID, FG_GREEN);
					//bottom rectangle
					Fill(sectionNum * sectionWidth + 10 - levelPosition, 0, sectionNum * sectionWidth + 15 - levelPosition,
						ScreenHeight() - s - 15, PIXEL_SOLID, FG_GREEN);
				}
				sectionNum++;
			}


			int birdX = (int)(ScreenWidth() / 3.0f);


			//check if bird hit pipe
			hasCollided = birdPosition < 2 || birdPosition > ScreenHeight() - 2 ||
				m_bufScreen[(int)(birdPosition + 0) * ScreenWidth() + birdX].Char.UnicodeChar != L' ' ||
				m_bufScreen[(int)(birdPosition + 1) * ScreenWidth() + birdX].Char.UnicodeChar != L' ' ||
				m_bufScreen[(int)(birdPosition + 0) * ScreenWidth() + birdX + 6].Char.UnicodeChar != L' ' ||
				m_bufScreen[(int)(birdPosition + 1) * ScreenWidth() + birdX + 6].Char.UnicodeChar != L' ';



			//Draw bird
			if (birdVel > 0)
			{
				DrawString(birdX, birdPosition + 0, L"\\\\\\");
				DrawString(birdX, birdPosition + 1, L"<\\\\\\=Q");
			}
			else
			{
				DrawString(birdX, birdPosition + 0, L"<///=Q");
				DrawString(birdX, birdPosition + 1, L"///");
			}

			//scorebaord
			DrawString(1, 1, L"Attempt: " + to_wstring(attemptNum) + L" Score: " + to_wstring(flapNum) + L" High Score: " + to_wstring(maxFlapNum));

			return true;
		}
	}
};

int main()
{
	FlappyBird game;
	game.ConstructConsole(80, 48, 16, 16);
	game.Start();

	return 0;
}
