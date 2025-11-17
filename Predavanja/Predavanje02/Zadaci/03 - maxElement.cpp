/*
MAX-ELEMENT (K) 
1 max = K[0] 
2 for i = 1 to n-1 do 
3   if (K[i] > max) then 
4     max = K[i] 
5   end_if 
6 end_for 
7 return max
*/

#include <iostream>
using namespace std;

int maxElement(int K[], int n) {
    int max = K[0]; // Pretpostavimo da je prvi element najveći
    for(int i = 1; i < n; i++) {
        if(K[i] > max)
            max = K[i];
    }
    return max;
}

int main() {
    int K[] = {3, 7, 2, 9, 5};
    int n = sizeof(K) / sizeof(K[0]);

    cout << "Najveći element je: " << maxElement(K, n) << endl;

    return 0;
}
