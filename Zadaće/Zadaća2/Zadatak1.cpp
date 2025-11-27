#include <iostream>
#include <exception>

using namespace std;
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
class DvostraniRed
{
    int VelicinaReda;
    Cvor<Tip>* PocetakReda;
    Cvor<Tip>* KrajReda;
public:
    DvostraniRed()
    {
        VelicinaReda=0;
        PocetakReda=nullptr;
        KrajReda=nullptr;
    }
    ~DvostraniRed()
    {
        this->brisi();
    }
    DvostraniRed(const DvostraniRed<Tip> &r)
    {
        this->VelicinaReda=r.VelicinaReda;
        auto pokazivac=r.PocetakReda;
        auto pomocni=pokazivac;
        auto sljedeci=pokazivac;
        for(int i=0;i<this->VelicinaReda;i++)
        {
            pomocni=new Cvor<Tip>(pokazivac->clan);
            if(i==0)
            {
                this->PocetakReda=pomocni;
                sljedeci=pomocni;
            }
            else
            {
                sljedeci->sljedeci=pomocni;
                pomocni->prethodni=sljedeci;
                sljedeci=sljedeci->sljedeci;

            }
            if(i!=this->VelicinaReda-1)
            {
                pomocni=pomocni->sljedeci;
            }
            pokazivac=pokazivac->sljedeci;
        }
        KrajReda=pomocni;
    }
    DvostraniRed(DvostraniRed<Tip> &&r)
    {
        this->PocetakReda=r.PocetakReda;
        this->VelicinaReda=r.VelicinaReda;
        this->KrajReda=r.KrajReda;
        r.PocetakReda=nullptr;
        r.KrajReda=nullptr;
        r.VelicinaReda=0;
    }
    DvostraniRed<Tip> &operator=(const DvostraniRed<Tip> &r)
    {
        if(this->PocetakReda!=r.PocetakReda && this->KrajReda!=r.KrajReda)
        {
            auto pokazivac=PocetakReda;
            auto sljedeci=pokazivac;
            sljedeci=nullptr;
            while(pokazivac!=nullptr)
            {
                sljedeci=pokazivac->sljedeci;
                delete pokazivac;
                pokazivac=sljedeci;
            }
            this->PocetakReda=nullptr;
            this->KrajReda=nullptr;
            this->VelicinaReda=r.VelicinaReda;
            auto pomocni=sljedeci;
            pokazivac=r.PocetakReda;
            for(int i=0;i<this->VelicinaReda;i++)
            {
                pomocni=new Cvor<Tip>(pokazivac->clan);
                if(i==0)
                {
                    this->PocetakReda=pomocni;
                    sljedeci=pomocni;
                }
                else
                {
                    sljedeci->sljedeci=pomocni;
                    pomocni->prethodni=sljedeci;
                    sljedeci=sljedeci->sljedeci;
                }
                if(i!=this->VelicinaReda-1)
                {
                    pomocni=pomocni->sljedeci;
                }
                pokazivac=pokazivac->sljedeci;
            }
            KrajReda=pomocni;
        }
        return *this;
    }
    Cvor<Tip> &operator =(DvostraniRed<Tip> &&r)
    {
        std::swap(this->PocetakReda,r.PocetakReda);
        std::swap(this->KrajReda,r.KrajReda);
        std::swap(this->VelicinaReda,r.VelicinaReda);
    }
    void brisi()
    {
        auto pokazivac=PocetakReda;
        auto sljedeci=PocetakReda;
        while(pokazivac!=nullptr)
        {
            sljedeci=pokazivac->sljedeci;
            delete pokazivac;
            pokazivac=sljedeci;
        }
        VelicinaReda=0;
        PocetakReda=nullptr;
        KrajReda=nullptr;
    }
    int brojElemenata() const
    {
        return this->VelicinaReda;
    }
    void staviNaVrh(const Tip& el)
    {
        auto dodajovaj=new Cvor<Tip>(el);
        if(VelicinaReda==0)
        {
            KrajReda=dodajovaj;
            PocetakReda=KrajReda;
        }
        else
        {
            auto pamti=KrajReda;
            KrajReda=dodajovaj;
            KrajReda->prethodni=pamti;
            pamti->sljedeci=KrajReda;
        }
        VelicinaReda++;
    }
    Tip skiniSaVrha()
    {
        if(VelicinaReda<=0)
        {
            throw std::range_error("Greska");
        }
        else if(VelicinaReda==1)
        {
            auto vracaj=PocetakReda->clan;
            delete PocetakReda;
            PocetakReda=nullptr;
            KrajReda=nullptr;
            VelicinaReda=0;
            return vracaj;
        }
        else
        {
            auto brisime=KrajReda;
            KrajReda=KrajReda->prethodni;
            VelicinaReda+=-1;
            auto vracaj=brisime->clan;
            delete brisime;
            KrajReda->sljedeci=nullptr;
            return vracaj;
        }
    }
    void staviNaCelo(const Tip& el)
    {
        auto dodajovaj=new Cvor<Tip>(el);
        if(VelicinaReda==0)
        {
            KrajReda=dodajovaj;
            PocetakReda=KrajReda;
        }
        else
        {
            auto pamti=PocetakReda;
            PocetakReda=dodajovaj;
            PocetakReda->sljedeci=pamti;
            pamti->prethodni=PocetakReda;
        }
        VelicinaReda++;
    }
    Tip skiniSaCela()
    {
        if(VelicinaReda<1)
        {
            throw std::range_error("Greska");
        }
        if(VelicinaReda==1)
        {
            auto vracaj=PocetakReda->clan;
            delete PocetakReda;
            PocetakReda=nullptr;
            KrajReda=nullptr;
            VelicinaReda=0;
            return vracaj;
        }
        else
        {
            auto brisime=PocetakReda;
            PocetakReda=PocetakReda->sljedeci;
            VelicinaReda+=-1;
            auto vracaj=brisime->clan;
            delete brisime;
            return vracaj;
        }
    }
    Tip &celo()
    {
        if(VelicinaReda<=0)
        {
            throw std::range_error("Greska");
        }
        return PocetakReda->clan;
    }
    Tip &vrh()
    {
        if(VelicinaReda<1)
        {
            throw std::range_error("Greska");
        }
        return KrajReda->clan;
    }
};

template<typename Tip>
void TestDvostraniRedStaviNaCelo(DvostraniRed<Tip> &red)
{
    red.staviNaCelo(1);
    red.staviNaCelo(2);
    std::cout << red.brojElemenata() << "\n";
}

template<typename Tip>
void TestDvostraniRedStaviNaVrh(DvostraniRed<Tip> &red)
{
    red.staviNaVrh(3);
    red.staviNaVrh(4);
    std::cout << red.brojElemenata() << "\n";
}

template<typename Tip>
void TestDvostraniRedcelo(DvostraniRed<Tip> &red)
{
    std::cout << red.celo() << " " << red.brojElemenata() << "\n";
}

template<typename Tip>
void TestDvostraniRedSkiniSaCela(DvostraniRed<Tip> &red)
{
    std::cout << red.skiniSaCela() << " ";
    std::cout << red.celo() << " " << red.brojElemenata() << "\n";
}

template<typename Tip>
void TestDvostraniRedvrh(DvostraniRed<Tip> &red)
{
    std::cout << red.vrh() << " " << red.brojElemenata() << "\n";
}

template<typename Tip>
void TestDvostraniRedSkiniSaVrha(DvostraniRed<Tip> &red)
{
    std::cout << red.skiniSaVrha() << " ";
    std::cout << red.vrh() << " " << red.brojElemenata() << "\n";
}

template<typename Tip>
void TestDvostraniRedbrojElemenata(DvostraniRed<Tip> &red)
{
    std::cout << red.brojElemenata() << "\n";
}

template<typename Tip>
void TestDvostraniRedbrisi(DvostraniRed<Tip> &red)
{
    red.brisi();
    std::cout << red.brojElemenata() << "\n";
}
int main ()
{
    DvostraniRed<int> MojRed;
    TestDvostraniRedStaviNaCelo(MojRed);
    TestDvostraniRedStaviNaVrh(MojRed);
    TestDvostraniRedcelo(MojRed);
    TestDvostraniRedSkiniSaCela(MojRed);
    TestDvostraniRedvrh(MojRed);
    TestDvostraniRedSkiniSaVrha(MojRed);
    TestDvostraniRedbrojElemenata(MojRed);
    TestDvostraniRedbrisi(MojRed);
    return 0;
}
