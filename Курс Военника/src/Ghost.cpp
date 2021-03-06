#include <iostream>

#include "Constants.h"

#include "Game.h"
#include "Ghost.h"
#include "SetConsoleAttributes.h"

using namespace std;

Ghost::Ghost(Game *const g)
: game(g) {}

// перемещать призрак на основе текущего режима каждый раз, когда таймер ожидания достигает 0
void Ghost::Move(int playerY, int playerX) {
    if (wait) {
        --wait;
    }
    else {
        switch (mode) {
        // если «Ожидание»
        // подпрыгивать вверх и вниз
        case 'w':
            Hide();
            if (y == GATE_Y + 2) {
                ++y;
            }
            else {
                --y;
            }
            Show();
            wait = GHOST_MAX;
            break;
        // если «Выход»
        // сделать путь, чтобы выйти из ворот
        // после выхода из ворот, если игрок super pacman, то 'бежать'
        // в противном случае, 'гнаться'
        case 'e':
            Hide();
            wait = GHOST_MAX;
            if (y > GATE_Y + 1) {
                --y;
            }
            else if (x < GATE_X) {
                ++x;
            }
            else if (x > GATE_X) {
                --x;
            }
            else if (y != GATE_Y - 1) {
                --y;
                SetCursorPosition(GATE_Y, GATE_X + 1);
                cout << game->GetLevel(GATE_Y, GATE_X + 1);
            }
            else {
                modeOld = mode;
                if (color == BLUE) {
                    mode = 'r';
                }
                else {
                    mode = 'c';
                }
                dirOld = 'w';
                wait = 0;
            }
            Show();
            break;
        // если «Ввод»
        // войдите в дом-призрак, затем 'Выход'
        case 'n':
            if (y != GATE_Y + 1) {
                dir = 's';
                ChangeCoords();
                SetCursorPosition(GATE_Y, GATE_X + 1);
                cout << game->GetLevel(GATE_Y, GATE_X + 1);
                wait = DEAD_MAX;
            }
            else {
                color = colorInit;
                mode = 'e';
                wait = GHOST_MAX;
                icon = GHOST_ICON;
            }
            break;
        // если «Рассеяние»
        // двигаться в случайном направлении
        case 's':
            GetOpposite();
            if (modeOld == 'e') {
                modeOld = mode;
            }
            if (mode != modeOld) {
                dir = dirOpp;
                ChangeCoords();
                modeOld = mode;
            }
            else {
                RandomDirection();
            }
            dirOld = dir;
            wait = GHOST_MAX;
            break;
        // если "преследование"
        // нацелить игрока
        case 'c':
            GetOpposite();
            if (modeOld == 'e') {
                modeOld = mode;
            }
            if (mode != modeOld) {
                dir = dirOpp;
                ChangeCoords();
                modeOld = mode;
            }
            else {
                bool down = y < playerY;
                bool up = y > playerY;
                bool right = x < playerX;
                bool left = x > playerX;
                bool favorableDirs[4] = { up, left, down, right };
                TargetObject(favorableDirs);
            }
            dirOld = dir;
            wait = GHOST_MAX;
            break;
        // если 'Бег'
        // избегать игрока
        case 'r':
            GetOpposite();
            if (modeOld == 'e') {
                modeOld = mode;
            }
            if (mode != modeOld) {
                dir = dirOpp;
                ChangeCoords();
                modeOld = mode;
            }
            else {
                bool down = !(y < playerY);
                bool up = !(y > playerY);
                bool right = !(x < playerX);
                bool left = !(x > playerX);
                bool favorableDirs[4] = { up, left, down, right };
                TargetObject(favorableDirs);
            }
            dirOld = dir;
            wait = RUN_MAX;
            break;
        // если 'умрешь'
        // нацелиться на ворота призрачного дома
        // когда ворота будут достигнуты,'войдите'
        case 'd':
            GetOpposite();
            if (y != GATE_Y - 1 || x != GATE_X) {
                bool down = y < GATE_Y - 1;
                bool up = y > GATE_Y - 1;
                bool right = x < GATE_X;
                bool left = x > GATE_X;
                bool favorableDirs[4] = { up, left, down, right };
                TargetObject(favorableDirs);
            }
            else {
                mode = 'n';
            }
            dirOld = dir;
            wait = DEAD_MAX;
        }
    }
}

void Ghost::TargetObject(bool favorableDirs[4]) {
    int good = 0;
    char goodDirs[4] = {' ',' ',' ',' '};
    for (int i = 0; i < 4; ++i) {
        dir = ALL_DIRS[i];
        if (favorableDirs[i] && TestForCollision() == false && dir != dirOpp) {
            goodDirs[good] = dir;
            ++good;
        }
    }
    if (good == 0) {
        RandomDirection();
    }
    else {
        dir = goodDirs[rand() % good];
        ChangeCoords();
    }
}

void Ghost::RandomDirection() {
    GetOpposite();
    // выберите случайное направление, которое не приведет к столкновению
    do {
        // выберите направление рандомна, которое не противоположно предыдущему направлению
        do {
            dir = ALL_DIRS[rand() % 4];
        } while (dir == dirOpp);
    } while (TestForCollision() == true);
    ChangeCoords();
}

bool Ghost::TestForCollision() {
    // если символ перед призраком - это пробел, двигайтесь в соответствующем направлении
    switch(dir) {
    case 'a':
        // при проходе через туннель
        if (x == 0 || strchr(NO_COLLISION_TILES, game->GetLevel(y, x - 1))) {
            return false;
        }
        break;
    case 'd':
        // при проходе через туннель
        if (x == LEVEL_WIDTH - 1 || strchr(NO_COLLISION_TILES, game->GetLevel(y, x + 1))) {
            return false;
        }
        break;
    case 'w':
        if (strchr(NO_COLLISION_TILES, game->GetLevel(y - 1, x))) {
            return false;
        }
        break;
    case 's':
        if (strchr(NO_COLLISION_TILES, game->GetLevel(y + 1, x))) {
            return false;
        }
        break;
    }
    return true;
}

void Ghost::ChangeCoords() {
    Hide();
    if (dir == 'a') {
        if (x == 0) {
            x = LEVEL_WIDTH - 1;
        }
        else {
            --x;
        }
    }
    if (dir == 'd') {
        if (x == LEVEL_WIDTH - 1) {
            x = 0;
        }
        else {
            ++x;
        }
    }
    if (dir == 'w') {
        --y;
    }
    if (dir == 's') {
        ++y;
    }
    Show();
}

void Ghost::GetOpposite() {
    if (dirOld == 'a') {
        dirOpp = 'd';
    }
    if (dirOld == 'd') {
        dirOpp = 'a';
    }
    if (dirOld == 'w') {
        dirOpp = 's';
    }
    if (dirOld == 's') {
        dirOpp = 'w';
    }
}

void Ghost::Dead() {
    color = WHITE;
    modeOld = mode;
    mode = 'd';
    icon = DEAD_GHOST_ICON;
}

void Ghost::Show() {
    SetTextColor(color);
    SetCursorPosition(y, x);
    cout << icon;
}

void Ghost::Hide() {
    SetTextColor(WHITE);
    if (game->GetLevel(y, x) == 'o') {
        SetTextColor(game->GetPelletColor());
    }
    SetCursorPosition(y, x);
    cout << game->GetLevel(y, x);
}
