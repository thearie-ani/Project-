#include <iostream>
#include <fstream>
#include <sstream>
#include <conio.h>
#include <windows.h>
#include <ctime>
#include "aftersignin.h"
#include "picosha2.h"

using namespace std;
const string ADMIN_SECRET_PASSWORD = "SuperSecret123";


string getTimeNow() {
    time_t now = time(0);
    char dt[50];
    strftime(dt, sizeof(dt), "%Y-%m-%d %H:%M:%S", localtime(&now));
    return string(dt);
}

class User {
public:
    string username;
    string password;
    string gmail;
    string role;

    User() {}

    User(string u, string p,string e, string r) {
        username = u;
        password = p;
        gmail =  e;
        role=r;
    }
};

class LoginSystem {
public:
    User users[50];
    int totalUsers;
    string fileName;

    LoginSystem(string filename) {
        fileName = filename;
        totalUsers = 0;

        ifstream file(filename);
        if (!file.is_open()) {
            ofstream f(filename);
            f << "username,password,email,role\n";
            f.close();
            return;
        }
        string line;

        getline(file, line); 
        getline(file, line);

        while (getline(file, line) && totalUsers < 50) {
            stringstream ss(line);
            string u, p,e,r;
            getline(ss, u, ',');
            getline(ss, p, ',');
            getline(ss, e, ',');
            getline(ss,r,',');
            users[totalUsers] = User(u, p, e,r);
            totalUsers++;
        }
        file.close();
    }

    bool checkLogin(string inputUser, string inputPass) {
        for (int i = 0; i < totalUsers; i++) {
            if (users[i].username == inputUser && users[i].password == inputPass) {
                return true;
            }
        }
        return false;
    }

    void SignUP() {
        string u, p, e, confirmEmail, r="user";

        cout << "\n\tCreate username: ";
        cin >> u;

        // Check if username exists
        for (int i = 0; i < totalUsers; i++) {
            if (users[i].username == u) {
                cout << "\n\tUsername already exists! Try again.\n";
                Sleep (2000);
                return;
            }
        }

        cout << "\tEnter Gmail: ";
        cin >> e;

        cout << "\tConfirm Gmail: ";
        cin >> confirmEmail;

        if (e != confirmEmail) {
            cout << "\nEmails do not match! Please try again.\n";
            return;
        }

        cout << "\tCreate password: ";
        p = inputPassword();

        // Check if admin secret password
        if (p == ADMIN_SECRET_PASSWORD) {
            r = "admin";
            cout << "\n\tYou are registering as an Admin!\n";
        }

        string encryptedPassword = picosha2::hash256_hex_string(p);

        users[totalUsers] = User(u, encryptedPassword, e, r);
        totalUsers++;

        // Write to CSV
        ofstream file(fileName, ios::app);
        file << "\n" << u << "," << encryptedPassword << "," << e << "," << r;

        file.close();

        cout << "\n\t\tAccount successfully created!\n";
        Sleep (2000);
    }

    //change to *
    string inputPassword() {
    string pass = "";
    char ch;

    while (true) {
        ch = _getch();

        if (ch == '\r') {       // ENTER key
            cout << endl;
            return pass;
        } else if (ch == '\b') {  // BACKSPACE
            if (!pass.empty()) {
                pass.pop_back();
                cout << "\b \b";  // erase last *
            }
        } else {                  // any normal character
            pass.push_back(ch);
            cout << '*';
        }
    }
}



    //log in history
    void logLoginAttempt(string username, string email, bool success) {
        ofstream file("login_history.csv", ios::app);
        file << username << ",";
        file<< (success ? "SUCCESS" : "FAIL") << "," ;
        file<< getTimeNow();
        file.close();
    }

};

// globle variable
LoginSystem login("userinfo.csv");

void signIn(){

    system ("cls");
    string username, password;
    if (login.totalUsers==0){
        cout<< "\n\t\t No account Yet.\n\t\tSignUp First.\n";
        Sleep (2000);
        return;
    }
    cout<< "\n\t\tLogin\n";
    cout << "\tEnter username: ";
    cin >> username;

    cout << "\tEnter password: ";
    password = login.inputPassword();  // masked input

    string encryptedPassword = picosha2::hash256_hex_string(password);

    bool success = false;
    string role = "";
    string emailFound = "";

        // find user
    for (int i = 0; i < login.totalUsers; i++) {
        if (login.users[i].username == username &&
            login.users[i].password == encryptedPassword) {
            success = true;
            role = login.users[i].role;
            emailFound = login.users[i].gmail;  // get email for logging
            break;
        }
    }

        // Track login attempt (this will write to login_history.csv)
    login.logLoginAttempt(username, emailFound, success);

    if (!success) {
        cout << "\nLogin failed. Username or password incorrect.\n";
        Sleep(2000);
        return;
    }

    cout << "\nLogin successful!\n";
    if (role == "admin")
        loginAsAdmin();
    else
        loginAsUser();
}


int main() {
    loadProductfromFile();  // importance
    loadBatchfromFile();

    while (true){
        system ("cls");
        cout<< "\n\tWellcome to Stock Management\n";
        cout << "\t\tSIGN IN/SIGN UP\n";
        cout << "\t1. SIGN IN\n";
        cout << "\t2. SIGN UP\n";
        cout<<"\tEnter your option: ";

        int option;
        cin >> option;

        switch (option) {
            case 1:
                signIn();
                break;

            case 2:
                login.SignUP();
                break;

            default:
                cout << "\n\t\tInvalid option!\n";
                Sleep (2000);
        }

    }

    return 0;
}