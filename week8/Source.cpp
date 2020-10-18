#include <iostream>
#include <windows.h>
#include <conio.h>
#include<time.h>

int x = 38, y = 20, numStar = 0, score = 0, shoot = 0;

void setcursor(bool visible)
{
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO lpCursor;
    lpCursor.bVisible = visible;
    lpCursor.dwSize = 20;
    SetConsoleCursorInfo(console, &lpCursor);
}

char cursor(int x, int y) {
    HANDLE hStd = GetStdHandle(STD_OUTPUT_HANDLE);
    char buf[2]; COORD c = { x,y }; DWORD num_read;
    if (!ReadConsoleOutputCharacter(hStd, (LPTSTR)buf, 1, c, (LPDWORD)&num_read)) return '\0';
    else return buf[0];
}

void setcolor(int fg, int bg) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, bg * 16 + fg);
}

void gotoxy(int x, int y)
{
    COORD c = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void draw_star() {

    setcolor(6, 9);
    while (numStar < 20)
    {
        gotoxy(rand() % 60 + 10, rand() % 3 + 2);
        std::cout << "O";
        numStar += 1;
    }
}

void draw_ship(int x, int y)
{
    gotoxy(x, y);
    setcolor(2, 4);
    std::cout << ("<=o=>");
}

void erase_ship(int x, int y)
{
    gotoxy(x, y);
    setcolor(0, 0);
    std::cout << ("     ");
}

void beam_ship(int x, int y)
{
    int n = 0;
    Beep(1700, 100);
    while (y + 1 > 0 && n == 0) {
        if (cursor(x, y) != 'O')
        {
            gotoxy(x, y);
            std::cout << "|";
            Sleep(50);
            gotoxy(x, y);
            setcolor(0, 0);
            std::cout << (" ");
            setcolor(2, 4);
            y--;
        }
        else if (cursor(x, y) == 'O') {
            Beep(350, 100);
            n = 1;
            shoot += 1;
            numStar -= 1;
            setcolor(4, 2);
            gotoxy(x, y);
            std::cout << "X";
            Sleep(50);
            gotoxy(x, y);
            setcolor(0, 0);
            std::cout << (" ");
            gotoxy(80, 0);
            setcolor(1, 0);
            score = score + 100 + 44 * shoot;
            std::cout << score;
        }
    }

}

void isKeyPressed(char ch, int h, int k) {
    if (ch == 'a') {
        do {
            erase_ship(h, k);
            draw_ship(--h, k);
            Sleep(100);
            if (_kbhit()) {
                ch = _getch();
            }
        } while (h > 0 && ch != 's');
        ch = '\0';
    }
    if (ch == 'd') {
        do {
            erase_ship(h, k);
            draw_ship(++h, k);
            Sleep(100);
            if (_kbhit()) {
                ch = _getch();
            }
        } while (h < 75 && ch != 's');
        ch = '\0';
    }
    if (ch == 'w') {
        if (k > 0) {
            erase_ship(h, k);
            draw_ship(h, --k);
        }
    }
    if (ch == 's') {
        if (k < 25) {
            erase_ship(h, k);
            draw_ship(h, ++k);
        }
    }


    Sleep(100);
    x = h, y = k;
}

void burst_beam(int h, int k, int bullet) {
    int posX[100], posY[100], a = h, b = k, p = 0;
    for (int i = 0; i < bullet; i++)
    {
        posX[i] = a;
        posY[i] = b;
        b += 2;
    }
    while (posY[bullet - 1] + 1 > 0)
    {
        p = 0;
        for (int i = 0; i < bullet; i++)
        {
            if (posY[i] + 1 > 0 && posY[i] <= k) {
                gotoxy(posX[i], posY[i]);
                std::cout << "|";
                p += 1;
            }
        }
        Sleep(100);
        for (int i = 0; i < bullet; i++)
        {
            if (posY[i] + 1 > 0 && posY[i] <= k)
            {
                gotoxy(posX[i], posY[i]);
                setcolor(0, 0);
                std::cout << (" ");
                setcolor(2, 4);
            }
        }
        for (int i = 0; i < bullet; i++)
        {
            posY[i] -= 1;
        }
        if (_kbhit()) {
            char ch = _getch();
            if (ch == 'a') {
                if (x > 0) {
                    erase_ship(x, y);
                    draw_ship(--x, y);
                    if (posY[0] > 0)
                    {
                        for (int i = p; i < bullet; i++)
                        {
                            posX[i] -= 1;
                        }
                    }

                }
                ch = '\0';
            }
            if (ch == 'd') {
                if (x < 75)
                {
                    erase_ship(x, y);
                    draw_ship(++x, y);
                    if (posY[0] > 0)
                    {
                        for (int i = p; i < bullet; i++)
                        {
                            posX[i] += 1;
                        }
                    }

                }
                ch = '\0';
            }
            fflush(stdin);
        }

    }
}
int main()
{
    srand(time(NULL));
    setcursor(0);
    char ch = ' ';
    draw_star();

    draw_ship(38, 20);
    do {
        draw_star();

        if (_kbhit()) {
            ch = _getch();
            if (ch == ' ') {
                if (y <= 25) {
                    beam_ship(x + 2, y - 1);

                }
            }
            else {
                isKeyPressed(ch, x, y);
            }
            fflush(stdin);
        }

    } while (ch != 'x');


    return 0;
}
