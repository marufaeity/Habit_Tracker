#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <memory>
using namespace std;

class Habit {
private:
    int id;
    string name;
    int targetStreak;
    int currentStreak;
    int totalProgress;
    static int nextId;

public:
    Habit() : id(0), name(""), targetStreak(0), currentStreak(0), totalProgress(0) {}
    Habit(string name, int targetStreak);
    int getId() const { return id; }
    string getName() const { return name; }
    int getTargetStreak() const { return targetStreak; }
    int getCurrentStreak() const { return currentStreak; }
    int getTotalProgress() const { return totalProgress; }
    void markProgress();
    friend ofstream &operator<<(ofstream &ofs, const Habit &habit);
    friend ifstream &operator>>(ifstream &ifs, Habit &habit);
    friend ostream &operator<<(ostream &os, const Habit &habit);
};

int Habit::nextId = 0;

Habit::Habit(string name, int targetStreak) {
    id = ++nextId;
    this->name = name;
    this->targetStreak = targetStreak;
    this->currentStreak = 0;
    this->totalProgress = 0;
}

void Habit::markProgress() {
    currentStreak++;
    totalProgress++;
    if (currentStreak == targetStreak) {
        cout << "\nCongratulations! You completed the streak for habit: " << name << "!\n";
        currentStreak = 0;
    }
}

ofstream &operator<<(ofstream &ofs, const Habit &habit) {
    ofs << habit.id << '\n' << habit.name << '\n' << habit.targetStreak << '\n'
        << habit.currentStreak << '\n' << habit.totalProgress << '\n';
    return ofs;
}

ifstream &operator>>(ifstream &ifs, Habit &habit) {
    ifs >> habit.id;
    ifs.ignore();
    getline(ifs, habit.name);
    ifs >> habit.targetStreak >> habit.currentStreak >> habit.totalProgress;
    return ifs;
}

ostream &operator<<(ostream &os, const Habit &habit) {
    os << "Habit ID: " << habit.id << "\nName: " << habit.name << "\nTarget Streak: "
       << habit.targetStreak << "\nCurrent Streak: " << habit.currentStreak
       << "\nTotal Progress: " << habit.totalProgress << "\n";
    return os;
}

class HabitTracker {
private:
    map<int, unique_ptr<Habit>> habits;
    void saveHabits() const;
    void loadHabits();

public:
    HabitTracker();
    void addHabit(string name, int targetStreak);
    void markProgress(int id);
    void viewHabits() const;
    void deleteHabit(int id);
    void weeklySummary() const;
};

HabitTracker::HabitTracker() {
    loadHabits();
}

void HabitTracker::addHabit(string name, int targetStreak) {
    auto newHabit = make_unique<Habit>(name, targetStreak);
    habits[newHabit->getId()] = move(newHabit);
    saveHabits();
    cout << "\nHabit added successfully!\n";
}

void HabitTracker::markProgress(int id) {
    if (habits.find(id) != habits.end()) {
        habits[id]->markProgress();
        saveHabits();
    } else {
        cout << "\nHabit not found!\n";
    }
}

void HabitTracker::viewHabits() const {
    if (habits.empty()) {
        cout << "\nNo habits to display.\n";
    } else {
        for (const auto &entry : habits) {
            cout << *(entry.second) << "\n";
        }
    }
}

void HabitTracker::deleteHabit(int id) {
    if (habits.erase(id)) {
        saveHabits();
        cout << "\nHabit deleted successfully.\n";
    } else {
        cout << "\nHabit not found!\n";
    }
}

void HabitTracker::weeklySummary() const {
    cout << "\nWeekly Progress Summary:\n";
    for (const auto &entry : habits) {
        cout << "Habit: " << entry.second->getName() << "\nTotal Progress: "
             << entry.second->getTotalProgress() << "\nCurrent Streak: "
             << entry.second->getCurrentStreak() << "\n-----------------------------\n";
    }
}

void HabitTracker::saveHabits() const {
    ofstream ofs("habits.data", ios::trunc);
    if (!ofs) {
        cerr << "Error: Unable to save habits to file.\n";
        return;
    }
    for (const auto &entry : habits) {
        ofs << *(entry.second);
    }
    ofs.close();
}

void HabitTracker::loadHabits() {
    ifstream ifs("habits.data");
    if (!ifs) {
        cerr << "Error: Unable to load habits from file.\n";
        return;
    }
    Habit habit;
    while (ifs >> habit) {
        habits[habit.getId()] = make_unique<Habit>(habit);
    }
    ifs.close();
}

int main() {
    HabitTracker tracker;
    int choice;

    do {
        cout << "\n*** Console Habit Tracker ***\n";
        cout << "1. Add Habit\n2. Mark Progress\n3. View Habits\n4. Delete Habit\n5. Weekly Summary\n6. Quit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                string name;
                int target;
                cout << "\nEnter habit name: ";
                cin.ignore();
                getline(cin, name);
                cout << "Enter target streak: ";
                cin >> target;
                tracker.addHabit(name, target);
                break;
            }
            case 2: {
                int id;
                cout << "\nEnter habit ID to mark progress: ";
                cin >> id;
                tracker.markProgress(id);
                break;
            }
            case 3:
                tracker.viewHabits();
                break;
            case 4: {
                int id;
                cout << "\nEnter habit ID to delete: ";
                cin >> id;
                tracker.deleteHabit(id);
                break;
            }
            case 5:
                tracker.weeklySummary();
                break;
            case 6:
                cout << "\nExiting...\n";
                break;
            default:
                cout << "\nInvalid choice. Try again.\n";
        }
    } while (choice != 6);

    return 0;
}
