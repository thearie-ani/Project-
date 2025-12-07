#include <iostream>
#include <algorithm>
#include <string>
#include <sstream>
#include <fstream>
#include <windows.h>
#include "save&load.h"

using namespace std;
class Date {
private:
    int d, m, y;

public:
    Date() : d(1), m(1), y(2000) {} // default constructor

    Date(int day, int month, int year) {
        d = day;
        m = month;
        y = year;
    }

    // Parse "DD/MM/YYYY"
    static Date fromString(const string& s) {
        int day, month, year;
        sscanf(s.c_str(), "%d/%d/%d", &day, &month, &year);
        return Date(day, month, year);
    }

    // Convert to a comparable day count
    int toDays() const {
        return y * 365 + m * 30 + d;
    }

    // Get today's date
    static Date today() {
        SYSTEMTIME t;
        GetLocalTime(&t);
        return Date(t.wDay, t.wMonth, t.wYear);
    }
};
int daysToExpiry(const string& expDateString) {
    Date exp = Date::fromString(expDateString);
    Date today = Date::today();

    return exp.toDays() - today.toDays();
}
string expiryStatus(const string& exp) {
    int days = daysToExpiry(exp);

    // to delete product that's expired
    //bool is_expired= false;

    if (days < 0){
        //is_expired = true;
        return "EXPIRED (" + to_string(-days) + " days ago)";
    }
    if (days <= 3)
        return "EXPIRING IN 3 DAYS";
    if (days <= 90)
        return "Near expiry (within 3 months)";

    return "OK (" + to_string(days) + " days left)";
}
void alertExpiryBatches() {

    Products* product = stock.getHead();
    bool found = false;

    while (product) {
        Batch* batch = product->lot.front;

        while (batch) {
            string alert = expiryStatus(batch->expiryDate);

            if (alert.find("OK") != 0) {
                found = true;
                cout << "Product: " << product->name << "\n"
                     << "  Expiry: " << batch->expiryDate << "\n"
                     << "  Status: " << alert << "\n\n";
            }

            batch = batch->next;
        }

        product = product->next;
    }

    if (!found) cout << "\tAll stock is safe.\n";

}



bool existedProduct(string name) {

    Products* temp = stock.getHead();
    while (temp) {
        string tempName = temp->name;
        if (tempName == name)
            return true;
        temp = temp->next;
    }
    return false;
}

void addproduct() {
    string name;
    float price;    
    system("cls");
    cout << "\n\t\t***Add More Product***\n\n";
    cout << "\tEnter product's information\n";
    cout << "\tName: ";
    cin >> name;
    transform(name.begin(), name.end(), name.begin(), ::toupper);

    if (existedProduct(name)) {
        cout << "\tThis Product already existed!\n";
        Sleep(2000);
        return;
    }

    cout << "\tPrice: ";
    cin >> price;
    cin.ignore();
    string exit;
    cout<< "\n\tcheck product details\n";
    cout<< "\tName: "<<name<<"\n"<< "\tPrice: "<<price<< "$\n";
    cout<< "Press Enter to confirm Addition or type (exit) to add again ...";
    getline (cin, exit);
    transform (exit.begin(), exit.end(), exit.begin(), ::toupper);
    if (exit== "EXIT"){
        return;
    } 
    int id = countProduct++;
    int totalProduct =0;
    Products* product = stock.addEnd(name, price, id, totalProduct);
    saveProductTofile(product);

    cout << "\tProduct added successfully\n";
    Sleep(2000);
}

void deleteproduct(){
    int idCh;
    system("cls");
    if(stock.displayProductForward()){
        cout<< "\tChoice product ID to remove: ";
        cin >> idCh;
        cin.ignore();
        Products *temp= stock.getProductById(idCh);
        string exit;
        cout<< "\n"<< "\tProduct NAME: "<< temp->name<< "\n"<< "\tProduct Price: "<< temp->price<< "$\n";
        cout<< "Press Enter to confirm Deletion or type(exit) to delete again ... ";
        getline(cin, exit);
        transform (exit.begin(), exit.end(), exit.begin(), ::toupper);
        if (exit== "EXIT"){
            return;
        }
        if (stock.deletespecific(idCh)){
            deleteProductfromFile(idCh);
        }
        Sleep(2000);
        return;
    }
    cout<< "\t*NO PRODUCT TO DELETE*";
    Sleep (2000);

}


void addnewBatch(){
    
    system ("cls");
    int id;
    cout<< "\t$$$ Availabel Product to add batch of (exp) $$$\n";
    if(!stock.displayProductForward()){
        cout << "\tAdd Product first!\n";
        Sleep (3000);
        return;
    }
    cout<< "\tSelete product (ID) to add batching of exp: ";
    cin>> id;
    
    Products *temp= stock.getHead();
    while(temp){
        if (temp->id==id){
            cout<< "\tID: "<< id<< "\tName: "<< temp->name<< "\t"<< temp->price<< "$ perbox\n";

            int qty;
            string exp;

            cout << "\n\tEnter quantity for this batch: ";
            cin >> qty;

            cout << "\tEnter expiry date (DD/MM/YYYY): ";
            cin >> exp;
            cin.ignore();

            string exit;
            cout<< "\n\tcheck details\n";
            cout<< "\tExpriryDate: "<<exp<<"\n"<< "\tQuantity: "<<qty<< "\n";
            cout<< "Press Enter to confirm Addition or type (exit) to add again ...";
            getline (cin, exit);
            transform (exit.begin(), exit.end(), exit.begin(), ::toupper);
            if (exit== "EXIT"){
            return;
            } 
            temp->totalQty += qty;
            temp->lot.addBatch(qty, exp);
            saveBatchtoFile();
            saveProductTofileUpdateQty();

            cout<< "\tBatch added...\n";
            Sleep (2000);
        }
        temp= temp->next;
    }
}


/// selling or more product out

void sellingP(Products *pro, int qty){
        while (qty > 0 && pro->lot.getFront() != nullptr){
            Batch *batch= pro->lot.getFront();
            if (batch->quantity<= qty){
                qty -= batch->quantity;
                pro->totalQty -= batch->quantity;
                pro->lot.dequeueBatch();
            }else {
                batch->quantity -= qty;
                pro->totalQty -= qty;
                qty = 0;
            }
        }
        if (qty>0) cout<< "\tNot enough stock!\n";

    }

void shipping(){
    int id, qty;
    system ("cls");
    stock.displayProductForward();
    cout<< "\tEnter ID to buy:...";
    cin>> id;
    Products *temp= stock.getProductById(id);
    cout<< "\tQuantity: ";
    cin>> qty;
    cin.ignore();
    string exit;
    float totalPrice= qty * temp->price;
    cout<< "\n\tcheck details\n";
    cout<< "\tID: "<<temp->id<<"\n"<< "\tName: "<< temp->name<< "\n\tQuantity: "<< qty<< "\n\tTotal: " << totalPrice<< "$\n";
    cout<< "Press Enter to confirm Selling or type (exit) to add again ...";
    getline (cin, exit);
    transform (exit.begin(), exit.end(), exit.begin(), ::toupper);
    if (exit== "EXIT"){
        return;
    } 
    if (qty> temp->totalQty) return;
    
    sellingP (temp, qty);

    //rewrite file after selling
    saveBatchtoFile();
    saveProductTofileUpdateQty();
    cout<< "\tSale Completed ...\n\n";
    Sleep (2000);

}




//inventory management
void updatestock(){
    // alert batch of product that's near exp
    system ("cls");
    alertExpiryBatches();

    cout<< "\tAlert Stock Near expiryDate\n";
    //   

    int choice;
    bool exit= false;
    while (!exit){
        cout<< "\t1. Add New Batch\n";
        cout<< "\t2. Shipping \n";
        cout<< "\t3. Exit\n";
        cout<< "Enter your choice: ";
        cin>> choice;

        if (choice == 1){
            addnewBatch();

        }else if (choice== 2){
            shipping();
        }else if (choice==3){
            exit= true;
        }else {
            cout<< "\tInvalid choice...";
            Sleep (2000);
        }
    }


}

void displaystock(){
    system("cls");
    cout<< "\tCheck your Today stock\n";
    Products *product= stock.getHead();
    while (product){
        cout<< "\tID: "<<product->id<< "\t"<< product->name<<"\t"<<product->price<<"$ "<<"\tTotal: "<< product->totalQty<< "boxs\n";
        product->lot.displayBatches();
        product= product->next;
        cout<< "\n";
    }
    cout<< "\tPress Enter to exit...";
    getchar(); getchar();
}


void loginAsAdmin() {
    loadProductfromFile();  // importance
    loadBatchfromFile();

    int choice;
    bool login = true;

    while (login) {
        system ("cls");
        cout<< "\t\tStock Management System\n\n";
        cout<< "\t1. Add Products\n"; 
        cout<< "\t2. Delete Products\n"; 
        cout<< "\t3. Update Stock\n"; 
        cout<< "\t4. Check Stock\n"; 
        cout<< "\t5. Logout\n"; 
        cout<< "Enter your choice: "; 
        cin>> choice; 
        if (choice==1){ 
            addproduct(); 
        }else if (choice==2){ 
            deleteproduct(); 
        }else if (choice==3){ 
            updatestock(); 
        }else if (choice==4){ 
            displaystock(); 
        }else if (choice==5){ 
            login=false; 
        }else { 
            cout<< "\t\tInvalid choice!!\n"; 
            Sleep(3000); 
        }
    }
}

int main() {
    loginAsAdmin();
    return 0;
}
