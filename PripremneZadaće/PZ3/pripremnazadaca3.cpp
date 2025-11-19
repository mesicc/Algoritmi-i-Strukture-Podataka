#include <iostream>
#include <stdexcept>

using namespace std;

//Generička klasa Stek
template <typename tip>
class Stek {
  struct Cvor {
      tip element;
      Cvor *prethodni;
  };
  Cvor *trenutni; //pokazivač unutar klase koji pokazuje na vrh steka (najnoviji ubačeni element "vrh")
  int broj;
  public:
    //konstruktor bez parametara
    Stek () : trenutni(nullptr), broj(0) {}
    //destruktor klase, brise dinamički alociranu memoriju
    ~Stek () {
      brisi();
    }
    Stek (const Stek &stek) {
      if (!stek.broj) { trenutni = nullptr; broj = 0; }
      else {
        broj = stek.broj;
        trenutni = new Cvor{stek.trenutni->element, nullptr};
        auto a = trenutni;
        auto b = stek.trenutni;
        for(int i = 0; i < broj-1; i++){
          b = b->prethodni;
          a->prethodni = new Cvor {b->element, nullptr};
          a = a->prethodni;
        }
      }
    }

    //operator dodjele
    Stek<tip>& operator= (const Stek &stek) {
      if(this != &stek) brisi();
      else return *this;
      if (!stek.broj) { trenutni = nullptr; broj = 0; }
      else {
        broj = stek.broj;
        trenutni = new Cvor{stek.trenutni->element, nullptr};
        auto a = trenutni;
        auto b = stek.trenutni;
        for(int i = 0; i < broj-1; i++){
          b = b->prethodni;
          a->prethodni = new Cvor {b->element, nullptr};
          a = a->prethodni;
        }
      }
      return *this; //referenca this a = b = c
    }

    void brisi() {
      for (int i = 0; i < broj; i++) {
        auto x = trenutni; //pohrani pokazivač na trenutni vrh
        trenutni = trenutni->prethodni; //pomakni trenutni na sljedeći (ispod vrha)
        delete x;
      }
      broj=0;
    }

    //stavi() element na vrh steka
    void stavi(const tip& el) {
      auto x = new Cvor{el, trenutni};
      trenutni = x; //novi čvor postaje novi vrh
      broj++;
    }

    //skini() - uklanja i vraća element s vrha steka
    tip skini() {
      if (!broj) throw std::range_error("Prazan stek");
      //trenutni je ustvari vrh steka
      auto a = trenutni->element; //kopira vrijednost iz vrha u privremenu varijablu a
      auto x = trenutni;
      trenutni = trenutni->prethodni;
      delete x;
      broj--;
      return a;
    }  

    tip& vrh() {
      if (!broj) throw std::range_error("Prazan stek");
      return trenutni->element;
    }                             
    int brojElemenata() {return broj;}
};  

template <typename tip>
bool testirajBrisanje(Stek<tip> &stek) {
  auto stek1(stek); //poziva konstruktor kopije, kreirajući stek1 kao kopiju originala
  stek1.brisi();
  try {
    stek1.skini();
  }
  catch(...) {
    return true;
  }
  return false;
}

template <typename tip>
bool testirajStavljanje(Stek<tip> &stek) {
  stek.stavi(tip{});
  if(stek.skini()!= tip{}) return false;
  return true;
}

template<typename tip>
bool testirajOperatorDodjele(Stek<tip> &stek) {
  Stek<tip> stek1;
  stek1 = stek; //testira samododjelu
  stek = stek;
  try {
    stek.vrh();
  }
  catch(...) {
    return false;
  }
  if(stek1.vrh() != stek.vrh()) return false;
  return true;
}

int main() {
  Stek<int> stek; //instanca steka
  for (int i = 0; i < 34; i++) {
    stek.stavi(i);
  }
  if (!testirajBrisanje(stek)) {std::cout << "greska testirajBrisanje()!"; return 0;}
  if (!testirajOperatorDodjele(stek)) {std::cout << "greska testirajOperatorDodjele()!"; return 0;}
  if (!testirajStavljanje(stek)) {std::cout << "greska testirajStavljanje!"; return 0;}
  std::cout << "Sve fje rade ispravno!"<< endl;
  return 0;
}