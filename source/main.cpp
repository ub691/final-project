#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <Windows.h>
void HideCursor(void);
void CursorJump(int x, int y);
bool Timer(int ms, int id);

int main()
{
	// 時序控制 - 許
	// 畫面更新 - 陳
	// 規則設計 - 梁

	// 上傳測試
}
bool Timer(int ms, int id)
{
	static clock_t t[10];
	if (clock() - t[id] > ms) {
		t[id] = clock();
		return true;
	}
	return false;
}
void HideCursor(void)
{
	CONSOLE_CURSOR_INFO curInfo;
	curInfo.dwSize = 1;
	curInfo.bVisible = FALSE;
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorInfo(handle, &curInfo);
}

void CursorJump(int x, int y)
{
	COORD pos; pos.X = x; pos.Y = y;
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(handle, pos);
}