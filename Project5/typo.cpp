#include <iostream>
#include <string>
#include <cassert>
using namespace std;

int testWord(const string dictionaryWord, string word) {

    cerr << "Testing '" << word << "' against '" << dictionaryWord << "'" << endl;

    //Test if words are exactly equal
    if (dictionaryWord == word) {
        cerr << "Exactly equal" << endl;
        return 0;
    }

    //Test if removing one letter from dictionaryWord gives word
    if (dictionaryWord.length() == word.length() + 1) {
        for (int i = 0; i < dictionaryWord.length(); i++) {
            string sansOneLetter = dictionaryWord.substr(0, i) + dictionaryWord.substr(i + 1, dictionaryWord.length());
            if (sansOneLetter == word) {
                cerr << "Removing index " << i << " from dictionary word works" << endl;
                return 1;
            }
        }
    }

    //Test if removing one letter from word gives dictionaryWord
    if (dictionaryWord.length() == word.length() - 1) {
        for (int i = 0; i < word.length(); i++) {
            string sansOneLetter = word.substr(0, i) + word.substr(i + 1, word.length());
            if (sansOneLetter == dictionaryWord) {
                cerr << "Removing index " << i << " from word works" << endl;
                return 1;
            }
        }
    }

    if (dictionaryWord.length() == word.length()) {
        int length = word.length();

        //Check if there's only one error (so simple replacement)
        int errors = 0;
        for (int i = 0; i < length; i++) {
            if (dictionaryWord[i] != word[i]) {
                errors++;
            }
        }
        if (errors <= 1) {
            cerr << "Exactly one mismatch found" << endl;
            return 1;
        }

        //Test swapping
        for (int i = 0; i < length - 1; i++) {

            //If they're the same, keep searching
            if (dictionaryWord[i] == word[i])continue;

            //If they're different but a swap would work...
            if (dictionaryWord[i + 1] == word[i] && dictionaryWord[i] == word[i + 1]) {
                //If the rest of the word is the same, sucess
                if (dictionaryWord.substr(i + 2) == word.substr(i + 2)) {
                    cerr << "Swap works, rest of word works" << endl;
                    return 1;
                }
                else {
                    cerr << "Swap found but rest of word doesn't work" << endl;
                    return 2;
                }
            }
            //Otherwise give up
            else {
                cerr << "Same length, error, swap impossible" << endl;
                return 2;
            }
        }


    }

    cerr << "No similarity found" << endl;

    //No simple typo found
    return 2;
}

int scoreTypo(const string dictionary[], int n, string word)
{
    if (n <= 0)return -1;

    int minTypoScore = 2;
    for (int i = 0; i < n; i++) {
        int typoScore = testWord(dictionary[i], word);
        if (typoScore < minTypoScore) {
            minTypoScore = typoScore;
        }
    }

    return minTypoScore;

}

int main()
{
    string dict1[6] = { "february", "pointer", "country", "forty", "conversation", "minuscule" };
    assert(scoreTypo(dict1, 0, "forty") == -1);
    assert(scoreTypo(dict1, 6, "forty") == 0);
    assert(scoreTypo(dict1, 6, "fourty") == 1);
    assert(scoreTypo(dict1, 6, "febuary") == 1);
    assert(scoreTypo(dict1, 6, "miniscule") == 1);
    assert(scoreTypo(dict1, 6, "poitner") == 1);
    assert(scoreTypo(dict1, 6, "conservation") == 2);
    cout << "All tests succeeded" << endl;
}