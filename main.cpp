#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <limits>
#include <cstdlib>
#include <cmath>
#include <algorithm>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

// Color codes
#ifdef _WIN32
void enableVirtualTerminalProcessing() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) return;
    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode)) return;
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
}

#define REDHB "\x1b[41m"
#define GRNHB "\x1b[42m"
#define RESET "\x1b[0m"
#else
#define REDHB "\e[0;101m"
#define GRNHB "\e[0;102m"
#define RESET "\e[0m"
#endif

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

int getIntInput(const string& prompt) {
    int value;
    cout << prompt;
    while (!(cin >> value)) {
        cout << REDHB << "Invalid input. Please enter a valid number: " << RESET;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return value;
}

float getFloatInput(const string& prompt) {
    float value;
    cout << prompt;
    while (!(cin >> value)) {
        cout << REDHB << "Invalid input. Please enter a valid number: " << RESET;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return value;
}

string getLineInput(const string& prompt) {
    string value;
    cout << prompt;
    getline(cin, value);
    return value;
}

void printBMIChangeFeedback(float oldBMI, float newBMI, float heightInMeters) {
    cout << "\nBMI Change Detected: ";

    if (newBMI > oldBMI) {
        cout << GRNHB << " BMI Increased " << RESET << endl;
    } else if (newBMI < oldBMI) {
        cout << GRNHB << " BMI Decreased " << RESET << endl;
    } else {
        cout << "No change." << endl;
        return;
    }

    cout << fixed << setprecision(2);
    cout << "Old BMI: " << oldBMI << ", New BMI: " << newBMI << endl;

    if (newBMI >= 18.5 && newBMI <= 24.9) {
        cout << GRNHB << "You are in the healthy weight range!" << RESET << endl;
    } else {
        float targetBMI = (newBMI < 18.5) ? 18.5 : 24.9;
        float targetWeight = targetBMI * (heightInMeters * heightInMeters);
        float currentWeight = newBMI * heightInMeters * heightInMeters;
        float weightDiff = targetWeight - currentWeight;

        cout << REDHB << "You are NOT in the healthy weight range!" << RESET << endl;
        if (weightDiff > 0) {
            cout << "You need to GAIN approximately " << fixed << setprecision(1) << weightDiff << " kg to reach a healthy BMI." << endl;
        } else {
            cout << "You need to LOSE approximately " << fixed << setprecision(1) << fabs(weightDiff) << " kg to reach a healthy BMI." << endl;
        }
    }
}

class UserData {
protected:
    string userName;
    float userWeight;
    string userHeight;
    int userAge;
    int maxBench;
    int maxSquat;
    int maxDeadlift;
    float bmi;
    int currentCalories;
    int recommendedCalories;
    vector<int> dailyCalorieLogs;
    float dailyWaterIntake;
    int dailySleepHours;
    string dietType; // New field for Veg/Non-Veg

public:
    UserData() : userWeight(0), userAge(0), maxBench(0), maxSquat(0), maxDeadlift(0),
                 bmi(0.0), currentCalories(0), recommendedCalories(0),
                 dailyWaterIntake(0), dailySleepHours(0) {}

    void setUserData(const string& name, float weight, const string& height, int age,
                     int bench, int squat, int deadlift, int calories, const string& diet) {
        userName = name;
        userWeight = weight;
        userHeight = height;
        userAge = age;
        maxBench = bench;
        maxSquat = squat;
        maxDeadlift = deadlift;
        currentCalories = calories;
        dietType = diet; // Set the user's diet type
        calculateBMI();
        calculateRecommendedCalories();
        displayProteinAndCalorieRecommendation();
    }

    float extractHeightInMeters(const string& heightStr) const {
        try {
            size_t pos = 0;
            int heightCM = stoi(heightStr, &pos);
            return heightCM / 100.0f;
        } catch (...) {
            cout << REDHB << "Invalid height format. Assuming 170cm." << RESET << endl;
            return 1.7f;
        }
    }

    void calculateBMI() {
        float heightInMeters = extractHeightInMeters(userHeight);
        if (heightInMeters > 0)
            bmi = userWeight / (heightInMeters * heightInMeters);
    }

    void calculateRecommendedCalories() {
        if (bmi < 18.5)
            recommendedCalories = currentCalories + 500;
        else if (bmi > 24.9)
            recommendedCalories = currentCalories - 500;
        else
            recommendedCalories = currentCalories;
    }

    void displayProteinAndCalorieRecommendation() {
        float proteinIntake;
        
        // Calculate protein intake based on activity level
        proteinIntake = userWeight * 1.2f; // Basic recommendation for sedentary
        if (dietType == "Non-Veg") {
            cout << "Recommended protein intake for non-vegetarian diet: " << proteinIntake << " grams\n";
        } else {
            cout << "Recommended protein intake for vegetarian diet: " << proteinIntake << " grams\n";
        }

        cout << "Recommended daily calorie intake: " << recommendedCalories << " calories\n";
    }

    void updateField();
    void logWaterAndSleep();
    void logTodayCalories();
    void displayUserData() const;
    string getUserName() const { return userName; }
    void saveToFile(ofstream& ofs) const;
    void loadFromFile(ifstream& ifs);
};

// Functions moved below to keep organized

class FitnessTracker {
private:
    vector<UserData> users;

public:
    void addUser(const UserData& user);
    void displayUsers() const;
    void updateUserByName(const string& name);
    void logWaterAndSleep(const string& name);
    void logCalories(const string& name);
    void saveToFile() const;
    void loadFromFile();
};

// Definitions

void UserData::updateField() {
    cout << "Select field to update:\n";
    cout << "1. Weight\n2. Height\n3. Age\n4. Max Bench\n5. Max Squat\n6. Max Deadlift\n7. Calorie Intake\n";
    int choice = getIntInput("Enter choice: ");

    switch (choice) {
        case 1: {
            float oldBMI = bmi;
            float oldWeight = userWeight;
            userWeight = getFloatInput("Enter new weight: ");
            calculateBMI();
            if (userWeight != oldWeight) {
                float heightMeters = extractHeightInMeters(userHeight);
                printBMIChangeFeedback(oldBMI, bmi, heightMeters);
            }
            break;
        }
        case 2:
            userHeight = getLineInput("Enter new height: ");
            calculateBMI();
            break;
        case 3:
            userAge = getIntInput("Enter new age: ");
            break;
        case 4:
            maxBench = getIntInput("Enter new max bench: ");
            break;
        case 5:
            maxSquat = getIntInput("Enter new max squat: ");
            break;
        case 6:
            maxDeadlift = getIntInput("Enter new max deadlift: ");
            break;
        case 7:
            currentCalories = getIntInput("Enter your new daily calorie intake: ");
            calculateRecommendedCalories();
            break;
        default:
            cout << REDHB << "Invalid choice." << RESET << endl;
    }
}

void UserData::logWaterAndSleep() {
    dailyWaterIntake = getFloatInput("Enter today's water intake (liters): ");
    dailySleepHours = getIntInput("Enter today's sleep hours: ");
    cout << GRNHB << "Water and sleep logged!" << RESET << endl;
}

void UserData::logTodayCalories() {
    vector<pair<string, float>> foodItems;
    float totalCalories = 0.0;

    while (true) {
        string food = getLineInput("Enter food item (or type 'done' to finish): ");
        if (food == "done") break;

        float grams = getFloatInput("Enter the grams of " + food + " you ate: ");
        float calories = getFloatInput("Enter the calories per 100g of " + food + ": ");
        totalCalories += (calories * grams) / 100;
    }

    cout << "Total calories for today: " << totalCalories << " kcal\n";
}

void UserData::displayUserData() const {
    cout << "User: " << userName << endl;
    cout << "Weight: " << userWeight << " kg\n";
    cout << "Height: " << userHeight << "\n";
    cout << "Age: " << userAge << " years\n";
    cout << "Max Bench: " << maxBench << " kg\n";
    cout << "Max Squat: " << maxSquat << " kg\n";
    cout << "Max Deadlift: " << maxDeadlift << " kg\n";
    cout << "BMI: " << bmi << endl;
    cout << "Daily Calories Intake: " << currentCalories << " kcal\n";
    cout << "Diet: " << dietType << endl;
}

void UserData::saveToFile(ofstream& ofs) const {
    ofs << userName << endl;
    ofs << userWeight << endl;
    ofs << userHeight << endl;
    ofs << userAge << endl;
    ofs << maxBench << endl;
    ofs << maxSquat << endl;
    ofs << maxDeadlift << endl;
    ofs << currentCalories << endl;
    ofs << dietType << endl;
}

void UserData::loadFromFile(ifstream& ifs) {
    if (ifs.is_open()) {
        getline(ifs, userName);
        ifs >> userWeight;
        ifs.ignore(); // Ignore newline
        getline(ifs, userHeight);
        ifs >> userAge;
        ifs >> maxBench;
        ifs >> maxSquat;
        ifs >> maxDeadlift;
        ifs >> currentCalories;
        ifs.ignore();
        getline(ifs, dietType);
    }
}

void FitnessTracker::addUser(const UserData& user) {
    users.push_back(user);
}

void FitnessTracker::displayUsers() const {
    if (users.empty()) {
        cout << "No users available.\n";
    } else {
        for (const auto& user : users) {
            user.displayUserData();
        }
    }
}

void FitnessTracker::updateUserByName(const string& name) {
    for (auto& user : users) {
        if (user.getUserName() == name) {
            user.updateField();
            return;
        }
    }
    cout << REDHB << "User not found!" << RESET << endl;
}

void FitnessTracker::logWaterAndSleep(const string& name) {
    for (auto& user : users) {
        if (user.getUserName() == name) {
            user.logWaterAndSleep();
            return;
        }
    }
    cout << REDHB << "User not found!" << RESET << endl;
}

void FitnessTracker::logCalories(const string& name) {
    for (auto& user : users) {
        if (user.getUserName() == name) {
            user.logTodayCalories();
            return;
        }
    }
    cout << REDHB << "User not found!" << RESET << endl;
}

void FitnessTracker::saveToFile() const {
    ofstream ofs("user_data.txt");
    for (const auto& user : users) {
        user.saveToFile(ofs);
    }
}

void FitnessTracker::loadFromFile() {
    ifstream ifs("user_data.txt");
    while (ifs.good()) {
        UserData user;
        user.loadFromFile(ifs);
        if (!user.getUserName().empty()) {
            addUser(user);
        }
    }
}

int main() {
    FitnessTracker tracker;
    tracker.loadFromFile();

    while (true) {
        clearScreen();
        cout << "Fitness Tracker Menu\n";
        cout << "1. Add User\n";
        cout << "2. Display All Users\n";
        cout << "3. Update User\n";
        cout << "4. Log Water and Sleep\n";
        cout << "5. Log Calories\n";
        cout << "6. Save Data\n";
        cout << "7. Exit\n";
        int choice = getIntInput("Choose an option: ");

        switch (choice) {
            case 1: {
                string name = getLineInput("Enter your name: ");
                float weight = getFloatInput("Enter your weight (kg): ");
                string height = getLineInput("Enter your height (cm): ");
                int age = getIntInput("Enter your age: ");
                int bench = getIntInput("Enter your max bench press (kg): ");
                int squat = getIntInput("Enter your max squat (kg): ");
                int deadlift = getIntInput("Enter your max deadlift (kg): ");
                int calories = getIntInput("Enter your daily calorie intake: ");
                string diet = getLineInput("Enter your diet type (Veg/Non-Veg): ");

                UserData user;
                user.setUserData(name, weight, height, age, bench, squat, deadlift, calories, diet);
                tracker.addUser(user);
                break;
            }
            case 2:
                tracker.displayUsers();
                break;
            case 3: {
                string name = getLineInput("Enter the name of the user to update: ");
                tracker.updateUserByName(name);
                break;
            }
            case 4: {
                string name = getLineInput("Enter the name of the user to log water and sleep for: ");
                tracker.logWaterAndSleep(name);
                break;
            }
            case 5: {
                string name = getLineInput("Enter the name of the user to log calories for: ");
                tracker.logCalories(name);
                break;
            }
            case 6:
                tracker.saveToFile();
                cout << GRNHB << "Data saved successfully!" << RESET << endl;
                break;
            case 7:
                tracker.saveToFile();
                return 0;
            default:
                cout << REDHB << "Invalid option, try again!" << RESET << endl;
        }
        system("pause");
    }

    return 0;
}