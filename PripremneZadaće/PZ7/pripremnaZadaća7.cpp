#include <iostream>
#include <stdexcept>

template<typename K, typename V>
class BinStabloMapa : public Mapa<K,V> {

    struct Cvor {
        K kljuc;
        V vrijednost;
        Cvor *lijevo, *desno, *roditelj;

        Cvor(const K &k, const V &v, Cvor *r = nullptr)
            : kljuc(k), vrijednost(v), lijevo(nullptr), desno(nullptr), roditelj(r) {}
    };

    Cvor *korijen;
    int velicina;

    // Pomoćna rekurzivna funkcija za brisanje stabla
    void obrisiStablo(Cvor *c) {
        if (!c) return;
        obrisiStablo(c->lijevo);
        obrisiStablo(c->desno);
        delete c;
    }

    // Pomoćno rekurzivno kopiranje
    Cvor* kopiraj(Cvor *c, Cvor *roditelj) {
        if (!c) return nullptr;
        Cvor *novi = new Cvor(c->kljuc, c->vrijednost, roditelj);
        novi->lijevo = kopiraj(c->lijevo, novi);
        novi->desno = kopiraj(c->desno, novi);
        return novi;
    }

    // Pronalaženje cvora po ključa
    Cvor* trazi(const K &k) const {
        Cvor *p = korijen;
        while (p != nullptr && p->kljuc != k) {
            if (k < p->kljuc) p = p->lijevo;
            else p = p->desno;
        }
        return p;
    }

public:

    BinStabloMapa() : korijen(nullptr), velicina(0) {}

    // Kopirajuci konstruktor
    BinStabloMapa(const BinStabloMapa &b) {
        korijen = kopiraj(b.korijen, nullptr);
        velicina = b.velicina;
    }

    // Operacija dodjele
    BinStabloMapa &operator=(const BinStabloMapa &b) {
        if (this == &b) return *this;
        obrisiStablo(korijen);
        korijen = kopiraj(b.korijen, nullptr);
        velicina = b.velicina;
        return *this;
    }

    ~BinStabloMapa() { obrisiStablo(korijen); }

    int brojElemenata() const override { return velicina; }

    void obrisi() override {
        obrisiStablo(korijen);
        korijen = nullptr;
        velicina = 0;
    }

    void obrisi(const K &k) override {
        Cvor *c = trazi(k);
        if (!c) return;

        // Slučaj 1: čvor je list
        if (!c->lijevo && !c->desno) {
            if (c->roditelj == nullptr) korijen = nullptr;
            else if (c->roditelj->lijevo == c) c->roditelj->lijevo = nullptr;
            else c->roditelj->desno = nullptr;
            delete c;
        }
        // Slučaj 2: ima samo jedno dijete
        else if (!c->lijevo || !c->desno) {
            Cvor *dijete = c->lijevo ? c->lijevo : c->desno;
            dijete->roditelj = c->roditelj;

            if (c->roditelj == nullptr) korijen = dijete;
            else if (c->roditelj->lijevo == c) c->roditelj->lijevo = dijete;
            else c->roditelj->desno = dijete;

            delete c;
        }
        // Slučaj 3: ima dva djeteta
        else {
            // Nađi najveći u lijevom podstablu (prethodnika)
            Cvor *p = c->lijevo;
            while (p->desno) p = p->desno;

            // kopiraj podatke
            c->kljuc = p->kljuc;
            c->vrijednost = p->vrijednost;

            // obriši taj čvor
            if (p->roditelj->desno == p) p->roditelj->desno = p->lijevo;
            else p->roditelj->lijevo = p->lijevo;

            if (p->lijevo) p->lijevo->roditelj = p->roditelj;

            delete p;
        }

        velicina--;
    }

    // operator []
    V &operator[](const K &k) override {
        Cvor *p = korijen, *roditelj = nullptr;

        while (p != nullptr) {
            if (k == p->kljuc) return p->vrijednost;
            roditelj = p;
            if (k < p->kljuc) p = p->lijevo;
            else p = p->desno;
        }

        // Ako ne postoji – kreiramo novi čvor
        Cvor *novi = new Cvor(k, V(), roditelj);
        if (roditelj == nullptr) korijen = novi;
        else if (k < roditelj->kljuc) roditelj->lijevo = novi;
        else roditelj->desno = novi;

        velicina++;
        return novi->vrijednost;
    }

    // konst verzija operatora []
    V operator[](const K &k) const override {
        Cvor *p = trazi(k);
        if (!p) return V();
        return p->vrijednost;
    }
};
