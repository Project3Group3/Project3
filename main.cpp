#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

class cases {
private:
    string date;
    char* age;          //group     0-9,10-19,...80+
    char* sex;                   //male, female, other, missing
    bool hosp;                  //true if hosp, false if not or missing
    bool icu;                   //""
    bool death;                 //true if death, false otherwise
    bool medcond;               //true if has underlying condition
public:
    cases(string d, char* a, char* s, bool h, bool i, bool dt, bool m) {
        date = d;
        age = a;
        sex = s;
        hosp = h;
        icu = i;
        death = dt;
        medcond = m;
    }
    string getDate();
    bool getDeath();
    char* getAge();
    char* getSex();
    bool getHosp();
    bool getIcu();
    bool getMedcond();
};

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
char* cases::getAge() {
    return age;
}
char* cases::getSex() {
    return sex;
}

int dateSubtraction(string date1, string date2) {       //hacky because doesnt take year into account
    int month1 = stoi(date1.substr(5,6));
    int day1 = stoi(date1.substr(8,9));
    int month2 = stoi(date2.substr(5,6));
    int day2 = stoi(date2.substr(8,9));
    if (month1 > month2) {
        swap(month1, month2);
        swap(day1, day2);
    }

    int sum = 0;
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
    sum = sum - day1 + 1 + day2;
    return sum;
}
string dateStepping(string date) {                      //hacky because doesnt take year into account   adds 1 to date and returns as yyyy/mm//dd
    int month = stoi(date.substr(5,6));
    int day = stoi(date.substr(8,9));
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
    if (day < 10 && month < 10) {
        return ("2020/0" + to_string(month) + "/0" + to_string(day));
    }
    else if (day < 10) {
        return ("2020/" + to_string(month) + "/0" + to_string(day));
    }
    else if (month < 10) {
        return ("2020/0" + to_string(month) + "/" + to_string(day));
    }
    return ("2020/" + to_string(month) + "/" + to_string(day));
}

class Map {
//private:
public:
    unordered_map<string, unordered_map<bool, vector<cases>>> map;
    void insertCase(cases c);
    int deathsOverTime(string date1, string date2);
    int casesOverTime(string date1, string date2);
    double deathRateOverTime(string date1, string date2);
};


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

double Map::deathRateOverTime(string date1, string date2) {
    double deaths = deathsOverTime(date1, date2);
    double cases = casesOverTime(date1, date2);
    return deaths/cases;
}


int main()  {
    Map covidMap;

    string line;    //dummy first line
    ifstream file1("/Users/jkim210/Documents/COVID-19_Case_Surveillance_Public_Use_Data.csv");      //need filepath
    //ifstream file1("/Users/jkim210/Documents/text.csv");                                          //my small tester file
    string a;       //date
    string b;
    string c;
    string d;
    string e;       //sex
    char* e2;
    string f;       //age group
    char* f2;
    string g;
    string h;       //hosp
    bool h2;
    string i;       //icu
    bool i2;
    string j;       //death
    bool j2;
    string k;       //medcond
    bool k2;
    if (file1.is_open()) {                          //data loader
        getline(file1, line);
        while (getline(file1, a, ',')) {
            getline(file1, b, ',');
            getline(file1, c, ',');
            getline(file1, d, ',');
            getline(file1, e, ',');
            getline(file1, f, ',');
            getline(file1, g, ',');
            getline(file1, h, ',');
            getline(file1, i, ',');
            getline(file1, j, ',');
            getline(file1, k, '\n');

            e2 = &e[0];
            f2 = &f[0];

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
            else if (j == "Yes")
                j2 = true;

            if (k == "Unknown" || k == "Missing" || k == "No") {
                k2 = false;
            }
            else if (k == "Yes")
                k2 = true;

            covidMap.insertCase(cases(a,f2,e2,h2,i2,j2,k2));
        }
    } else
        cout << "cannot open file";

    //give menu options
}

/*int main() {                  //small hardcoded tester
    Map coronavirus;
    cases one("2020/03/23", '40-49', 'male', true, true, true, false);
    cases two("2020/08/04", '30-39', 'female', true, false, true, false);
    cases three("2020/04/16", '20-29', 'female', false, false, false, false);
    coronavirus.insertCase(one);
    coronavirus.insertCase(two);
    coronavirus.insertCase(three);
    cout << coronavirus.map["2020/03/23"][true].size();
    cout << coronavirus.map["2020/08/04"][true].size();
    cout << coronavirus.map["2020/04/16"][false].size();
    cout << "\n";
    cout << coronavirus.deathRateOverTime("2020/03/23", "2020/08/04");
    return 0;
}*/