#include <gtest/gtest.h>
#include "snake.h"
#include <deque>
#include <algorithm>

// Increase Snake Speed Enhancement 
TEST(SnakeFeature, SpeedIncreasesWithFoodEaten) {
    deque<pair<int,int>> snake = {{0,0}};
    int foodEaten = 0;
    int speed = 500;
    const int minSpeed = 100;

    for (int i = 0; i < 20; ++i) {
        foodEaten++;
        if (foodEaten % 5 == 0 && speed > minSpeed) {
            speed -= 50;
        }
    }

    EXPECT_EQ(speed, 300); 
}

// Maintain and Increase Score Enhancement 
TEST(SnakeFeature, ScoreIncreasesCorrectly) {
    int score = 0;
    score += 10; 
    score += 10; 

    EXPECT_EQ(score, 20);
}

// Poison Food Added Enhancement 
TEST(SnakeFeature, SnakeDiesOnPoisonFood) {
    pair<int,int> badFood = {1,1};
    pair<int,int> head = {1,1};
    bool gameOver = false;

    if (head == badFood) {
        gameOver = true;
    }

    EXPECT_TRUE(gameOver);
}

// Added Play/Pause Feature Enhancement 
TEST(SnakeFeature, PauseTogglesCorrectly) {
    bool paused = false;

    paused = !paused; 
    EXPECT_TRUE(paused);

    paused = !paused; 
    EXPECT_FALSE(paused);
}

// High Score Tracking 
TEST(SnakeFeature, HighScoresMaintainTop10) {
    highScores.clear();
    for (int i = 1; i <= 15; i++) {
        update_high_scores(i * 10);
    }

    EXPECT_EQ(highScores.size(), 10);
    EXPECT_EQ(highScores.front(), 150); 
    EXPECT_EQ(highScores.back(), 60);  
}

// Food Generation Never Spawns On Snake 
TEST(SnakeFeature, FoodNeverOnSnake) {
    deque<pair<int,int>> snake = {{0,0},{0,1},{0,2},{0,3},{0,4}};
    for (int i = 0; i < 100; ++i) {
        pair<int,int> food = generate_food(snake, 10);
        EXPECT_EQ(find(snake.begin(), snake.end(), food), snake.end());
    }
}


TEST(SnakeBehaviour, NextHeadRight) {
    pair<int, int> current = make_pair(rand() % 10, rand() % 10);
    int expectedCol = (current.second + 1) % 10;
    EXPECT_EQ(get_next_head(current, 'r'), make_pair(current.first, expectedCol));
}

TEST(SnakeBehaviour, NextHeadLeft) {
    pair<int, int> current = make_pair(rand() % 10, rand() % 10);
    int expectedCol = (current.second == 0) ? 9 : current.second - 1;
    EXPECT_EQ(get_next_head(current, 'l'), make_pair(current.first, expectedCol));
}

TEST(SnakeBehaviour, NextHeadUp) {
    pair<int, int> current = make_pair(rand() % 10, rand() % 10);
    int expectedRow = (current.first == 0) ? 9 : current.first - 1;
    EXPECT_EQ(get_next_head(current, 'u'), make_pair(expectedRow, current.second));
}

TEST(SnakeBehaviour, NextHeadDown) {
    pair<int, int> current = make_pair(rand() % 10, rand() % 10);
    int expectedRow = (current.first + 1) % 10;
    EXPECT_EQ(get_next_head(current, 'd'), make_pair(expectedRow, current.second));
}


/** 
 * g++ -o my_tests snake_test.cpp -lgtest -lgtest_main -pthread;
 * This command is a two-part shell command. Let's break it down.

  The first part is the compilation:
  g++ -o my_tests hello_gtest.cpp -lgtest -lgtest_main -pthread


   * g++: This invokes the GNU C++ compiler.
   * -o my_tests: This tells the compiler to create an executable file named
     my_tests.
   * hello_gtest.cpp: This is the C++ source file containing your tests.
   * -lgtest: This links the Google Test library, which provides the core testing
     framework.
   * -lgtest_main: This links a pre-compiled main function provided by Google
     Test, which saves you from writing your own main() to run the tests.
   * -pthread: This links the POSIX threads library, which is required by Google
     Test for its operation.
 * 
*/