#include "DLL.h"
using namespace std;

void saveProductTofile(Products* prod) {
    ofstream file("product.csv", ios::app);
    file << prod->id << " " << prod->name << " " << prod->price << "\n";
    file.close();
}

// do as rewrite all data to file when deleting in linked list
void deleteProductfromFile(Products *prod){
    ofstream file("product.csv", ios::out); // overwrite file

    Products* temp = stock.getHead();
    while (temp) {
        file << temp->id << " " << temp->name << " " << temp->price << "\n";
        temp = temp->next;
    }

    file.close();
}


void loadProductfromFile() {
    fstream file;
    file.open("product.csv", ios::in);
    if (!file.is_open()) return;

    int id;
    string name;
    float price;

    while (file >> id >> name >> price) {
        stock.addEnd(name, price, id);
        countProduct = max(countProduct, id + 1);
    }
    file.close();
}




void saveBatchtoFile() {
    fstream file("allstock.csv", ios::out); // overwrite
    Products* product = stock.getHead();
    while (product) {
        file <<"*** " <<product->id<<" "<< product->name<<'\n';
        Batch* batch = product->lot.front;
        while (batch) {
            file <<" " << batch->expiryDate <<" "<< batch->quantity <<  "\n";
            batch = batch->next;
        }
        product = product->next;
    }
    file.close();
}

void loadBatchfromFile() {
    fstream file("allstock.csv", ios::in);
    if (!file.is_open()) return;

    string tag, name, exp;
    int id, qty;

    Products *currentproduct= nullptr;
    while (file>> tag){

        if (tag== "***"){
            file>> id;
            file>> name;

            currentproduct= stock.getProductById(id);
        }else {
            exp = tag;
            file >> qty;
            if (currentproduct){
                currentproduct->lot.addBatch(qty, exp);
            }
        }
    }
    file.close();

}
