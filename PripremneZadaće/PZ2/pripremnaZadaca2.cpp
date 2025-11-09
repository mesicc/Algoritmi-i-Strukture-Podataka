#include <iostream>
using namespace std;

template <typename Tip> class Lista {
public:
  //01. Kreiranje konstruktora bez parametara
  Lista() {} 
  //02. Metoda brojElemenata() koja vraća broj elemenata stavljenih u listu (na početku nula)
  virtual int brojElemenata() const = 0;
  virtual Tip &trenutni() = 0; 
  virtual Tip trenutni() const = 0;
  virtual bool prethodni() = 0;   
  virtual bool sljedeci() = 0;
  virtual void pocetak() = 0;
  virtual void kraj() = 0;
  virtual void obrisi() = 0;                   
  virtual void dodajIspred(const Tip &el) = 0;
  virtual void dodajIza(const Tip &el) = 0;     
  virtual Tip &operator[](int a) = 0;
  virtual const Tip operator[](int a) const = 0;
  virtual ~Lista() {}
};

template <typename Tip> class NizLista : public Lista<Tip> {
protected:
  int kapacitet; 
  int duzina;    
  int tekuci;    
  Tip **L; 
  void realokacija(){
    auto a = L;
    kapacitet*=2;
    L = new Tip* [kapacitet]{};
    for(int i=0; i<duzina; i++) L[i] = a[i];
    delete[] a;
  }
  void dealokacija(){
    for(int i=0;i<duzina;i++) delete L[i]; delete[] L; L=nullptr;
  }
public:
  NizLista(const NizLista &lista){
    kapacitet = lista.kapacitet; duzina = lista.duzina; L = new Tip*[kapacitet]{};
    for (int i = 0; i < duzina; i++) {
      L[i] = new Tip(*(lista.L[i]));
    }
  }
  NizLista<Tip> &operator= (const NizLista &lista){
    if (this == &lista) return *this; // Samododjeljivanje
    dealokacija();
    kapacitet = lista.kapacitet; duzina = lista.duzina; L = new Tip*[kapacitet]{};
    for (int i = 0; i < duzina; i++) {
      L[i] = new Tip(*(lista.L[i]));
    }
    return *this;
  }
  NizLista(int size=50) {
    kapacitet = size;
    duzina = tekuci = 0;
    L = new Tip*[kapacitet]{};
  }
  int brojElemenata() const override { return duzina; }
  Tip trenutni() const override { 
    if (!duzina) throw "GRESKA:Lista prazna!\n";
    return *(L[tekuci]); 
  }
  Tip &trenutni() override { 
    if (!duzina) throw "GRESKA:Lista prazna!\n";
    return *L[tekuci]; 
  }
  bool prethodni() override {
    if (!duzina) throw "GRESKA:Lista prazna!\n";
    if (tekuci != 0){
      tekuci--;
      return true;
    }
    return false;
  }
  bool sljedeci() override {
    if (!duzina) throw "GRESKA:Lista prazna!\n";
    if (tekuci < duzina-1){
      tekuci++;
      return true;
    }
    return false;
  }
  void pocetak() override {
    if (!duzina) throw "GRESKA:Lista prazna!\n";
    tekuci = 0;
  }
  void kraj() override {
    if (!duzina) throw "GRESKA:Lista prazna!\n";
    tekuci = duzina-1;
  }
  void obrisi() override {
    if (!duzina) throw "GRESKA:Lista prazna!\n";
    delete L[tekuci];
    for (int i=tekuci; i<duzina-1; i++){
      L[i] = L[i+1];
    }
    L[duzina-1] = nullptr;
    duzina--;
    if (duzina==tekuci && tekuci!=0) tekuci--;
  }
  void dodajIspred(const Tip &el) override {
    if(duzina>=kapacitet) realokacija();
    for (int i = duzina; i > tekuci; i--){
      L[i] = L[i-1];
    }
    L[tekuci] = new Tip{el};
    duzina++;
    if(duzina!=1) tekuci++;
  }
  
  void dodajIza(const Tip &el) override {
    if(duzina>=kapacitet) realokacija();
    for (int i = duzina; i > tekuci+1; i--){
      L[i] = L[i-1];
    }
    if (duzina) L[tekuci+1] = new Tip{el};
    else L[tekuci] = new Tip{el};
    duzina++;
  }
  Tip &operator[] (int a) override {
    if (a<0 || a>=duzina) throw "GRESKA:Nedozvoljen indeks!\n";
    return *L[a];
  }
  const Tip operator[] (int a) const override {
    if (a<0 || a>=duzina) throw "GRESKA:Nedozvoljen indeks!\n";
    return *L[a];
  }
  ~NizLista() { dealokacija(); }
};

template <typename Tip> class JednostrukaLista : public Lista<Tip> {
private:
  struct Cvor {
    Tip element;
    Cvor *sljedeci;
  };

  Cvor *prvi, *posljednji, *tekuci, *prije_tekuci;
  int broj;

public:
  JednostrukaLista (): prvi(nullptr), posljednji(nullptr), tekuci(nullptr), broj(0){}
  JednostrukaLista (const JednostrukaLista &lista) {
    broj = lista.broj;
    if (lista.prvi==nullptr) { prvi=nullptr; posljednji=nullptr; tekuci=nullptr; broj=0; }
    else{
      Cvor* a = tekuci = prvi = new Cvor {lista.prvi->element, nullptr};
      auto b = lista.prvi;
      for (int i = 1; i < broj; i++){
        b = b->sljedeci;
        a->sljedeci = new Cvor {b->element, nullptr};
        a = a->sljedeci;
        if(b == lista.tekuci) a = tekuci;
        if(b == lista.posljednji) a = posljednji;
      }
    }
  }
  JednostrukaLista<Tip> &operator= (const JednostrukaLista &lista) {
    while (prvi != nullptr) {
      this->obrisi();
    }
    tekuci = nullptr;
    broj = 0;

    Cvor *pomocni = lista.prvi;
    while (pomocni != nullptr) {
      this->dodajIza(pomocni->element);
      pomocni = pomocni->sljedeci;
    }
    return *this;
  }
  ~JednostrukaLista() {
    while (prvi) {
      auto a = prvi;
      prvi = prvi->sljedeci;
      delete a;
      broj--;
    }
  }
  int brojElemenata() const {
    return broj;
  }
  Tip &trenutni() {
    if (prvi==nullptr) throw "GRESKA:Lista prazna!\n";
    return tekuci->element;
  }
  Tip trenutni() const {
    if (prvi==nullptr) throw "GRESKA:Lista prazna!\n";
    return tekuci->element;
  }
  bool prethodni() {
    if (prvi==nullptr) throw "GRESKA:Lista prazna!\n";
    if(tekuci==prvi) return false;
    auto a = tekuci;
    tekuci = prvi; 
    while (tekuci->sljedeci != a) tekuci=tekuci->sljedeci;
    return true;
  }
  bool sljedeci() {
    if (prvi==nullptr) throw "GRESKA:Lista prazna!\n";
    if(tekuci==posljednji) return false;
    tekuci = tekuci->sljedeci;
    return true;
  }
  void pocetak() {
    if (prvi==nullptr) throw "GRESKA:Lista prazna!\n";
    tekuci = prvi;
  }
  void kraj() {
    if (prvi==nullptr) throw "GRESKA:Lista prazna!\n";
    tekuci = posljednji;
  }
  void obrisi() {
    if (prvi==nullptr) throw "GRESKA:Lista prazna!\n";
    if (tekuci == nullptr)
      return;
    if (tekuci == prvi) {
      Cvor *brisem = prvi;
      prvi = prvi->sljedeci;
      if (prvi == nullptr) 
        posljednji = nullptr;
      delete brisem;
      tekuci = prvi;
    } else {
      Cvor *pp = prvi;
      while (pp->sljedeci != tekuci)
        pp = pp->sljedeci;
      pp->sljedeci = tekuci->sljedeci;
      if (tekuci ==
          posljednji) 
        posljednji = pp;
      delete tekuci;
      if (pp->sljedeci == nullptr)
        tekuci = pp;
      else
        tekuci = pp->sljedeci;
    }
    broj--;
  }
  void dodajIspred(const Tip &el) {
    if(tekuci==nullptr) {prije_tekuci = nullptr; prvi = posljednji = tekuci = new Cvor{el, nullptr};}
    else{
      if(tekuci==prvi){
        prvi=new Cvor{el, tekuci}; prije_tekuci = prvi;
      }
      else {
        auto prije_tekuceg=prije_tekuci;
        while(prije_tekuceg->sljedeci != tekuci) prije_tekuceg = prije_tekuceg->sljedeci;
        prije_tekuci = prije_tekuceg;
        prije_tekuceg->sljedeci = new Cvor{el, tekuci};
      }
    }
    broj++;
  }
  void dodajIza(const Tip &el) {
    if(tekuci==nullptr) prvi = posljednji = tekuci = new Cvor{
el, nullptr};
    else{
      if(tekuci==posljednji){
        tekuci->sljedeci=new Cvor{el, nullptr};
        posljednji=tekuci->sljedeci;
      }
      else {
        auto a = tekuci->sljedeci;
        tekuci->sljedeci = new Cvor{el, a};
      }
    }
    broj++;
  }
  Tip &operator[](int a) {
    if (a<0 || a>=broj) throw "GRESKA:Nedozvoljen indeks!\n";
    auto k = prvi;
    for(int i=0; i<a; i++)
      k = k->sljedeci;
    return k->element;
  }
  const Tip operator[](int a) const {
    if (a<0 || a>=broj) throw "GRESKA:Nedozvoljen indeks!\n";
    auto k = prvi;
    for(int i=0; i<a; i++)
      k = k->sljedeci;
    return k->element;
  }
};

//obrisi, dodajIspred, dodajIza

template <typename Tip>
bool testirajPocetak(Lista<Tip> *lista) {
  if (lista->brojElemenata()){
    lista->pocetak();
    if ((*lista)[0] == lista->trenutni()) return true;
  }
  return false;
}

template <typename Tip>
bool testirajKraj(Lista<Tip> *lista) {
  if (lista->brojElemenata()){
    lista->kraj();
    if ((*lista)[(lista->brojElemenata())-1] == lista->trenutni()) return true;
  }
  return false;
}

template <typename Tip>
bool testirajPrethodniIDodavanje(Lista<Tip> *lista) {
  if (lista->trenutni()!=(*lista)[0]){
    lista->dodajIspred(459247235);
    lista->prethodni();
    if (lista->trenutni() == 459247235) return true;
  }
  else if(lista->trenutni()==(*lista)[0]){
    if(lista->prethodni()==false) return true;
  }
  return false;
}

template <typename Tip>
bool testirajSljedeciIDodavanje(Lista<Tip> *lista) {
  if (lista->trenutni()!=(*lista)[(lista->brojElemenata())-1]){
    lista->dodajIza(459247235);
    lista->sljedeci();
    if (lista->trenutni() == 459247235) return true;
  }
  else if(lista->trenutni()==(*lista)[(lista->brojElemenata())-1]){
    if(lista->sljedeci()==false) return true;
  }
  return false;
}

template <typename Tip>
bool testirajDodavanjeIspred(Lista<Tip> *lista) {
  auto a = lista->brojElemenata();
  lista->dodajIspred(459247235);
  lista->prethodni();
  if (lista->trenutni() == 459247235 && lista->brojElemenata()-1==a) return true;
  return false;
}

template <typename Tip>
bool testirajDodavanjeIza(Lista<Tip> *lista) {
  auto a = lista->brojElemenata();
  lista->dodajIza(459247235);
  lista->sljedeci();
  if (lista->trenutni() == 459247235 && lista->brojElemenata()-1==a) return true;
  return false;
}

template <typename Tip>
bool testirajBrisanje(Lista<Tip> *lista) {
  auto a = lista->brojElemenata();
  if(a==0) return true;
  lista->obrisi();
  if (a!=0 && lista->brojElemenata()+1==a) return true;
  return false;
}

int main() {
  Lista<int> *l;
  l = new NizLista<int>;
  for(int i=0;i<32;i++){
    if(i%2==0) l->dodajIza(i);
    else l->dodajIspred(i);
  }
  long long int g = 314159265358979;
  for (int i=0;i<15; i++){
    auto k = g%10;
    g/=10;
    if (k%7==0)if (!testirajPocetak(l)) {std::cout << "greska"; return 0;}
    if (k%7==1)if (!testirajKraj(l)) {std::cout << "greska"; return 0;}
    if (k%7==2)if (!testirajPrethodniIDodavanje(l)) {std::cout << "greska"; return 0;}
    if (k%7==3)if (!testirajSljedeciIDodavanje(l)) {std::cout << "greska"; return 0;}
    if (k%7==4)if (!testirajDodavanjeIspred(l)) {std::cout << "greska"; return 0;}
    if (k%7==5)if (!testirajDodavanjeIza(l)) {std::cout << "greska"; return 0;}
    if (k%7==6)if (!testirajBrisanje(l)) {std::cout << "greska"; return 0;}
  }
  std::cout << "sve fje tacno rade za NizLista" << std::endl;
  // moja logika je bila napraviti sto je moguce vise randomiziranu situaciju u listi, gdje ce se provjeriti
  // da li dolazi do greske pri situacijama gdje se nepredvidivo elementi dodaju, brisu, pomjera im se pozicija...
  // isto radimo za JednostranuListu
  delete l;
  l = new NizLista<int>;
  for(int i=0;i<32;i++){
    if(i%2==0) l->dodajIza(i);
    else l->dodajIspred(i);
  }
  long long int h = 314159265358979;
  for (int i=0;i<15; i++){
    auto k = h%10;
    h/=10;
    if (k%7==0)if (!testirajPocetak(l)) {std::cout << "greska"; return 0;}
    if (k%7==1)if (!testirajKraj(l)) {std::cout << "greska"; return 0;}
    if (k%7==2)if (!testirajPrethodniIDodavanje(l)) {std::cout << "greska"; return 0;}
    if (k%7==3)if (!testirajSljedeciIDodavanje(l)) {std::cout << "greska"; return 0;}
    if (k%7==4)if (!testirajDodavanjeIspred(l)) {std::cout << "greska"; return 0;}
    if (k%7==5)if (!testirajDodavanjeIza(l)) {std::cout << "greska"; return 0;}
    if (k%7==6)if (!testirajBrisanje(l)) {std::cout << "greska"; return 0;}
  }
  std::cout << "sve fje tacno rade za JednostranaLista" << std::endl;
  delete l;
  return 0;
}