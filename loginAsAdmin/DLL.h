#include <string>
#include "QUEUE.h"
using namespace std;

class Products {
public:
    int id;
    string name;
    float price;
    Products* previous, * next;

    InventoryLot lot;

    Products(string nameP, float priceP, int idP) {
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

    Products* addEnd(string name, float price, int id) {
        Products* newProduct = new Products(name, price, id);

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

    Products* deletespecific(int idP){
        Products* temp = head;
        while (temp) {
            if (temp->id == idP) {

                // case 1: head node
                if (temp == head) {
                    head = temp->next;
                    if (head) head->previous = nullptr;
                }
                // case 2: tail node
                else if (temp == tail) {
                    tail = temp->previous;
                    if (tail) tail->next = nullptr;
                }
                // case 3: middle node
                else {
                    temp->previous->next = temp->next;
                    temp->next->previous = temp->previous;
                }
                Products *delP= temp;
                string exit;
                cout<< "\n"<< "\tProduct NAME: "<< delP->name<< "\n"<< "\tProduct Price: "<< delP->price<< "$\n";
                cout<< "Press Enter to confirm Deletion or type(exit) to delete again ... ";
                getline(cin, exit);
                transform (exit.begin(), exit.end(), exit.begin(), ::toupper);
                if (exit== "EXIT"){
                    return nullptr;
                }
                delete temp;
                cout<< "\tDelete Successfully!!\n";
                return delP; // success
            }
            temp = temp->next;
        }
        return nullptr;
    }

    Products *getProductById(int id) {
        Products* temp = head;  
        while (temp) {
            if (temp->id == id) {
                return temp;    // found
            }
            temp = temp->next;
        }
        return nullptr;           // not found
    }


    bool displayProductForward(){
        if (head== nullptr){
            cout<< "\tStock is Empty!\n";
            return false;
        }
        Products  *temp= head;
        cout<< "\t\tStock\n\n";
        cout<< "\tID\t|\tNAME\t|\tPrice($)(per box)\n";
        while (temp!= nullptr){

            cout<<"\t"<< temp->id<< "\t|\t"<<temp->name<<"\t|\t"<<temp->price<<"\n";
            temp= temp->next;
        }
        cout<< "\n";
        return true;
    }
};

// globle variable

Stock stock;
int countProduct = 0 ;