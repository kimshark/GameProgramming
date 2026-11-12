#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>

#define BOX_WIDTH 15   // 게임판 가로 칸 수 
#define BOX_HEIGHT 15  // 게임판 세로 칸 수

// 블록 쌓인 상태를 저장하는 배열
int block_stack[BOX_WIDTH * 2 + 10] = { 0 };

void IntroGame(void);
int PlayGame(int *scoreOut);
void DrawRectangle(void);
void Gotoxy(int x, int y);
void SetColor(int color);
int GetMaxBlockHeight(void);
void ClearInputBuffer(void);

int main(void)
{
    int bestScore = 0;
    
    // 난수 생성 초기화
    srand((unsigned int)time(NULL));

    IntroGame();

    while (1)
    {
        int currentScore = 0;
        int gameOver = PlayGame(&currentScore);

        if (currentScore > bestScore)
        {
            bestScore = currentScore;
        }

        Gotoxy(1, BOX_HEIGHT + 6);
        SetColor(7); // 기본 흰색
        printf("========================================\n");
        if (gameOver)
        {
            printf(" 💀 게임 오버! 천장에 도달했습니다!\n");
        }
        else
        {
            printf(" 🎉 15번의 기회를 모두 사용했습니다!\n");
        }
        printf(" 현재 점수(쌓인 블록): %d 점 | 최고 기록: %d 점\n", currentScore, bestScore);
        printf(" 다시 시도하시겠습니까? (Y / N): ");

        char choice;
        while (1)
        {
            if (_kbhit())
            {
                choice = _getch();
                if (choice == 'y' || choice == 'Y') break;
                if (choice == 'n' || choice == 'N')
                {
                    Gotoxy(1, BOX_HEIGHT + 11);
                    printf("게임을 종료합니다. 수고하셨습니다!\n");
                    return 0;
                }
            }
        }
    }

    return 0;
}

void IntroGame(void)
{
    system("cls");
    SetColor(11); // 밝은 하늘색
    printf("========================================\n");
    printf("         [ 업그레이드 블록 쌓기 ]       \n");
    printf("========================================\n");
    SetColor(7);
    printf("\n");
    printf(" 1. 블록이 좌우로 움직일 때 스페이스바를 누르세요.\n");
    printf(" 2. 키를 꾹 누르고 있어도 한 번만 입력됩니다! (꼼수 방지)\n");
    printf(" 3. 블록의 시작 위치는 매번 랜덤입니다.\n");
    printf(" 4. 블록이 천장에 닿으면 게임 오버됩니다.\n\n");
    SetColor(14); // 노란색
    printf(" 아무 키나 누르면 게임을 시작합니다...");
    SetColor(7);
    _getch();
}

int PlayGame(int *scoreOut)
{
    int count = 0;
    int isGameOver = 0;

    for (int i = 0; i < BOX_WIDTH * 2 + 10; i++)
    {
        block_stack[i] = 0;
    }

    system("cls");
    DrawRectangle();

    // 우측 정보란 안내
    Gotoxy(BOX_WIDTH * 2 + 5, 3);
    SetColor(13); // 보라색
    printf(" 목표 높이: %2d칸", BOX_HEIGHT);
    SetColor(7);

    while (count < BOX_HEIGHT)
    {
        int currentMaxHeight = GetMaxBlockHeight();

        if (currentMaxHeight >= BOX_HEIGHT)
        {
            isGameOver = 1;
            break;
        }

        Gotoxy(BOX_WIDTH * 2 + 5, 5);
        printf(" 시도 횟수: %2d / %d", count + 1, BOX_HEIGHT);
        Gotoxy(BOX_WIDTH * 2 + 5, 6);
        printf(" 쌓인 높이: %2d 칸    ", currentMaxHeight);

        int colIndex = rand() % BOX_WIDTH;
        int x = 2 + (colIndex * 2);
        int y = 2;
        int temp = 2;

        ClearInputBuffer();

        // 좌우 왕복 루프
        while (!_kbhit())
        {
            Gotoxy(x, y);
            printf("  ");

            x += temp;
            
            int minX = 2;
            int maxX = 2 + (BOX_WIDTH - 1) * 2;

            if (x > maxX)
            {
                x = maxX;
                temp = -2;
            }
            if (x < minX)
            {
                x = minX;
                temp = 2;
            }

            Gotoxy(x, y);
            SetColor(11); // 청록색 움직이는 블록
            printf("■");
            SetColor(7);

            Sleep(50);
        }

        char key = _getch();
        ClearInputBuffer();

        int targetCol = (x - 2) / 2;
        
        // [핵심 수정] 낙하 위치를 바닥 프레임(BOX_HEIGHT + 2)보다 한 칸 위인 (BOX_HEIGHT + 1)로 설정하여 프레임을 침범하지 않음
        int targetY = BOX_HEIGHT + 1 - block_stack[targetCol];

        // 블록 낙하 애니메이션
        for (int currY = 2; currY < targetY; currY++)
        {
            Gotoxy(x, currY);
            SetColor(10); // 초록색 낙하 효과
            printf("■");
            Sleep(15);
            Gotoxy(x, currY);
            printf("  ");
        }

        // 안착
        block_stack[targetCol] += 1;
        Gotoxy(x, targetY);
        SetColor(14); // 노란색 쌓인 블록
        printf("■");
        SetColor(7);

        count++;
    }

    *scoreOut = GetMaxBlockHeight();
    return isGameOver;
}

void Gotoxy(int x, int y)
{
    COORD Pos = { (SHORT)(x - 1), (SHORT)(y - 1) };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

void SetColor(int color)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void DrawRectangle(void)
{
    int i, j;
    unsigned char a = 0xa6;
    unsigned char b[7];
    for (i = 1; i < 7; i++)
        b[i] = 0xa0 + i;

    // 상단 테두리
    printf("%c%c", a, b[3]); // ┌
    for (i = 0; i < BOX_WIDTH * 2; i++)
        printf("%c%c", a, b[1]); // ─
    printf("%c%c", a, b[4]); // ┐
    printf("\n");

    // 중간 빈 공간 
    for (i = 0; i < BOX_HEIGHT; i++)
    {
        printf("%c%c", a, b[2]); // │
        for (j = 0; j < BOX_WIDTH; j++)
            printf("  "); 
        printf("%c%c", a, b[2]); // │
        printf("\n");
    }

    // 하단 테두리
    printf("%c%c", a, b[6]); // └
    for (i = 0; i < BOX_WIDTH * 2; i++)
        printf("%c%c", a, b[1]); // ─
    printf("%c%c", a, b[5]); // ┘
    printf("\n");
}

int GetMaxBlockHeight(void)
{
    int max = 0;
    for (int i = 0; i < BOX_WIDTH; i++)
    {
        if (block_stack[i] > max)
        {
            max = block_stack[i];
        }
    }
    return max;
}

void ClearInputBuffer(void)
{
    while (_kbhit())
    {
        _getch();
    }
}
