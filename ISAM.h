#ifndef LAB02_1_ISAM_H
#define LAB02_1_ISAM_H


#include <iostream>
#include <algorithm>
#include <fstream>

using namespace std;

const TAM_PAGE = 512;


struct Record{
    char nombre[30];
    char carrera[20];

    string getkey(){
        return nombre;
    }
};

template <typename T = int>
struct IndexPage{
//    int M = 2;
    T key[2]; // sizeof(T) = 10   [M]
    long children[3]; //         [M+1]
    int count;

    IndexPage() = default;

};

struct DataPage{
    Record records[2]; // sizeof(Record) = 60   [N]
    int count ;
    long next;   // Puntero a Disco
public:

};

template <typename  T>
class ISAM {
private:
    string filename;
public:
    ISAM() = default;
    vector<Record> localizar(string Key){
        fstream inFile(filename, ios::binary | ios::in);
        IndexPage<T> P{};
//        inFile.seekg(0);
        inFile.read((char*) &P, sizeof(IndexPage<T>));
        auto it = lower_bound(begin( P.key), end(P.key));
        long temp = 0;
        if(it == end(P.key)){
            temp = *(end(P.children) - 1) ;

        }else{
            temp = P.children[ it - begin(P.children)];

        }

        fstream inFile2("indice2", ios::binary | ios::in);
        IndexPage<T> Index2{};
        inFile2.seekg(temp);
        inFile2.read((char*) &Index2, sizeof(IndexPage<T>));
        auto it2 = lower_bound(begin( Index2.key), end(Index2.key));
        temp = 0;
        if(it2 == end(Index2.key)){
            temp = *(end(Index2.children) - 1) ;
        }else{
            temp = Index2.children[ it2 - begin(Index2.children)];
        }

        fstream inFile3("indice3", ios::binary | ios::in);
        IndexPage<T> Index3{};
        inFile3.seekg(temp);
        inFile3.read((char*) &Index3, sizeof(IndexPage<T>));
        auto it3 = lower_bound(begin( Index3.key), end(Index3.key));
        temp = 0;
        if(it3== end(Index3.key)){
            temp = *(end(Index3.children) - 1) ;
        }else{
            temp = Index3.children[ it3 - begin(Index3.children)];
        }

        fstream inFileDatos("DataPage1" , ios::binary | ios::in);
        DataPage Page1;
        vector<Record> res;
        long next = temp;
        while(next != -1) {
            inFileDatos.seekg(temp);
            inFileDatos.read((char*)& Page1, sizeof(DataPage));
            for (int i = 0; i < Page1.count; i++) {
                if (Page1.records[i].getkey() == Key) {
                    res.push_back(Page1.records[i]);
                }
            }
            next = Page1.next;
        }
        return res;
    }

    long localizarPage(long pos){
        fstream inFile(filename, ios::binary | ios::app);
        DataPage page;
        inFile.seekg(pos);
    }


};


#endif //LAB02_1_ISAM_H
