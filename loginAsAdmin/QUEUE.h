#include <string>
using namespace std;


class Batch{
    public:
    int quantity;
    string expiryDate;
    Batch *next;
    Batch(int quantityB, string exp){
        quantity= quantityB;
        expiryDate= exp;
        next= nullptr;
    }

};

class InventoryLot {
    public:
    Batch *front, *back;

    InventoryLot (){
        front= back= nullptr;
    }

    int priorityExp( const string exp){
        int day, month, year;
        sscanf (exp.c_str(), "%d/%d/%d", &day, &month, &year);
        return year*365 + month*12 + day;
    }

    Batch *addBatch(int qty, const string exp){
        Batch* newBatch= new Batch (qty, exp);
        int newPriority= priorityExp(exp);

        if(front==nullptr){
            front= back= newBatch;
            return newBatch;
        }

        int frontPriority = priorityExp(front->expiryDate);
        if (newPriority< frontPriority){
            newBatch->next= front;
            front= newBatch;
            return newBatch;
        }

        Batch *temp= front;
        while (temp->next && priorityExp(temp->next->expiryDate) <= newPriority){
            temp= temp->next;
        }
        newBatch->next= temp->next;
        temp->next= newBatch;
        
        if (!newBatch->next){
            back= newBatch;
        }
        return newBatch;
    }

    void displayBatches() {
        Batch* temp = front;
        int i = 1;
        while (temp) {
            cout << "\tBatch " << i << ": Qty=" << temp->quantity
                 << ", Expiry=" << temp->expiryDate << "\n";
            temp = temp->next;
            i++;
        }
    }
};

