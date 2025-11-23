//#include <endian.h>
#include <iostream>
#include <new>
#include <stdexcept>

using namespace std;

template<typename Tip>
class Lista
{
public:
    Lista() {}
    virtual ~Lista() {};
    virtual int brojElemenata() const = 0;
    virtual Tip trenutni() const = 0;
    virtual Tip &trenutni() = 0;
    virtual bool prethodni() = 0;
    virtual bool sljedeci() = 0;
    virtual void pocetak() = 0;
    virtual void kraj() = 0;
    virtual void obrisi() = 0;
    virtual void dodajIspred(const Tip& el) = 0;
    virtual void dodajIza(const Tip& el) = 0;
    virtual Tip operator [](int indeks) const = 0;
    virtual Tip &operator [](int indeks) = 0;
};

template<typename Tip>
struct Cvor
{
    Tip clan;
    Cvor<Tip>* sljedeci=nullptr;
    Cvor<Tip>* prethodni=nullptr;
    Cvor(Tip Clan)
    {
        clan=Clan;
    }
};

template<typename Tip>
class DvostrukaLista: public Lista<Tip>
{
    int VelicinaListe=0;
    Cvor<Tip>* PocetakListe=nullptr;
    Cvor<Tip>* KrajListe=nullptr;
    Cvor<Tip>* poknatrenutni=nullptr;
public:
    DvostrukaLista()
    {
        VelicinaListe=0;
        PocetakListe=nullptr;
        KrajListe=nullptr;
        poknatrenutni=nullptr;
    }
    ~DvostrukaLista()
    {
        while(this->VelicinaListe>0)
        {
            this->obrisi();
        }
    }
    DvostrukaLista(const DvostrukaLista<Tip> &l)
    {
        this->VelicinaListe=l.VelicinaListe;
        auto pokazivac=l.PocetakListe;
        auto pomocni=pokazivac;
        auto sljedeci=pokazivac;
        for(int i=0;i<this->VelicinaListe;i++)
        {
            pomocni=new Cvor<Tip>(pokazivac->clan);
            if(i==0)
            {
                this->PocetakListe=pomocni;
                sljedeci=pomocni;
            }
            else
            {
                sljedeci->sljedeci=pomocni;
                pomocni->prethodni=sljedeci;
                sljedeci=sljedeci->sljedeci;
            }
            if(l.poknatrenutni==pokazivac)
            {
                this->poknatrenutni=pomocni;
            }
            if(i!=this->VelicinaListe-1)
            {
                pomocni=pomocni->sljedeci;
            }
            pokazivac=pokazivac->sljedeci;
        }
        this->KrajListe=pomocni;
    }
    DvostrukaLista(DvostrukaLista<Tip> &&l)
    {
        this->PocetakListe=l.PocetakListe;
        this->KrajListe=l.KrajListe;
        this->poknatrenutni=l.poknatrenutni;
        this->VelicinaListe=l.VelicinaListe;
        l.PocetakListe=nullptr;
        l.KrajListe=nullptr;
        l.poknatrenutni=nullptr;
        l.VelicinaListe=0;
    }
    DvostrukaLista<Tip> &operator =(const DvostrukaLista<Tip> &l)
    {
        if(this->PocetakListe!=l.PocetakListe && this->KrajListe!=l.KrajListe)
        {
            auto pokazivac=PocetakListe;
            auto sljedeci=pokazivac;
            while(pokazivac!=nullptr)
            {

                sljedeci=pokazivac->sljedeci;
                delete pokazivac;
                pokazivac=sljedeci;
            }
            this->PocetakListe=nullptr;
            this->KrajListe=nullptr;
            this->poknatrenutni=nullptr;
            this->VelicinaListe=l.VelicinaListe;
            auto pomocni=sljedeci;
            pokazivac=l.PocetakListe;
            for(int i=0;i<this->VelicinaListe;i++)
            {
                pomocni=new Cvor<Tip>(pokazivac->clan);
                if(i==0)
                {
                    this->PocetakListe=pomocni;
                    sljedeci=pomocni;
                }
                else
                {
                    sljedeci->sljedeci=pomocni;
                    pomocni->prethodni=sljedeci;
                    sljedeci=sljedeci->sljedeci;
                }
                if(l.poknatrenutni==pokazivac)
                {
                    this->poknatrenutni=pomocni;
                }
                if(i!=this->VelicinaListe-1)
                {
                    pomocni=pomocni->sljedeci;
                }
                pokazivac=pokazivac->sljedeci;
            }
            this->KrajListe=pomocni;
        }
        return *this;
    }
    DvostrukaLista<Tip> &operator =(DvostrukaLista<Tip> &&l)
    {
        std::swap(VelicinaListe,l.VelicinaListe);
        std::swap(PocetakListe,l.PocetakListe);
        std::swap(KrajListe,l.KrajListe);
        std::swap(poknatrenutni,l.poknatrenutni);
    }
    int brojElemenata() const
    {
        return VelicinaListe;
    }
    Tip trenutni() const
    {
        if(VelicinaListe<1)
        {
            throw std::range_error("Greska");
        }
        return poknatrenutni->clan;
    }
    Tip &trenutni()
    {
        if(VelicinaListe<1)
        {
            throw std::range_error("Greska");
        }
        return poknatrenutni->clan;
    }
    bool prethodni()
    {
        if(VelicinaListe<1)
        {
            return false;
        }
        poknatrenutni=poknatrenutni->prethodni;
        return true;
    }
    bool sljedeci()
    {
        if(VelicinaListe<1)
        {
            return false;
        }
        if(poknatrenutni==KrajListe)
        {
            return false;
        }
        poknatrenutni=poknatrenutni->sljedeci;
        return true;
    }
    void pocetak()
    {
        if(VelicinaListe<1)
        {
            throw std::range_error("Greska");
        }
        poknatrenutni=PocetakListe;
    }
    void kraj()
    {
        if(VelicinaListe<1)
        {
            throw std::range_error("Greska");
        }
        poknatrenutni=KrajListe;
    }
    void dodajIspred(const Tip& el)
    {
        if(VelicinaListe==0)
        {
            PocetakListe=new Cvor<Tip>(el);
            poknatrenutni=PocetakListe;
            KrajListe=PocetakListe;
        }
        else if(poknatrenutni==PocetakListe)
        {
            PocetakListe=new Cvor<Tip>(el);
            PocetakListe->sljedeci=poknatrenutni;
            poknatrenutni->prethodni=PocetakListe;
        }
        else
        {
            auto novi=new Cvor<Tip>(el);
            (poknatrenutni->prethodni)->sljedeci=novi;
            novi->prethodni=poknatrenutni->prethodni;
            poknatrenutni->prethodni=novi;
            novi->sljedeci=poknatrenutni;
        }
        VelicinaListe+=1;
    }
    void dodajIza(const Tip& el)
    {
        if(VelicinaListe==0)
        {
            poknatrenutni=new Cvor<Tip>(el);
            PocetakListe=poknatrenutni;
            KrajListe=PocetakListe;
        }
        else if(poknatrenutni==KrajListe)
        {
            KrajListe=new Cvor<Tip>(el);
            KrajListe->prethodni=poknatrenutni;
            poknatrenutni->sljedeci=KrajListe;
        }
        else
        {
            auto novi=new Cvor<Tip>(el);
            (poknatrenutni->sljedeci)->prethodni=novi;
            novi->sljedeci=poknatrenutni->sljedeci;
            novi->prethodni=poknatrenutni;
            poknatrenutni->sljedeci=novi;
        }
        VelicinaListe+=1;
    }
    void obrisi()
    {
        if(VelicinaListe<1)
        {
            throw std::range_error("Greska");
        }
        if(poknatrenutni==KrajListe && poknatrenutni==PocetakListe)
        {
            delete poknatrenutni;
            VelicinaListe=0;
            poknatrenutni=nullptr;
            PocetakListe=nullptr;
            KrajListe=nullptr;
            VelicinaListe+=1;
        }
        else if(poknatrenutni==KrajListe)
        {
            auto pamti=KrajListe;
            KrajListe=KrajListe->prethodni;
            KrajListe->sljedeci=nullptr;
            delete pamti;
            poknatrenutni=KrajListe;
        }
        else if(poknatrenutni==PocetakListe)
        {
            auto pamti=PocetakListe;
            PocetakListe=PocetakListe->sljedeci;
            PocetakListe->prethodni=nullptr;
            delete pamti;
            poknatrenutni=PocetakListe;
        }
        else
        {
            auto pamti=poknatrenutni->sljedeci;
            (poknatrenutni->prethodni)->sljedeci=poknatrenutni->sljedeci;
            (poknatrenutni->sljedeci)->prethodni=poknatrenutni->prethodni;
            delete poknatrenutni;
            poknatrenutni=pamti;
        }
        VelicinaListe+=-1;
    }
    Tip operator[](int indeks) const
    {
        if(indeks<0 || indeks>=VelicinaListe)
        {
            throw std::range_error("Greska");
        }
        auto pokazivac=PocetakListe;
        if(indeks<=VelicinaListe-1-indeks)
        {
            pokazivac=PocetakListe;
            for(int i=0;i<indeks;i++)
            {
                pokazivac=pokazivac->sljedeci;
            }
        }
        else
        {
            pokazivac=KrajListe;
            for(int i=VelicinaListe-1;i>indeks;i--)
            {
                pokazivac=pokazivac->prethodni;
            }
        }
        return pokazivac->clan;
    }
    Tip &operator[](int indeks)
    {
        if(indeks<0 || indeks>=VelicinaListe)
        {
            throw std::range_error("Greska");
        }
        auto pokazivac=PocetakListe;
        if(indeks<=VelicinaListe-1-indeks)
        {
            pokazivac=PocetakListe;
            for(int i=0;i<indeks;i++)
            {
                pokazivac=pokazivac->sljedeci;
            }
        }
        else
        {
            pokazivac=KrajListe;
            for(int i=VelicinaListe-1;i>indeks;i--)
            {
                pokazivac=pokazivac->prethodni;
            }
        }
        return pokazivac->clan;
    }
};

template<typename Tip>
Tip dajMaksimum(const Lista<Tip>& n)
{
        if(n.brojElemenata()<1)
        {
            throw std::range_error("Greska");
        }
        Tip max=n[0];
        Tip uporedi;
        for(int i=0;i<n.brojElemenata();i++)
        {
            uporedi=n[i];
            if(uporedi>max)
            {
                max=uporedi;
            }
        }
        return max;
}

// Test Funkcije

template<typename Tip>
void TestDvostrukaListaDodajIspred(DvostrukaLista<Tip> &TestLista)
{
    TestLista.dodajIspred(1);
    TestLista.dodajIspred(2);
    TestLista.dodajIspred(3);
    std::cout << TestLista.brojElemenata() << " " << TestLista.trenutni() << "\n";
}

template<typename Tip>
void TestDvostrukaListaDodajIza(DvostrukaLista<Tip> &TestLista)
{
    TestLista.dodajIza(1);
    TestLista.dodajIza(2);
    TestLista.dodajIza(3);
    std::cout << TestLista.brojElemenata() << " " << TestLista.trenutni() << "\n";
}

template<typename Tip>
void TestDvostrukaListaObrisi(DvostrukaLista<Tip> &TestLista)
{
    std::cout << TestLista.brojElemenata() << " " << TestLista.trenutni() << "\n";
    TestLista.obrisi();
    std::cout << TestLista.brojElemenata() << " " << TestLista.trenutni() << "\n";
}

template<typename Tip>
void TestDvostrukaListaBrojElemenata(DvostrukaLista<Tip> &TestLista)
{
    std::cout << TestLista.brojElemenata() << " ";
    TestLista.dodajIspred(10);
    std::cout << TestLista.brojElemenata() << "\n";
}

template<typename Tip>
void TestDvostrukaListaPocetak(DvostrukaLista<Tip> &TestLista)
{
    std::cout << TestLista.trenutni() << " ";
    TestLista.pocetak();
    std::cout << TestLista.trenutni() << "\n";
}

template<typename Tip>
void TestDvostrukaListaKraj(DvostrukaLista<Tip> &TestLista)
{
    std::cout << TestLista.trenutni() << " ";
    TestLista.kraj();
    std::cout << TestLista.trenutni() << "\n";
}

template<typename Tip>
void TestDvostrukaListaTrenutni(DvostrukaLista<Tip> &TestLista)
{
    std::cout << TestLista.trenutni() << "\n";
}

template<typename Tip>
void TestDvostrukaListaOperator(DvostrukaLista<Tip> &TestLista)
{
    for(int i=0;i<TestLista.brojElemenata(); i++)
    {
        std::cout << TestLista[i] << " ";
    }
    std::cout << "\n";
}

template<typename Tip>
void TestDvostrukaListaPrethodni(DvostrukaLista<Tip> &TestLista)
{
    std::cout << TestLista.trenutni() << " ";
    TestLista.prethodni();
    std::cout << TestLista.trenutni() << "\n";
}

template<typename Tip>
void TestDvostrukaListaSljedeci(DvostrukaLista<Tip> &TestLista)
{
    std::cout << TestLista.trenutni() << " ";
    TestLista.sljedeci();
    std::cout << TestLista.trenutni() << "\n";
}

template<typename Tip>
void TestdajMaximum(DvostrukaLista<Tip> &TestLista)
{
    std::cout << dajMaksimum(TestLista) << "\n";
}
int main ()
{
    DvostrukaLista<int> lista;
for (int i(1); i<=5; i++)
	lista.dodajIspred(i);
{
    // Konstruktor kopije
    DvostrukaLista<int> lista2(lista);
    DvostrukaLista<int> lista3;
    // Operator dodjele
    (lista3=lista).brojElemenata();
    // Destruktivna samododjela
    (lista=lista).brojElemenata();

    lista.obrisi();
    std::cout << lista2.brojElemenata() << " " << lista2[2];
    std::cout << " " << lista3.brojElemenata() << " " << lista3[2] << " ";
}
std::cout << lista.brojElemenata() << " " << lista[2];


    std::cout << "\n\n";
    DvostrukaLista<int> TestLista2;
    TestDvostrukaListaDodajIspred(TestLista2);
    TestDvostrukaListaDodajIza(TestLista2);
    TestDvostrukaListaObrisi(TestLista2);
    TestDvostrukaListaBrojElemenata(TestLista2);
    TestDvostrukaListaPocetak(TestLista2);
    TestDvostrukaListaKraj(TestLista2);
    TestDvostrukaListaTrenutni(TestLista2);
    TestDvostrukaListaOperator(TestLista2);
    TestDvostrukaListaPrethodni(TestLista2);
    TestDvostrukaListaSljedeci(TestLista2);
    TestdajMaximum(TestLista2);
    std::cout << "Thank you";
    return 0;
}