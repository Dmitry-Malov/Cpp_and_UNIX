#include <iostream>
using namespace std;

void hanoi(int n, int start_pos, int finish_pos) {
    if (n == 0){
        return;
    }
    int free_pos = 6 - start_pos - finish_pos;
    hanoi(n - 1, start_pos, free_pos);
    cout << "ring " << n << " from " << start_pos << " to " << finish_pos << endl;
    hanoi(n - 1, free_pos, finish_pos);
}

int main(){
    int n;
    cin >> n;
    hanoi(n, 1, 3);
    return 0;
}


