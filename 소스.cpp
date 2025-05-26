#include <iostream>
#include <conio.h>   //getch(), kbhit() 함수 사용 -> 키 입력 
#include <time.h>    //srand(), time() 함수 사용 -> 오브젝트의 좌표 랜덤 설정
#include <windows.h> //Sleep(), 콘솔 제어 함수 사용
#include <fstream>   //파일 입출력 -> game user 목록 관리
#include <string>
#include <vector> //오브젝트 기능을 위해 벡터 사용
#define Max 30 
static const int MAX_TAIL = 1000; //뱀의 최대 길이

//색깔 출력을 위한 ANSI 시퀀스
#define RED "\033[31m"
#define RESET "\033[0m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define PURPLE "\033[35m"

using namespace std;

//게임 화면 크기 변수
int BoardX = 60;
int BoardY = 20;

//전역 함수
void save_game(); //게임 상태 파일로 저장
void set_screen(); //콘솔 화면 지우기

//장애물 종류 열거형
enum objectType { OBSTACLE, MINE, BUFF };

//템플릿 함수: 승/패 메세지 출력
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

//사용자 정보 관리 클래스
class User
{
    string name; //플레이어 이름
    string winner; //최고 점수자 이름
    int high_score; //최고 점수
    int scores; //현재 게임 점수

public:
    User()
    {
        winner = '\0';
        name = "\0";
        high_score = 0;
        scores = 0;
    }

    //플레이어 이름 입력
    void get()
    {
        cout << "\tWho's playing: ";
        cin >> name;
    }

    //현재 점수 설정
    void set_score(int s)
    {
        scores = s;
    }

    //userdata.txt에 이름과 점수 저장
    void save()
    {
        ofstream user("userdata.txt", ios::app);
        if (!user.is_open())
        {
            cout << "user file not opened..." << endl;
            exit(0);
        }
        user << name << endl
            << scores << endl;
    }

    //이름 반환
    string return_name()
    {
        return name;
    }

    //userdata.txt에서 최고 점수자와 점수 읽기
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

    //유저 리스트 출력
    void show_list()
    {
        ifstream read1("userdata.txt");
        try
        {
            if (read1.is_open())
            {
                string user, scor;

                //이름과 점수를 줄 단위로 읽어서 출력
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


//Snake 클래스: 뱀의 머리/꼬리 위치, 길이, 이동 방향 관리
class Snake : public User
{
public:
    int headX, headY; //머리 좌표
    int size; //뱀의 길이
    int* tailX; //꼬리 x좌표 배열
    int* tailY; //꼬리 y좌표 배열
    char direction; //이동 방향 문자

    virtual void polymorphism()
    {
        cout << "Polymorphism used..." << endl;
    }

    //생성자: 기본 길이 설정, 꼬리 배열 할당
    Snake(int s = 1)
    {
        headX = BoardX / 2; //뱀 초기 머리 위치
        headY = BoardY / 2; 
        direction = '\0';
        size = s;
        tailX = new int[MAX_TAIL]; //배열 동적 할당
        tailY = new int[MAX_TAIL]; 
        for (int i = 0; i < size; i++) //몸통 초기화
        {
            tailX[i] = tailY[i] = 0;
        }
    }

    //방향에 따라 머리 이동
    void move_snake()
    {
        switch (direction)
        {
            case 'a': headX--; break;
            case 's': headY++; break;
            case 'd': headX++; break;
            case 'w': headY--; break;

            //멀티 플레이용 키 처리
            case 'j': headX--; break;
            case 'k': headY++; break;
            case 'l': headX++; break;
            case 'i': headY--; break;
        }
    }
  
    //꼬리가 머리를 따라 이동하도록 업데이트
    void tail_movement()
    {
        int prevX = tailX[0]; //이전 좌표 저장
        int prevY = tailY[0];
          
        int prev2X, prev2Y;
        tailX[0] = headX; //뱀이 움직일 때, 배열에 이전 좌표 저장
        tailY[0] = headY;
        for (int i = 1; i < size; i++)
        {
            prev2X = tailX[i]; // 뱀 좌표 업데이트
            prev2Y = tailY[i];
            tailX[i] = prevX;
            tailY[i] = prevY;
            prevX = prev2X;
            prevY = prev2Y;
        }
    }

    //뱀 길이 증가
    void increase_size()
    {
        if (size < MAX_TAIL)
        {
            size++;
        }
    }

    //연산자 오버로딩: 깊은 복사
    void operator=(Snake& s)
    {
        headX = s.headX;
        headY = s.headY;
        for (int i = 0; i < size; i++)
        {
            tailX[i] = s.tailX[i];
            tailY[i] = s.tailY[i];
        }
        direction = s.direction;
    }
} S; //플레이어 1
Snake S2; //플레이어 2

//Frui 클래스: 과일 위치, 점수, 최대 점수 관리
class Fruit : public Snake
{
public:
    int fruitX; //좌표
    int fruitY;
    int score; //플레이어 1, 2의 점수
    int score2;
    int max_score; //레벨업을 위한 목표 점수

    Fruit()
    {
        score = score2 = 0;
        max_score = 40;
        fruit_random_pos();
    }

    //과일을 무작위로 재배치
    void fruit_random_pos()
    {
        fruitX = 1 + (rand() % (BoardX - 2));
        fruitY = 1 + (rand() % (BoardY - 2));
    }

    //플레이어 1의 과일 먹기 처리
    void fruit_eat()
    {
        if (S.headX == fruitX && S.headY == fruitY)
        {
            fruit_random_pos(); //과일 위치 재배치
            score += 10; //점수 증가
            S.increase_size(); //과일을 먹었다면 사이즈+1
        }
        S.tail_movement(); //꼬리 위치 처리
    }

    //플레이어 2의 과일 먹기 처리
    void fruit_eat2()
    {
        if (S2.headX == fruitX && S2.headY == fruitY) 
        {
            fruit_random_pos();
            score2 += 10;         
            S2.increase_size();
        }
        S2.tail_movement();
    }
} F;

// Object 클래스: 장애물/지뢰/버프를 관리
class Object 
{
public:
    int x, y; //오브젝트의 위치
    objectType type; //오브젝트의 종류

    Object()
        : x(0), y(0), type(OBSTACLE) {}

    Object(int _x, int _y, objectType _t)
        : x(_x), y(_y), type(_t) {}
};

vector<Object> objects; //모든 오브젝트를 관리할 벡터

//level 클래스: 레벨별 장애물 배치, 레벨업 관리
class level : public Fruit
{
public:
    int lev; //현재 레벨
    int max_level; //최대 레벨
    char mode; //모드 종류, G/A/M
    level() : lev(1), max_level(10) {}

    //새 오브젝트 위치가 기존 요소와 겹치는지 검사
    bool isOverlap(int nx, int ny)
    {
        if (nx == F.fruitX && ny == F.fruitY) return true;
        if (nx == S.headX && ny == S.headY) return true;
        for (int i = 0; i < S.size; ++i) {
            if (S.tailX[i] == nx && S.tailY[i] == ny)
                return true;
        }
        for (auto& o : objects) {
            if (o.x == nx && o.y == ny)
                return true;
        }
        return false;
    }

    //레벨업 처리: 목표 점수 달성 시 장애물 추가, 화면 축소 등 
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

            //장애물 1개 생성
            {
                int ox, oy;
                do
                {
                    ox = 1 + rand() % (BoardX - 2);
                    oy = 1 + rand() % (BoardY - 2);
                } while (isOverlap(ox, oy));
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

//Game 클래스: 실제 게임 진행 흐름, 화면 표시, 충돌 처리
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

    //게임판 출력
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

    //한 프레임 치 진행: 이동, 충돌, 과일, 레벨업, 화면 갱신
    void game_flow()
    {
        //멀티모드: 머리끼리 충돌 즉시 게임 오버
        if (L.mode == 'M' && S.headX == S2.headX && S.headY == S2.headY) {
            set_screen();
            cout << RED << "\n\n\tHeads collided! Game Over.\n" << RESET;
            Sleep(1000);
            flow_after_gameover();
            return;  // 나머지 로직 스킵
        }

        //오브젝트 충돌 처리
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
        //일반 게임 흐름
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

    //게임 오버 판단
    bool game_over()
    {
        for (int i = 5; i < S.size; i++)
            //자기 자신에게 부딪혔을 때
            if (S.tailX[i] == S.headX && S.tailY[i] == S.headY)
                return 1;
        if (L.mode == 'M')
        {
            //멀티모드일 때, 두번째 뱀도 확인
            for (int i = 5; i < S2.size; i++)
                if (S2.tailX[i] == S2.headX && S2.tailY[i] == S2.headY)
                    return true;  // 추가
        }
        if (L.mode == 'G'||L.mode=='M')
        {
            //advanced mode가 아니라면, 벽 통과 시 반대편으로 이동
            if (S.headX >= BoardX)
                S.headX = 0;
            else if (S.headX < 0)
                S.headX = BoardX - 1;
            if (S.headY >= BoardY)
                S.headY = 0;
            else if (S.headY < 0)
                S.headY = BoardY - 1;

            //멀티모드의 경우 두번째 뱀도 처리
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
            //advanced 모드라면 게임 종료
            if (S.headX == 0 || S.headX == BoardX - 1 || S.headY == 0 || S.headY == BoardY)
                return 1;
        }
        return 0;
    }

    //게임 종료 시, 재시작/종료 선택 처리
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
                //게임을 재시작한다면

                //뱀 초기화
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
                set_screen(); //화면 초기화
            }
            else if (flow == 2)
            {
                //게임 재시작하지 않는다면
                U.set_score(F.score);
                U.save(); //유저 정보 저장
                exit(0); //종료
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

    //멀티모드 시, 출력값 위해 유저 1과 2의 사망 여부 관리
    bool game_over1() 
    {
        // 1) 서로의 머리가 부딪혔을 때
        if (S.headX == S2.headX && S.headY == S2.headY)
            return true;
        // 2) 유저 1이 자신의 몸과 부딪혔을 때
        for (int i = 5; i < S.size; ++i) {
            if (S.tailX[i] == S.headX && S.tailY[i] == S.headY)
                return true;
        }
        return false;
    }
    bool game_over2() 
    {
        // 1) 서로의 머리가 부딪혔을 때
        if (S2.headX == S.headX && S2.headY == S.headY)
            return true;
        // 2) 유저 2가 자신의 몸과 부딪혔을 때
        for (int i = 5; i < S2.size; ++i) 
        {
            if (S2.tailX[i] == S2.headX && S2.tailY[i] == S2.headY)
                return true;
        }
        return false;
    }
}*G;

//게임 상태를 파일에 저장하는 함수
void save_game()
{
    ofstream out("Game.txt"); //Game.txt 파일을 쓰기 모드로 열기
    try
    {
        if (out.is_open())
        {
            //화면 크기, 보드 크기 정보 저장
            out << BoardX << endl;
            out << BoardY << endl;
            //플레이어 1의 머리 위치, 뱀 길이, 방향 저장
            out << S.headX << endl;
            out << S.headY << endl;
            out << S.size << endl;
            out << S.direction << endl;
            //뱀 꼬리 좌표를 순서대로 저장
            for (int i = 0; i < S.size; i++)
            {
                out << S.tailX[i] << endl;
                out << S.tailY[i] << endl;
            }
            //과일 위치, 최대 점수, 현재 점수 저장
            out << F.fruitX << endl;
            out << F.fruitY << endl;
            out << F.max_score << endl;
            out << F.score << endl;
            out << L.lev << endl;
            out << G->mode;
            out.close();

            cout << "Game Saved...";
            exit(0); //종료
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

//저장된 파일로부터 게임을 불러오는 함수
void resume_game()
{
    ifstream in("Game.txt"); //Game.txt 파일을 읽기 모드로 열기
    try
    {
        if (in.is_open())
        {
            //저장된 순서대로 데이터를 읽어서 변수에 복원
            in >> BoardX;
            in >> BoardY;
            in >> S.headX; 
            in >> S.headY;
            in >> S.size;
            in >> S.direction;
            //꼬리 배열 재할당 후 좌표 읽어오기
            delete[] S.tailX;
            delete[] S.tailY;
            S.tailX = new int[S.size];
            S.tailY = new int[S.size];
            for (int i = 0; i < S.size; i++)
            {
                in >> S.tailX[i];
                in >> S.tailY[i];
            }

            //과일 위치, 최대 점수, 현재 점수 복원
            in >> F.fruitX;
            in >> F.fruitY;
            in >> F.max_score;
            in >> F.score;

            //레벨과 모드 정보 복원
            in >> L.lev;
            in >> G->mode;

            //방향 초기화
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

//콘솔 화면 전체를 지운 후, 새로 그리도록 초기화하는 함수
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

//콘솔 커서를 보이지 않도록 설정
void hide_cursor()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}

//키 입력을 감지하여 뱀의 방향을 변경하거나 저장 키 처리
void handle_input() 
{
    if (kbhit()) 
    {
        char ch = getch();
        switch (ch)
        {
            //WASD 키로 플레이어 1 이동
            case 'w': case 'a': case 's': case 'd':
                S.direction = ch;
                break;
            //IJKL 키로 플레이어 2 이동 (멀티일 떄만)
            case 'i': case 'j': case 'k': case 'l':
                if (L.mode == 'M') S2.direction = ch; // 멀티일 때만 P2
                break;
            //x키를 누르면 게임 저장
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

    srand((unsigned)time(nullptr)); //랜덤 시드 설정
    hide_cursor(); //커서 숨기기

    int flow;
    system("color 8A"); //콘솔 컬러 설정
    set_screen(); //화면 초기화

    //메인 메뉴 문자열
    char s1[Max];
    char s2[Max];
    char s3[Max];
    char s4[Max];

    strcpy(s1, "1.New Game"); //새 게임 시작
    strcpy(s2, "2.Resume Previous Game"); //이전 게임 불러오기
    strcpy(s3, "3.Show the User list."); //사용자 목록 보기
    strcpy(s4, "4.Exit Game"); //게임 종료

    //메뉴 출력
    cout << "\n\t" << s1;
    cout << "\n\t" << s2;
    cout << "\n\t" << s3;
    cout << "\n\t" << s4 << "\n\t";
    cin >> flow; //사용자 선택 입력

    try
    {
        if (flow == 1 || flow == 2 || flow == 3)
        {
            switch (flow)
            {
            case 1: //새 게임
            {
                U.get(); //플레이어 이름 입력
                Game G1;
                G = &G1;
                set_screen();

                //모드 선택 메뉴
                cout << "\n\tSelect One Mode:\n";
                cout << "\t1.General Mode\n";
                cout << "\t2.Advanced Mode\n";
                cout << "\t3.Multiplayer Mode\n\t";
                cin >> flow;

                if (flow == 1)
                    L.mode = 'G'; //일반 모드
                else if (flow == 2)
                    L.mode = 'A'; //어드밴스 모드
                else if (flow == 3)
                {
                    L.mode = 'M'; //멀티플레이어 모드

                    //플레이어2 초기화
                    S2.headX = BoardX / 4;
                    S2.headY = BoardY / 2;
                    S2.direction = '\0';
                    S2.size = 1;
                    delete[] S2.tailX;  delete[] S2.tailY;
                    S2.tailX = new int[MAX_TAIL];
                    S2.tailY = new int[MAX_TAIL];
                }

                set_screen();

                //게임 루프 시작
                do {
                    handle_input(); //키 입력 처리
                    S.move_snake(); //플레이어1 이동
                    if (L.mode == 'M') S2.move_snake(); //플레이어2 이동
                   
                    //한 프레임 진행
                    G->game_flow();

                    // 멀티 모드 충돌(게임 오버) 체크
                    if (L.mode == 'M' && (G->game_over1() || G->game_over2())) 
                    {
                        set_screen();

                        // 승자 판정 및 점수 저장
                        if (G->game_over1() && !G->game_over2()) 
                        {
                            cout << "\nPlayer2 Wins!\n";
                            U.set_score(F.score2);
                        }
                        else if (!G->game_over1() && G->game_over2()) 
                        {
                            cout << "\nPlayer1 Wins!\n";
                            U.set_score(F.score);
                        }
                        else 
                        {
                            // 동시 충돌
                            if (F.score > F.score2) { cout << "\nPlayer1 Wins!\n"; U.set_score(F.score); }
                            else if (F.score2 > F.score) { cout << "\nPlayer2 Wins!\n"; U.set_score(F.score2); }
                            else { cout << "\nDraw!\n";       U.set_score(F.score); }
                        }

                        U.save(); //결과 저장

                        G->flow_after_gameover(); //재시작 or 종료
                    }
                    Sleep(75); //프레임 조절 
                } while (1);

                break;
            }
            case 2: //이전 게임 불러오기
            {
                Game G2;
                G = &G2;
                set_screen();
                resume_game(); //저장된 상태 복원

                //게임 루프, case1과 동일
                do {
                    handle_input();
                    S.move_snake();

                    if (L.mode == 'M') {
                        S2.move_snake();
                    }

                    G->game_flow();

                    if (L.mode == 'M' && (G->game_over1() || G->game_over2())) 
                    {
                        set_screen();

                        if (G->game_over1() && !G->game_over2()) 
                        {
                            cout << "\nPlayer2 Wins!\n";
                            U.set_score(F.score2);
                        }
                        else if (!G->game_over1() && G->game_over2()) 
                        {
                            cout << "\nPlayer1 Wins!\n";
                            U.set_score(F.score);
                        }
                        else 
                        {
                            if (F.score > F.score2) { cout << "\nPlayer1 Wins!\n"; U.set_score(F.score); }
                            else if (F.score2 > F.score) { cout << "\nPlayer2 Wins!\n"; U.set_score(F.score2); }
                            else { cout << "\nDraw!\n";       U.set_score(F.score); }
                        }

                        U.save();

                        G->flow_after_gameover();
                    }

                    Sleep(75);
                } while (1);
                break;
            }

            case 3: //유저 목록 출력
            {
                U.show_list();
            }
            case 4: //종료
            {
                exit(0);
                break;
            }
            }
        }
        else
            throw flow; //잘못된 입력 예외 처리
    }
    catch (int f)
    {
        cout << "Entered number is invalid...";
        exit(0);
    }
}
