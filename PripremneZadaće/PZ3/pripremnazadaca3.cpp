#include <iostream>
#include <stdexcept>

using namespace std;

template <typename tip>
class Stek {
  struct Cvor {
      tip element;
      Cvor *prethodni;
  };
  Cvor *trenutni;
  int broj;
  public:
    Stek () : trenutni(nullptr), broj(0) {}
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
      return *this;
    }
    void brisi() {
      for (int i = 0; i < broj; i++) {
        auto x = trenutni;
        trenutni = trenutni->prethodni;
        delete x;
      }
      broj=0;
    }
    void stavi(const tip& el) {
      auto x = new Cvor{el, trenutni};
      trenutni = x;
      broj++;
    }
    tip skini() {
      if (!broj) throw std::range_error("Prazan stek");
      auto a = trenutni->element;
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
  auto stek1(stek);
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
  if(stek.skini()!=tip{}) return false;
  return true;
}

template<typename tip>
bool testirajOperatorDodjele(Stek<tip> &stek) {
  Stek<tip> stek1;
  stek1 = stek;
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
  Stek<int> stek;
  for (int i = 0; i < 34; i++) {
    stek.stavi(i);
  }
  if (!testirajBrisanje(stek)) {std::cout << "greska!"; return 0;}
  if (!testirajOperatorDodjele(stek)) {std::cout << "greska!"; return 0;}
  if (!testirajStavljanje(stek)) {std::cout << "greska!"; return 0;}
  std::cout << "Sve fje rade ispravno!"<< endl;
  return 0;
}