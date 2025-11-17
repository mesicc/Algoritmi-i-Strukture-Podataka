/*
SUMA (n)
1 s = 0
2 for i = 1 to n do
3   s = s + i
4 end_for
5 return s
*/

#include <iostream>
using namespace std;

int suma(int n) {
    int s = 0;
    for(int i = 1; i <= n; i++) {
        s = s + i;
    }
    return s;
}

int main() {
    int n = 5;
    cout << "Suma od 1 do " << n << " je: " << suma(n) << endl;
    return 0;
}


/*
🔹 Šta radi algoritam?

Zbraja sve brojeve od 1 do n i vraća sumu.

Primjer: ako je n = 5
→ 1 + 2 + 3 + 4 + 5 = 15
*/