#include <iostream>
#include <vector>

using namespace std;

int const N = 40;

template <class T> inline void sum(T & p, int n, vector<T> d){
    p=0;
    for(int i=0; i<n; i++){
        p += d[i];
    }
}

int main(){
    int accum;
    vector<int> data;

    for(int i=0; i<N; i++){
        data.push_back(i);
    }

    sum(accum,N,data);

    cout << "sum is " << accum << endl;

    return 0;
}