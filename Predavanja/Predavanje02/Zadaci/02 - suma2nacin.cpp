/*
SUMA-M (n)
1 s = n*(n+1)/2
2 return s
*/

#include <iostream>
using namespace std;

int sumaM(int n) {
    int s = n * (n + 1) / 2;
    return s;
}

int main() {
    int n = 5;
    cout << "Suma od 1 do " << n << " je: " << sumaM(n) << endl;
    return 0;
}


/*
Ovaj algoritam koristi matematičku formulu za sumu prvih n prirodnih brojeva:

     𝑛(𝑛+1)
𝑠 = --------
       2


⏱️ Vremenska složenost

✔️ Izvrši se samo jedna operacija (nema petlje)
→ O(1) — konstantna vremenska složenost


Razlika u odnosu na algoritam SUMA(n) - 01 - suma.cpp

| Algoritam | Metoda              | Operacija       | Složenost |
| --------- | ------------------- | --------------- | --------- |
| SUMA(n)   | Petlja              | n dodavanja     | O(n)      |
| SUMA-M(n) | Matematička formula | Jedno računanje | O(1)      |


*/