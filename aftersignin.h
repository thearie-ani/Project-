#ifndef AFTERSIGNI_H
#define AFTERSIGNI_H

#include <iostream>
#include <algorithm>
#include <string>
#include <fstream>
#include <windows.h>
#include "save&load.h"
#include "Date.h"

using namespace std;



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

            // Check expiry
            if (!isExpiryDateAcceptable(exp) || !canAddBatchFromRefDate(exp)) {
                cout << "\tERROR: Already Exprired . Cannot add this batch.\n";
                Sleep(2000);
                break;
            }

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



void alertExpiryBatches() {

    Products* product = stock.getHead();
    bool found = false;

    while (product) {
        Batch* batch = product->lot.front;

        cout << "Product: " << product->name << "\n";
        while (batch) {
            string alert = expiryStatus(batch->expiryDate);
            if (alert.find("OK") != 0) {
                found = true;
                cout << "  Expiry: " << batch->expiryDate << "\n"
                     << "  Status: " << alert << "\n\n";
            }

            batch = batch->next;
        }

        product = product->next;
    }

    if (!found) cout << "\tAll stock is safe.\n";

}

/// selling or move product out

void sellingP(){
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

    while (qty > 0 && temp->lot.getFront() != nullptr){
        Batch *batch= temp->lot.getFront();
        if (batch->quantity<= qty){
            qty -= batch->quantity;
            temp->totalQty -= batch->quantity;
            temp->lot.dequeueBatch();
        }else {
            batch->quantity -= qty;
            temp->totalQty -= qty;
            qty = 0;
        }
    }
    if (qty>0) cout<< "\tNot enough stock!\n";

    //rewrite file after selling
    saveBatchtoFile();
    saveProductTofileUpdateQty();
    cout<< "\tSale Completed ...\n\n";
    Sleep (2000);
}

void moveout(){
    system ("cls");
    Products* product = stock.getHead();
    bool found = false;

    while (product) {
        Batch* batch = product->lot.front;

        while (batch) {
            string alert = expiryStatus(batch->expiryDate);
            if (alert.find("EXPIRED") == 0) {
                cout << "Product: " << product->name << "\n"
                << "\tExpired: "<< batch->expiryDate<< "\n"<< "\tQuantity: "<< batch->quantity<<"\n";
            }
            batch = batch->next;
        }

        product = product->next;
    }
    cin.ignore();
    string exit;
    cout<< "\tMove All these Expired out.\n";
    cout << "\tPress Enter to Comfirm or type (Exit) to exit: ";
    getline (cin, exit);
    transform (exit.begin(), exit.end(), exit.begin(), ::toupper);
    if (exit== "EXIT"){
        return;
    } 
    cout<< "\n\tMoved out...\n";

    Products *temp= stock.getHead();
    while (temp) {
        Batch* batch = temp->lot.front;
        while (batch) {
            string alert = expiryStatus(batch->expiryDate);
            if (alert.find("EXPIRED") == 0) {
                found= true;
                temp->totalQty -= batch->quantity;
                temp->lot.dequeueBatch();
            }
            batch = batch->next;
        }
        temp = temp->next;
    }
    saveBatchtoFile();
    saveProductTofileUpdateQty();
    Sleep (2000);

    if (!found){
        cout << "\tAll stock is safe.\n";
        Sleep (2000);
    }
}

void shipping(){
    
    system ("cls");
    int choice;
    bool exit= false;
    while (!exit){
        system ("cls");
        cout<< "\n\t1. Selling\n"<< "\t2. Move Expiered Product out\n"<< "\t3. Exit\n";
        cout<< "Selection the option: ";
        cin>> choice;
        if (choice==1){
            sellingP();
        } else if (choice== 2){
            moveout();
        }else if (choice==3){
            exit= true;
        }else {
            cout<<"\tInvalid Choice.\n";
            Sleep (2000);
        }
    }
}




//inventory management
void updatestock(){
    system ("cls");
    Products *temp= stock.getHead();
    if (temp== nullptr){
        cout<<"\tStock is Empty.\n";
        return;
    }
    // alert batch of product that's near exp
    cout<< "\tAlert Stock Near expiryDate\n";
    alertExpiryBatches();


    int choice;
    bool exit= false;
    while (!exit){
        cout<< "\n\n\t1. Add New Batch\n";
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
    cout<< "\t\tCheck your Today stock\n";
    Products *product= stock.getHead();
    if (product== nullptr){
        cout<< "\n\t\tNo Product in Stock.\n\n";
        Sleep (2000);
        return;
    }
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

void resultSearch(Products *temp){
    if (!temp){
        cout<< "\n\t No this Product ID in stock.\n";
        Sleep (2000);
        return;
    }
    cout<< "\n\t\tResult\n";
    cout << "\tID: "<< temp->id<< "\n\tName: "<< temp->name<< "\n\tPrice(per/box): "<< temp->price<< "$" <<"\n\tTotal: "<< temp->totalQty<<" (box)\n";
    cout<< "\tPress Enter to exit..";
    getchar(); getchar();
}

void searchbyID(){
    int id;
    cout<< "\n\n\tEnter Product ID: ";
    cin >> id;
    Products *temp= stock.getProductById(id);
    resultSearch(temp);
}

void searchbyName(){
    string name;
    cout<< "\n\n\tEnter Product Name: ";
    cin >> name;
    transform (name.begin(), name.end(), name.begin(), ::toupper);
    Products *temp= stock.getProductByName(name);
    resultSearch(temp);
}


void searchProduct(){
    int choice;
    bool exit= false;
    while (!exit){
        system ("cls");
        cout<< "\n\t1. Search by ID\n"<< "\t2. Search by Name\n"<< "\t3. Exit\n";
        cout<< "Selection the option: ";
        cin>> choice;
        if (choice==1){
            searchbyID();
        } else if (choice== 2){
            searchbyName();
        }else if (choice==3){
            exit= true;
        }else {
            cout<<"\tInvalid Choice.\n";
            Sleep (2000);
        }
    }
}

void loginAsUser(){

    int choice;
    bool login= true;
    while (login){
        system ("cls");
        cout<<"\n\tWelcome to Our Stock\n";
        cout<<"\t1. View Stock\n"<< "\t2. Search Product\n"<<"\t3. Logout\n";
        cout<<"\tEnter your choice (1-3): ";
        cin>> choice;
        if (choice==1){
            displaystock();
        }else if (choice== 2){
            searchProduct();
        }else if (choice== 3){
            login= false;
        }else {
            cout<< "Invalid Choice!!\n";
            Sleep (2000);
        }
    }
}

#endif