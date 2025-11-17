/*
TRAZI-SEKV (K, k) 
1 i = 0 
2 while (i ≤ n-1) do 
3   if (K[i] == k) then 
4     return i 
5   else 
6     i = i+1 
7   end_if
8 end_while 
9 return -1
*/

#include <iostream>
using namespace std;

int traziSekv(int K[], int n, int k) {
    int i = 0;
    while(i <= n - 1) {
        if(K[i] == k)
            return i; // pronađen element, vrati indeks
        else
            i = i + 1;
    }
    return -1; // nije pronađen
}

int main() {
    int K[] = {4, 7, 1, 9, 5};
    int n = sizeof(K) / sizeof(K[0]);
    int k = 9;

    int indeks = traziSekv(K, n, k);
    if(indeks != -1)
        cout << "Element " << k << " pronađen na indeksu: " << indeks << endl;
    else
        cout << "Element " << k << " nije pronađen!" << endl;

    return 0;
}

/*
✔️ Objašnjenje algoritma
Algoritam prolazi kroz svaki element niza redom, dok:

  - ne pronađe traženi element k → tada vraća indeks

  - ili dođe do kraja niza → tada vraća -1 (nije pronađen)

❗ Radi se o sekvencijalnoj pretrazi.

🧠 Vremenska složenost
  - Najbolji slučaj: element je prvi → 1 poređenje → O(1)

  - Prosječni slučaj: element je negdje u sredini → O(n)

  - Najgori slučaj: element je zadnji ili ne postoji → O(n)

Sveukupno se kaže da je algoritam O(n).
*/