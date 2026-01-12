#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

int MaxBrojVektora(const std::vector<int> &a){
    int max=a.at(0);
    for(int i=0;i<a.size();i++) if(a.at(i)>max) max=a.at(i);
    return max;
}

void radixSort(std::vector<int> &a) {
    int brCifaraMax=0, max=MaxBrojVektora(a);
    while(max!=0) {
        max/=10;
        brCifaraMax++;
    }
    std::vector<std::vector<int>> q(10);
    for(int i=0;i<brCifaraMax;i++) {
        int stepen=pow(10,i);
        for(int o=0;o<q.size();o++) q[o].resize(0);
        for(int j=0;j<a.size();j++)
        q[(a[j]%(10*stepen))/stepen].push_back(a[j]);
        a.clear();
        for(int k=0;k<q.size();k++)
        for(int l=0;l<q[k].size();l++) a.push_back(q[k][l]);
    }
}

void popraviDolje(std::vector<int> &a, int i, int velicina) {
    while(!(i>=(velicina/2) && i<velicina)) { //jeLiList(a,i)
    int veci=2*i+1;//lijevoDijete(i);
    int dd=2*i+2;//desnoDijete(i);
    if(dd<velicina && a[dd]>a[veci]) veci=dd;
    if(a[i]>a[veci]) return;
    int tmp=a[i];
    a[i]=a[veci];
    a[veci]=tmp;
    i=veci;
}
}

void stvoriGomilu(std::vector<int> &a) {
    int velicina=a.size();
    for(int i=(velicina/2); i>=0;i--) popraviDolje(a,i,a.size());
}

void popraviGore(std::vector<int> &a, int i) {
    while(i!=0 && a[i]>a[(i-1)/2]) {
        int tmp=a[i];
        a[i] = a[(i-1)/2];
        a[(i-1)/2]=tmp;
        i=(i-1)/2;
    }
}

void umetniUGomilu(std::vector<int> &a,int umetnuti, int&velicina) {
    if(velicina>a.size()) throw "Gomila je puna";
    a.push_back(umetnuti);
    velicina++;
    popraviGore(a,velicina-1);
}
int izbaciPrvi(std::vector<int> &a, int &velicina) {
    if(a.size()==0) throw "Gomila je prazna";
    velicina--;
    int tmp=a[0];
    a[0]=a[velicina];
    a[velicina]=tmp;
    if(velicina!=0) popraviDolje(a,0,velicina);
    return a[velicina];
}
void gomilaSort(std::vector<int> &a) {
    stvoriGomilu(a);
    int velicina=a.size();
    for(int i=a.size()-1; i>0;i--) {
        int tmp=a[0];
        a[0]=a[i];
        a[i]=tmp;
        velicina--;
        popraviDolje(a,0,velicina);
    }
}

int main(){ 
    vector<int> v;
    int vel = 0;
    umetniUGomilu(v, 123, vel);
    umetniUGomilu(v, 456, vel);
    umetniUGomilu(v, 789, vel);
    umetniUGomilu(v, 415, vel);
    umetniUGomilu(v, 742, vel);
    umetniUGomilu(v, 252, vel);
    izbaciPrvi(v, vel);
    for(auto element: v){
        cout<<element<<" ";
    }
    return 0;
}