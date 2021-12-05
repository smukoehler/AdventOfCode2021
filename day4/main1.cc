#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

struct board
{
    int values[5][5];
};

void PrintBoard(board card)
{
    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 5; ++j)
        {
            std::cout << card.values[j][i] << ", ";
            if (j == 4)
            {
                std::cout << std::endl;
            }
        }
    }
    std::cout << std::endl;

    return;
}

std::vector<int> VectorizeCallList(std::string call_list)
{
    std::vector<int> call_numbers;
    std::stringstream call_stream(call_list); //create string stream from the string
    while (call_stream.good())
    {
        std::string number;
        std::getline(call_stream, number, ','); //get first string delimited by comma
        call_numbers.push_back(std::stoi(number));
    }
    return call_numbers;
}

void FindInBoard(board &card, int number)
{
    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 5; ++j)
        {
            if (card.values[i][j] == number)
            {
                card.values[i][j] = -1;
            }
        }
    }
    return;
}

bool FindBingo(board card)
{
    // Check for bingo rows.
    for (int i = 0; i < 5; ++i)
    {
        int row_sum = 0;
        for (int j = 0; j < 5; ++j)
        {
            row_sum = row_sum + card.values[i][j];
        }
        if (row_sum == -5){
            return true;
        }
    }

    // Check for bingo cols.
    for (int i = 0; i < 5; ++i)
    {
        int col_sum = 0;
        for (int j = 0; j < 5; ++j)
        {
            col_sum = col_sum + card.values[j][i];
        }
        if (col_sum == -5){
            return true;
        }
    }

    return false;
}

struct Winner {
    bool won = false;
    int last_number;
    board card;
};

Winner PlayBingo(std::vector<board> boards, std::vector<int> call_numbers) {
    for (auto &number : call_numbers)
    {
        for (auto &card : boards)
        {
            FindInBoard(card, number);
            if (FindBingo(card))
            {
                std::cout << "Bingo!" << std::endl;
                Winner winner{.won = true, .last_number = number, .card = card};
                return winner;
            }
        }
    }
    Winner not_won;
    return not_won;
}

int ScoreWinner(board card) {
    int score = 0;
    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 5; ++j)
        {
            if (card.values[i][j] != -1) {
                score = score + card.values[i][j];
            }
        }
    }
    return score;
}

int main()
{
    std::string word;
    std::string call_list;
    int num_words = 0;
    board card;
    int row = 0;
    int col = 0;
    std::vector<board> boards;
    std::ifstream myfile("input.txt");
    if (myfile.is_open())
    {
        while (myfile >> word)
        {
            if (num_words == 0)
            {
                call_list = word;
            }
            else
            {
                // Parse bingo boards.
                // std::cout << word << std::endl;
                card.values[row][col] = std::stoi(word);
                ++row;
                if (row > 4)
                {
                    row = 0;
                    ++col;
                    if (col == 5)
                    {
                        boards.push_back(card);
                        col = 0;
                    }
                }
            }
            ++num_words;
        }
        myfile.close();
    }

    std::cout << "Number of words: " << num_words << std::endl;
    std::cout << "Call list: " << call_list << std::endl;

    // Parse the call list into a vector.
    std::vector<int> call_numbers = VectorizeCallList(call_list);

    // Play bingo.
    Winner winner = PlayBingo(boards, call_numbers);

    PrintBoard(winner.card);

    std::cout << "Winning call number: " << winner.last_number << std::endl;

    int score = ScoreWinner(winner.card);
    std::cout << "Score of the winning card: " << score << std::endl;

    std::cout << "Final score: " << score * winner.last_number << std::endl;

    return 0;
}