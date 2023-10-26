#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

// each of our text files contains 1000 words
#define LISTSIZE 1000

// values for colors and score (EXACT == right letter, right place; CLOSE == right letter, wrong place; WRONG == wrong letter)
#define EXACT 2
#define CLOSE 1
#define WRONG 0

// ANSI color codes for boxed in letters
#define GREEN "\x1b[42m"
#define YELLOW "\x1b[43m"
#define RED "\x1b[41m"
#define RESET "\e[0;39m"

// user-defined function prototypes
string get_guess(int wordsize);
int check_word(string guess, int wordsize, int status[], string choice);
void print_word(string guess, int wordsize, int status[]);

int main(int argc, char * argv[])
{
    int wordsize = 5;
    // ensure proper usage
    if (argc != 2)
    {
        cout << "Usage: ./wordle wordsize\n";
        cout << "Deafaulting to 5 letter word\n\n";
    }
    else
    {
        int inputsize = atoi(argv[1]);
        if (inputsize >= 5 && inputsize <= 8)
        {
            wordsize = inputsize;
        }
        else
        {
            cout << "Usage: ./wordle wordsize\n";
            cout << "wordsize must be >=5 and <= 8\n";
            cout << "Deafaulting to 5 letter word\n\n";
        }
    }

    // open correct file, each file has exactly LISTSIZE words
    string wl_filename = to_string(wordsize) + ".txt";

    ifstream wordlist; 
    wordlist.open(wl_filename);
    if (!wordlist)
    {
        cout << "Error opening file " << wl_filename << "\n";
        return 1;
    }

    // load word file into a vector
    vector<string> options(LISTSIZE);

    if (wordlist.is_open())
    {
        while (wordlist)
        {
            string in_word;
            getline(wordlist, in_word);
            options.push_back(in_word);
        }
    }

    // pseudorandomly select a word for this game
    srand(time(NULL));
    string choice = options[rand() % LISTSIZE];

    // allow one more guess than the length of the word
    int guesses = wordsize + 1;
    bool won = false;

    // print greeting, using ANSI color codes to demonstrate
    cout << GREEN "This is WORDLE" RESET "\n";
    cout << "You have " << guesses << " tries to guess the " << wordsize <<"-letter word I'm thinking of\n";

    // main game loop, one iteration for each guess
    for (int i = 0; i < guesses; i++)
    {
        // obtain user's guess
        string guess = get_guess(wordsize);

        // array to hold guess status, initially set to zero
        int status[wordsize];

        // Calculate score for the guess
        int score = check_word(guess, wordsize, status, choice);

        cout << "Guess " << i + 1 << ": \n";

        // Print the guess
        print_word(guess, wordsize, status);

        // if they guessed it exactly right, set terminate loop
        if (score == EXACT * wordsize)
        {
            won = true;
            break;
        }
    }

    // Print the game's result
    if (won)
    {
        cout << "YOU WON!\n";
    }
    else
    {
        cout << "The word was: " << choice << "\n";
    }

    // that's all folks!
    return 0;
}

string get_guess(int wordsize)
{
    string guess = "";

    // ensure users actually provide a guess that is the correct length
    do
    {
        string temp;
        cout << "Input a " << wordsize << "-letter word: ";
        cin >> temp;
        if (temp.size() == wordsize)
        {
            guess = temp;
        }
    }
    while (guess.size() != wordsize);

    return guess;
}

int check_word(string guess, int wordsize, int status[], string choice)
{
    int score = 0;

    for (int i = 0; i < wordsize; i++)
    {
        for (int j = 0; j < wordsize; j++)
        {
            if (guess[i] == choice[j])
            {
                if (j == i)
                {
                    status[i] = EXACT;
                    score += EXACT;
                    break;
                }
                else
                {
                    status[i] = CLOSE;
                    score += CLOSE;
                }
            }
        }
    }
    return score;
}

void print_word(string guess, int wordsize, int status[])
{
    for (int i = 0; i < wordsize; i++)
    {
        if (status[i] == EXACT)
        {
            cout << GREEN  << guess[i] << RESET " ";
        }
        else if (status[i] == CLOSE)
        {
            cout << YELLOW  << guess[i] << RESET " ";
        }
        else
        {
            cout << RED  << guess[i] << RESET " ";
        }
    }
    printf(RESET "\n");
    return;
}