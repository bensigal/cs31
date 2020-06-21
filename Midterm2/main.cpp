#include <iostream>
#include <cassert>
using namespace std;

// SUBMIT AS YOUR ANSWER ONLY THE CODE BETWEEN THIS COMMENT AND THE
// COMMENT AT THE END OF THE longestIncreasing FUNCTION

int longestIncreasing(const int a[], int n, int& begin, int& end) {

    int maxSequenceLength = 0;
    int sequenceLength = 0;
    int lastInSequence = a[0] - 1;
    int beginOfCurrent = 0;


    for (int i = 0; i < n; i++) {

        if (a[i] > lastInSequence) {
            sequenceLength++;
            if (sequenceLength > maxSequenceLength) {
                maxSequenceLength = sequenceLength;
                end = i;
                begin = beginOfCurrent;
            }
        }
        else {
            sequenceLength = 1;
            beginOfCurrent = i;
        }

        //cerr << "Current: " << a[i] << ", Last in sequence: " << lastInSequence << ", Sequence Length: " << sequenceLength << endl;

        lastInSequence = a[i];

    }

    return maxSequenceLength;

}

// SUBMIT AS YOUR ANSWER ONLY THE CODE BETWEEN THIS COMMENT AND THE
// COMMENT AT THE START OF THE longestIncreasing FUNCTION

int main()
{
    int x[16] = {
          10,  3,  8, 16, 20,  7,  9, 12, 16, 19,  4,  7,  7,  8, 10, 11

    }; //  0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15

    int b = -999;
    int e = -999;
    assert(longestIncreasing(x, 16, b, e) == 5);
    assert(b == 5);
    assert(e == 9);

    // 10 3 has two increasing sequences of length 1 (10 and 3) and none longer.
    // Of the two sequences, 10 is closer to the beginning of the array.
    assert(longestIncreasing(x, 2, b, e) == 1 && b == 0 && e == 0);

    cout << "All tests succeeded" << endl;
}