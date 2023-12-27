#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <Windows.h>
void HideCursor(void);
void CursorJump(int x, int y);
bool Timer(int ms, int id);

int main()
{
	// �ɧǱ��� - �\
	// �e����s - ��
	// �W�h�]�p - ��

	// �W�Ǵ���
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