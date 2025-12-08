class Date {
public:
    int d, m, y;

    Date() : d(1), m(1), y(2000) {} 

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

    if (days < 0){
        return "EXPIRED (" + to_string(-days) + " days ago)";
    }
    if (days <= 3)
        return "EXPIRING IN 3 DAYS";
    if (days <= 90)
        return "Near expiry (within 3 months)";

        
    return "OK (" + to_string(days) + " days left)";
}


bool isExpiryDateAcceptable(const string& expDate) {
    int days = daysToExpiry(expDate);

    if (days < 0) {
        cout << "\tERROR: Already expired!\n";
        return false;
    }
    if (days == 0) {
        cout << "\tWARNING: Expires today!\n";
        char c;
        cout << "\tContinue? (y/n): ";
        cin >> c;
        cin.ignore();
        return (c == 'y' || c == 'Y');
    }
    if (days <= 3) {
        cout << "\tWARNING: Very short expiry (" << days << " days)\n";
        char c;
        cout << "\tContinue? (y/n): ";
        cin >> c;
        cin.ignore();
        return (c == 'y' || c == 'Y');
    }
    return true;
}

// Check if expiry is on or after 10/12/2025
bool canAddBatchFromRefDate(const string& expiryDate) {
    int d, m, y;
    sscanf(expiryDate.c_str(), "%d/%d/%d", &d, &m, &y);

    Date today= Date::today();
    int todayD, todayM, todayY;
    todayD= today.d;
    todayM= today.m;
    todayY= today.y;

    if (y < todayY) return false;
    if (y == todayY && m < todayM) return false;
    if (y == todayY && m == todayM && d < todayD) return false;

    return true;
}
