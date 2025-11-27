#include <iostream>
#include <exception>
#include <vector>

using namespace std;
template<typename Tip>
struct Cvor
{
    Tip clan;
    Cvor<Tip> *pok;
    Cvor(Tip Clan)
    {
        clan=Clan;
        pok=nullptr;
    }
};

template<typename Tip>
class Stek
{
    int VelicinaSteka;
    Cvor<Tip>* PocetakSteka;
public:
    Stek()
    {
        VelicinaSteka=0;
        PocetakSteka=nullptr;
    }
    ~Stek()
    {
        this->brisi();
    }

    Stek(const Stek<Tip> &stek)
    {
        VelicinaSteka=stek.VelicinaSteka;
        Cvor<Tip>* pamti=nullptr;
        Cvor<Tip>* novi=nullptr;
        auto pomocni=stek.PocetakSteka;
        for(int i=0;i<VelicinaSteka;i++)
        {
            pamti=new Cvor<Tip>((*pomocni).clan);
            if(i==0)
            {
                PocetakSteka=pamti;
                novi=pamti;
            }
            else 
            {
                (*novi).pok=pamti;
                novi=(*novi).pok;
            }
            if(i!=VelicinaSteka-1)
            {
                pamti=(*pamti).pok;
            }
            pomocni=(*pomocni).pok;
        }
    }
    Stek(Stek<Tip> &&stek)
    {
        PocetakSteka=stek.PocetakSteka;
        VelicinaSteka=stek.VelicinaSteka;
    }
    Stek<Tip> &operator =(const Stek<Tip> &stek)
    {
        if((*this).PocetakSteka==stek.PocetakSteka && VelicinaSteka==stek.VelicinaSteka)
        {
            return *this;
        }
        auto k=PocetakSteka;
        Cvor<Tip>* l;
        while(k!=nullptr)
        {
            l=(*k).pok;
            delete k;
            k=l;
        }
        VelicinaSteka=stek.VelicinaSteka;
        PocetakSteka=nullptr;
        Cvor<Tip>* pamti=nullptr;
        k=stek.PocetakSteka;
        l=nullptr;
        for(int i=0;i<VelicinaSteka;i++)
        {
            pamti=new Cvor<Tip>((*k).clan);
            if(i==0)
            {
                PocetakSteka=pamti;
                l=pamti;
            }
            else
            {
                (*l).pok=pamti; l=(*l).pok;
            }
            if(i!=VelicinaSteka-1)
            {
                pamti=(*pamti).pok;
            }
            k=(*k).pok;
        }
        return *this;
    }
    Stek<Tip> &operator =(Stek<Tip> &&stek)
    {
        std::swap(this,stek);
    }
    void brisi()
    {
        auto k=PocetakSteka;
        while(k!=nullptr)
        {
            auto pamti=(*k).pok;
            delete k;
            k=pamti;
        }
        PocetakSteka=nullptr;
        VelicinaSteka=0;
    }
    int brojElemenata() const
    {
        return VelicinaSteka;
    }
    void stavi(const Tip& el)
    {
        auto pamti=PocetakSteka;
        auto Novi=new Cvor<Tip>(el);
        PocetakSteka=Novi;
        (*PocetakSteka).pok=pamti;
        VelicinaSteka+=1;
    }
    Tip skini()
    {
        if(VelicinaSteka<=0)
        {
            throw std::range_error("Greska");
        }
        auto pamti=PocetakSteka;
        PocetakSteka=(*PocetakSteka).pok;
        auto vracaj=(*pamti).clan;
        delete pamti;
        VelicinaSteka--;
        return vracaj;
    }
    Tip &vrh()
    {
        if(VelicinaSteka<=0)
        {
            throw std::range_error("Greska");
        }
        return (*PocetakSteka).clan;
    }
};

int BinarySearch(std::vector<int> Vektor, int pocetak, int kraj, int element)
{
    if(kraj>=pocetak)
    {
        int sredina=pocetak+(kraj-pocetak)/2;
        if(Vektor.at(sredina)==element)
        {
            return sredina;
        }
        if(Vektor.at(sredina)>element)
        {
            return BinarySearch(Vektor, pocetak, sredina-1, element);
        }
        else
        {
            return BinarySearch(Vektor, sredina + 1, kraj, element);
        }
    }
    return -1;
}

void pretraga(Stek<vector<int> >& s, int trazeni)
{
    int index=-1;
    std::vector<int> Vektor;
    int n=s.brojElemenata();
    if(s.brojElemenata()>0)
    {
        Vektor=s.skini();
        index=BinarySearch(Vektor,0,Vektor.size()-1,trazeni);
        if(s.brojElemenata()>0 && index==-1)
        {
            pretraga(s,trazeni);
        }
    }
    if(index==-1 && s.brojElemenata()==0)
    {
        std::cout << "Nema elementa";
    }
    else if(index!=-1)
    {
        std::cout << index << " " << s.brojElemenata();
    }
    s.stavi(Vektor);
}

void TestZaPretragu1()
{
    Stek<std::vector<int>> stek;
    std::vector<int> a = {0,1,2,3,4,5,6,7,8,9,10};
    std::vector<int> b = {11,12,13,14,15,16,17,18,19,20};
    stek.stavi(a);
    stek.stavi(b);
    // Treba ispisati 0 0;
    pretraga(stek, 0);
}

void TestZaPretragu2()
{
    Stek<std::vector<int>> stek;
    std::vector<int> a = {1, 5, 10};
    std::vector<int> b = {20, 100, 300};
    stek.stavi(a);
    stek.stavi(b);
    // Nema tog elementa pa treba ispisati "Nema elementa"
    pretraga(stek, 46);
}

void TestZaPretragu3()
{
    Stek<std::vector<int>> stek;
    std::vector<int> a = {2, 3};
    std::vector<int> b = {4, 5, 6};
    std::vector<int> c = {7, 8, 9, 10};
    std::vector<int> d = {11,12,13,14,15,16,17,18,19,20,21};
    stek.stavi(a);
    stek.stavi(b);
    stek.stavi(c);
    stek.stavi(d);
    // Treba ispisati 10, 3
    pretraga(stek, 21);
}

int main()
{
    TestZaPretragu1();
    std::cout << "\n";
    TestZaPretragu2();
    std::cout << "\n";
    TestZaPretragu3();
    return 0;
}