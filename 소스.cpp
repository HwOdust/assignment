#include <iostream>
#include <conio.h>   //getch() and kbhit()
#include <time.h>    //including time
#include <windows.h> //sleep functions
#include <fstream>   //file handling
#include <string>
#include <vector> //장애물 관리

#define Max 30
static const int MAX_TAIL = 1000;

// Color Escape Sequences
#define RED "\033[31m"
#define RESET "\033[0m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define PURPLE "\033[35m"

using namespace std;
int BoardX = 60;
int BoardY = 20;
void save_game();
void set_screen();
enum objectType { OBSTACLE, MINE, BUFF };
template <class T>

void show(T a)
{
    if (a == 1)
    {
        cout << "GAME WON.......:)" << endl;
        exit(0);
    }
    else
    {
        cout << "GAME Lost.....:(" << endl;
        exit(0);
    }
}

class User
{
    // Class User Added For Multiplayer Features

    string name;
    string winner;
    int high_score, scores;

public:
    // constructor called

    User()
    {
        winner = '\0';
        name = "\0";
        high_score = 0;
        scores = 0;
    }
    void get()
    {
        cout << "Who's playing: ";
        cin >> name;
    }
    void set_score(int s)
    {
        scores = s;
    }

    // Saving User name and scores

    void save()
    {
        ofstream user("userdata.txt", ios::app);
        try
        {
            if (user.is_open())
            {
                user << name << endl
                    << scores << endl;
                user.close();
            }
            else
                throw 0;
        }
        catch (int a)
        {
            cout << "user file not opened..." << endl;
            exit(0);
        }
    }

    string return_name()
    {
        return name;
    }

    // Function to calculate high score and save it in the file

    void get_highscore()
    {
        ifstream read("userdata.txt");
        try
        {
            if (read.is_open())
            {
                string a;
                int s = 0;
                while (!read.eof())
                {
                    read >> a;
                    read >> s;
                    if (high_score < s)
                    {
                        winner = a;
                        high_score = s;
                    }
                    read.peek();
                }
                read.close();
            }
            else
                throw 0;
        }
        catch (int a)
        {
            cout << "read file not opened.." << endl;
            exit(0);
        }
    }

    // function used to Show the list of the Users present in the file ;

    void show_list()
    {
        ifstream read1("userdata.txt");
        try
        {
            if (read1.is_open())
            {
                string user, scor;
                while (!read1.eof())
                {
                    getline(read1, user);
                    cout << "Name: " << user;
                    getline(read1, scor);
                    cout << "\tscore: " << scor << endl;
                    read1.peek();
                }
                read1.close();
            }
            else
                throw 0;
        }
        catch (int a)
        {
            cout << "read1 file not opened.." << endl;
            exit(0);
        }
    }

    int return_highscore()
    {
        get_highscore();
        return high_score;
    }

    string return_winner()
    {
        return winner;
    }
} U;

class Snake : public User
{
public:
    int headX;
    int headY;
    int size;
    int* tailX;
    int* tailY;     // snake position, pointers for tail
    char direction; // for controlling the direction
    virtual void polymorphism()
    {
        cout << "Polymorphism used..." << endl;
    }
    Snake(int s = 1)
    {
        headX = BoardX / 2;
        headY = BoardY / 2; // snake initial position
        direction = '\0';
        size = s;
        tailX = new int[MAX_TAIL];
        tailY = new int[MAX_TAIL]; // allocating dynamic arrays for the tail
        for (int i = 0; i < size; i++)
        {
            tailX[i] = tailY[i] = 0; // initiallizing tail
        }
    }

    void move_snake()
    {
        switch (direction)
        {
        case 'a': headX--; break;
        case 's': headY++; break;
        case 'd': headX++; break;
        case 'w': headY--; break;
        case 'j': headX--; break;
        case 'k':headY++; break;
        case 'l':headX++; break;
        case 'i':headY--; break;
        }
    }

    void tail_movement()
    {
        int prevX = tailX[0]; // initial conditions for the tail
        int prevY = tailY[0]; // store the initial position of = 0
        int prev2X, prev2Y;
        tailX[0] = headX;
        tailY[0] = headY; // when snake moves, its previous position is stored in the array
        for (int i = 1; i < size; i++)
        {
            prev2X = tailX[i]; // for loop for growing the tail
            prev2Y = tailY[i];
            tailX[i] = prevX;
            tailY[i] = prevY;
            prevX = prev2X;
            prevY = prev2Y;
        }
    }

    void increase_size()
    {
        if (size < MAX_TAIL)
        {
            size++;
        }
    }

    // Operator Overloading Used !!

    void operator=(Snake& s)
    {
        headX = s.headX; // OPERATOR OVERLOADING USED FOR COPYING VALUES !
        headY = s.headY;
        for (int i = 0; i < size; i++)
        {
            tailX[i] = s.tailX[i];
            tailY[i] = s.tailY[i];
        }
        direction = s.direction;
    }
} S;

Snake S2;

class Fruit : public Snake
{
public:
    int fruitX;
    int fruitY;
    int score;
    int score2;
    int max_score;
    Fruit()
    {
        score = score2 = 0;
        max_score = 40;
        fruit_random_pos();
    }
    void fruit_random_pos()
    {
        fruitX = 1 + (rand() % (BoardX - 2));
        fruitY = 1 + (rand() % (BoardY - 2));
    }
    void fruit_eat()
    {
        if (S.headX == fruitX && S.headY == fruitY)
        {
            fruit_random_pos();
            score += 10;
            S.increase_size(); // increase the size of the snake when fruit is eaten
        }
        S.tail_movement();
    }
    void fruit_eat2()
    {
        if (S2.headX == fruitX && S2.headY == fruitY) {
            fruit_random_pos();
            score2 += 10;           // 추가
            S2.increase_size();     // 추가
        }
        S2.tail_movement();         // 추가
    }
} F;

class Object {
public:
    int x, y;
    objectType type;

    Object()
        : x(0), y(0), type(OBSTACLE) {}

    Object(int _x, int _y, objectType _t)
        : x(_x), y(_y), type(_t) {}
};

vector<Object> objects; //모든 오브젝트를 관리할 벡터


class level : public Fruit
{
public:
    int lev;
    int max_level;
    char mode;
    level() : lev(1), max_level(10) {}

    bool isOverlap(int nx, int ny)
    {
        // 과일과 겹쳤는지
        if (nx == F.fruitX && ny == F.fruitY) return true;
        // 뱀 머리와 겹쳤는지
        if (nx == S.headX && ny == S.headY) return true;
        // 뱀 꼬리와 겹쳤는지
        for (int i = 0; i < S.size; ++i) {
            if (S.tailX[i] == nx && S.tailY[i] == ny)
                return true;
        }
        // 기존 오브젝트와 겹쳤는지 (전역 vector<Object> objects 사용)
        for (auto& o : objects) {
            if (o.x == nx && o.y == ny)
                return true;
        }
        return false;
    }

    void incr_level()
    {
        if (lev < max_level && F.score >= F.max_score)
        {
            set_screen();
            cout << YELLOW << "\n\n\n\t\tLevel " << lev << " Completed......" << endl;
            cout << "\n\t\tNext Level....." << RESET << endl;
            Sleep(800);

            lev++;
            F.score = 0;
            objects.clear(); //이전 레벨 오브젝트 초기화

            //장애물 생성
            {
                int ox, oy;
                do
                {
                    ox = 1 + rand() % (BoardX - 2);
                    oy = 1 + rand() % (BoardY - 2);
                } while (isOverlap(ox, oy)); //좌표 중복 확인
                objects.emplace_back(ox, oy, OBSTACLE);
            }

            //지뢰 개수 조절 및 생성
            {
                for (int i = 0; i < lev; i++)
                {
                    int mx, my;
                    do {
                        mx = 1 + rand() % (BoardX - 2);
                        my = 1 + rand() % (BoardY - 2);
                    } while (isOverlap(mx, my));
                    objects.emplace_back(mx, my, MINE);
                }
            }

            //버프 개수 조절 및 생성
            for (int i = 0; i < lev; ++i) {
                int bx, by;
                do {
                    bx = 1 + rand() % (BoardX - 2);
                    by = 1 + rand() % (BoardY - 2);
                } while (isOverlap(bx, by));
                objects.emplace_back(bx, by, BUFF);
            }


            //화면 크기 축소, 뱀/과일 초기 위치 재설정
            F.max_score += 50;
            BoardX -= 3;
            BoardY -= 3;
            S.size = 1;
            S.headX = BoardX / 2;
            S.headY = BoardY / 2;
            S.direction = 'd';
            delete[] S.tailX;
            delete[] S.tailY;
            S.tailX = new int[MAX_TAIL];
            S.tailY = new int[MAX_TAIL];
            F.fruit_random_pos();

            set_screen();

            system("color 8A");
        }
        else if (lev > max_level)
        {
            show(1); //최종 승리
        }
    }
} L;

class Game : public level
{
public:

    //좌표 겹치는지 확인하는 함수
    bool isOverlap(int nx, int ny)
    {
        if (nx == F.fruitX && ny == F.fruitY) //과일과 겹치면
            return true;
        if (nx == S.headX && ny == S.headY)
            return true;
        for (int i = 0; i < S.size; ++i)
            if (S.tailX[i] == nx && S.tailY[i] == ny) //뱀과 겹치면
                return true;
        for (auto& o : objects) //기존 오브젝트와 겹치면
            if (o.x == nx && o.y == ny)
                return true;
        return false;
    }


    void show_board()
    {
        for (int i = 0; i < BoardY; i++)
        {
            for (int j = 0; j < BoardX; j++)
            {
                if (i == 0 || i == BoardY - 1 || j == 0 || j == BoardX - 1) //보드판 출력
                    cout << BLUE << "|" << RESET;
                else if (i == F.fruitY && j == F.fruitX) //과일 출력
                    cout << YELLOW << "$" << RESET;
                else if (i == S.headY && j == S.headX) //뱀 머리 출력
                    cout << RED << "0" << RESET;
                else if (L.mode == 'M' && i == S2.headY && j == S2.headX) //멀티모드일 때만, s2 출력
                    cout << PURPLE << "0" << RESET;  // 추가
                else
                {
                    // 오브젝트 출력
                    bool drawn = false;
                    for (auto& o : objects)
                    {
                        if (o.x == j && o.y == i)
                        {
                            if (o.type == OBSTACLE) cout << RED << "X" << RESET;
                            if (o.type == MINE)     cout << BLUE << "#" << RESET;
                            if (o.type == BUFF)     cout << GREEN << "@" << RESET;
                            drawn = true;
                            break;
                        }
                    }
                    if (drawn) continue;

                    //뱀 꼬리 출력
                    bool tail_print = false;
                    for (int k = 0; k < S.size; k++)
                    {
                        if (S.tailX[k] == j && S.tailY[k] == i)
                        {
                            cout << RED << "o" << RESET;
                            tail_print = true;
                        }
                    }
                    if (L.mode == 'M') {
                        for (int k = 0; k < S2.size; k++)
                        {
                            if (S2.tailX[k] == j && S2.tailY[k] == i)
                            {
                                cout << PURPLE << "o" << RESET;  // 추가
                                tail_print = true;
                            }
                        }
                    }
                    if (tail_print)continue;


                    cout << " ";//그 외 빈칸
                }
            }
            cout << endl;
        }
    }


    void game_flow()
    {
        //0.멀티모드: 머리끼리 닿으면 바로 게임 종료
        if (L.mode == 'M' && S.headX == S2.headX && S.headY == S2.headY) {
            set_screen();
            cout << RED << "\n\n\tHeads collided! Game Over.\n" << RESET;
            Sleep(1000);
            flow_after_gameover();
            return;  // 나머지 로직 스킵
        }

        //1. 오브젝트 충돌 처리
        for (auto it = objects.begin(); it != objects.end(); ++it)
        {
            if (S.headX == it->x && S.headY == it->y)
            {
                objectType t = it->type;
                objects.erase(it); //충돌한 오브젝트 제거

                if (t == OBSTACLE) //장애물이라면
                {
                    set_screen();
                    cout << RED << "\n\n\tYou hit an obstacle! Game Over." << RESET << endl;
                    Sleep(1000);
                    show(0);  // 게임 종료
                }
                else if (t == MINE)  //지뢰라면
                {
                    if (F.score >= 10)F.score -= 10; //점수 감소
                }
                else if (t == BUFF) //버프라면
                {
                    if (S.size > 1) S.size--; //길이 1 감소
                }
                break;  // 한 번만 처리
            }

        }
        //2. 일반 게임 흐름
        if (game_over() == 0) //게임 오버가 아니라면
        {
            F.fruit_eat(); //과일 체크
            if (L.mode == 'M')F.fruit_eat2();

            if (L.mode != 'M')L.incr_level(); //레벨 체크

            set_screen();

            //사용자 정보 및 스코어 출력
            if (L.mode == 'M')
            {
                cout << "\n\tP1 Score: " << F.score
                    << "\tP2 Score: " << F.score2 << endl;
            }
            cout << YELLOW << "\n\n\t\t\tYour Name: " << U.return_name() << RESET;
            cout << "\n\tPrevios Winner: " << U.return_winner() << "\tPrevious High Score: " << U.return_highscore();
            cout << "\n\tMode: " << L.mode << "\t\tLevel: " << L.lev;
            if (L.mode != 'M')cout << "\tScore: " << F.score << "\n";
            else cout << "\n";
            show_board();
        }
        else //게임 오버라면
        {
            if (U.return_highscore() < F.score)
            {
                set_screen();
                system("color 8A");

                cout << YELLOW << "\n\n\n\t\tGAME OVER.....:(" << endl;
                cout << "\n\tBut " << U.return_name() << " is a NEW HIGH SCORER....:)\n"
                    << RESET;
                Sleep(1500);
            }
            else
                cout << "\t\tGAME OVER......";

            flow_after_gameover(); //재시작 또는 종료 처리
        }
        Sleep(30); //프레임 속도 조절
    }
    bool game_over()
    {
        //if (L.mode == 'M')return 0;

        for (int i = 5; i < S.size; i++)
            if (S.tailX[i] == S.headX && S.tailY[i] == S.headY)
                return 1;
        if (L.mode == 'M') {
            for (int i = 5; i < S2.size; i++)
                if (S2.tailX[i] == S2.headX && S2.tailY[i] == S2.headY)
                    return true;  // 추가
        }
        if (L.mode == 'G'||L.mode=='M')
        {
            if (S.headX >= BoardX)
                S.headX = 0;
            else if (S.headX < 0)
                S.headX = BoardX - 1;
            if (S.headY >= BoardY)
                S.headY = 0;
            else if (S.headY < 0)
                S.headY = BoardY - 1;

            if(L.mode=='M')
            {
                if (S2.headX >= BoardX)
                    S2.headX = 0;
                else if (S2.headX < 0)
                    S2.headX = BoardX - 1;
                if (S2.headY >= BoardY)
                    S2.headY = 0;
                else if (S2.headY < 0)
                    S2.headY = BoardY - 1;
            }

        }
        else if (L.mode == 'A')
        {
            if (S.headX == 0 || S.headX == BoardX - 1 || S.headY == 0 || S.headY == BoardY)
                return 1;
        }
        return 0;
    }
    void flow_after_gameover()
    {
        int flow;
        cout << "\n\n\n\t\t1.Wanna Play again?\n ";
        cout << "\t\t2.Exit Game\n";
        cin >> flow;
        try
        {
            if (flow == 1)
            {
                S.headX = BoardX / 2;
                S.headY = BoardY / 2;
                S.direction = '\0';
                S.size = 1;
                F.score = 0;
                delete S.tailX;
                delete S.tailY;
                S.tailX = new int[S.size];
                S.tailY = new int[S.size];

                if (L.mode == 'M') 
                {
                    S2.headX = BoardX / 4;   
                    S2.headY = BoardY / 2;
                    S2.direction = '\0';
                    S2.size = 1;
                    F.score2 = 0;
                    delete[] S2.tailX;
                    delete[] S2.tailY;
                    S2.tailX = new int[MAX_TAIL];
                    S2.tailY = new int[MAX_TAIL];
                }

                set_screen();
            }
            else if (flow == 2)
            {
                U.set_score(F.score);
                U.save();
                exit(0);
            }
            else
                throw flow;
        }
        catch (int a)
        {
            cout << "You entered " << a << " which is invalid" << endl;
            exit(0);
        }
    }

    bool game_over1() {
        // 1) 헤드-투-헤드 충돌
        if (S.headX == S2.headX && S.headY == S2.headY)
            return true;
        // 2) P1 자기 몸통과 충돌
        for (int i = 5; i < S.size; ++i) {
            if (S.tailX[i] == S.headX && S.tailY[i] == S.headY)
                return true;
        }
        return false;
    }

    // P2 충돌 검사 (멀티 전용)
    bool game_over2() {
        // 1) 헤드-투-헤드 충돌
        if (S2.headX == S.headX && S2.headY == S.headY)
            return true;
        // 2) P2 자기 몸통과 충돌
        for (int i = 5; i < S2.size; ++i) {
            if (S2.tailX[i] == S2.headX && S2.tailY[i] == S2.headY)
                return true;
        }
        return false;
    }

}*G;

// FILE HANDLIND USED FOR SAVING AND RESUMING GAME !!

void save_game() // Save Game ftn for storing data in the file .
{
    ofstream out("Game.txt"); //  OFSTREAM Class used !
    try
    {
        if (out.is_open())
        {
            out << BoardX << endl;
            out << BoardY << endl;
            out << S.headX << endl; //  All the data written in the file
            out << S.headY << endl;
            out << S.size << endl;
            out << S.direction << endl;
            for (int i = 0; i < S.size; i++)
            {
                out << S.tailX[i] << endl;
                out << S.tailY[i] << endl;
            }
            out << F.fruitX << endl;
            out << F.fruitY << endl;
            out << F.max_score << endl;
            out << F.score << endl;
            out << L.lev << endl;
            out << G->mode;
            out.close();
            cout << "Game Saved...";
            exit(0);
        }
        else
            throw 0;
    }
    catch (int a)

    {
        cout << "File is not opened..." << endl;
        exit(0);
    }
}

void resume_game() // Resume Game ftn for reading data from the file .
{
    ifstream in("Game.txt"); // IFSTREAM Class Used !
    try
    {
        if (in.is_open())
        {
            in >> BoardX;
            in >> BoardY;
            in >> S.headX; // All the data read from the file
            in >> S.headY;
            in >> S.size;
            in >> S.direction;
            delete[] S.tailX;
            delete[] S.tailY;
            S.tailX = new int[S.size];
            S.tailY = new int[S.size];
            for (int i = 0; i < S.size; i++)
            {
                in >> S.tailX[i];
                in >> S.tailY[i];
            }
            in >> F.fruitX;
            in >> F.fruitY;
            in >> F.max_score;
            in >> F.score;
            in >> L.lev;
            in >> G->mode;
            S.direction = '\0';
            in.close();
        }
        else
            throw 0;
    }
    catch (int a)
    {
        cout << "File is not opened..." << endl;
        exit(0);
    }
}

void set_screen()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD count;
    DWORD cellCount;
    COORD homeCoords = { 0, 0 };

    if (hConsole == INVALID_HANDLE_VALUE) return;

    if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) return;
    cellCount = csbi.dwSize.X * csbi.dwSize.Y;

    FillConsoleOutputCharacter(hConsole, ' ', cellCount, homeCoords, &count);
    FillConsoleOutputAttribute(hConsole, csbi.wAttributes, cellCount, homeCoords, &count);
    SetConsoleCursorPosition(hConsole, homeCoords);
}

void hide_cursor()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}

// (main 함수 위, 모든 클래스 선언 아래에 추가)
void handle_input() {
    if (kbhit()) 
    {
        char ch = getch();
        switch (ch) {
        case 'w': case 'a': case 's': case 'd':
            S.direction = ch;
            break;
        case 'i': case 'j': case 'k': case 'l':
            if (L.mode == 'M') S2.direction = ch; // 멀티일 때만 P2
            break;
        case 'x':
            save_game();
            break;
        default:
            ; // 나머지는 무시
        }
    }
}


int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    srand((unsigned)time(nullptr));

    int flow;
    system("color 8A");
    set_screen();
    // String Function : Strcpy Used !!!

    char s1[Max];
    char s2[Max];
    char s3[Max];
    char s4[Max];

    strcpy(s1, "1.New Game"); // Copying Text to str 1 !!!
    strcpy(s2, "2.Resume Previous Game");
    strcpy(s3, "3.Show the User list.");
    strcpy(s4, "4.Exit Game");

    // Printing All the strings  Step by Step !!!

    cout << "\n\t" << s1;
    cout << "\n\t" << s2;
    cout << "\n\t" << s3;
    cout << "\n\t" << s4 << "\n\t";
    cin >> flow;

    hide_cursor();

    try
    {
        if (flow == 1 || flow == 2 || flow == 3)
        {
            switch (flow)
            {
            case 1:
            {
                U.get();
                Game G1;
                G = &G1;
                set_screen();
                cout << "\n\tSelect One Mode:\n";
                cout << "\t1.General Mode\n";
                cout << "\t2.Advanced Mode\n\t";
                cout << "\t3.Multiplayer Mode\n\t";
                cin >> flow;
                if (flow == 1)
                    L.mode = 'G';
                else if (flow == 2)
                    L.mode = 'A';
                else if (flow == 3)
                {
                    L.mode = 'M';
                    // --- 추가: 플레이어2 뱀 초기 위치/상태 초기화 ---
                    S2.headX = BoardX / 4;
                    S2.headY = BoardY / 2;
                    S2.direction = '\0';
                    S2.size = 1;
                    delete[] S2.tailX;  delete[] S2.tailY;
                    S2.tailX = new int[MAX_TAIL];
                    S2.tailY = new int[MAX_TAIL];
                }

                set_screen();
                do {
                    handle_input();

                    // 1P 이동
                    S.move_snake();

                    // 2P 이동 (멀티 모드일 때만)
                    if (L.mode == 'M') {
                        S2.move_snake();
                    }

                    // 보드 갱신
                    G->game_flow();

                    // 멀티 모드 충돌(게임 오버) 체크
                    if (L.mode == 'M' && (G->game_over1() || G->game_over2())) 
                    {
                        set_screen();

                        // 승자 판정 및 점수 저장
                        if (G->game_over1() && !G->game_over2()) {
                            cout << "\nPlayer2 Wins!\n";
                            U.set_score(F.score2);
                        }
                        else if (!G->game_over1() && G->game_over2()) {
                            cout << "\nPlayer1 Wins!\n";
                            U.set_score(F.score);
                        }
                        else {
                            // 동시 충돌
                            if (F.score > F.score2) { cout << "\nPlayer1 Wins!\n"; U.set_score(F.score); }
                            else if (F.score2 > F.score) { cout << "\nPlayer2 Wins!\n"; U.set_score(F.score2); }
                            else { cout << "\nDraw!\n";       U.set_score(F.score); }
                        }

                        // 유저 점수 파일에 저장
                        U.save();

                        // 싱글 모드와 동일한 게임 오버 후 흐름(재시작 or 종료)
                        G->flow_after_gameover();
                    }

                    Sleep(75);
                } while (1);

                break;
            }
            case 2:
            {
                Game G2;
                G = &G2;
                set_screen();
                resume_game();
                do {
                    // 1) 키 입력
                    handle_input();

                    // 2) 1P 이동 & 꼬리
                    S.move_snake();

                    // 3) 2P 이동 & 꼬리 (멀티일 때만)
                    if (L.mode == 'M') {
                        S2.move_snake();
                    }

                    // 4) 화면 갱신
                    G->game_flow();

                    // 5) 멀티 모드일 때만 머리 충돌 체크
                    if (L.mode == 'M' && (G->game_over1() || G->game_over2())) {
                        set_screen();

                        // 승패 판정 및 점수 저장
                        if (G->game_over1() && !G->game_over2()) {
                            cout << "\nPlayer2 Wins!\n";
                            U.set_score(F.score2);
                        }
                        else if (!G->game_over1() && G->game_over2()) {
                            cout << "\nPlayer1 Wins!\n";
                            U.set_score(F.score);
                        }
                        else {
                            // 동시 충돌 시 점수 비교
                            if (F.score > F.score2) { cout << "\nPlayer1 Wins!\n"; U.set_score(F.score); }
                            else if (F.score2 > F.score) { cout << "\nPlayer2 Wins!\n"; U.set_score(F.score2); }
                            else { cout << "\nDraw!\n";       U.set_score(F.score); }
                        }

                        // 결과 저장
                        U.save();

                        // 재시작 or 종료
                        G->flow_after_gameover();
                    }

                    Sleep(75);
                } while (1);
                break;
            }

            case 3:
            {
                U.show_list();
            }
            case 4:
            {
                exit(0);
                break;
            }
            }
        }
        else
            throw flow;
    }
    catch (int f)
    {
        cout << "Entered number is invalid...";
        exit(0);
    }
}