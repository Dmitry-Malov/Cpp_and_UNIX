#include <iostream>
#include <string>
#include <list>

using namespace std;

void printList(const list<string>& lst) {
    for (const string& item : lst) {
        cout << item << endl;
    }
    cout << endl;
}

void towerOfHanoi(int n, int from, int to, list<string>& moves) {

    if (n == 0) return;

    towerOfHanoi(n - 1, from, 6 - from - to, moves);

    moves.push_back("Disk " + to_string(n) + " from pin " + to_string(from) + " to pin " + to_string(to));

    towerOfHanoi(n - 1, 6 - from - to, to, moves);
}


int main() {
    int disks = 4  ;
    list<string> moves;

    towerOfHanoi(disks, 1, 3, moves);

    cout << "Solve Hanoi_Tower for " << disks << " disks:" << endl;
    printList(moves);

    moves.clear();
    return 0;
}
