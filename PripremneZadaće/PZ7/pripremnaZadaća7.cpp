#include <iostream>
#include <stdexcept>
#include <string>
#include <set>

// ---- Baza: apstraktna klasa Mapa ----
template<typename K, typename V>
class Mapa {
public:
    virtual ~Mapa() {}
    virtual V &operator[](const K &k) = 0;
    virtual V operator[](const K &k) const = 0;
    virtual int brojElemenata() const = 0;
    virtual void obrisi() = 0;
    virtual void obrisi(const K &k) = 0;
};

// ---- BinStabloMapa ----
template<typename K, typename V>
class BinStabloMapa : public Mapa<K, V> {
public:
    struct Cvor {
        K kljuc;
        V vrijednost;
        Cvor *lijevo;
        Cvor *desno;
        Cvor *roditelj;
        Cvor(const K &k, const V &v, Cvor *r = nullptr)
            : kljuc(k), vrijednost(v), lijevo(nullptr), desno(nullptr), roditelj(r) {}
    };

private:
    Cvor *korijen;
    int velicina;

    Cvor* trazi(const K &k) const {
        Cvor *p = korijen;
        while (p != nullptr) {
            if (k == p->kljuc) return p;
            if (k < p->kljuc) p = p->lijevo;
            else p = p->desno;
        }
        return nullptr;
    }

    void obrisiStablo(Cvor *c) {
        if (!c) return;
        obrisiStablo(c->lijevo);
        obrisiStablo(c->desno);
        delete c;
    }

public:
    BinStabloMapa() : korijen(nullptr), velicina(0) {}
    ~BinStabloMapa() override { obrisiStablo(korijen); }

    int brojElemenata() const override { return velicina; }

    void obrisi() override { obrisiStablo(korijen); korijen = nullptr; velicina = 0; }
    void obrisi(const K &k) override {
        // ostavljeno prazno radi jednostavnosti
    }

    V &operator[](const K &k) override {
        Cvor *p = korijen;
        Cvor *roditelj = nullptr;

        while (p != nullptr) {
            if (k == p->kljuc) return p->vrijednost;
            roditelj = p;
            if (k < p->kljuc) p = p->lijevo;
            else p = p->desno;
        }

        Cvor *novi = new Cvor(k, V(), roditelj);
        if (!roditelj) korijen = novi;
        else if (k < roditelj->kljuc) roditelj->lijevo = novi;
        else roditelj->desno = novi;

        velicina++;
        return novi->vrijednost;
    }

    V operator[](const K &k) const override {
        Cvor *p = trazi(k);
        if (!p) return V();
        return p->vrijednost;
    }

    // Getter korijena za obilazak
    Cvor* getKorijen() const { return korijen; }
};

// ---- Funkcija za preorder obilazak ----
template<typename K, typename V>
void preorder(typename BinStabloMapa<K,V>::Cvor *c) {
    if (!c) return;
    std::cout << c->kljuc << " ";
    preorder<K,V>(c->lijevo);
    preorder<K,V>(c->desno);
}


int main() {
    std::string ime;
    std::cout << "Unesite svoje ime i prezime: ";
    std::getline(std::cin, ime);

    // Uklanjanje ponavljajucih slova i razmaka
    std::string jedinstveno;
    std::set<char> pojavila;
    for(char c : ime) {
        if (c == ' ') continue;
        if (pojavila.find(c) == pojavila.end()) {
            jedinstveno += c;
            pojavila.insert(c);
        }
    }

    std::cout << "Jedinstvena slova: " << jedinstveno << "\n";

    // Ubacivanje u BinStabloMapa
    BinStabloMapa<char,int> stablo;
    for(char c : jedinstveno) {
        stablo[c] = 1;
    }

    std::cout << "Broj cvorova u stablu: " << stablo.brojElemenata() << "\n";

    // Preorder ispis (Korijen - Lijevo - Desno)
    std::cout << "Slova u preorder (redoslijed unosa u stablo): ";
    preorder<char,int>(stablo.getKorijen());
    std::cout << "\n";

    return 0;
}
