#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;

long long power(int base, int exponent) {
    if (exponent < 0) return 0;
    long long result = 1;
    for (int i = 0; i < exponent; i++) {
        result *= base;
    }
    return result;
}

int main(int argc, char* argv[]) {

    int operand1 = atoi(argv[1]);
    string op = argv[2];
    int operand2 = atoi(argv[3]);

    if (op == "plus") {
        cout << operand1 + operand2 << endl;
    }
    else if (op == "minus") {
        cout << operand1 - operand2 << endl;
    }
    else if (op == "power") {
        cout << power(operand1, operand2) << endl;
    }
    else {
        cerr << "Error: Unsupported operator '" << op << "'" << endl;
        return 1;
    }

    return 0;
}
