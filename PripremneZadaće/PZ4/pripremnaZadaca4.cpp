#include <iostream>
#include <stdexcept>

using namespace std;

template <typename TipEl> class Red {
private:
  struct Cvor {
    TipEl a;
    Cvor *sljedeci;
  };

  Cvor *vrh_reda;
  Cvor *kraj_reda;
  int broj_elemenata;

public:
  Red() : broj_elemenata(0), vrh_reda(nullptr), kraj_reda(nullptr){};
  ~Red() {
    kraj_reda = nullptr;
    while (vrh_reda != nullptr) {
      Cvor *temp = vrh_reda;
      vrh_reda = vrh_reda->sljedeci;
      delete temp;
    }
    broj_elemenata = 0;
  };
  Red(const Red &red) {
    broj_elemenata = 0;
    vrh_reda = nullptr;
    if (red.broj_elemenata == 0)
      kraj_reda = nullptr;
    else {
      Cvor *temp = red.vrh_reda;
      while (temp != nullptr) {
        stavi(temp->a);
        temp = temp->sljedeci;
      }
    }
  };
  Red &operator=(const Red &red) {
    if (this == &red)
      return *this;

    brisi();
    broj_elemenata = 0;
    if (red.broj_elemenata == 0)
      kraj_reda = nullptr;
    else {
      Cvor *temp = red.vrh_reda;
      while (temp != nullptr) {
        stavi(temp->a);
        temp = temp->sljedeci;
      }
    }
    return *this;
  };

  void brisi() {
    kraj_reda = nullptr;
    while (vrh_reda != nullptr) {
      Cvor *temp = vrh_reda;
      vrh_reda = vrh_reda->sljedeci;
      delete temp;
    }
    broj_elemenata = 0;
  };

  void stavi(const TipEl &el) {
    Cvor *novi = new Cvor;
    novi->a = el;
    if (vrh_reda == nullptr ) {
      vrh_reda = novi;
      kraj_reda = novi;
      vrh_reda->sljedeci = nullptr;
      kraj_reda->sljedeci = nullptr;
    } else {
      kraj_reda->sljedeci = novi;
      kraj_reda = novi;
      kraj_reda->sljedeci = nullptr;
    }
    broj_elemenata++;
  };

  TipEl skini() {
    if (broj_elemenata == 0)
      throw std::range_error("Prazan red");
    ////////////////////////////////////////////////////////////////////
    if (broj_elemenata == 1)
      kraj_reda = nullptr;
    TipEl vrati = vrh_reda->a;
    Cvor *temp = vrh_reda;
    vrh_reda = vrh_reda->sljedeci;
    delete temp;
    broj_elemenata--;
    return vrati;
  };

  TipEl &celo() {
    if (broj_elemenata == 0)
      throw std::range_error("Prazan red");
    return vrh_reda->a;
  };
  int brojElemenata() { return broj_elemenata; };
};

int main() {
    Red<int> red;

    red.stavi(5);
    red.stavi(10);
    red.stavi(25);
    red.stavi(35);
    red.stavi(15);

    std::cout << "Broj elemenata u redu: " << red.brojElemenata() << endl;

    std::cout << "Element na celu reda: " << red.celo() << endl;

    std::cout << "Uklonjeni elementi: ";
    while (red.brojElemenata() > 0) {
        std::cout << red.skini() << " ";
    }
    std::cout << std::endl;

    if (red.brojElemenata() == 0) {
        std::cout << "Red je prazan" << endl;
    } else {
        std::cout << "Red nije prazan" << endl;
    }

    return 0;
}