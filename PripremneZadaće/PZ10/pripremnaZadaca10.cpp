#include <iostream>
#include <vector>
#include <queue>
#include <stdexcept>

template <typename TipOznake> class UsmjereniGraf;
template <typename TipOznake> class Cvor;
template <typename TipOznake> class Grana;
template <typename TipOznake> class GranaIterator;
template <typename TipOznake> class MatricaGraf;

template <typename TipOznake>
class Cvor {
    UsmjereniGraf<TipOznake>* graf_;
    int rb_;
public:
    Cvor(UsmjereniGraf<TipOznake>* graf = nullptr, int rb = 0) : graf_(graf), rb_(rb) {}

    int dajRedniBroj() const { return rb_; }

    TipOznake dajOznaku() const;
    void postaviOznaku(const TipOznake& oznaka);
};

template <typename TipOznake>
class Grana {
    UsmjereniGraf<TipOznake>* graf_;
    int polazni_, dolazni_;
public:
    Grana(UsmjereniGraf<TipOznake>* graf = nullptr, int p = 0, int d = 0)
        : graf_(graf), polazni_(p), dolazni_(d) {}

    float dajTezinu() const;
    void postaviTezinu(float tezina);

    TipOznake dajOznaku() const;
    void postaviOznaku(const TipOznake& oznaka);

    Cvor<TipOznake> dajPolazniCvor() const { return Cvor<TipOznake>(graf_, polazni_); }
    Cvor<TipOznake> dajDolazniCvor() const { return Cvor<TipOznake>(graf_, dolazni_); }
};

template <typename TipOznake>
class UsmjereniGraf {
public:
    virtual ~UsmjereniGraf() {}

    virtual int dajBrojCvorova() const = 0;
    virtual void postaviBrojCvorova(int brojCvorova) = 0;

    virtual void dodajGranu(int polazni, int dolazni, float tezina = 0) = 0;
    virtual void obrisiGranu(int polazni, int dolazni) = 0;

    virtual void postaviTezinuGrane(int polazni, int dolazni, float tezina) = 0;
    virtual float dajTezinuGrane(int polazni, int dolazni) const = 0;

    virtual bool postojiGrana(int polazni, int dolazni) const = 0;

    virtual void postaviOznakuCvora(int cvor, const TipOznake& oznaka) = 0;
    virtual TipOznake dajOznakuCvora(int cvor) const = 0;

    virtual void postaviOznakuGrane(int polazni, int dolazni, const TipOznake& oznaka) = 0;
    virtual TipOznake dajOznakuGrane(int polazni, int dolazni) const = 0;

    virtual Cvor<TipOznake> dajCvor(int cvor) = 0;
    virtual Grana<TipOznake> dajGranu(int polazni, int dolazni) = 0;

    virtual GranaIterator<TipOznake> dajGranePocetak() = 0;
    virtual GranaIterator<TipOznake> dajGraneKraj() = 0;
};

template <typename TipOznake>
class GranaIterator {
    UsmjereniGraf<TipOznake>* graf_;
    int i_, j_;
    void skociNaSljedecuPostojecu() {
        if (!graf_) return;
        int n = graf_->dajBrojCvorova();
        while (i_ < n) {
            while (j_ < n) {
                if (graf_->postojiGrana(i_, j_)) return;
                ++j_;
            }
            ++i_;
            j_ = 0;
        }
    }
public:
    GranaIterator(UsmjereniGraf<TipOznake>* graf = nullptr, int i = 0, int j = 0)
        : graf_(graf), i_(i), j_(j) {
        skociNaSljedecuPostojecu();
    }

    Grana<TipOznake> operator*() { return graf_->dajGranu(i_, j_); }

    bool operator==(const GranaIterator& iter) const {
        return graf_ == iter.graf_ && i_ == iter.i_ && j_ == iter.j_;
    }
    bool operator!=(const GranaIterator& iter) const { return !(*this == iter); }

    GranaIterator& operator++() {
        if (!graf_) throw std::logic_error("Neispravan iterator");
        int n = graf_->dajBrojCvorova();
        if (i_ >= n) throw std::out_of_range("Iterator je na kraju grafa");

        ++j_;
        skociNaSljedecuPostojecu();
        return *this;
    }

    GranaIterator operator++(int) {
        GranaIterator tmp(*this);
        ++(*this);
        return tmp;
    }

    friend class MatricaGraf<TipOznake>;
};

template <typename TipOznake>
class MatricaGraf : public UsmjereniGraf<TipOznake> {
    struct Elem {
        bool postoji;
        float tezina;
        TipOznake oznaka;
        Elem() : postoji(false), tezina(0), oznaka(TipOznake()) {}
    };

    int n_;
    std::vector<std::vector<Elem>> mat_;
    std::vector<TipOznake> oznakeCvorova_;

    void provjeriCvor(int v) const {
        if (v < 0 || v >= n_) throw std::domain_error("Neispravan cvor");
    }
    void provjeriGranu(int p, int d) const {
        provjeriCvor(p);
        provjeriCvor(d);
        if (!mat_[p][d].postoji) throw std::domain_error("Grana ne postoji");
    }

public:
    explicit MatricaGraf(int brojCvorova)
        : n_(brojCvorova),
          mat_(brojCvorova, std::vector<Elem>(brojCvorova)),
          oznakeCvorova_(brojCvorova, TipOznake()) {
        if (brojCvorova < 0) throw std::domain_error("Neispravan broj cvorova");
    }

    int dajBrojCvorova() const override { return n_; }

    void postaviBrojCvorova(int brojCvorova) override {
        if (brojCvorova < n_) throw std::domain_error("Smanjenje broja cvorova nije dozvoljeno");
        if (brojCvorova == n_) return;

        mat_.resize(brojCvorova);
        for (int i = 0; i < brojCvorova; i++) mat_[i].resize(brojCvorova);
        oznakeCvorova_.resize(brojCvorova, TipOznake());
        n_ = brojCvorova;
    }

    void dodajGranu(int polazni, int dolazni, float tezina = 0) override {
        provjeriCvor(polazni);
        provjeriCvor(dolazni);
        mat_[polazni][dolazni].postoji = true;
        mat_[polazni][dolazni].tezina = tezina;
    }

    void obrisiGranu(int polazni, int dolazni) override {
        provjeriGranu(polazni, dolazni);
        mat_[polazni][dolazni].postoji = false;
        mat_[polazni][dolazni].tezina = 0;
        mat_[polazni][dolazni].oznaka = TipOznake();
    }

    void postaviTezinuGrane(int polazni, int dolazni, float tezina) override {
        provjeriGranu(polazni, dolazni);
        mat_[polazni][dolazni].tezina = tezina;
    }

    float dajTezinuGrane(int polazni, int dolazni) const override {
        provjeriGranu(polazni, dolazni);
        return mat_[polazni][dolazni].tezina;
    }

    bool postojiGrana(int polazni, int dolazni) const override {
        provjeriCvor(polazni);
        provjeriCvor(dolazni);
        return mat_[polazni][dolazni].postoji;
    }

    void postaviOznakuCvora(int cvor, const TipOznake& oznaka) override {
        provjeriCvor(cvor);
        oznakeCvorova_[cvor] = oznaka;
    }

    TipOznake dajOznakuCvora(int cvor) const override {
        provjeriCvor(cvor);
        return oznakeCvorova_[cvor];
    }

    void postaviOznakuGrane(int polazni, int dolazni, const TipOznake& oznaka) override {
        provjeriGranu(polazni, dolazni);
        mat_[polazni][dolazni].oznaka = oznaka;
    }

    TipOznake dajOznakuGrane(int polazni, int dolazni) const override {
        provjeriGranu(polazni, dolazni);
        return mat_[polazni][dolazni].oznaka;
    }

    Cvor<TipOznake> dajCvor(int cvor) override {
        provjeriCvor(cvor);
        return Cvor<TipOznake>(this, cvor);
    }

    Grana<TipOznake> dajGranu(int polazni, int dolazni) override {
        provjeriGranu(polazni, dolazni);
        return Grana<TipOznake>(this, polazni, dolazni);
    }

    GranaIterator<TipOznake> dajGranePocetak() override {
        return GranaIterator<TipOznake>(this, 0, 0);
    }

    GranaIterator<TipOznake> dajGraneKraj() override {
        return GranaIterator<TipOznake>(this, n_, 0);
    }

    std::vector<int> dajSusjede(int u) const {
        provjeriCvor(u);
        std::vector<int> susjedi;
        susjedi.reserve(n_);
        for (int v = 0; v < n_; v++) {
            if (mat_[u][v].postoji) susjedi.push_back(v);
        }
        return susjedi;
    }
};

template <typename TipOznake>
TipOznake Cvor<TipOznake>::dajOznaku() const {
    if (!graf_) throw std::logic_error("Cvor nije vezan za graf");
    return graf_->dajOznakuCvora(rb_);
}

template <typename TipOznake>
void Cvor<TipOznake>::postaviOznaku(const TipOznake& oznaka) {
    if (!graf_) throw std::logic_error("Cvor nije vezan za graf");
    graf_->postaviOznakuCvora(rb_, oznaka);
}

template <typename TipOznake>
float Grana<TipOznake>::dajTezinu() const {
    if (!graf_) throw std::logic_error("Grana nije vezana za graf");
    return graf_->dajTezinuGrane(polazni_, dolazni_);
}

template <typename TipOznake>
void Grana<TipOznake>::postaviTezinu(float tezina) {
    if (!graf_) throw std::logic_error("Grana nije vezana za graf");
    graf_->postaviTezinuGrane(polazni_, dolazni_, tezina);
}

template <typename TipOznake>
TipOznake Grana<TipOznake>::dajOznaku() const {
    if (!graf_) throw std::logic_error("Grana nije vezana za graf");
    return graf_->dajOznakuGrane(polazni_, dolazni_);
}

template <typename TipOznake>
void Grana<TipOznake>::postaviOznaku(const TipOznake& oznaka) {
    if (!graf_) throw std::logic_error("Grana nije vezana za graf");
    graf_->postaviOznakuGrane(polazni_, dolazni_, oznaka);
}

template <typename TipOznake>
void bfs(UsmjereniGraf<TipOznake>* graf, int pocetni) {
    if (!graf) throw std::logic_error("Graf je nullptr");
    int n = graf->dajBrojCvorova();
    if (pocetni < 0 || pocetni >= n) throw std::domain_error("Neispravan pocetni cvor");

    std::vector<bool> posjecen(n, false);
    std::queue<int> q;

    posjecen[pocetni] = true;
    q.push(pocetni);

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        std::cout << u << " ";

        for (int v = 0; v < n; v++) {
            if (graf->postojiGrana(u, v) && !posjecen[v]) {
                posjecen[v] = true;
                q.push(v);
            }
        }
    }
    std::cout << std::endl;
}

template <typename TipOznake>
void dfsRek(UsmjereniGraf<TipOznake>* graf, int u, std::vector<bool>& posjecen) {
    posjecen[u] = true;
    std::cout << u << " ";

    int n = graf->dajBrojCvorova();
    for (int v = 0; v < n; v++) {
        if (graf->postojiGrana(u, v) && !posjecen[v]) dfsRek(graf, v, posjecen);
    }
}

template <typename TipOznake>
void dfs(UsmjereniGraf<TipOznake>* graf, int pocetni) {
    if (!graf) throw std::logic_error("Graf je nullptr");
    int n = graf->dajBrojCvorova();
    if (pocetni < 0 || pocetni >= n) throw std::domain_error("Neispravan pocetni cvor");

    std::vector<bool> posjecen(n, false);
    dfsRek(graf, pocetni, posjecen);
    std::cout << std::endl;
}


int main() {
    try {
        UsmjereniGraf<bool>* g = new MatricaGraf<bool>(3);
        g->dodajGranu(0, 1, 2.5f);
        g->dodajGranu(1, 0, 1.2f);
        g->dodajGranu(1, 2, 0.1f);
        g->dodajGranu(0, 0, 3.14f);

        for (GranaIterator<bool> iter = g->dajGranePocetak();
             iter != g->dajGraneKraj(); ++iter) {
            std::cout << (*iter).dajPolazniCvor().dajRedniBroj() << " "
                      << (*iter).dajDolazniCvor().dajRedniBroj() << " "
                      << (*iter).dajTezinu() << std::endl;
        }

        delete g;
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    } catch (const char izuzetak[]) {
        std::cout << izuzetak << std::endl;
    }
    return 0;
}
