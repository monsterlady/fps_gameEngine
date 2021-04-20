// 3D_FPS.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <windows.h>
#include <string>
using namespace std;

int nScreenWidth = 120;
int nScreenHeight = 40;

float fPlayerX = 8.0f;
float fPlayerY = 8.0f;
float fPlayerA = 0.0f;

int nMapHeight = 16;
int nMapWidth = 16;


float fFOV = 3.14159 / 4.0;
float fDepth = 16.0f;

wstring wgetMap() {

	wstring map;
	map += L"#########.......";
	map += L"#...............";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#......#.......#";
	map += L"#.......#......#";
	map += L"#........#.....#";
	map += L"#.........#....#";
	map += L"################";
	return map;
}

int main()
{
	wchar_t* screen = new wchar_t[nScreenWidth * nScreenHeight];
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

	wstring map;
	map = wgetMap();

	while (true)
	{
		for (int x = 0; x < nScreenWidth; x++)
		{
			// 計算每一列投影的的光線追蹤
			float fRayAngle = (fPlayerA - fFOV / 2.0f) + ((float)x / (float)nScreenWidth) * fFOV;
			//設置到墻壁的距離
			float fDistanceToWalll = 0;
			bool bIsHitWall = false;
			//玩家眼睛的向量單位
			float fEyeX = sinf(fRayAngle);
			float fEyeY = cosf(fRayAngle);

			//如果光線追蹤沒有射擊到墻壁則改變其距離
			while (!bIsHitWall && fDistanceToWalll < fDepth)
			{
				fDistanceToWalll += 0.1f;

				int nTextX = (int)(fPlayerX + fEyeX * fDistanceToWalll);
				int nTextY = (int)(fPlayerY + fEyeY * fDistanceToWalll);

				//測試視線是否超越邊界
				if (nTextX < 0 || nTextX >= nMapWidth || nTextY < 0 || nTextY >= nMapHeight)
				{
					bIsHitWall = true;
					fDistanceToWalll = fDepth;
				}
				else {
					if (map[nTextY * nMapWidth + nTextX] == '#') {
						bIsHitWall = true;
					}
				}
			}

			//計算天花板到地面的距離
			int nCeiling = (float)(nScreenHeight / 2.0) - nScreenHeight / ((float)fDistanceToWalll);
			int nFloor = nScreenHeight - nCeiling;

			//渲染橫軸
			for (int y = 0; y < nScreenHeight; y++)
			{
				if (y < nCeiling) {
					//天空
					screen[y * nScreenWidth + x] = ' ';
				}
				else if(y > nCeiling && y <= nFloor) {
					//墻
					screen[y * nScreenWidth + x] = '#';
				}
				else {
					//地板
					screen[y * nScreenWidth + x] = ' ';
				}
			}
		}

		screen[nScreenWidth * nScreenHeight - 1] = '\0';
		WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
	}
	return 0;
}


// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
