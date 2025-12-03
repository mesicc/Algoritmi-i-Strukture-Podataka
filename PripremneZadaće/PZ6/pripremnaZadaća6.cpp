#include <iostream>
#include <cmath>
#include <vector>
#include <list>
#include <stdexcept>
using namespace std;

/*
==========================================================
                ZADATAK 1 – Apstraktna klasa Mapa
==========================================================
- generička klasa sa dva tipa
- konstruktor bez parametara
- operator[] (const + non-const)
- brojElemenata()
- obrisi()
- obrisi(kljuc)
- klasa mora biti apstraktna
==========================================================
*/

template <typename T1, typename T2>
class Mapa{
    public:
    Mapa(){}                                        // ✔ Zadatak 1: konstruktor bez parametara

    virtual T2 &operator[](T1 el)=0;                // ✔ Zadatak 1: nekonstantni operator[]
    virtual const T2 operator[](T1 el) const=0;     // ✔ Zadatak 1: konstantni operator[]

    virtual int brojElemenata()const=0;             // ✔ Zadatak 1: brojElemenata()

    virtual void obrisi()=0;                        // ✔ Zadatak 1: brisanje svih elemenata
    virtual void obrisi(const T1& kljuc)=0;         // ✔ Zadatak 1: brisanje elementa po ključu

    virtual ~Mapa(){};                              // ✔ Zadatak 1: virtualni destruktor
};


/*
==========================================================
                ZADATAK 2 – Klasa NizMapa
==========================================================
- izvedena iz Mapa
- implementacija pomoću dinamičkog niza
- čuvaju se parovi (kljuc, vrijednost)
- automatsko proširenje kapaciteta
- podrška za kopiranje i dodjelu
- sve operacije rade u skladu s postavkom
- mapa ostaje funkcionalna nakon brisanja
==========================================================
*/

template <typename T1,typename T2>
class NizMapa : public Mapa<T1,T2>{

    // ✔ Zadatak 2: privatni atributi
    int velicina, br_elemenata;
    pair<T1,T2> **p;

    public:

    // ✔ Zadatak 2: konstruktor koji kreira mapu kapaciteta 100000
    NizMapa():Mapa<T1,T2>(),velicina(100000),br_elemenata(0),p(new pair<T1,T2>*[velicina]()){}

    // ✔ Zadatak 2: kopirni konstruktor (duboko kopiranje)
    NizMapa(const NizMapa<T1,T2> &m){
        velicina=m.velicina;
        br_elemenata=m.br_elemenata;
        if(m.br_elemenata==0)
            p=nullptr;
        else{
            p=new pair<T1,T2>*[velicina]();
        }
        for(int i=0;i<m.br_elemenata;i++)
            p[i]=new pair<T1,T2>(*m.p[i]);
    }

    // ✔ Zadatak 2: destruktor (oslobađa memoriju)
    ~NizMapa(){
        for(int i=0;i<br_elemenata;i++)
            delete p[i];
        delete[] p;
        br_elemenata=0;
        velicina=0;
        p=nullptr;
    }

    // ✔ Zadatak 2: operator dodjele (kopiraj i zamijeni)
    NizMapa &operator=(NizMapa<T1,T2> m){
        swap(velicina,m.velicina);
        swap(br_elemenata,m.br_elemenata);
        swap(p,m.p);
        return *this;
    }

    // ✔ Zadatak 1 + 2: vraća broj elemenata u mapi
    int brojElemenata() const{
        return br_elemenata;
    }

    // ✔ Zadatak 1 + 2: obriši sve elemente (ostaje funkcionalna)
    void obrisi(){
        if(br_elemenata==0)
            throw std::logic_error("Nema elemenata");
        for(int i=0;i<br_elemenata;i++)
            delete p[i];
        br_elemenata=0;
    }

    // ✔ Zadatak 1 + 2: brisanje jednog elementa po ključu
    void obrisi(const T1& kljuc){
        for(int i=0;i<br_elemenata;i++){
            if(p[i]->first!=kljuc)
                continue;
            delete p[i];
            for(int j=i;j<br_elemenata-1;j++)
                p[j]=p[j+1];
            p[br_elemenata]=nullptr;
            br_elemenata--;
            return;
        }
        throw std::logic_error("Nema elemenata");
    }

    // ✔ Zadatak 1 + 2: nekonstantni operator[]
    // – dodaje novi element ako ne postoji
    // – automatski proširuje kapacitet
    T2 &operator[](T1 el){
        if(velicina==br_elemenata){
            // ✔ Zadatak 2: proširenje dinamičkog niza
            pair<T1,T2> **pomocni;    
            pomocni=new pair<T1,T2>*[velicina*2]();
            for(int i=0;i<br_elemenata;i++)
            {
                pomocni[i]= new pair<T1,T2>(*p[i]);
                delete p[i];
            }
            delete[] p;
            velicina=velicina*2;
            p=pomocni;
        }
        // ✔ traženje ključa
        for(int i=0;i<br_elemenata;i++)
            if(p[i]->first==el)return p[i]->second; 
        
        // ✔ dodavanje novog elementa
        p[br_elemenata]=new pair<T1,T2>(el,T2());
        br_elemenata++;
        return p[br_elemenata-1]->second;
    }

    // ✔ Zadatak 1 + 2: konstantni operator[]
    const T2 operator[](T1 el) const{
        for(int i=0;i<br_elemenata;i++)
            if(p[i]->first==el)return p[i]->second; 
        return T2(); // default vrijednost
    }
};


/*
==========================================================
                 TEST FUNKCIJE (Zadatak 2)
==========================================================
- Testiranje osnovne funkcionalnosti
- Sve što postavka traži: min 5 testova
==========================================================
*/

void testBrojElemenata() {
    NizMapa<int, std::string> mapa;
    std::cout << "Broj elemenata: " << mapa.brojElemenata() << std::endl;

    mapa[1] = "jedan";
    mapa[2] = "dva";
    std::cout << "Broj elemenata nakon dodavanja: " << mapa.brojElemenata() << std::endl;
}

void testOperator() {
    NizMapa<int, std::string> mapa;
    mapa[1] = "jedan";
    mapa[2] = "dva";

    std::cout << "Element sa kljucem 1: " << mapa[1] << std::endl;
    std::cout << "Element sa kljucem 2: " << mapa[2] << std::endl;

    const NizMapa<int, std::string> constMapa = mapa;
    std::cout << "Const element sa kljucem 1: " << constMapa[1] << std::endl;
    std::cout << "Const element sa kljucem 2: " << constMapa[2] << std::endl;
    std::cout << "Const element sa nepostojećim kljucem 3: " << constMapa[3] << std::endl;
}

void testObrisi() {
    NizMapa<int, std::string> mapa;
    mapa[1] = "jedan";
    mapa[2] = "dva";

    std::cout << "Broj elemenata pre brisanja: " << mapa.brojElemenata() << std::endl;
    mapa.obrisi(1);
    std::cout << "Broj elemenata nakon brisanja: " << mapa.brojElemenata() << std::endl;
    std::cout << "Element sa kljucem 1 nakon brisanja: " << mapa[1] << std::endl;

    try {
        mapa.obrisi(3);
    } catch (const std::logic_error& e) {
        std::cout << "Izuzetak uhvaćen: " << e.what() << std::endl;
    }
}

void testObrisiSve() {
    NizMapa<int, std::string> mapa;
    mapa[1] = "jedan";
    mapa[2] = "dva";

    std::cout << "Broj elemenata pre brisanja: " << mapa.brojElemenata() << std::endl;
    mapa.obrisi();
    std::cout << "Broj elemenata nakon brisanja: " << mapa.brojElemenata() << std::endl;
}

int main() {
    testBrojElemenata();
    testOperator();
    testObrisi();
    testObrisiSve();

    std::cout << "Svi testovi uspesno izvrseni!" << std::endl;

    return 0;
}
