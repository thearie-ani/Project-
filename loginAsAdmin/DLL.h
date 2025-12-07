#include "QUEUE.h"
using namespace std;

// to manage product 

class Products {
public:
    int id, totalQty;
    string name;
    float price;
    Products* previous, * next;

    InventoryLot lot;

    Products(string nameP, float priceP, int idP, int total) {
        totalQty= total;
        id = idP;
        name = nameP;
        price = priceP;
        previous = next = nullptr;
    }
};

class Stock {
private:
    Products* head, * tail;

public:
    Stock() {
        head = tail = nullptr;
    }

    Products* addEnd(string name, float price, int id, int total) {
        Products* newProduct = new Products(name, price, id, total);

        if (head == nullptr) {
            head = tail = newProduct;
        } else {
            tail->next = newProduct;
            newProduct->previous = tail;
            tail = newProduct;
        }
        return newProduct;
    }

    Products* getHead() {
        return head;
    }

    Products *getProductById(int id){
        Products *temp = head;
        while (temp){
            if (temp->id == id){
                return temp;
            }
            temp= temp->next;
        }
        return nullptr;
    }

    bool deletespecific(int idP) {
        Products* temp = getProductById(idP);

        if (!temp) return false;

        if (temp == head) {
            head = temp->next;
            if (head)  head->previous = nullptr;
        }else if (temp == tail) {
            tail = temp->previous;
            if (tail) tail->next = nullptr;
        }else {
            temp->previous->next = temp->next;
            temp->next->previous = temp->previous;
        }

        delete temp;

        cout << "\tDelete Successfully!\n";
        return true;
    }



    bool displayProductForward(){
        if (head== nullptr){
            cout<< "\tStock is Empty!\n";
            return false;
        }
        Products  *temp= head;
        cout<< "\t\tStock\n\n";
        cout<< "\tID\t|\tNAME\t|\tPrice($)|\tTotal (box)\n";
        while (temp!= nullptr){

            cout<<"\t"<< temp->id<< "\t|\t"<<temp->name<<"\t|\t"<<temp->price<< "\t|\t"<< temp->totalQty << "\n";
            temp= temp->next;
        }
        cout<< "\n";
        return true;
    }
};

// globle variable

Stock stock;
int countProduct = 1; // start id of product from 1