#include <iostream>
#include <cassert>
#include <vector>
#include <algorithm>

#define BOARD_SIZE 6

typedef std::vector<int> Field;
typedef std::vector<Field> Row;
typedef std::vector<Row> Board;

class PuzzleBoard
{
private:
public:
    Board board;
    int totalSizes = BOARD_SIZE * BOARD_SIZE * BOARD_SIZE;
    std::vector<int> upperClue;
    std::vector<int> downClue;
    std::vector<int> leftClue;
    std::vector<int> rightClue;

    PuzzleBoard(const std::vector<int> &clues);
    void setClueArray(const std::vector<int> &clues);
    void removeSize(int row, int column, int removeThisSize);
    void printBoard();
};

PuzzleBoard::PuzzleBoard(const std::vector<int> &clues)
{
    Field field;
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        field.push_back(i + 1);
    }
    Row row(BOARD_SIZE, field);

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        board.push_back(row);
    }

    setClueArray(clues);

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        //check upper clue
        if (upperClue[i] == 1)
        {
            board[0][i] = {BOARD_SIZE};
        }
        if (upperClue[i] > 1)
        {
            for (int j = 0; j < (upperClue[i] - 1); j++)
            {
                for (int k = 0; k < (upperClue[i] - j - 1); k++)
                    removeSize(j, i, BOARD_SIZE - k);
            }
        }

        //check left clue
        if (leftClue[i] == 1)
        {
            board[i][0] = {BOARD_SIZE};
        }
        if (leftClue[i] > 1)
        {
            for (int j = 0; j < (leftClue[i] - 1); j++)
            {
                for (int k = 0; k < (leftClue[i] - j - 1); k++)
                    removeSize(i, j, BOARD_SIZE - k);
            }
        }
        //check right clue
        if (rightClue[i] == 1)
        {
            board[i][BOARD_SIZE - 1] = {BOARD_SIZE};
        }
        if (rightClue[i] > 1)
        {
            for (int j = 0; j < (rightClue[i] - 1); j++)
            {
                for (int k = 0; k < (rightClue[i] - j - 1); k++)
                    removeSize(i, BOARD_SIZE - j - 1, BOARD_SIZE - k);
            }
        }
        //check down clue
        if (downClue[i] == 1)
        {
            board[BOARD_SIZE - 1][i] = {BOARD_SIZE};
        }
        if (downClue[i] > 1)
        {
            for (int j = 0; j < (downClue[i] - 1); j++)
            {
                for (int k = 0; k < (downClue[i] - j - 1); k++)
                    removeSize(BOARD_SIZE - j - 1, i, BOARD_SIZE - k);
            }
        }
    }

    printBoard();
}

void PuzzleBoard::setClueArray(const std::vector<int> &clues)
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        upperClue.push_back(clues[i]);
        downClue.push_back(clues[3 * BOARD_SIZE - 1 - i]);
        leftClue.push_back(clues[4 * BOARD_SIZE - 1 - i]);
        rightClue.push_back(clues[i + BOARD_SIZE]);
    }
}

void PuzzleBoard::removeSize(int row, int column, int removeThisSize)
{
    //std::cout << "remove: " << removeThisSize << " row: " << row << " Column: " << column << std::endl;
    int sizesBeforeErasing = board[row][column].size();
    if (sizesBeforeErasing == 1)
    {
        return;
    }

    board[row][column].erase(std::remove(board[row][column].begin(), board[row][column].end(), removeThisSize), board[row][column].end());
    totalSizes = totalSizes + board[row][column].size() - sizesBeforeErasing;

    if (board[row][column].size() == 1)
    {

        for (int i = 0; i < row; i++)
        {
            removeSize(i, column, board[row][column][0]);
        }

        for (int i = row + 1; i < BOARD_SIZE; i++)
        {
            removeSize(i, column, board[row][column][0]);
        }

        for (int i = 0; i < column; i++)
        {
            removeSize(row, i, board[row][column][0]);
        }

        for (int i = column + 1; i < BOARD_SIZE; i++)
        {
            removeSize(row, i, board[row][column][0]);
        }

        std::cout << std::endl;
    }
}

void PuzzleBoard::printBoard()
{
    std::cout << std::endl
              << "Board have " << totalSizes << " elements: " << std::endl;
    std::cout << "       ";
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        std::cout << upperClue[i] << "       ";
    }
    std::cout << std::endl
              << "*********************************************************" << std::endl;

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        std::cout << leftClue[i] << " * ";
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            for (int k = 0; k < board[i][j].size(); k++)
            {
                std::cout << board[i][j][k];
            }
            for (int k = 0; k < (BOARD_SIZE - board[i][j].size()); k++)
            {
                std::cout << " ";
            }
            std::cout << "  ";
        }
        std::cout << "* " << rightClue[i] << std::endl;
    }
    std::cout << "*********************************************************" << std::endl;
    std::cout << "       ";
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        std::cout << downClue[i] << "       ";
    }
}

PuzzleBoard reduceElement(PuzzleBoard temp_board)
{
    int sizes_in_field;
    PuzzleBoard new_board = temp_board;

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            sizes_in_field = temp_board.board[i][j].size();
            if (sizes_in_field > 1)
            {
                for (int k = 0; k < sizes_in_field; k++)
                {

                    for (int l = 0; l < sizes_in_field; l++)
                    {
                        if (k != l)
                        {
                            std::cout << std::endl
                                      << "For row: " << i << " For column: " << j << " remove=" << temp_board.board[i][j][l] << std::endl;
                            new_board.removeSize(i, j, temp_board.board[i][j][l]);
                        }
                    }
                    std::cout << std::endl
                              << "For row: " << i << " For column: " << j << " sizes_in_field=" << sizes_in_field << std::endl;
                    new_board.printBoard();

                    if (new_board.totalSizes == BOARD_SIZE * BOARD_SIZE)
                    {
                        return new_board;
                    }
                    new_board = reduceElement(new_board);

                    if (new_board.totalSizes == BOARD_SIZE * BOARD_SIZE)
                    {
                        return new_board;
                    }
                    new_board = temp_board;
                }
            }
        }
    }
}

std::vector<std::vector<int>> SolvePuzzle(const std::vector<int> &clues)
{
    std::cout << std::endl
              << "Solve function" << std::endl;
    std::vector<std::vector<int>> final_board;

    PuzzleBoard puzzle_board(clues);
    puzzle_board = reduceElement(puzzle_board);
    return final_board;
}

int main()
{
    /* static std::vector<std::vector<int>> clues = {
    {3, 2, 2, 3, 2, 1,
     1, 2, 3, 3, 2, 2,
     5, 1, 2, 2, 4, 3,
     3, 2, 1, 2, 2, 4},
        {0, 0, 0, 2, 2, 0,
         0, 0, 0, 6, 3, 0,
         0, 4, 0, 0, 0, 0,
         4, 4, 0, 3, 0, 0},
    {
        0, 3, 0, 5, 3, 4,
            0, 0, 0, 0, 0, 1,
            0, 3, 0, 3, 2, 3,
            3, 2, 0, 3, 1, 0
    }
};

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
{
    {5, 2, 6, 1, 4, 3},
        {6, 4, 3, 2, 5, 1},
        {3, 1, 5, 4, 6, 2},
        {2, 6, 1, 5, 3, 4},
        {4, 3, 2, 6, 1, 5},
    {
        1, 5, 4, 3, 2, 6
    }
}
}
;
*/
    static std::vector<int> clues0 =
        {3, 2, 2, 3, 2, 1,
         1, 2, 3, 3, 2, 2,
         5, 1, 2, 2, 4, 3,
         3, 2, 1, 2, 2, 4};

    static std::vector<int> clues1 =
        {0, 0, 0, 2, 2, 0,
         0, 0, 0, 6, 3, 0,
         0, 4, 0, 0, 0, 0,
         4, 4, 0, 3, 0, 0};

    SolvePuzzle(clues0);

    /*
    PuzzleBoard test1({3, 2, 2, 3, 2, 1,
                       1, 2, 3, 3, 2, 2,
                       5, 1, 2, 2, 4, 3,
                       3, 2, 1, 2, 2, 4});

    PuzzleBoard test2({0, 0, 0, 2, 2, 0,
                       0, 0, 0, 6, 3, 0,
                       0, 4, 0, 0, 0, 0,
                       4, 4, 0, 3, 0, 0});
*/
    //   assert(SolvePuzzle(clues[0]) == expected[0]);
    //   assert(SolvePuzzle(clues[1]) == expected[1]);
    //   assert(SolvePuzzle(clues[2]) == expected[2]);
}