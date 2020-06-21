#include <string>
#include <iostream>

using namespace std;

string toUppercase(string input) {
    string output = "";
    for (int i = 0; i < input.size(); i++) {
        output += toupper(input.at(i));
    }
    return output;
}

bool isValidUppercaseStateCode(string stateCode)
{
    const string codes =
        "AL.AK.AZ.AR.CA.CO.CT.DE.DC.FL.GA.GU.HI.ID.IL.IN.IA.KS."
        "KY.LA.ME.MD.MA.MI.MN.MS.MO.MP.MT.NE.NV.NH.NJ.NM.NY.NC."
        "ND.OH.OK.OR.PA.PR.RI.SC.SD.TN.TX.UT.VT.VA.WA.WV.WI.WY";
    return (stateCode.size() == 2 &&
        stateCode.find('.') == string::npos &&  // no '.' in stateCode
        codes.find(stateCode) != string::npos);  // match found
}

bool isValidStateCode(string stateCode) {
    return isValidUppercaseStateCode(toUppercase(stateCode));
}

//Returns the index of the last character in a valid state order
//If there is not a valid state order, returns -1
//Eg with the inputs "CA3+MN32-", 4
//Will return 8 as that's when the MN order ends
int getEndOfCase(string orders, int startIndex) {

    //Make sure string is long enough to read state code
    if (orders.size() < startIndex + 2) {
        cerr << "There are not enough characters for there to be a state code at index " << startIndex << endl;
        return -1;
    }

    //First two characters must be valid state code
    string firstTwoCharacters = "";
    firstTwoCharacters += orders.at(startIndex);
    firstTwoCharacters += orders.at(startIndex + 1);

    if (!isValidStateCode(firstTwoCharacters)) {
        cerr << "First two letters of case starting at " << startIndex << ", " << firstTwoCharacters <<
            " are not a state code, fail" << endl;
        return -1;
    }

    //Starting with the character after the first two, figure out where the last digit
    //character in a row is.
    int lastDigitIndex = startIndex + 1;

    //If the next character is a digit, move the counter to it. Continue until you're
    //pointing at the last one
    while (true) {

        if (orders.size() <= lastDigitIndex + 1) {
            cerr << "String ended while looking for digits at index " << lastDigitIndex << endl;
            return -1;
        }

        //If there's no digit after lastDigitIndex, lastDigitIndex is in the right place, exit loop.
        if (!isdigit(orders.at(lastDigitIndex + 1))) {
            break;
        }

        //If it is a digit, move last digit index to there.
        lastDigitIndex++;
    }

    //If you never moved the counter, there are no digits and it is invalid.
    if (lastDigitIndex == startIndex + 1) {
        cerr << "No digits found after state code starting at " << startIndex << endl;
        return -1;
    }

    //Get the character after the last digit. Check if it's - or + as it should be.
    //If it is, return its index
    char lastChar = orders.at(lastDigitIndex + 1);
    if (lastChar == '-' || lastChar == '+') {
        cerr << "Successfully found case from " << startIndex << " through " << lastDigitIndex + 1 << endl;
        return lastDigitIndex + 1;
    }
    else {
        cerr << "Character after the digits (index " << lastDigitIndex + 1 << ") is neither + nor -" << endl;
        return -1;
    }
}

int casesInOrder(string orders, int startIndex, int lastIndex) {

    int total = 0;
    //Iterate over the characters besides the first two (state) and last (+/-)
    //Value of total for the input "CA123" in each step:
    // 0 -> 0 -> 1
    // 1 -> 10 -> 12
    // 12 -> 120 -> 123
    for (int i = startIndex + 2; i < lastIndex; i++) {
        total *= 10;
        total += orders.at(i) - '0';
    }

    cerr << "Counted " << total << " cases from " << startIndex << " to " << lastIndex << endl;

    return total;
}

bool hasValidSyntax(string orders) {

    int index = 0;

    //While there are any characters left to look at
    while (orders.size() > index) {
        //Move index to after this case
        index = getEndOfCase(orders, index) + 1;
        //If getEndOfCase returned -1, something went wrong
        if (index == 0) {
            return false;
        }
    }
    return true;
}

int countCases(string orders, char status, int& caseCount) {

    int startIndex = 0;
    int total = 0;

    if (status != '+' && status != '-') {
        return 3;
    }
    if (!hasValidSyntax(orders)) {
        return 1;
    }
    //While there are any characters left to look at
    while (orders.size() > startIndex) {

        int lastIndex = getEndOfCase(orders, startIndex);
        int cases = casesInOrder(orders, startIndex, lastIndex);

        if (cases == 0) {
            return 2;
        }

        if (orders.at(lastIndex) == status) {
            total += cases;
        }

        startIndex = lastIndex + 1;

    }

    caseCount = total;
    return 0;
}

int main() {

    int caseCount = 0;
    string orders = "CA123-TX123+MN104-";

    countCases(orders, '-', caseCount);

    cerr << caseCount << endl;
}