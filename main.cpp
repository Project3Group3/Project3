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
    string age;          //group     0-9,10-19,...80+
    string sex;                   //male, female, other, missing
    bool hosp;                  //true if hosp, false if not or missing
    bool icu;                   //""
    bool death;                 //true if death, false otherwise
    bool medcond;               //true if has underlying condition
public:
    cases(string d, string a, string s, bool h, bool i, bool dt, bool m) {
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
    string getAge();
    string getSex();
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
string cases::getAge() {
    return age;
}
string cases::getSex() {
    return sex;
}

int dateSubtraction(string date1, string date2) {       //hacky because doesnt take year into account
    int month1 = stoi(date1.substr(5,2));
    int day1 = stoi(date1.substr(8,2));
    int month2 = stoi(date2.substr(5,2));
    int day2 = stoi(date2.substr(8,2));
    if (month1 > month2) {
        swap(month1, month2);
        swap(day1, day2);
    }
    else if (day1 > day2)
        swap(day1, day2);

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
                date2 = "2020/10/16";                                                       //hardcoded end date NOTE
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
        }
    }
    while (runLoop);
}