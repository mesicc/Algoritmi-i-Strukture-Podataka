#include <iostream>
#include <fstream>
#include <string>
#include <chrono>

using namespace std;

/* ===================== BUBBLE SORT ===================== */
template <typename Tip>
void bubble_sort(Tip* niz, int vel) {
    for (int i = 0; i < vel - 1; i++) {
        for (int j = 0; j < vel - i - 1; j++) {
            if (niz[j] > niz[j + 1]) {
                swap(niz[j], niz[j + 1]);
            }
        }
    }
}

/* ===================== SELECTION SORT ===================== */
template <typename Tip>
void selection_sort(Tip* niz, int vel) {
    for (int i = 0; i < vel - 1; i++) {
        int minIndex = i;
        for (int j = i + 1; j < vel; j++) {
            if (niz[j] < niz[minIndex])
                minIndex = j;
        }
        swap(niz[i], niz[minIndex]);
    }
}

/* ===================== QUICK SORT ===================== */
template <typename Tip>
int partition(Tip* niz, int low, int high) {
    Tip pivot = niz[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (niz[j] <= pivot) {
            i++;
            swap(niz[i], niz[j]);
        }
    }
    swap(niz[i + 1], niz[high]);
    return i + 1;
}

template <typename Tip>
void quick_sort_pom(Tip* niz, int low, int high) {
    if (low < high) {
        int pi = partition(niz, low, high);
        quick_sort_pom(niz, low, pi - 1);
        quick_sort_pom(niz, pi + 1, high);
    }
}

template <typename Tip>
void quick_sort(Tip* niz, int vel) {
    quick_sort_pom(niz, 0, vel - 1);
}

/* ===================== MERGE SORT ===================== */
template <typename Tip>
void merge(Tip* niz, int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;

    Tip* L = new Tip[n1];
    Tip* R = new Tip[n2];

    for (int i = 0; i < n1; i++)
        L[i] = niz[l + i];
    for (int j = 0; j < n2; j++)
        R[j] = niz[m + 1 + j];

    int i = 0, j = 0, k = l;

    while (i < n1 && j < n2) {
        if (L[i] <= R[j])
            niz[k++] = L[i++];
        else
            niz[k++] = R[j++];
    }

    while (i < n1)
        niz[k++] = L[i++];

    while (j < n2)
        niz[k++] = R[j++];

    delete[] L;
    delete[] R;
}

template <typename Tip>
void merge_sort_pom(Tip* niz, int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        merge_sort_pom(niz, l, m);
        merge_sort_pom(niz, m + 1, r);
        merge(niz, l, m, r);
    }
}

template <typename Tip>
void merge_sort(Tip* niz, int vel) {
    merge_sort_pom(niz, 0, vel - 1);
}

/* ===================== UCITAVANJE ===================== */
void ucitaj(string filename, int*& niz, int& vel) {
    ifstream file(filename);
    if (!file) {
        throw runtime_error("Ne mogu otvoriti datoteku!");
    }

    vel = 0;
    int temp;
    while (file >> temp)
        vel++;

    file.clear();
    file.seekg(0);

    niz = new int[vel];
    for (int i = 0; i < vel; i++)
        file >> niz[i];

    file.close();
}

/* ===================== PROVJERA SORTIRANOSTI ===================== */
bool provjeri_sortiran(int* niz, int vel) {
    for (int i = 1; i < vel; i++) {
        if (niz[i] < niz[i - 1])
            return false;
    }
    return true;
}

/* ===================== MAIN ===================== */
int main() {
    int* niz = nullptr;
    int vel;
    string filename;

    cout << "Unesite putanju do datoteke: ";
    cin >> filename;

    ucitaj(filename, niz, vel);

    cout << "Izaberite algoritam:\n";
    cout << "1 - Bubble sort\n";
    cout << "2 - Selection sort\n";
    cout << "3 - Quick sort\n";
    cout << "4 - Merge sort\n";
    cout << "Izbor: ";

    int izbor;
    cin >> izbor;

    auto start = chrono::high_resolution_clock::now();

    switch (izbor) {
        case 1: bubble_sort(niz, vel); break;
        case 2: selection_sort(niz, vel); break;
        case 3: quick_sort(niz, vel); break;
        case 4: merge_sort(niz, vel); break;
        default:
            cout << "Pogresan izbor!" << endl;
            delete[] niz;
            return 0;
    }

    auto end = chrono::high_resolution_clock::now();
    auto trajanje = chrono::duration_cast<chrono::milliseconds>(end - start);

    cout << "Vrijeme izvrsavanja: " << trajanje.count() << " ms" << endl;

    if (provjeri_sortiran(niz, vel))
        cout << "Niz je ispravno sortiran." << endl;
    else
        cout << "Niz NIJE ispravno sortiran!" << endl;

    delete[] niz;
    return 0;
}
