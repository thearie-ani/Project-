#include <iostream>
#include <algorithm>
#include <string>
#include <sstream>
#include <fstream>
#include <windows.h>
#include "save&load.h"

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
    int id = ++countProduct;
    Products* product = stock.addEnd(name, price, id);
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

        Products *deletedProduct = stock.deletespecific(idCh);
        deleteProductfromFile(deletedProduct);

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

            temp->lot.addBatch(qty, exp);
            saveBatchtoFile();
            cout<< "\tBatch added...\n";
            Sleep (2000);
        }
        temp= temp->next;
    }
}


//not yet
void shipping(){


}




//inventory management
void updatestock(){
    // alert batch of product that's near exp

    cout<< "\tAlert Stock Near expiryDate\n";
    //   

    int choice;
    bool exit= false;
    while (!exit){
        system ("cls");
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
        cout<< "\tID: "<<product->id<< "\t"<< product->name<<"\t"<<product->price<<"$ \n";
        Batch *batch= product->lot.front;
        bool nobatch=true;
        int number=0 ;
        while(batch){
            cout<<"Batch"<< ++number<< "=>\tExp: "<< batch->expiryDate<< "\tQuantity: "<< batch->quantity<<"\n";
            batch= batch->next;
            nobatch= false;
        }
        if (nobatch) cout<<"\t Not in stock.\n";
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
