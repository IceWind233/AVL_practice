#include <iostream>
#include <set>
#include <vector>
#include "avl.hpp"

using namespace std;

int main(int argc, char **argv){
    vector<int> a;
    a.push_back(1);
    a.emplace_back(2);
    set<int> b;

    b.end();

    AVL<char> avl{'a', 'b', 'c', 'd'};
    avl.erase('d');
    avl.print();

    return 0;
}