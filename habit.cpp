#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <iomanip>
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
    Habit() {}
    Habit(string name, int targetStreak) {
        this->id = ++nextId;
        this->name = name;
        this->targetStreak = targetStreak;
        this->currentStreak = 0;
        this->totalProgress = 0;
    }

    int getId() const { return id; }
    string getName() const { return name; }
    int getTargetStreak() const { return targetStreak; }
    int getCurrentStreak() const { return currentStreak; }
    int getTotalProgress() const { return totalProgress; }

    void markProgress() {
        currentStreak++;
        totalProgress++;
        if (currentStreak == targetStreak) {
            cout << "\nCongratulations! You've completed the streak for habit: " << name << "!" << endl;
            currentStreak = 0;
        }
    }

    friend ostream &operator<<(ostream &os, const Habit &habit);
    friend ofstream &operator<<(ofstream &ofs, const Habit &habit);
    friend ifstream &operator>>(ifstream &ifs, Habit &habit);
};

int Habit::nextId = 0;

ostream &operator<<(ostream &os, const Habit &habit) {
    os << "Habit ID: " << habit.id << "\n"
       << "Name: " << habit.name << "\n"
       << "Target Streak: " << habit.targetStreak << "\n"
       << "Current Streak: " << habit.currentStreak << "\n"
       << "Total Progress: " << habit.totalProgress << "\n";
    return os;
}

ofstream &operator<<(ofstream &ofs, const Habit &habit) {
    ofs << habit.id << '\n'
        << habit.name << '\n'
        << habit.targetStreak << '\n'
        << habit.currentStreak << '\n'
        << habit.totalProgress << '\n';
    return ofs;
}

ifstream &operator>>(ifstream &ifs, Habit &habit) {
    ifs >> habit.id;
    ifs.ignore();
    getline(ifs, habit.name);
    ifs >> habit.targetStreak >> habit.currentStreak >> habit.totalProgress;
    return ifs;
}

class HabitTracker {
private:
    map<int, Habit> habits;

public:
    HabitTracker() {
        loadHabits();
    }

    void addHabit(const string &name, int targetStreak) {
        Habit newHabit(name, targetStreak);
        habits[newHabit.getId()] = newHabit;
        saveHabits();
        cout << "\nHabit added successfully!" << endl;
    }

    void markProgress(int id) {
        if (habits.find(id) != habits.end()) {
            habits[id].markProgress();
            saveHabits();
        } else {
            cout << "\nHabit not found!" << endl;
        }
    }

    void viewHabits() const {
        if (habits.empty()) {
            cout << "\nNo habits to display." << endl;
        } else {
            for (const auto &entry : habits) {
                cout << entry.second << endl;
            }
        }
    }

    void deleteHabit(int id) {
        if (habits.erase(id)) {
            saveHabits();
            cout << "\nHabit deleted successfully." << endl;
        } else {
            cout << "\nHabit not found!" << endl;
        }
    }

    void weeklySummary() const {
        if (habits.empty()) {
            cout << "\nNo habits to summarize." << endl;
        } else {
            cout << "\nWeekly Progress Summary:\n";
            for (const auto &entry : habits) {
                const Habit &habit = entry.second;
                cout << "Habit: " << habit.getName() << "\n"
                     << "Total Progress: " << habit.getTotalProgress() << "\n"
                     << "Current Streak: " << habit.getCurrentStreak() << "\n";
                cout << "-----------------------------\n";
            }
        }
    }

    void saveHabits() const {
        ofstream ofs("habits.data", ios::trunc);
        for (const auto &entry : habits) {
            ofs << entry.second;
        }
        ofs.close();
    }

    void loadHabits() {
        ifstream ifs("habits.data");
        if (!ifs) return;

        Habit habit;
        while (ifs >> habit) {
            habits[habit.getId()] = habit;
        }
        ifs.close();
    }
};

int main() {
    HabitTracker tracker;
    int choice;

    do {
        cout << "\n*** Console Habit Tracker ***\n";
        cout << "1. Add Habit\n";
        cout << "2. Mark Progress\n";
        cout << "3. View Habits\n";
        cout << "4. Delete Habit\n";
        cout << "5. Weekly Summary\n";
        cout << "6. Quit\n";
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
                cout << "\nExiting" << endl;
                break;
            default:
                cout << "\nInvalid choice. Please try again." << endl;
        }
    } while (choice != 6);

    return 0;
}
