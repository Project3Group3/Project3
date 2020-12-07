#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <queue>
#include <stack>
#include <functional>
#include <iomanip>
#include <chrono>
using namespace std;

class cases {
private:
    string date;
    string age;          //group     0-9,10-19,...80+
    string sex;                   //male, female, other, missing
    bool hosp;                  //true if hosp, false if not or missing
    bool icu;                   //""
    bool death;                 //true if death, false otherwise
    bool medcond;               //true if has underlying condition
    bool risk;                   //holds the patient's risk factor
    int ID;                     //Holds an ID for the patient to differentiate cases

    
    friend class CovidHeap;

    //Added by Ryan Roth on 12/3/2020
    //Name: calculateRisk()
    //Purpose: uses the information given about a case's subject to estimate their risk of death for COVID-19
    void calculateRisk();

public:
    static int IDcntr;      //Contains a counter for the number of cases in the map

    cases(string d, string a, string s, bool h, bool i, bool dt, bool m) {
        date = d;
        age = a;
        sex = s;
        hosp = h;
        icu = i;
        death = dt;
        medcond = m;
        calculateRisk();
        ID = IDcntr;
        IDcntr++;
    }
    cases(string d, string a, string s, bool h, bool i, bool dt, bool m, int id) {              //FOR MAP'S TOPK ALGORITHM ONLY
        date = d;
        age = a;
        sex = s;
        hosp = h;
        icu = i;
        death = dt;
        medcond = m;
        risk = -1;
        ID = id;
    }
    cases() {
        date = "";
        age = "";
        sex = "";
        hosp = false;
        icu = false;
        death = false;
        medcond = false;
        risk = 0;
        ID = -1;
    }
    string getDate();
    bool getDeath();
    string getAge();
    string getSex();
    bool getHosp();
    bool getIcu();
    bool getMedcond();
    int getRisk();

    bool equals(cases cases2);
    
    bool operator()(const cases& case1, const cases& case2) { //compares two cases, returns true if case1 > case2 signifying that case1 has a higher risk factor.
        if (abs(case1.risk - case2.risk) < 0.00000001) { //the risk factors are equal, sort by greatest ID
            return case1.ID > case2.ID;
        }
        else { //if the risks are not equal, return true when risk of case1 is greater
            return case1.risk > case2.risk;
        }
    }

};

bool cases::equals(cases cases2) {
    return (this->getDate() == cases2.getDate() &&
        this->getSex() == cases2.getSex() &&
        this->getAge() == cases2.getAge() &&
        this->getDeath() == cases2.getDeath() &&
        this->getHosp() == cases2.getHosp() &&
        this->getIcu() == cases2.getIcu() &&
        this->getMedcond() == cases2.getMedcond() &&
        this->getRisk() == cases2.getRisk());
}

string cases::getDate() {
    return date;
}
bool cases::getDeath() {
    return death;
}
bool cases::getHosp() {
    return hosp;
}
bool cases::getIcu() {
    return icu;
}
bool cases::getMedcond() {
    return medcond;
}
string cases::getAge() {
    return age;
}
string cases::getSex() {
    return sex;
}

int cases::getRisk() {
    return risk;
}

int cases::IDcntr = 0;

void cases::calculateRisk() { //Create a more complex Risk caclulation function
    risk = 0;
    //age is the single biggest factor in determining someone's death risk for COVID-19, so it will be weighted the highest
    if (this->age == "0 - 9 Years") {
        risk += 0;
    }
    else if (this->age == "10 - 19 Years") {
        risk += 0;
    }
    else if (this->age == "20 - 29 Years") {
        //if the patient has a medical condition, there will be a risk of death albeit relatively small
        if (medcond) {
            risk += 2;
        }
        else {
            risk += 0;
        }
    }
    else if (this->age == "30 - 39 Years") {
        if (medcond) {
            risk += 3;
        }
        else {
            risk += 1;
        }
    }
    else if (this->age == "40 - 49 Years") {
        if (medcond) {
            risk += 4;
        }
        else {
            risk += 2;
        }
    }
    else if (this->age == "50 - 59 Years") {
        if (medcond) {
            risk += 5;
        }
        else {
            risk += 3;
        }
    }
    else if (this->age == "60 - 69 Years") {
        if (medcond) {
            risk += 7;
        }
        else {
            risk += 5;
        }
    }
    else if (this->age == "70 - 79 Years") {
        if (medcond) {
            risk += 9;
        }
        else {
            risk += 5;
        }
    }
    else if (this->age == "80+ Years") {
        if (medcond) {
            risk += 10;
        }
        else {
            risk += 9;
        }
    }
}

int dateSubtraction(string date1, string date2) {  
    int year1 = stoi(date1.substr(0, 4));
    int month1 = stoi(date1.substr(5, 2));
    int day1 = stoi(date1.substr(8, 2));
    int year2 = stoi(date2.substr(0, 4));
    int month2 = stoi(date2.substr(5, 2));
    int day2 = stoi(date2.substr(8, 2));
    if (year1 > year2) {
        swap(year1, year2);
        swap(month1, month2);
        swap(day1, day2);
    }
    else if (month1 > month2&& year1 == year2) {
        swap(month1, month2);
        swap(day1, day2);
    }
    else if (day1 > day2&& year1 == year2 && month1 == month2)
        swap(day1, day2);

    int sum = 0;
    int yearDiff = 0;
    if (year2 > year1) {
        yearDiff = year2 - year1;

        for (int i = 0; i < yearDiff; i++) {
            if ((year1 + i) % 4 == 0)
                sum += 366;
            else
                sum += 365;
        }

        while (month1 > month2) {
            switch (month1) {
            case 1:
                sum -= 31;
                break;
            case 2:
                sum -= 29;
                break;
            case 3:
                sum -= 31;
                break;
            case 4:
                sum -= 30;
                break;
            case 5:
                sum -= 31;
                break;
            case 6:
                sum -= 30;
                break;
            case 7:
                sum -= 31;
                break;
            case 8:
                sum -= 31;
                break;
            case 9:
                sum -= 30;
                break;
            case 10:
                sum -= 31;
                break;
            case 11:
                sum -= 30;
                break;
            case 12:
                sum -= 31;
                break;
            }
            month1--;
        }
        while (month1 < month2) {
            switch (month1) {
            case 1:
                sum += 31;
                break;
            case 2:
                sum += 29;
                break;
            case 3:
                sum += 31;
                break;
            case 4:
                sum += 30;
                break;
            case 5:
                sum += 31;
                break;
            case 6:
                sum += 30;
                break;
            case 7:
                sum += 31;
                break;
            case 8:
                sum += 31;
                break;
            case 9:
                sum += 30;
                break;
            case 10:
                sum += 31;
                break;
            case 11:
                sum += 30;
                break;
            case 12:
                sum += 31;
                break;
            }
            month1++;
        }
    }
    else {
        while (month1 < month2) {
            switch (month1) {
            case 1:
                sum += 31;
                break;
            case 2:
                sum += 29;
                break;
            case 3:
                sum += 31;
                break;
            case 4:
                sum += 30;
                break;
            case 5:
                sum += 31;
                break;
            case 6:
                sum += 30;
                break;
            case 7:
                sum += 31;
                break;
            case 8:
                sum += 31;
                break;
            case 9:
                sum += 30;
                break;
            case 10:
                sum += 31;
                break;
            case 11:
                sum += 30;
                break;
            case 12:
                sum += 31;
                break;
            }
            month1++;
        }
    }

    sum = sum - day1 + 1 + day2;
    return sum;
}

string dateStepping(string date) {                    //adds 1 to date and returns as yyyy/mm/dd
    int month = stoi(date.substr(5,2));
    int day = stoi(date.substr(8,2));
    int year = stoi(date.substr(0,4));
    switch (month) {
        case 1:
            day++;
            if (day > 31) {
                month++;
                day = 1;
            }
            break;
        case 2:
            day++;
            if (day > 29) {
                month++;
                day = 1;
            }
            break;
        case 3:
            day++;
            if (day > 31) {
                month++;
                day = 1;
            }
            break;
        case 4:
            day++;
            if (day > 30) {
                month++;
                day = 1;
            }
            break;
        case 5:
            day++;
            if (day > 31) {
                month++;
                day = 1;
            }
            break;
        case 6:
            day++;
            if (day > 30) {
                month++;
                day = 1;
            }
            break;
        case 7:
            day++;
            if (day > 31) {
                month++;
                day = 1;
            }
            break;
        case 8:
            day++;
            if (day > 31) {
                month++;
                day = 1;
            }
            break;
        case 9:
            day++;
            if (day > 30) {
                month++;
                day = 1;
            }
            break;
        case 10:
            day++;
            if (day > 31) {
                month++;
                day = 1;
            }
            break;
        case 11:
            day++;
            if (day > 30) {
                month++;
                day = 1;
            }
            break;
        case 12:
            day++;
            if (day > 31) {
                month++;
                day = 1;
            }
            break;
    }
    if (month > 12) {
        year++;
        month = 1;
    }
    if (day < 10 && month < 10) {
        return (to_string(year) + "/0" + to_string(month) + "/0" + to_string(day));
    }
    else if (day < 10) {
        return (to_string(year) + "/" + to_string(month) + "/0" + to_string(day));
    }
    else if (month < 10) {
        return (to_string(year) + "/0" + to_string(month) + "/" + to_string(day));
    }
    return (to_string(year) + "/" + to_string(month) + "/" + to_string(day));
}


class Map {
private:
    unordered_map<string, unordered_map<bool, vector<cases>>> map;
    
    //returns a vector of the top k most at risk cases for the whole data set
    void topkFull(vector<cases>& caseVect, int k);
    //returns a vector of the top k most at risk cases for a single day
    void topkDate(vector<cases>& caseVect, int k);
    void topkRange(vector<cases>& vec, string date1, string date2, int k);

public:
    void insertCase(cases c);

    int deathsOverTime(string date1, string date2);
    int casesOverTime(string date1, string date2);
    int maxDeathsDayOverTime(string date1, string date2);
    double deathRateOverTime(string date1, string date2);

    void agesOfCases(string date1, string date2);
    void agesOfDeaths(string date1, string date2);

    void sexesOfCases(string date1, string date2);
    void sexesOfDeaths(string date1, string date2);

    int casesWithMedcond(string date1, string date2);
    int deathsWithMedcond(string date1, string date2);

    int casesWithHosp(string date1, string date2);
    int deathsWithHosp(string date1, string date2);

    int casesWithICU(string date1, string date2);
    int deathsWithICU(string date1, string date2);

    void topkDeathPercentFull(int k);
    void topkDeathPercentDate(string date, int k);
    
    void topkHospPercentFull(int k);
    void topkHospPercentDate(string date, int k);

    void topkICUPercentFull(int k);
    void topkICUPercentDate(string date, int k);

    friend class CovidHeap; //allow covidHeap to access the map without making it accessible to main
};


void Map::topkDeathPercentFull(int k) {
    vector<cases> caseVect;
    topkRange(caseVect, "2020/01/01", "2020/11/19", k);
    double deathSum = 0;
    double deathPercent = 0;
    for (auto iter = caseVect.begin(); iter != caseVect.end(); iter++) {
        if (iter->getDeath()) {
            deathSum++;
        }
    }
    deathPercent = deathSum / caseVect.size();
    deathPercent *= 100;
    cout << "The percentage of the top " << k << " most high risk cases that resulted in death is: ";
    cout << fixed << setprecision(2) << deathPercent << endl;
}

void Map::topkDeathPercentDate(string date, int k) {
    vector<cases> caseVect;
    topkRange(caseVect, date, date, k);
    double deathSum = 0;
    double deathPercent = 0;
    for (auto iter = caseVect.begin(); iter != caseVect.end(); iter++) {
        if (iter->getDeath()) {
            deathSum++;
        }
    }
    deathPercent = deathSum / caseVect.size();
    deathPercent *= 100;
    cout << "The percentage of the top " << k << " most high risk cases that resulted in death is: ";
    cout << fixed << setprecision(2) << deathPercent << endl;
}

void Map::topkHospPercentFull(int k) {
    vector<cases> caseVect;
    topkRange(caseVect, "2020/01/01", "2020/11/19", k);
    double hospSum = 0;
    double hospPercent = 0;
    for (auto iter = caseVect.begin(); iter != caseVect.end(); iter++) {
        if (iter->getHosp()) {
            hospSum++;
        }
    }
    hospPercent = hospSum / caseVect.size();
    hospPercent *= 100;
    cout << "The percentage of the top " << k << " most high risk cases that resulted in hospitalization is: ";
    cout << fixed << setprecision(2) << hospPercent << endl;
}

void Map::topkHospPercentDate(string date, int k) {
    vector<cases> caseVect;
    topkRange(caseVect, date, date, k);
    double hospSum = 0;
    double hospPercent = 0;
    for (auto iter = caseVect.begin(); iter != caseVect.end(); iter++) {
        if (iter->getHosp()) {
            hospSum++;
        }
    }
    hospPercent = hospSum / caseVect.size();
    hospPercent *= 100;
    cout << "The percentage of the top " << k << " most high risk cases that resulted in hospitalization is: ";
    cout << fixed << setprecision(2) << hospPercent << endl;
}

void Map::topkICUPercentFull(int k) {
    vector<cases> caseVect;
    topkRange(caseVect, "2020/01/01", "2020/11/19", k);
    double ICUSum = 0;
    double ICUPercent = 0;
    for (auto iter = caseVect.begin(); iter != caseVect.end(); iter++) {
        if (iter->getIcu()) {
            ICUSum++;
        }
    }
    ICUPercent = ICUSum / caseVect.size();
    ICUPercent *= 100;
    cout << "The percentage of the top " << k << " most high risk cases that resulted in intensive care is: ";
    cout << fixed << setprecision(2) << ICUPercent << endl;
}

void Map::topkICUPercentDate(string date, int k) {
    vector<cases> caseVect;
    topkRange(caseVect, date, date, k);
    double ICUSum = 0;
    double ICUPercent = 0;
    for (auto iter = caseVect.begin(); iter != caseVect.end(); iter++) {
        if (iter->getIcu()) {
            ICUSum++;
        }
    }
    ICUPercent = ICUSum / caseVect.size();
    ICUPercent *= 100;
    cout << "The percentage of the top " << k << " most high risk cases that resulted in intensive care is: ";
    cout << fixed << setprecision(2) << ICUPercent << endl;
}

void Map::topkRange(vector<cases>& vec, string date1, string date2, int k) {

    cases smallestCase("2020/01/01", "0 - 9 Years", "Other", false, false, false, false, -1);
    cases greatestCase = smallestCase;
    cases case1;
    string dateBegin = date1;
    bool inVec = false;
    int length = dateSubtraction(date1, date2);
    for (int o = 0; o < k; o++) {                                               //find k largest cases
        for (int i = 0; i < length; i++) {                                      //running through the dates to find the largest case
            for (int j = 0; j < map[date1][true].size(); j++) {                 //deaths
                if (case1(map[date1][true][j], greatestCase)) {                 //current case is bigger than greatest case
                    for (int a = 0; a < vec.size(); a++) {                      //check if case is already added to vector
                        if (vec[a].equals(map[date1][true][j]))
                            inVec = true;
                    }
                    if (!inVec)                                                 //if not already in vector, set greatestCase to curr
                        greatestCase = map[date1][true][j];
                }
                inVec = false;
            }
            for (int l = 0; l < map[date1][false].size(); l++) {                //lives
                if (case1(map[date1][false][l], greatestCase)) {
                    for (int b = 0; b < vec.size(); b++) {
                        if (vec[b].equals(map[date1][false][l]))                      //case not already added to vector
                            inVec = true;
                    }
                    if (!inVec)
                        greatestCase = map[date1][false][l];
                }
                inVec = false;
            }
            date1 = dateStepping(date1);
        }
        vec.push_back(greatestCase);
        greatestCase = smallestCase;
        date1 = dateBegin;
    }
}


void Map::insertCase(cases c) {
    map[c.getDate()][c.getDeath()].push_back(c);
}

int Map::deathsOverTime(string date1, string date2) {
    int size = 0;
    int length = dateSubtraction(date1, date2);
    for (int i = 0; i < length; i++) {
        size += map[date1][true].size();
        date1 = dateStepping(date1);
    }
    return size;
}

int Map::casesOverTime(string date1, string date2) {
    int size = 0;
    int length = dateSubtraction(date1, date2);
    for (int i = 0; i < length; i++) {
        size += map[date1][true].size();
        size += map[date1][false].size();
        date1 = dateStepping(date1);
    }
    return size;
}

int Map::deathsWithMedcond(string date1, string date2) {
    int size = 0;
    int length = dateSubtraction(date1, date2);
    for (int i = 0; i < length; i++) {
        for (int j = 0; j < map[date1][true].size(); j++) {
            if (map[date1][true][j].getMedcond())
                size++;
        }
        date1 = dateStepping(date1);
    }
    return size;
}

int Map::casesWithMedcond(string date1, string date2) {
    int size = 0;
    int length = dateSubtraction(date1, date2);
    for (int i = 0; i < length; i++) {
        for (int j = 0; j < map[date1][true].size(); j++) {
            if (map[date1][true][j].getMedcond())
                size++;
        }
        for (int k = 0; k < map[date1][false].size(); k++) {
            if (map[date1][false][k].getMedcond())
                size++;
        }
        date1 = dateStepping(date1);
    }
    return size;
}

int Map::casesWithHosp(string date1, string date2) {
    int size = 0;
    int length = dateSubtraction(date1, date2);
    for (int i = 0; i < length; i++) {
        for (int j = 0; j < map[date1][true].size(); j++) {
            if (map[date1][true][j].getHosp())
                size++;
        }
        for (int k = 0; k < map[date1][false].size(); k++) {
            if (map[date1][false][k].getHosp())
                size++;
        }
        date1 = dateStepping(date1);
    }
    return size;
}

int Map::deathsWithHosp(string date1, string date2) {
    int size = 0;
    int length = dateSubtraction(date1, date2);
    for (int i = 0; i < length; i++) {
        for (int j = 0; j < map[date1][true].size(); j++) {
            if (map[date1][true][j].getHosp())
                size++;
        }
        date1 = dateStepping(date1);
    }
    return size;
}

int Map::casesWithICU(string date1, string date2) {
    int size = 0;
    int length = dateSubtraction(date1, date2);
    for (int i = 0; i < length; i++) {
        for (int j = 0; j < map[date1][true].size(); j++) {
            if (map[date1][true][j].getIcu())
                size++;
        }
        for (int k = 0; k < map[date1][false].size(); k++) {
            if (map[date1][false][k].getIcu())
                size++;
        }
        date1 = dateStepping(date1);
    }
    return size;
}

int Map::deathsWithICU(string date1, string date2) {
    int size = 0;
    int length = dateSubtraction(date1, date2);
    for (int i = 0; i < length; i++) {
        for (int j = 0; j < map[date1][true].size(); j++) {
            if (map[date1][true][j].getIcu())
                size++;
        }
        date1 = dateStepping(date1);
    }
    return size;
}

void Map::agesOfCases(string date1, string date2) {
    int zero = 0;
    int ten = 0;
    int twenty = 0;
    int thirty = 0;
    int forty = 0;
    int fifty = 0;
    int sixty = 0;
    int seventy = 0;
    int eighty = 0;
    string s;
    int length = dateSubtraction(date1, date2);
    for (int i = 0; i < length; i++) {
        for (int j = 0; j < map[date1][true].size(); j++) {
            s = map[date1][true][j].getAge();
            if (s == "0 - 9 Years")
                zero++;
            if (s == "10 - 19 Years")
                ten++;
            if (s == "20 - 29 Years")
                twenty++;
            if (s == "30 - 39 Years")
                thirty++;
            if (s == "40 - 49 Years")
                forty++;
            if (s == "50 - 59 Years")
                fifty++;
            if (s == "60 - 69 Years")
                sixty++;
            if (s == "70 - 79 Years")
                seventy++;
            if (s == "80+ Years")
                eighty++;
        }
        for (int k = 0; k < map[date1][false].size(); k++) {
            s = map[date1][false][k].getAge();
            if (s == "0 - 9 Years")
                zero++;
            if (s == "10 - 19 Years")
                ten++;
            if (s == "20 - 29 Years")
                twenty++;
            if (s == "30 - 39 Years")
                thirty++;
            if (s == "40 - 49 Years")
                forty++;
            if (s == "50 - 59 Years")
                fifty++;
            if (s == "60 - 69 Years")
                sixty++;
            if (s == "70 - 79 Years")
                seventy++;
            if (s == "80+ Years")
                eighty++;
        }
        date1 = dateStepping(date1);
    }
    cout << "Cases of people aged 0 - 9: " << zero << "\n";
    cout << "Cases of people aged 10 - 19: " << ten << "\n";
    cout << "Cases of people aged 20 - 29: " << twenty << "\n";
    cout << "Cases of people aged 30 - 39: " << thirty << "\n";
    cout << "Cases of people aged 40 - 49: " << forty << "\n";
    cout << "Cases of people aged 50 - 59: " << fifty << "\n";
    cout << "Cases of people aged 60 - 69: " << sixty << "\n";
    cout << "Cases of people aged 70 - 79: " << seventy << "\n";
    cout << "Cases of people aged 80+: " << eighty << "\n\n";
}

void Map::agesOfDeaths(string date1, string date2) {
    int zero = 0;
    int ten = 0;
    int twenty = 0;
    int thirty = 0;
    int forty = 0;
    int fifty = 0;
    int sixty = 0;
    int seventy = 0;
    int eighty = 0;
    string s;
    int length = dateSubtraction(date1, date2);
    for (int i = 0; i < length; i++) {
        for (int j = 0; j < map[date1][true].size(); j++) {
            s = map[date1][true][j].getAge();
            if (s == "0 - 9 Years")
                zero++;
            if (s == "10 - 19 Years")
                ten++;
            if (s == "20 - 29 Years")
                twenty++;
            if (s == "30 - 39 Years")
                thirty++;
            if (s == "40 - 49 Years")
                forty++;
            if (s == "50 - 59 Years")
                fifty++;
            if (s == "60 - 69 Years")
                sixty++;
            if (s == "70 - 79 Years")
                seventy++;
            if (s == "80+ Years")
                eighty++;
        }
        date1 = dateStepping(date1);
    }
    cout << "Deaths of people aged 0 - 9: " << zero << "\n";
    cout << "Deaths of people aged 10 - 19: " << ten << "\n";
    cout << "Deaths of people aged 20 - 29: " << twenty << "\n";
    cout << "Deaths of people aged 30 - 39: " << thirty << "\n";
    cout << "Deaths of people aged 40 - 49: " << forty << "\n";
    cout << "Deaths of people aged 50 - 59: " << fifty << "\n";
    cout << "Deaths of people aged 60 - 69: " << sixty << "\n";
    cout << "Deaths of people aged 70 - 79: " << seventy << "\n";
    cout << "Deaths of people aged 80+: " << eighty << "\n\n";
}

void Map::sexesOfCases(string date1, string date2) {
    int male = 0;
    int female = 0;
    int other = 0;
    int unknown = 0;
    string s;
    int length = dateSubtraction(date1, date2);
    for (int i = 0; i < length; i++) {
        for (int j = 0; j < map[date1][true].size(); j++) {
            s = map[date1][true][j].getSex();
            if (s == "Female")
                female++;
            else if (s == "Male")
                male++;
            else if (s == "Other")
                other++;
            else
                unknown++;
        }
        for (int k = 0; k < map[date1][false].size(); k++) {
            s = map[date1][false][k].getSex();
            if (s == "Female")
                female++;
            else if (s == "Male")
                male++;
            else if (s == "Other")
                other++;
            else
                unknown++;
        }
        date1 = dateStepping(date1);
    }
    cout << "Male cases: " << male << "\n";
    cout << "Female cases: " << female << "\n";
    cout << "Other gender cases: " << other << "\n";
    cout << "Unknown gender cases: " << unknown << "\n\n";
}

void Map::sexesOfDeaths(string date1, string date2) {
    int male = 0;
    int female = 0;
    int other = 0;
    int unknown = 0;
    string s;
    int length = dateSubtraction(date1, date2);
    for (int i = 0; i < length; i++) {
        for (int j = 0; j < map[date1][true].size(); j++) {
            s = map[date1][true][j].getSex();
            if (s == "Female")
                female++;
            else if (s == "Male")
                male++;
            else if (s == "Other")
                other++;
            else
                unknown++;
        }
        date1 = dateStepping(date1);
    }
    cout << "Male deaths: " << male << "\n";
    cout << "Female deaths: " << female << "\n";
    cout << "Other gender deaths: " << other << "\n";
    cout << "Unknown gender deaths: " << unknown << "\n\n";
}

int Map::maxDeathsDayOverTime(string date1, string date2) {
    int large = 0;
    int length = dateSubtraction(date1, date2);
    for (int i = 0; i < length; i++) {
        if (map[date1][true].size() > large)
            large = map[date1][true].size();
        date1 = dateStepping(date1);
    }
    return large;
}

double Map::deathRateOverTime(string date1, string date2) {
    double deaths = deathsOverTime(date1, date2);
    double cases = casesOverTime(date1, date2);
    return deaths/cases;
}

//The date class is used in the heap data structure and it is stored at the first level heap. A date object contains the number of deaths for a given day as a variable as well as
//a heap containing all cases for a day with their priority set to their risk of death to COVID-19
class Date {
private:
    string date;
    int deaths;
    vector<cases> caseHeap; //heap that stores the cases by their risk factor
    stack<cases> removeStack; //objects that are removed from the heap are stored here to be added back after the operation is finished.
    //fills a heap with the top k most at risk cases from a single date
    void topkDate(vector<cases>& heap, int k);
    friend class CovidHeap;
public:
    Date(string date, int deaths); //initializes a date with the date, deaths, and empty heap and stack.
    Date() {
        date = "";
        deaths = 0;
    }
    void insertCase(cases& c); //insert an individual case into the date's case heap. Called from a CovidHeap object.


    //returns true when date1 has more deaths than date2
    bool operator()(const Date& date1, const Date& date2) {
        return date1.deaths > date2.deaths;
    }
    
};

Date::Date(string date, int deaths) {
    this->date = date;
    this->deaths = deaths;
}

void Date::insertCase(cases& c) {
    caseHeap.push_back(c);
    push_heap(caseHeap.begin(), caseHeap.end(), cases());
}

void Date::topkDate(vector<cases>& heap, int k) {
    int cntr = 0;
    while (cntr < k && !caseHeap.empty()) {
        //while the case heap is not empty, pop the top k cases off and put them in the return heap
        removeStack.push(caseHeap.front());
        heap.push_back(caseHeap.front());
        push_heap(heap.begin(), heap.end(), cases());
        pop_heap(caseHeap.begin(), caseHeap.end(), cases());
        caseHeap.pop_back();
        cntr++;
    }
    while (!removeStack.empty()) {
        //empty the removestack back into the heap
        caseHeap.push_back(removeStack.top());
        push_heap(caseHeap.begin(), caseHeap.end(), cases());
        removeStack.pop();
    }
}

//The CovidHeap class contains a heap of date heaps organized by deaths on a given date.
class CovidHeap {
private:
    vector<Date> dateHeap; //contains a heap of dates sorted by death


    void insertDate(unordered_map<bool, vector<cases>>& dateMap, string date); //pass in a specific date from the CovidMap as a parameter to create a date object and insert allof the cases.
    //fills the vector with the top k most at risk cases from the full data set.
    void topkFull(vector<cases>& caseVect, int k);
    void topkDate(vector<cases>& caseVect, int k, string d);
    
public:
    //Constructor
    CovidHeap(Map& covidMap);

    //publicly accessible algorithms

    //these functions calculate and display the percent of cases resulting in deaths for the top k cases
    void topkDeathPercentFull(int k);
    void topkDeathPercentDate(string date, int k);

    void topkHospPercentFull(int k);
    void topkHospPercentDate(string date, int k);

    void topkICUPercentFull(int k);
    void topkICUPercentDate(string date, int k);

    bool kValid(int k);
    
};

void CovidHeap::topkDeathPercentFull(int k) {
    vector<cases> caseVect;
    topkFull(caseVect, k);
    double deathSum = 0;
    double deathPercent = 0;
    for (auto iter = caseVect.begin(); iter != caseVect.end(); iter++) {
        //if the case resulted in death, increment the death counter
        if (iter->getDeath()) {
            deathSum++;
        }
    }
    
    deathPercent = deathSum / k;
    deathPercent *= 100;
    cout << "The percentage of the top " << k << " most high risk cases that resulted in death is: ";
    cout << fixed << setprecision(2) << deathPercent << endl;
}

void CovidHeap::topkDeathPercentDate(string date, int k) {
    vector<cases> caseVect;
    topkDate(caseVect, k, date);
    double deathSum = 0;
    double deathPercent = 0;
    for (auto iter = caseVect.begin(); iter != caseVect.end(); iter++) {
        if (iter->getDeath()) {
            deathSum++;
        }
    }

    deathPercent = deathSum / k;
    deathPercent *= 100;
    cout << "The percentage of the top " << k << " most high risk cases that resulted in death is: ";
    cout << fixed << setprecision(2) << deathPercent << endl;
}

void CovidHeap::topkHospPercentFull(int k) {
    vector<cases> caseVect;
    topkFull(caseVect, k);
    double hospSum = 0;
    double hospPercent = 0;
    for (auto iter = caseVect.begin(); iter != caseVect.end(); iter++) {
        if (iter->getHosp()) {
            hospSum++;
        }
    }

    hospPercent = hospSum / k;
    hospPercent *= 100;
    cout << "The percentage of the top " << k << " most high risk cases that resulted in hospitilization is: ";
    cout << fixed << setprecision(2) << hospPercent << endl;
}

void CovidHeap::topkHospPercentDate(string date, int k) {
    vector<cases> caseVect;
    topkDate(caseVect, k, date);
    double hospSum = 0;
    double hospPercent = 0;
    for (auto iter = caseVect.begin(); iter != caseVect.end(); iter++) {
        if (iter->getHosp()) {
            hospSum++;
        }
    }

    hospPercent = hospSum / k;
    hospPercent *= 100;
    cout << "The percentage of the top " << k << " most high risk cases that resulted in hospitilization is: ";
    cout << fixed << setprecision(2) << hospPercent << endl;
}

void CovidHeap::topkICUPercentFull(int k) {
    vector<cases> caseVect;
    topkFull(caseVect, k);
    double ICUSum = 0;
    double ICUPercent = 0;
    for (auto iter = caseVect.begin(); iter != caseVect.end(); iter++) {
        if (iter->getIcu()) {
            ICUSum++;
        }
    }
    ICUPercent = ICUSum / k;
    ICUPercent *= 100;
    cout << "The percentage of the top " << k << " most high risk cases that resulted in intensive care is: ";
    cout << fixed << setprecision(2) << ICUPercent << endl;
}

void CovidHeap::topkICUPercentDate(string date, int k) {
    vector<cases> caseVect;
    topkDate(caseVect, k, date);
    double ICUSum = 0;
    double ICUPercent = 0;
    for (auto iter = caseVect.begin(); iter != caseVect.end(); iter++) {
        if (iter->getIcu()) {
            ICUSum++;
        }
    }
    ICUPercent = ICUSum / k;
    ICUPercent *= 100;
    cout << "The percentage of the top " << k << " most high risk cases that resulted in intensive care is: ";
    cout << fixed << setprecision(2) << ICUPercent << endl;
}

CovidHeap::CovidHeap(Map& covidMap) {
    //iterate through the top level of the covid map and call insert Date for each member
    for (auto iter = covidMap.map.begin(); iter != covidMap.map.end(); iter++) {
        insertDate(iter->second, iter->first);
    }
    make_heap(dateHeap.begin(), dateHeap.end(), Date());
}

void CovidHeap::insertDate(unordered_map<bool, vector<cases>>& dateMap, string date) {
    Date newDate(date, dateMap[true].size()); //create a new date with the deaths corresponding to the given date
    vector<cases>& caseVectT = dateMap[true];
    vector<cases>& caseVectF = dateMap[false];

    for (auto iter = caseVectT.begin(); iter != caseVectT.end(); iter++) {
        newDate.insertCase(*iter); //add each case to the date object
    }
    for (auto iter = caseVectF.begin(); iter != caseVectF.end(); iter++) {
        newDate.insertCase(*iter);
    }

    dateHeap.push_back(newDate);
}

void CovidHeap::topkFull(vector<cases>& caseVect, int k) {
    vector<cases> heap;
    //Get the top k cases from each date onto a heap
    for (auto iter = dateHeap.begin(); iter != dateHeap.end(); iter++) {
        iter->topkDate(heap, k);
    }
    int cntr = 0;
    while (cntr < k &&  !heap.empty()) {
        caseVect.push_back(heap.front());
        pop_heap(heap.begin(), heap.end(), cases());
        heap.pop_back();
        cntr++;
    }
}

void CovidHeap::topkDate(vector<cases>& caseVect, int k, string d) {

    //Find the correct date in the date heap
    for (auto iter = dateHeap.begin(); iter != dateHeap.end(); iter++) {
        if (iter->date == d) {
            iter->topkDate(caseVect, k);
        }
    }
  
}

bool CovidHeap::kValid(int k) {
    if (k < 1 || k >= cases::IDcntr) {
        cout << "Input is invalid. Please enter a number between 1 and " << cases::IDcntr << endl;
        return false;
    } 
    return true;
}


int main()  {
    Map covidMap;

    string line;    //dummy first line
    ifstream file1("COVID-19_Case_Surveillance_Public_Use_Data.csv");      //need filepath
    //ifstream file1("/Users/jkim210/Documents/text.csv");                                          //my small tester file
    string a;       //date
    string b;
    string c;
    string d;
    string e;       //sex
    //char* e2;
    string f;       //age group
    //char* f2;
    string g;
    string h;       //hosp
    bool h2;
    string i;       //icu
    bool i2;
    string j;       //death
    bool j2;
    string k;       //medcond
    bool k2;

    string z;       //dummy for ethnicities

    //data loader
    if (file1.is_open()) {
        getline(file1, line);
        while (getline(file1, a, ',')) {
            getline(file1, b, ',');
            getline(file1, c, ',');
            getline(file1, d, ',');
            getline(file1, e, ',');
            getline(file1, f, ',');
            getline(file1, g, ',');
            if (!(g == "Unknown" || g == "NA" || g == "Hispanic/Latino")) {
                getline(file1, z, ',');
            }
            getline(file1, h, ',');
            getline(file1, i, ',');
            getline(file1, j, ',');
            getline(file1, k, '\n');

            //e2 = &e[0];
            //f2 = &f[0];

            if (h == "Unknown" || h == "Missing" || h == "No") {
                h2 = false;
            }
            else if (h == "Yes")
                h2 = true;

            if (i == "Unknown" || i == "Missing" || i == "No") {
                i2 = false;
            }
            else if (i == "Yes")
                i2 = true;

            if (j == "Unknown" || j == "Missing" || j == "No") {
                j2 = false;
            }
            if (j == "Yes") {
                j2 = true;
            }

            if (k == "Unknown" || k == "Missing" || k == "No") {
                k2 = false;
            }
            else if (k == "Yes")
                k2 = true;
            covidMap.insertCase(cases(a,f,e,h2,i2,j2,k2));
        }
    }
    else
        cout << "cannot open file";

    CovidHeap covidHeap(covidMap);

    //menu options

    bool runLoop = true;

    int choice;
    int choice2;
    string date1;
    string date2;
    //cin >> choice;
    //cin.get();

    //if (choice == 0)
    //    runLoop = false;
    cout << "COVID-19 Data Visualizer" << "\n\n";
    do {

        cout << "Press 1 if you're searching for data from a specific date" << "\n";
        cout << "Press 2 if you're searching for data from a specific time period" << "\n";
        cout << "Press 3 if you're searching for data from the whole data set" << "\n";
        cout << "Press 0 to quit" << "\n";
        cin >> choice;
        cin.get();

        if (choice == 0)
            break;

        switch (choice) {
            case 1:
                cout << "Enter the date in the form yyyy/mm/dd: ";
                cin >> date1;
                cout << "Select a number to see information on:" << "\n";
                cout << "1: Cases only" << "\n";
                cout << "2: Deaths only" << "\n";
                cout << "3: The effects of age on cases" << "\n";
                cout << "4: The effects of sex on cases" << "\n";
                cout << "5: The effects of pre-existing medical conditions on cases" << "\n";
                cout << "6: The effects of hospitalizations on cases" << "\n";
                cout << "7: The effects of being admitted to the ICU on cases" << "\n";
                cout << "8: The top k most at risk cases" << endl;

                cin >> choice2;
                switch (choice2) {
                    case 1:
                        cout << "Number of cases on " << date1 << ": " << covidMap.casesOverTime(date1, date1) << "\n\n";
                        break;
                    case 2:
                        cout << "Number of deaths on " << date1 << ": " << covidMap.deathsOverTime(date1, date1)
                             << "\n";
                        cout << "Death rate: " << covidMap.deathRateOverTime(date1, date1) << "\n\n";
                        break;
                    case 3:
                        cout << "Age groups of cases on " << date1 << ": \n";
                        covidMap.agesOfCases(date1, date1);
                        cout << "Age groups of deaths on " << date1 << ": \n";
                        covidMap.agesOfDeaths(date1, date1);
                        break;
                    case 4:
                        cout << "Sexes of cases on " << date1 << ": \n";
                        covidMap.sexesOfCases(date1, date1);
                        cout << "Sexes of deaths on " << date1 << ": \n";
                        covidMap.sexesOfDeaths(date1, date1);
                        break;
                    case 5:
                        cout << "Number of cases with medical conditions on " << date1 << ": "
                             << covidMap.casesWithMedcond(date1, date1) << "\n";
                        cout << "Number of deaths of cases with medical conditions on " << date1 << ": "
                             << covidMap.deathsWithMedcond(date1, date1) << "\n\n";
                        break;
                    case 6:
                        cout << "Number of cases that were hospitalized on " << date1 << ": "
                             << covidMap.casesWithHosp(date1, date1) << "\n";
                        cout << "Number of deaths of cases that were hospitalized on " << date1 << ": "
                             << covidMap.deathsWithHosp(date1, date1) << "\n\n";
                        break;
                    case 7:
                        cout << "Number of cases that were admitted to the ICU on " << date1 << ": "
                             << covidMap.casesWithICU(date1, date1) << "\n";
                        cout << "Number of deaths of cases that were admitted to the ICU on " << date1 << ": "
                             << covidMap.deathsWithICU(date1, date1) << "\n\n";
                        break;
                    case 8:
                        int k;
                        cout << "Please enter a number k between 1 and " << cases::IDcntr - 1 << ": " << endl;
                        cin >> k;
                        bool isValid = covidHeap.kValid(k);
                        if (isValid) {
                            //check if the input k is valid. If it is, run the processes and desired outputs
                            int select;
                            cout << "Input 0 to use the heap, 1 to use the heap and map (warning, runs slow on large k)" << endl;
                            cin >> select;
                            if (select == 1) {
                                //call functions using the map
                                cout << "The output using the map stucture is: " << endl;
                                auto startMap = chrono::high_resolution_clock::now();
                                //run the map algorithms
                                covidMap.topkDeathPercentDate(date1, k);
                                covidMap.topkHospPercentDate(date1, k);
                                covidMap.topkICUPercentDate(date1, k);

                                auto stopMap = chrono::high_resolution_clock::now();

                                auto durationMap = chrono::duration_cast<chrono::milliseconds>(stopMap - startMap);
                                cout << "The time elapsed to calculate the map output was: " << durationMap.count() << " milliseconds" << endl;
                                cout << endl;
                            }
                          

                            //call functions using the heap
                            cout << "The output using the heap structure is: " << endl;
                            auto startHeap = chrono::high_resolution_clock::now();
                            covidHeap.topkDeathPercentDate(date1, k);
                            covidHeap.topkHospPercentDate(date1, k);
                            covidHeap.topkICUPercentDate(date1, k);
                            auto stopHeap = chrono::high_resolution_clock::now();

                            auto durationHeap = chrono::duration_cast<chrono::milliseconds>(stopHeap - startHeap);

                            cout << "The time elapsed to calculate the heap output was: " << durationHeap.count() << " milliseconds" << endl;
                            cout << endl;

                        }
                        break;
                }

                break;
            case 2:
                cout << "Enter the beginning date in the form yyyy/mm/dd: ";
                cin >> date1;
                cout << "Enter the ending date: ";
                cin >> date2;
                cout << "Select a number to see information on:" << "\n";
                cout << "1: Cases only" << "\n";
                cout << "2: Deaths only" << "\n";
                cout << "3: The effects of age on cases" << "\n";
                cout << "4: The effects of sex on cases" << "\n";
                cout << "5: The effects of pre-existing medical conditions on cases" << "\n";
                cout << "6: The effects of hospitalizations on cases" << "\n";
                cout << "7: The effects of being admitted to the ICU on cases" << "\n";

                cin >> choice2;
                switch (choice2) {
                    case 1:
                        cout << "Number of cases between " << date1 << " and " << date2 << ": "
                             << covidMap.casesOverTime(date1, date2) << "\n\n";
                        break;
                    case 2:
                        cout << "Number of deaths between " << date1 << " and " << date2 << ": "
                             << covidMap.deathsOverTime(date1, date2) << "\n";
                        cout << "Death rate: " << covidMap.deathRateOverTime(date1, date2) << "\n\n";
                        break;
                    case 3:
                        cout << "Age groups of cases between " << date1 << " and " << date2 << ": \n";
                        covidMap.agesOfCases(date1, date2);
                        cout << "Age groups of deaths between " << date1 << " and " << date2 << ": \n";
                        covidMap.agesOfDeaths(date1, date2);
                        break;
                    case 4:
                        cout << "Sexes of cases between " << date1 << " and " << date2 << ": \n";
                        covidMap.sexesOfCases(date1, date2);
                        cout << "Sexes of deaths between " << date1 << " and " << date2 << ": \n";
                        covidMap.sexesOfDeaths(date1, date2);
                        break;
                    case 5:
                        cout << "Number of cases with medical conditions between " << date1 << " and " << date2 << ": "
                             << covidMap.casesWithMedcond(date1, date2) << "\n";
                        cout << "Number of deaths of cases with medical conditions between " << date1 << " and "
                             << date2 << ": "
                             << covidMap.deathsWithMedcond(date1, date2) << "\n\n";
                        break;
                    case 6:
                        cout << "Number of cases that were hospitalized between " << date1 << " and " << date2 << ": "
                             << covidMap.casesWithHosp(date1, date2) << "\n";
                        cout << "Number of deaths of cases that were hospitalized between " << date1 << " and " << date2
                             << ": "
                             << covidMap.deathsWithHosp(date1, date2) << "\n\n";
                        break;
                    case 7:
                        cout << "Number of cases that were admitted to the ICU between " << date1 << " and " << date2
                             << ": "
                             << covidMap.casesWithICU(date1, date2) << "\n";
                        cout << "Number of deaths of cases that were admitted to the ICU between " << date1 << " and "
                             << date2 << ": "
                             << covidMap.deathsWithICU(date1, date2) << "\n\n";
                        break;
                }
                break;
            case 3:
                date1 = "2020/01/01";
                date2 = "2020/11/19";                                                       
                cout << "Select a number to see information on:" << "\n";
                cout << "1: Cases only" << "\n";
                cout << "2: Deaths only" << "\n";
                cout << "3: The effects of age on cases" << "\n";
                cout << "4: The effects of sex on cases" << "\n";
                cout << "5: The effects of pre-existing medical conditions on cases" << "\n";
                cout << "6: The effects of hospitalizations on cases" << "\n";
                cout << "7: The effects of being admitted to the ICU on cases" << "\n";
                cout << "8: The top k most at risk cases" << endl;
                cin >> choice2;

                switch (choice2) {
                    case 1:
                        cout << "Number of cases between " << date1 << " and " << date2 << ": "
                             << covidMap.casesOverTime(date1, date2) << "\n\n";
                        break;
                    case 2:
                        cout << "Number of deaths between " << date1 << " and " << date2 << ": "
                             << covidMap.deathsOverTime(date1, date2) << "\n";
                        cout << "Death rate: " << covidMap.deathRateOverTime(date1, date2) << "\n\n";
                        break;
                    case 3:
                        cout << "Age groups of cases between " << date1 << " and " << date2 << ": \n";
                        covidMap.agesOfCases(date1, date2);
                        cout << "Age groups of deaths between " << date1 << " and " << date2 << ": \n";
                        covidMap.agesOfDeaths(date1, date2);
                        break;
                    case 4:
                        cout << "Sexes of cases between " << date1 << " and " << date2 << ": \n";
                        covidMap.sexesOfCases(date1, date2);
                        cout << "Sexes of deaths between " << date1 << " and " << date2 << ": \n";
                        covidMap.sexesOfDeaths(date1, date2);
                        break;
                    case 5:
                        cout << "Number of cases with medical conditions between " << date1 << " and " << date2 << ": "
                             << covidMap.casesWithMedcond(date1, date2) << "\n";
                        cout << "Number of deaths of cases with medical conditions between " << date1 << " and "
                             << date2 << ": "
                             << covidMap.deathsWithMedcond(date1, date2) << "\n\n";
                        break;
                    case 6:
                        cout << "Number of cases that were hospitalized between " << date1 << " and " << date2 << ": "
                             << covidMap.casesWithHosp(date1, date2) << "\n";
                        cout << "Number of deaths of cases that were hospitalized between " << date1 << " and " << date2
                             << ": "
                             << covidMap.deathsWithHosp(date1, date2) << "\n\n";
                        break;
                    case 7:
                        cout << "Number of cases that were admitted to the ICU between " << date1 << " and " << date2
                             << ": "
                             << covidMap.casesWithICU(date1, date2) << "\n";
                        cout << "Number of deaths of cases that were admitted to the ICU between " << date1 << " and "
                             << date2 << ": "
                             << covidMap.deathsWithICU(date1, date2) << "\n\n";
                        break;
                    case 8:
                        int k;
                        cout << "Please enter a number k between 1 and " << cases::IDcntr - 1 << ": " << endl;
                        cin >> k;
                        bool isValid = covidHeap.kValid(k);
                        if (isValid) {
                            //check if the input k is valid. If it is, run the processes and desired outputs
                            int select;
                            cout << "Input 0 to use the heap, 1 to use the heap and map (warning, runs slow on large k)" << endl;
                            cin >> select;
                            if (select == 1) {
                                //call functions using the map
                                cout << "The output using the map stucture is: " << endl;
                                auto startMap = chrono::high_resolution_clock::now();
                                //run the map algorithms
                                covidMap.topkDeathPercentFull(k);
                                covidMap.topkHospPercentFull(k);
                                covidMap.topkICUPercentFull(k);

                                auto stopMap = chrono::high_resolution_clock::now();

                                auto durationMap = chrono::duration_cast<chrono::milliseconds>(stopMap - startMap);
                                cout << "The time elapsed to calculate the map output was: " << durationMap.count() << " milliseconds" << endl;
                                cout << endl;
                            }

                            //call functions using the heap
                            cout << "The output using the heap structure is: " << endl;
                            auto startHeap = chrono::high_resolution_clock::now();
                            covidHeap.topkDeathPercentFull(k);
                            covidHeap.topkHospPercentFull(k);
                            covidHeap.topkICUPercentFull(k);
                            auto stopHeap = chrono::high_resolution_clock::now();
                            
                            auto durationHeap = chrono::duration_cast<chrono::milliseconds>(stopHeap - startHeap);

                            cout << "The time elapsed to calculate the heap output was: " << durationHeap.count() << " milliseconds" << endl;
                            cout << endl;
                            
                        }
                        break;
                }
                break;
        }
    }
    while (runLoop);
}