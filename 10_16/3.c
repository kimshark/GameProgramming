#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <locale.h>

void draw_horizontal_slide(int x, int y, int length, const char *s);
void draw_vertical_slide(int x, int y, int length, const char *s);
void draw_rectangle(int c, int r);
void move_arrow_key(char key, int *x1, int *y1, int x_b, int y_b);
void gotoxy(int x, int y);

int main(void)
{
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    setlocale(LC_ALL, "");

    const char *slide = "■";
    int x = 1, y = 1;
    int h_slide_length, v_slide_length;
    char key;

    printf("슬라이드바 표시\n\n");
    printf("수평 슬라이드바의 길이(최대 50)를 \n입력하고 Enter > ");
    scanf("%d", &h_slide_length);
    printf("수직 슬라이드바의 길이(최대 15)를 \n입력하고 Enter > ");
    scanf("%d", &v_slide_length);
    
    // 너무 큰 값이 들어와 화면이 잘리는 것 방지
    if (h_slide_length > 50) h_slide_length = 50;
    if (v_slide_length > 15) v_slide_length = 15;

    system("cls");
    
    do
    {
        draw_vertical_slide(1, y, v_slide_length, slide);
        draw_horizontal_slide(x, v_slide_length + 3, h_slide_length, slide);
        
        key = getch();
        
        // 방향키 입력 시 확장 키 처리
        if (key == 224 || key == 0) {
            key = getch();
        }
        
        move_arrow_key(key, &x, &y, h_slide_length, v_slide_length);
    } while (key != 27); // ESC 누르면 종료

    return 0;
}

void draw_rectangle(int c, int r)
{
    int i, j;
    // 유니코드 박스 드로잉 문자를 사용하여 사각형 테두리 생성
    printf("┌");
    for (i = 0; i < c; i++) printf("─");
    printf("┐\n");

    for (i = 0; i < r; i++)
    {
        printf("│");
        for (j = 0; j < c; j++)
            printf(" ");
        printf("│\n");
    }

    printf("└");
    for (i = 0; i < c; i++) printf("─");
    printf("┘\n");
}

void draw_horizontal_slide(int x, int y, int length, const char *s)
{
    // 수평 슬라이드 우측 잘림 방지를 위해 박스 길이를 안전하게 제어
    gotoxy(1, y);
    draw_rectangle(length, 1);
    
    // 슬라이더 이동 범위 제한
    if (x > length) x = length;
    
    gotoxy(x + 1, y + 1);
    printf("%s", s);
    
    // 현재 좌표 숫자를 박스 우측 안전한 위치에 출력
    gotoxy(length + 4, y);
    printf("%2d", x);
}

void draw_vertical_slide(int x, int y, int length, const char *s)
{
    gotoxy(x, 1);
    draw_rectangle(2, length);
    
    if (y > length) y = length;
    
    gotoxy(x + 1, y + 1);
    printf("%s", s);
    
    gotoxy(x + 6, length + 1);
    printf("%2d", y);
}

void gotoxy(int x, int y)
{
    COORD Pos = {(SHORT)(x - 1), (SHORT)(y - 1)};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

void move_arrow_key(char key, int *x1, int *y1, int x_b, int y_b)
{
    switch (key)
    {
    case 72: // 위쪽(상)
        *y1 = *y1 - 1;
        if (*y1 < 1) *y1 = 1;
        break;
    case 75: // 왼쪽(좌)
        *x1 = *x1 - 1;
        if (*x1 < 1) *x1 = 1;
        break;
    case 77: // 오른쪽(우)
        *x1 = *x1 + 1;
        if (*x1 > x_b) *x1 = x_b;
        break;
    case 80: // 아래쪽(하)
        *y1 = *y1 + 1;
        if (*y1 > y_b) *y1 = y_b;
        break;
    default:
        return;
    }
}
