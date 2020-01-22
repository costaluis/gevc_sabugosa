#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

template <class T> inline T mean(vector<T> data){
    T temp = 0;
    for(int i=0; i<data.size(); i++){
        temp += data[i];
    }
    return temp / data.size();
}

template <class T> inline T median(vector<T> & data){
    sort(data.begin(),data.end());
    return data[data.size()/2];
}

template <class T> inline T mode(vector<T> & data){
    sort(data.begin(),data.end());
    T temp = data[0];
    T bigger_t = 0;
    int bigger_time = 0;
    int times = 0;
    for(int i=0; i<data.size(); i++){
        if(data[i] == temp){
            times++;
        }else{
            if(bigger_time < times){
                bigger_time = times;
                bigger_t = temp;
            }
            temp = data[i];
            times = 1;
        }
    }
    return bigger_t;
}

template <class T> inline void principal(vector<T> data, int N){
    T temp;
    for(int i=0; i<N; i++){
        cin >> temp;
        data.push_back(temp);
    }
    cout << "Mean: " << mean(data) << endl << "Median: " << median(data) << endl << "Mode: " << mode(data) << endl;
    cout << "Sort Vector:";
    for(int i=0; i< data.size(); i++){
        cout << " " << data[i];
    }
    cout << endl;
}

int main(){
    int size;
    string type_data;
    cout << "Enter the size of array: ";
    cin >> size;
    cout << "Enter type of array: " << endl << endl << "##Options - int, float, double" << endl << endl << "-->";
    cin >> type_data;
    
    if(type_data == "int"){
        vector<int> data;
        principal(data,size);
    }else{
        if(type_data == "float"){
            vector<float> data;
            principal(data,size);
        }else{
            if(type_data == "double"){
                vector<double> data;
                principal(data,size);
            }else{
                cout << "Invalid Option" << endl;
            }
        }
    }
    
    return 0;
}