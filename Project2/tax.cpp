#include <iostream>
#include <string>
using namespace std;

//Called upon execution of program
int main() {

	//Set doubles to be printed with 2 decimal places
	cout.setf(ios::fixed);
	cout.precision(2);

	//Declare variables that will be used to store the input
	string name;
	double taxable_income;
	string occupation;
	int	number_of_children;
	
	//Ask for and store name
	cout << "Name: ";
	getline(cin, name);

	//Ask for and store taxable income as a double
	cout << "Taxable income: ";
	cin >> taxable_income;
	//Ignore anything else up to and including the newline
	cin.ignore(10000, '\n');

	//Ask for and store occupation
	cout << "Occupation: ";
	getline(cin, occupation);

	//Ask for and store number of children as an integer
	cout << "Number of children: ";
	cin >> number_of_children;
	//Ignore anything else up to and including the newline
	cin.ignore(10000, '\n');

	cout << "---" << endl;

	//Check for each of the errors listed in the spec. If found, print error message and end the program
	if (name == "") {
		cout << "You must enter a name" << endl;
		return 1;
	}
	if (taxable_income < 0) {
		cout << "The taxable income must be nonnegative" << endl;
		return 1;
	}
	if (occupation == "") {
		cout << "You must enter an occupation" << endl;
		return 1;
	}
	if (number_of_children < 0) {
		cout << "The number of children must be nonnegative" << endl;
		return 1;
	}

	//Used to store how much income falls under each bracket.
	double bracket_1_income = 0.0;
	double bracket_2_income = 0.0;
	double bracket_3_income = 0.0;

	//Determine how much income falls into first bracket - All, unless that's more than 50000.
	if (taxable_income < 50000) {
		bracket_1_income = taxable_income;
	}
	else {
		bracket_1_income = 50000.0;
	}
	//Determine bracket 2 income similarly: all income greater than 50k but less than 120k
	if (taxable_income < 50000) {
		bracket_2_income = 0.0;
	}
	else if (taxable_income < 120000) {
		bracket_2_income = taxable_income - 50000.0;
	}
	else {
		bracket_2_income = 70000.0;
	}
	//Determine bracket 2 income similarly: all income greater than 120k
	if (taxable_income < 120000) {
		bracket_3_income = 0.0;
	}
	else {
		bracket_3_income = taxable_income - 120000.0;
	}

	//Used to store the total amount of tax so far
	double tax = 0.0;

	//Tax first bracket of income at 4%
	tax += bracket_1_income * 0.04;

	//Tax second bracket at 6% or 5% depending on occupation
	if (occupation == "engineer" || occupation == "scientist") {
		tax += bracket_2_income * 0.05;
	}
	else {
		tax += bracket_2_income * 0.06;
	}

	//Tax third bracket of income at 9%
	tax += bracket_3_income * 0.09;

	//Reduce tax by $200 per child
	tax -= 200.0 * number_of_children;

	//Tax is set to 0 at minimum
	if (tax < 0) {
		tax = 0.0;
	}

	//Print output
	cout << name << " would pay $" << tax << endl;

}