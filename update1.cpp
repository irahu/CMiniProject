#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// =====================================
// 🔹 ABSTRACT BASE CLASS (OOP: Abstraction)
// =====================================
class BaseSystem {
public:
    virtual void showMenu() = 0;
    virtual bool handleChoice(char choice) = 0;
    virtual ~BaseSystem() {}
};

// =====================================
// 🔹 LOGIN / SIGNUP SYSTEM
// =====================================
class UserSystem : public BaseSystem {
private:
    string username, email, password;
    fstream file;

public:
    void showMenu() override {
        cout << "\n========= USER MENU =========\n";
        cout << "1. Login\n";
        cout << "2. Sign Up\n";
        cout << "3. Forgot Password\n";
        cout << "4. Exit\n";
        cout << "=============================\n";
    }

    bool handleChoice(char choice) override;
    bool loginUser();
    void signUpUser();
    void recoverPassword();
};

// =====================================
// 🔹 STUDENT MANAGEMENT SYSTEM
// =====================================
class StudentSystem : public BaseSystem {
private:
    string roll, name, father, address, searchKey;
    fstream file;

public:
    void showMenu() override {
        cout << "\n========= STUDENT MENU =========\n";
        cout << "1. Add Student\n";
        cout << "2. View All Students\n";
        cout << "3. Search Student\n";
        cout << "4. Update Student\n";
        cout << "5. Delete Student\n";
        cout << "6. Logout\n";
        cout << "===============================\n";
    }

    bool handleChoice(char choice) override;

private:
    bool addStudent();
    bool viewStudents();
    bool searchStudent();
    bool updateStudent();
    bool deleteStudent();
};

// =====================================
// 🔹 SYSTEM MANAGER (Composition)
// =====================================
class SystemManager {
private:
    UserSystem user;
    StudentSystem student;

public:
    void start();
};

// =====================================
//  MAIN FUNCTION
// =====================================
int main() {
    SystemManager system;
    system.start();
    return 0;
}

// =====================================
// 🟢 USER SYSTEM IMPLEMENTATION
// =====================================
bool UserSystem::handleChoice(char choice) {
    switch (choice) {
        case '1': return loginUser();
        case '2': signUpUser(); break;
        case '3': recoverPassword(); break;
        case '4': cout << "Exiting...\n"; exit(0);
        default: cout << "Invalid option!\n";
    }
    return false;
}

bool UserSystem::loginUser() {
    string u, p;
    cout << "\nEnter Username: ";
    getline(cin, u);
    cout << "Enter Password: ";
    getline(cin, p);

    file.open("loginData.txt", ios::in);
    if (!file) {
        cout << "No user records found!\n";
        return false;
    }

    bool found = false;
    while (getline(file, username, '*')) {
        getline(file, email, '*');
        getline(file, password, '\n');
        if (username == u && password == p) {
            cout << "\n✅ Login Successful! Welcome, " << username << "!\n";
            found = true;
            break;
        }
    }
    file.close();
    if (!found) cout << "Invalid credentials!\n";
    return found;
}

void UserSystem::signUpUser() {
    cout << "\n--- Create New Account ---\n";
    cout << "Enter Username: ";
    getline(cin, username);
    cout << "Enter Email: ";
    getline(cin, email);
    cout << "Enter Password: ";
    getline(cin, password);

    file.open("loginData.txt", ios::out | ios::app);
    file << username << "*" << email << "*" << password << endl;
    file.close();

    cout << "\n Account Created Successfully!\n";
}

void UserSystem::recoverPassword() {
    string u, e;
    cout << "\n--- Password Recovery ---\n";
    cout << "Enter Username: ";
    getline(cin, u);
    cout << "Enter Email: ";
    getline(cin, e);

    file.open("loginData.txt", ios::in);
    if (!file) {
        cout << "No user records found!\n";
        return;
    }

    bool found = false;
    while (getline(file, username, '*')) {
        getline(file, email, '*');
        getline(file, password, '\n');
        if (username == u && email == e) {
            cout << "\n Account Found! Your Password: " << password << endl;
            found = true;
            break;
        }
    }
    file.close();
    if (!found) cout << " Account not found!\n";
}

// =====================================
// 🟣 STUDENT SYSTEM IMPLEMENTATION
// =====================================
bool StudentSystem::handleChoice(char choice) {
    switch (choice) {
        case '1': return addStudent();
        case '2': return viewStudents();
        case '3': return searchStudent();
        case '4': return updateStudent();
        case '5': return deleteStudent();
        case '6': cout << "Logging out...\n"; return true;
        default: cout << "Invalid option!\n";
    }
    return false;
}

bool StudentSystem::addStudent() {
    cout << "\n--- Add New Student ---\n";
    cout << "Roll No: "; getline(cin, roll);
    cout << "Name: "; getline(cin, name);
    cout << "Father's Name: "; getline(cin, father);
    cout << "Address: "; getline(cin, address);

    file.open("stuData.txt", ios::out | ios::app);
    file << roll << "*" << name << "*" << father << "*" << address << endl;
    file.close();

    cout << "✅ Student Added Successfully!\n";
    return true;
}

bool StudentSystem::viewStudents() {
    file.open("stuData.txt", ios::in);
    if (!file) {
        cout << "\nNo records found!\n";
    } else {
        cout << "\n--- Student Records ---\n";
        while (getline(file, roll, '*')) {
            getline(file, name, '*');
            getline(file, father, '*');
            getline(file, address, '\n');
            cout << "\nRoll: " << roll
                << "\nName: " << name
                << "\nFather: " << father
                << "\nAddress: " << address << endl;
    }
        file.close();
    }
    return true;
}

bool StudentSystem::searchStudent() {
    cout << "\nEnter Roll Number: ";
    getline(cin, searchKey);

    file.open("stuData.txt", ios::in);
    if (!file) {
        cout << "No records found!\n";
        return true;
    }

    bool found = false;
    while (getline(file, roll, '*')) {
        getline(file, name, '*');
        getline(file, father, '*');
        getline(file, address, '\n');
        if (roll == searchKey) {
            cout << "\n Record Found!\n";
            cout << "Roll: " << roll
                << "\nName: " << name
                << "\nFather: " << father
                << "\nAddress: " << address << endl;
            found = true;
            break;
        }
    }
    file.close();
    if (!found) cout << " Record Not Found!\n";
    return true;
}

bool StudentSystem::updateStudent() {
    cout << "\nEnter Roll Number to Update: ";
    getline(cin, searchKey);

    ifstream fin("stuData.txt");
    ofstream fout("temp.txt");
    bool found = false;

    while (getline(fin, roll, '*')) {
        getline(fin, name, '*');
        getline(fin, father, '*');
        getline(fin, address, '\n');

        if (roll == searchKey) {
            cout << "\nEnter New Name: "; getline(cin, name);
            cout << "Enter New Father Name: "; getline(cin, father);
            cout << "Enter New Address: "; getline(cin, address);
            found = true;
        }
        fout << roll << "*" << name << "*" << father << "*" << address << endl;
    }

    fin.close(); fout.close();
    remove("stuData.txt");
    rename("temp.txt", "stuData.txt");

    if (found)
        cout << " Record Updated Successfully!\n";
    else
        cout << " Record Not Found!\n";
    return true;
}

bool StudentSystem::deleteStudent() {
    cout << "\nEnter Roll Number to Delete: ";
    getline(cin, searchKey);

    ifstream fin("stuData.txt");
    ofstream fout("temp.txt");
    bool found = false;

    while (getline(fin, roll, '*')) {
        getline(fin, name, '*');
        getline(fin, father, '*');
        getline(fin, address, '\n');
        if (roll != searchKey)
            fout << roll << "*" << name << "*" << father << "*" << address << endl;
        else
            found = true;
    }

    fin.close(); fout.close();
    remove("stuData.txt");
    rename("temp.txt", "stuData.txt");

    if (found)
        cout << "Record Deleted!\n";
    else
        cout << " Record Not Found!\n";
    return true;
}

// =====================================
// 🧠 SYSTEM MANAGER IMPLEMENTATION
// =====================================
void SystemManager::start() {
    while (true) {
        user.showMenu();
        char choice;
        cout << "Enter choice: ";
        cin >> choice;
        cin.ignore();

        bool loggedIn = user.handleChoice(choice);
        if (loggedIn) {
            bool logout = false;
            while (!logout) {
                student.showMenu();
                cout << "Enter choice: ";
                cin >> choice;
                cin.ignore();
                logout = student.handleChoice(choice);
            }
        }
    }
}
