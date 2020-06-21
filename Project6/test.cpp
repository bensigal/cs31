#include <iostream>

using namespace std;

void deleteG(char* param)
{
	int i = 0;
	while (param[i] != '\0')
	{
		if (param[i] == 'g' || param[i] == 'G')
			strcpy(param + i, param + i + 1);
		i++;
	}
}

int main() {
	char test[8] = "GharGjg";
	deleteG(test);
	cout << test << endl;
	return 0;
}