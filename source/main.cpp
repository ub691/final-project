#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>

/******************** 按鍵設置 ********************/

#define UP 72		// 上
#define DOWN 80		// 下
#define LEFT 75		// 左
#define RIGHT 77	// 右

#define RowL 99		// C
#define RowR 120	// X
#define HOLD 122	// Z

#define Quit 27		// esc

/******************** 變數宣告 ********************/

int Canva[23] = { 0 };

const char Block[7][4][4] = {
	{0x2, 0x2, 0x2, 0x2, 0x0, 0x0, 0xf, 0x0, 0x4, 0x4, 0x4, 0x4, 0x0, 0xf, 0x0, 0x0},	// I
	{0x1, 0x1, 0x3, 0x0, 0x0, 0x4, 0x7, 0x0, 0x6, 0x4, 0x4, 0x0, 0x7, 0x1, 0x0, 0x0},	// J
	{0x4, 0x4, 0x6, 0x0, 0x7, 0x4, 0x0, 0x0, 0x3, 0x1, 0x1, 0x0, 0x0, 0x1, 0x7, 0x0},	// L
	{0x0, 0x6, 0x6, 0x0, 0x0, 0x6, 0x6, 0x0, 0x0, 0x6, 0x6, 0x0, 0x0, 0x6, 0x6, 0x0},	// O
	{0x0, 0x3, 0x6, 0x0, 0x4, 0x6, 0x2, 0x0, 0x3, 0x6, 0x0, 0x0, 0x2, 0x3, 0x1, 0x0},	// S
	{0x0, 0x2, 0x7, 0x0, 0x0, 0x2, 0x3, 0x2, 0x0, 0x0, 0x7, 0x2, 0x0, 0x2, 0x6, 0x2},	// T
	{0x0, 0x6, 0x3, 0x0, 0x2, 0x6, 0x4, 0x0, 0x6, 0x3, 0x0, 0x0, 0x1, 0x3, 0x2, 0x0}	// Z
};

int X, Y;
int newT, Type, Form;
int HolT; bool Holdable;
int Score = 0, Level, Delay;

/******************** 函式宣告 ********************/

void initDisplay(void);
void initCanva(void);
void initData(void);
void HideCursor(void);
void CursorJump(int x, int y);

void Asking(void);
void TheGame(void);

void NewBlock(void);
void BlockDone(void);
bool Timer(int ms, int id);
bool IsLegal(int type, int form, int x, int y);
void EWBlock(int type, int form, int x, int y, bool cmd);
void PrintBlock(int type, int x, int y);
void Reflash(void);
void HoldBlock(void);
void Check(void);
void EndGame(void);

int main()
{
	initDisplay();	// 預設面板
	initCanva();	// 初始化遊戲區域
	HideCursor();	// 隱藏光標

	srand(time(NULL));//亂樹種子

	Asking();		// 命令詢問
	initData();		// 初始化數據

	TheGame();		// 遊戲開始
}

void initDisplay(void)
{
	CursorJump(0, 0);
	system("title Tetris");
	//     1          12                   33
	puts(" ------------------------------------------ ");
	puts("|          |                    |          |");
	puts("|          |                    |          |");
	puts("|          |                    |          |");
	puts("|          |                    |          |");
	puts("|----------|                    |----------|");
	puts("|   HOLD   |                    |   NEXT   |");
	puts("|          |                    |          |");
	puts("|          |                    |          |");
	puts("| Fall: ↑  |                    |          |");
	puts("| Drop: ↓  |                    |  Score:  |");
	puts("| MovL: ←  |                    |          |");
	puts("| MovR: →  |                    |          |");
	puts("|          |                    |          |");
	puts("| RowL: x  |                    |          |");
	puts("| RowR: c  |                    |          |");
	puts("| HOLD: z  |                    |          |");
	puts("|          |                    |          |");
	puts("| Quit:ESC |                    |          |");
	puts("|          |                    |          |");
	puts("|          |                    |          |");
	puts(" ------------------------------------------ ");
}

void initData(void)
{
	X = 8; Y = 0;
	newT = rand() % 7;
	Type = 0; Form = 0;
	HolT = 7; Holdable = true;
	Score = -10; Level = 0; Delay = 1000;
}

void initCanva(void)
{
	for (int i = 0; i < 22; i++)
		Canva[i] = 0x8010;
	Canva[22] = 0xffff;
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
	COORD pos; pos.X = x;pos.Y = y;
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(handle, pos);
}

void Asking(void)
{
	if (Score != 0) { CursorJump(16, 5); printf("Score:%6d", Score); }
	CursorJump(15, 7); printf("Press to start");
	CursorJump(15, 8); printf(" ESC to Quit.");
	char btn;
	while (btn = getch()) {
		if (btn == Quit) { system("cls");  exit(0); }
		else return;
	}
}

void TheGame(void)
{
	NewBlock();
	while (1) {
		if (Timer(20, 0)) Reflash();
		if (Timer(Delay, 1)) {
			EWBlock(Type, Form, X, Y, 0);
			if (IsLegal(Type, Form, X, Y + 1))
				EWBlock(Type, Form, X, ++Y, 1);
			else BlockDone();
		}
		if (kbhit() != 0) {
			EWBlock(Type, Form, X, Y, 0);
			char BTN = getch();
			switch (BTN)
			{
			case UP: while (IsLegal(Type, Form, X, Y + 1)) Y++; BlockDone(); break;
			case DOWN:  if (IsLegal(Type, Form, X, Y + 1)) Y++; break;
			case LEFT:  if (IsLegal(Type, Form, X + 1, Y)) X++; break;
			case RIGHT: if (IsLegal(Type, Form, X - 1, Y)) X--; break;

			case RowL:
				if (++Form > 3) Form = 0;
				if (!IsLegal(Type, Form, X, Y)) Form--;
				break;
			case RowR:
				if (--Form < 0) Form = 3;
				if (!IsLegal(Type, Form, X, Y)) Form++;
				break;

			case HOLD: if (Holdable) HoldBlock(); break;

			case Quit: system("cls"); exit(0); break;
			default: break;
			}
			EWBlock(Type, Form, X, Y, 1);
		}
	}
}

void NewBlock(void)
{
	Check();
	Type = newT;
	newT = rand() % 7;
	Form = 0;
	X = 8; Y = 0;
	PrintBlock(newT, 34, 1);
	if (IsLegal(Type, Form, X, Y))
		EWBlock(Type, Form, X, Y, 1);
	else EndGame();
}

void BlockDone(void)
{
	EWBlock(Type, Form, X, Y, 1);
	Holdable = true; TheGame();
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

bool IsLegal(int type, int form, int x, int y)
{
	int tmp1, tmp2; int k = 0;
	for (int i = y; i < y + 4; i++) {
		tmp1 = Canva[i] | (Block[type][form][k] << x);
		tmp2 = Canva[i] + (Block[type][form][k] << x);
		if (tmp1 != tmp2) return false; else k++;
	}
	return true;
}

void EWBlock(int type, int form, int x, int y, bool cmd)
{
	int k = 0;
	for (int i = y; i < y + 4; i++) {
		if (cmd) Canva[i] |= (Block[type][form][k++] << x);
		else     Canva[i] ^= (Block[type][form][k++] << x);
	}
}

void PrintBlock(int type, int x, int y)
{
	int shift = 0x8;
	for (int i = 0; i < 4; i++) {
		CursorJump(x, y + i);
		for (int j = 0; j < 4; j++) {
			if (Block[type][0][i] & (shift >> j))
				printf("[]");
			else printf("  ");
		}
	}
}

void Reflash(void)
{
	const int shift = 0x4000;
	for (int i = 2; i < 22; i++) {
		CursorJump(12, i - 1);
		for (int j = 0; j < 10; j++) {
			if (Canva[i] & (shift >> j))
				printf("[]");
			else printf("  ");
		}
	}
}

void HoldBlock(void)
{
	int tmp;
	PrintBlock(Type, 2, 1);
	if (Holdable) {
		Holdable = false;
		tmp = Type;
		Type = HolT;
		HolT = tmp;
	}
	if (Type > 6) TheGame();
	X = 8; Y = 0; Form = 0;
}

void Check(void)
{
	int line = 0;
	Score += 10;
	for (int i = Y + 3; i >= Y; i--) {
		if (Canva[i] == 0xfff0) {
			for (int j = i; j > 0; j--)
				Canva[j] = Canva[j - 1];
			Canva[0] = 0x8010; i++;
			line++;
		}
	}
	switch (line)
	{
	case 1: Score += 20 * (Level + 1); break;
	case 2: Score += 50 * (Level + 1); break;
	case 3: Score += 150 * (Level + 1); break;
	case 4: Score += 600 * (Level + 1); break;
	default: break;
	}
	CursorJump(35, 12); printf("%6d", Score);
	Level = Score / 2000; if (Level > 19) Level = 19;
	Delay = 1020 - Level * 50;
}

void EndGame(void)
{
	int i = 20;
	while (i > 0) {
		if (Timer(50, 0)) {
			CursorJump(12, i--);
			printf("[][][][][][][][][][]");
		}
	}
	while (i < 20) {
		if (Timer(50, 0)) {
			CursorJump(12, ++i);
			printf("                    ");
		}
	}
	if (kbhit() != 0);
	main();
}