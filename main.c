#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <Windows.h>

typedef enum UserFlags
{
	USER_WHITE = 0, USER_BLACK = 1, USER_MAX
}UserFlags;

typedef enum GameStatus
{
	STATUS_ERROR = -1,
	STATUS_END = 0,
	STATUS_RUN = 1,
	STATUS_WHITE_WIN = 5,
	STATUS_BLACK_WIN = 6,
}GameStatus;

typedef struct Coord
{
	int x, y;
}Coord;

typedef char(*Board)[15];
#define RENDER_CLEAR system("cls");
#define WHITE_STONE 'o'
#define BLACK_STONE 'x'
#define PLAYER_COUNT 2

void f_backboard(Board map, int width, int height);
void draw(Board map, const Coord* pos, UserFlags userFlag, GameStatus* result);
void InputPos(Coord* player);
GameStatus vaild(Board map, int size);

int main()
{	
	int size;
	char map[15][15] = { {0}, };
	
	printf("바둑판의 크기를 정해주세요.\n");
	scanf("%d", &size);
	
	const char* messages[PLAYER_COUNT] = {
		"놓을 검은 돌의 위치를 정해주세요:(y좌표)(x좌표)",
#if (PLAYER_COUNT == 2)
		"놓을 하얀 돌의 위치를 정해주세요:(y좌표)(x좌표)"
#endif
	};

#if (PLAYER_COUNT == 2)
	Coord players[PLAYER_COUNT] = {
		{0, 0}, // PLAYER 1 { X, Y }
		{0, 0}  // PLAYER 2 { X, Y }
	};
#else
	Coord players[PLAYER_COUNT] = { {0,0} };
#endif


	int isRun = STATUS_RUN; 
	while (isRun == STATUS_RUN)
	{
		int x, y;

		for (UserFlags user = USER_WHITE; user < PLAYER_COUNT; ++user)
		{
			f_backboard(&map, size, size);
			printf("%s", messages[user]);
			InputPos(&players[user]);
			draw(&map, &players[user], user, &isRun);
			if (isRun != STATUS_RUN)
				break;
			// vaild(&map, &isRun);
			isRun = vaild(&map, size);

			RENDER_CLEAR
		}
	}


	if (isRun == STATUS_END)
		printf("게임이 종료되었습니다.\n");
	else if (isRun == STATUS_ERROR)
		printf("error");
	else if (isRun == STATUS_WHITE_WIN)
		printf("흰색이 승리하였습니다.\n");
	else if (isRun == STATUS_BLACK_WIN)
		printf("검은색이 승리하였습니다.\n");

	return 0;
}

void f_backboard(Board map, int width, int height)
{	
	int y, x;

	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			auto data = map[y][x];
			if (map[y][x] == 0)
				printf("%c", '+');
			else
				printf("%c", map[y][x]);
		}
		printf("\n");
	}
}

void draw(Board map, const Coord* pos, UserFlags userFlag, GameStatus* result)
{
	int x = pos->x;
	int y = pos->y;
	if (map[y][x] != 0)
	{
		(*result) = STATUS_ERROR;
		return;
	}

	if (userFlag == USER_WHITE)
	{
		map[y][x] = 'o';
	}
	else if (userFlag == USER_BLACK)
		map[y][x] = 'x';
}

void InputPos(Coord * player)
{
	scanf("%d%d", &(*player).x, &(*player).y);
}

GameStatus vaild(Board map, int size)
{
	GameStatus result = STATUS_RUN;

	int b_count = 0;
	int w_count = 0;

	// 가로
	for (int y = 0; y < size; ++y)
	{
		for (int x = 0; x < size; ++x)
		{
			if (map[y][x] == WHITE_STONE)
				w_count++;
			else if (map[y][x] == BLACK_STONE)
				b_count++;
			if (w_count >= 5)
				result = STATUS_WHITE_WIN;
			if (b_count >= 5)
				result = STATUS_BLACK_WIN;
		}
		w_count = 0;
		b_count = 0;
	}


	b_count = 0;
	w_count = 0;
	// 세로
	for (int x = 0; x < size; ++x)
	{
		for (int y = 0; y < size; ++y)
		{
			if (map[y][x] == WHITE_STONE)
				w_count++;
			else if (map[y][x] == BLACK_STONE)
				b_count++;
			if (w_count >= 5)
				result = STATUS_WHITE_WIN;
			if (b_count >= 5)
				result = STATUS_BLACK_WIN;
		}
		w_count = 0;
		b_count = 0;
	}

	return (result);
}
