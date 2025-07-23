#include <iostream>
#include <string>
using namespace std;

const int MAX = 10;  

void generate_powerset(char set[], char current[], int index, int curr_size, int n) {
    if (index == n) {
        
        cout << "{ ";
        for (int i = 0; i < curr_size; ++i) {
            cout << current[i] << " ";
        }
        cout << "}" << endl;
        return;
    }

    generate_powerset(set, current, index + 1, curr_size, n);

    current[curr_size] = set[index];
    generate_powerset(set, current, index + 1, curr_size + 1, n);
}

int main() {
    char set[] = {'a', 'b', 'c'};
    int n = 3;

    char current[MAX]; 
    generate_powerset(set, current, 0, 0, n);

    return 0;
}
