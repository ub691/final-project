#include <stdio.h>
#include <time.h>
#include <stdlib.h>

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