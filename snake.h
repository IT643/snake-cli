#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h> // for system clear
#include <map>
#include <deque>
#include <algorithm>
using namespace std;
using std::chrono::system_clock;
using namespace std::this_thread;
char direction='r';
vector<int> highScores;
bool paused = false;


void input_handler(){
    // change terminal settings
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    // turn off canonical mode and echo
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    map<char, char> keymap = {{'d', 'r'}, {'a', 'l'}, {'w', 'u'}, {'s', 'd'}, {'q', 'q'}};
    while (true) {
        char input = getchar();
        if (keymap.find(input) != keymap.end()) {
            // This now correctly modifies the single, shared 'direction' variable
            direction = keymap[input];
        }else if (input == 'q'){
            exit(0);
        }else if (input == 'p') {
            paused = !paused;
        }
        // You could add an exit condition here, e.g., if (input == 'q') break;
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}


void render_game(int size, deque<pair<int, int>> &snake, pair<int, int> food, pair<int, int> badFood, int score){
    for(size_t i=0;i<size;i++){
        for(size_t j=0;j<size;j++){
            if (i == food.first && j == food.second){
                cout << "🍎";
            }else if (i == badFood.first && j == badFood.second){
                cout << "💀";
            }else if (find(snake.begin(), snake.end(), make_pair(int(i), int(j))) != snake.end()) {
                cout << "🐍";
            }else{
                cout << "⬜";
            }
        }
        cout << endl;
    }

    cout << " Score: " << score << (paused ? " | PAUSED (press 'p' to resume)" : "") << endl;
}

pair<int,int> get_next_head(pair<int,int> current, char direction){
    pair<int, int> next; 
    if(direction =='r'){
        next = make_pair(current.first,(current.second+1) % 10);
    }else if (direction=='l')
    {
        next = make_pair(current.first, current.second==0?9:current.second-1);
    }else if(direction =='d'){
            next = make_pair((current.first+1)%10,current.second);
        }else if (direction=='u'){
            next = make_pair(current.first==0?9:current.first-1, current.second);
        }
    return next;
    
}

pair<int, int> generate_food(const deque<pair<int, int>> &snake, int size, pair<int,int> avoid = {-1,-1}) {
    pair<int, int> food;
    do {
        food = make_pair(rand() % size, rand() % size);
    } while (find(snake.begin(), snake.end(), food) != snake.end() || 
        (food == avoid)
    );
    return food;
}

void update_high_scores(int score) {
    highScores.push_back(score);
    sort(highScores.rbegin(), highScores.rend()); 
    if (highScores.size() > 10) {
        highScores.pop_back(); 
    }
}

void show_high_scores() {
    cout << "\nTOP 10 HIGH SCORES : \n";
    for (size_t i = 0; i < highScores.size(); i++) {
        cout << i + 1 << ". " << highScores[i] << endl;
    }
}

void game_play(){
    system("clear");
    deque<pair<int, int>> snake;
    snake.push_back(make_pair(0,0));

    int score = 0;

    pair<int, int> food = generate_food(snake, 10);
    pair<int, int> badFood = generate_food(snake, 10, food);
    for(pair<int, int> head=make_pair(0,1); ; ){
        // send the cursor to the top
        cout << "\033[H";
        // check self collision

        if(paused) {
            render_game(10, snake, food, badFood, score);
            sleep_for(200ms);
            head = snake.back();
            continue;
        }

        head = get_next_head(head, direction);

        if (find(snake.begin(), snake.end(), head) != snake.end()) {
            system("clear");
            cout << "Game Over" << endl;

            int finalScore = snake.size();
            cout << "Your score: " << finalScore << endl;

            update_high_scores(finalScore);
            show_high_scores();
            break;
        }else if (head.first == food.first && head.second == food.second) {
            // grow snake
            food = generate_food(snake, 10, badFood);
            snake.push_back(head);     
            score += 10;       
        }else if (head.first == badFood.first && head.second == badFood.second) {
            system("clear");
            cout << "Game Over (ate bad food 💀)" << endl;
            int finalScore = snake.size();
            cout << "Your score: " << finalScore << endl;
            update_high_scores(finalScore);
            show_high_scores();
            break;
        }else{
            // move snake
            snake.push_back(head);
            snake.pop_front();
        }

        if (rand() % 5 == 0) { 
            badFood = generate_food(snake, 10, food);
        }

        render_game(10, snake, food, badFood, score);
        cout << "length of snake: " << snake.size() << endl;
    
        sleep_for(500ms);
    }
}
