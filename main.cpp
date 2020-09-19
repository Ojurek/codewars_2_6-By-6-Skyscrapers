#include <iostream>
#include <cassert>
#include <vector>
#include <algorithm>
#include <array>
#include <set>

#define BOARD_SIZE 6

typedef std::vector<int> Field;
typedef std::vector<Field> Row;
typedef std::vector<Row> Board;

static Board finalBoard;

class PuzzleBoard
{
private:
public:
    Board board;

    int totalSizes = BOARD_SIZE * BOARD_SIZE * BOARD_SIZE;
    std::array<int, BOARD_SIZE> rowSizes;
    std::array<int, BOARD_SIZE> columnSizes;
    std::vector<int> upperClue;
    std::vector<int> downClue;
    std::vector<int> leftClue;
    std::vector<int> rightClue;

    PuzzleBoard(const std::vector<int> &clues);
    void setClueArray(const std::vector<int> &clues);
    bool removeSize(int row, int column, int removeThisSize);
    bool checkRow(int row);
    bool checkColumn(int column);
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
        rowSizes[i] = BOARD_SIZE * BOARD_SIZE;
        columnSizes[i] = BOARD_SIZE * BOARD_SIZE;
    }

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        //check upper clue
        if (upperClue[i] == 1)
        {
            for (int j = 1; j < BOARD_SIZE; j++)
            {
                removeSize(0, i, j);
            }
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
            for (int j = 1; j < BOARD_SIZE; j++)
            {
                removeSize(i, 0, j);
            }
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
            for (int j = 1; j < BOARD_SIZE; j++)
            {
                removeSize(i, (BOARD_SIZE - 1), j);
            }
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
            for (int j = 1; j < BOARD_SIZE; j++)
            {
                removeSize((BOARD_SIZE - 1), i, j);
            }
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

bool PuzzleBoard::removeSize(int row, int column, int removeThisSize)
{
    int deletedElements;
    //std::cout << "remove: " << removeThisSize << " row: " << row << " Column: " << column << std::endl;
    int sizesBeforeErasing = board[row][column].size();
    if (sizesBeforeErasing == 1)
    {
        return true; //misleading
    }
    printBoard();
    board[row][column].erase(std::remove(board[row][column].begin(), board[row][column].end(), removeThisSize), board[row][column].end());

    deletedElements = sizesBeforeErasing - board[row][column].size();
    assert((deletedElements == 0) || (deletedElements == 1));
    totalSizes -= deletedElements;
    rowSizes[row] -= deletedElements;
    columnSizes[column] -= deletedElements;

    if ((deletedElements > 0) && (rowSizes[row] == BOARD_SIZE))
    {
        if (checkRow(row) == false)
        {
            return false;
        }
    }

    if ((deletedElements > 0) && (columnSizes[column] == BOARD_SIZE))
    {
        if (checkColumn(column) == false)
        {
            return false;
        }
    }

    std::cout << " Total Sizes = " << totalSizes << std::endl;

    if ((deletedElements > 0) && (board[row][column].size() == 1))
    {
        for (int i = 0; i < row; i++)
        {
            if (removeSize(i, column, board[row][column][0]) == false)
            {
                return false;
            }
        }

        for (int i = row + 1; i < BOARD_SIZE; i++)
        {
            if (removeSize(i, column, board[row][column][0]) == false)
            {
                return false;
            }
        }

        for (int i = 0; i < column; i++)
        {
            if (removeSize(row, i, board[row][column][0]) == false)
            {
                return false;
            }
        }

        for (int i = column + 1; i < BOARD_SIZE; i++)
        {
            if (removeSize(row, i, board[row][column][0]) == false)
            {
                return false;
            }
        }

        std::cout << std::endl;
    }
    return true;
}

bool PuzzleBoard::checkRow(int row)
{
    std::cout << std::endl
              << "checkRow: " << row << std::endl;
    std::set<int> allSizes;
    int highestSeenBuilding = 0;
    int numberOfSeenBuildings = 0;

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        allSizes.insert(board[row][i][0]);
    }
    if (allSizes.size() != BOARD_SIZE)
    {
        return false;
    }

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        if (board[row][i][0] > highestSeenBuilding)
        {
            numberOfSeenBuildings++;
            highestSeenBuilding = board[row][i][0];
        }
    }

    if (numberOfSeenBuildings != leftClue[row])
    {
        return false;
    }

    highestSeenBuilding = 0;
    numberOfSeenBuildings = 0;

    for (int i = (BOARD_SIZE - 1); i >= 0; i--)
    {
        if (board[row][i][0] > highestSeenBuilding)
        {
            numberOfSeenBuildings++;
            highestSeenBuilding = board[row][i][0];
        }
    }

    if (numberOfSeenBuildings != rightClue[row])
    {
        return false;
    }

    return true;
}

bool PuzzleBoard::checkColumn(int column)
{
    std::cout << std::endl
              << "checkColumn " << column << std::endl;
    std::set<int> allSizes;
    int highestSeenBuilding = 0;
    int numberOfSeenBuildings = 0;

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        allSizes.insert(board[i][column][0]);
    }

    if (allSizes.size() != BOARD_SIZE)
    {
        return false;
    }

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        if (board[i][column][0] > highestSeenBuilding)
        {
            numberOfSeenBuildings++;
            highestSeenBuilding = board[i][column][0];
        }
    }

    if (numberOfSeenBuildings != upperClue[column])
    {
        return false;
    }

    highestSeenBuilding = 0;
    numberOfSeenBuildings = 0;

    for (int i = (BOARD_SIZE - 1); i >= 0; i--)
    {
        if (board[i][column][0] > highestSeenBuilding)
        {
            numberOfSeenBuildings++;
            highestSeenBuilding = board[i][column][0];
        }
    }

    if (numberOfSeenBuildings != downClue[column])
    {
        return false;
    }

    return true;
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
        std::cout << "* " << rightClue[i] << "* "
                  << "elements: " << rowSizes[i] << std::endl;
    }
    std::cout << "*********************************************************" << std::endl;
    std::cout << "       ";
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        std::cout << downClue[i] << "       ";
    }
}

bool reduceElement(PuzzleBoard temp_board)
{
    int sizes_in_field;
    PuzzleBoard new_board = temp_board;
    bool breaked = false;

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
                            if (new_board.removeSize(i, j, temp_board.board[i][j][l]) == false)
                            {
                                new_board = temp_board;
                                breaked = true;
                                break;
                            }
                        }
                    }

                    std::cout << std::endl
                              << "For row: " << i << " For column: " << j << " sizes_in_field=" << sizes_in_field << std::endl;
                    new_board.printBoard();
                    if (breaked != true)
                    {
                        if (new_board.totalSizes == BOARD_SIZE * BOARD_SIZE)
                        {
                            finalBoard = new_board.board;
                            return true;
                        }
                        if (reduceElement(new_board) == false)
                        {
                            std::cout << "reduce element returned false********************************" << std::endl;
                            //return false;
                        }
                        else if (new_board.totalSizes == BOARD_SIZE * BOARD_SIZE)
                        {
                            finalBoard = new_board.board;
                            return true;
                        }
                    }
                    new_board = temp_board;
                    breaked = false;
                }
                std::cout << "all elements for row " << i << ", column " << j << " tested" << std::endl;
                return false;
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
    reduceElement(puzzle_board);
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