#include <iostream>
#include <cstdlib>

using namespace std;

template <class T> inline T sum(T vetor[], int size){
    T result = vetor[0];
    for(int i=1; i<size; i++){
        result += vetor[i];
    }
    return result;
}

int main(){
    const int size = 10;
    int v1[size];
    double v2[size];
    for(int i=0; i<size; i++){
        v1[i] = i;
        v2[i] = i/10.0;
    }

    int sum_v1 = sum(v1,size);
    double sum_v2 = sum(v2,size);

    cout << "Soma de inteiros: " << sum_v1 << endl <<
    "Soma de decimais: " << sum_v2 << endl;

    return 0;

}

