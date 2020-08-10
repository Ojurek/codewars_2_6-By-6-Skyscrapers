#include <iostream>
#include <cassert>
#include <vector>
#include <algorithm>
#include <set>

class PuzzleBoard
{
private:
public:
    std::vector<int> upperClue;
    std::vector<int> downClue;
    std::vector<int> leftClue;
    std::vector<int> rightClue;

    PuzzleBoard(const std::vector<int> &clues);
    void setClueArray(const std::vector<int> &clues);
    int removeSize(int size);
};

PuzzleBoard::PuzzleBoard(const std::vector<int> &clues)
{
    std::vector<std::vector<std::vector<int>>> board;
    for (std::vector<std::vector<int>> square : board)
    {
        square.push_back({1, 2, 3, 4, 5, 6});
    }

    setClueArray(clues);
}

void PuzzleBoard::setClueArray(const std::vector<int> &clues)
{
    for (int i = 0; i < 6; i++)
    {
        upperClue.push_back(clues[i]);
        downClue.push_back(clues[17 - i]);
        leftClue.push_back(clues[23 - i]);
        rightClue.push_back(clues[i + 6]);
    }
}

std::vector<std::vector<int>> SolvePuzzle(const std::vector<int> &clues)
{
    std::vector<std::vector<int>> board;

    return board;
}

int main()
{
    /*   std::set<int> cont{10, 20, 1, 2, 3};

    auto print = [](const int &n) { std::cout << " " << n; };

    std::cout << "Start:";
    std::for_each(cont.begin(), cont.end(), print);
    std::cout << '\n';

    // Extract node handle and change key

    auto search = cont.find(2);
    if (search != cont.end())
    {
        cont.erase(search);
    }
    std::cout << "After extract and before insert:";
    std::for_each(cont.begin(), cont.end(), print);
    std::cout << '\n';
*/
    static std::vector<std::vector<int>> clues = {
        {3, 2, 2, 3, 2, 1,
         1, 2, 3, 3, 2, 2,
         5, 1, 2, 2, 4, 3,
         3, 2, 1, 2, 2, 4},
        {0, 0, 0, 2, 2, 0,
         0, 0, 0, 6, 3, 0,
         0, 4, 0, 0, 0, 0,
         4, 4, 0, 3, 0, 0},
        {0, 3, 0, 5, 3, 4,
         0, 0, 0, 0, 0, 1,
         0, 3, 0, 3, 2, 3,
         3, 2, 0, 3, 1, 0}};

    static std::vector<std::vector<std::vector<int>>> expected = {
        {{2, 1, 4, 3, 5, 6},
         {1, 6, 3, 2, 4, 5},
         {4, 3, 6, 5, 1, 2},
         {6, 5, 2, 1, 3, 4},
         {5, 4, 1, 6, 2, 3},
         {3, 2, 5, 4, 6, 1}},
        {{5, 6, 1, 4, 3, 2},
         {4, 1, 3, 2, 6, 5},
         {2, 3, 6, 1, 5, 4},
         {6, 5, 4, 3, 2, 1},
         {1, 2, 5, 6, 4, 3},
         {3, 4, 2, 5, 1, 6}},
        {{5, 2, 6, 1, 4, 3},
         {6, 4, 3, 2, 5, 1},
         {3, 1, 5, 4, 6, 2},
         {2, 6, 1, 5, 3, 4},
         {4, 3, 2, 6, 1, 5},
         {1, 5, 4, 3, 2, 6}}};

    PuzzleBoard test1({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23});

    assert(SolvePuzzle(clues[0]) == expected[0]);
    assert(SolvePuzzle(clues[1]) == expected[1]);
    assert(SolvePuzzle(clues[2]) == expected[2]);
}