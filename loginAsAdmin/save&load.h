#include "DLL.h"
using namespace std;

// save data to file and load data from file

// save every new product with the new line in file
void saveProductTofile(Products* prod) {
    ofstream file("product.csv", ios::app);
    file << prod->id << " " << prod->name << " " << prod->price <<" "<< prod->totalQty <<"\n";
    file.close();
}

// rewrite product to file again every time it gets more quantity
void saveProductTofileUpdateQty (){

    fstream file ("product.csv", ios::out);
    Products* product = stock.getHead();
    while (product) {
        file << product->id << " " << product->name << " " << product->price <<" "<< product->totalQty <<"\n";
        product = product->next;
    }
    file.close();

    
}

// do as rewrite all data to file when deleting in linked list
void deleteProductfromFile(int id){
    ofstream file("product.csv", ios::out); // overwrite file

    Products* temp = stock.getHead();
    while (temp) {
        file << temp->id << " " << temp->name << " " << temp->price<<" "<< temp->totalQty <<"\n";
        temp = temp->next;
    }

    file.close();
}

// get data that we used to add from file even if the program stop but still has the old data
void loadProductfromFile() {
    fstream file;
    file.open("product.csv", ios::in);
    if (!file.is_open()) return;

    int id, total;
    string name;
    float price;

    while (file >> id >> name >> price >> total) {
        stock.addEnd(name, price, id, total );
        countProduct = max(countProduct, id + 1);
    }
    file.close();
}



// save batch of product to another file
void saveBatchtoFile() {
    fstream file("allstock.csv", ios::out); // overwrite
    Products* product = stock.getHead();
    while (product) {
        file <<"*** " <<product->id<<" "<< product->name<< " "<< product->price <<" "<<product->totalQty <<'\n';
        Batch* batch = product->lot.front;
        while (batch) {
            file <<" " << batch->expiryDate <<" "<< batch->quantity <<  "\n";
            batch = batch->next;
        }
        product = product->next;
    }
    file.close();
}
// load data from batch file
void loadBatchfromFile() {
    fstream file("allstock.csv", ios::in);
    if (!file.is_open()) return;

    string tag, name, exp;
    int id, qty , total;
    float price;

    Products *currentproduct= nullptr;
    while (file>> tag){

        if (tag== "***"){
            file>> id;
            file>> name;
            file>> price;
            file>> total;

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
