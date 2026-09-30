#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

// Forward Declaration for Friend Concept
class AcademicAudit; 

// ==========================================
// 1. BASE CLASS - INHERITANCE (Week 7)
// ==========================================
class Person {
protected:
    string name;
    int id;

public:
    Person() {
        name = "";
        id = 0;
    }
    Person(string n, int i) {
        name = n;
        id = i;
    }

    // Virtual Function for Runtime Polymorphism (Week 10)
    virtual void displayDetails() {
        cout << "ID: " << id << " | Name: " << name << endl;
    }

    int getId() { return id; }
    string getName() { return name; }
    virtual ~Person() {} // Virtual Destructor (Week 5/10)
};

// ==========================================
// 2. DERIVED CLASS - STUDENT
// ==========================================
class Student : public Person {
private:
    float gpa;

public:
    Student() : Person() {
        gpa = 0.0;
    }
    // Calling Base Class Constructor
    Student(int r, string n, float g) : Person(n, r) {
        gpa = g;
    }

    float getGPA() { return gpa; }

    // Overriding the Virtual Function (Week 10)
    void displayDetails() override {
        cout << left << setw(10) << id 
             << setw(25) << name 
             << setw(5) << fixed << setprecision(2) << gpa << endl;
    }
};

// ==========================================
// 3. DERIVED CLASS - TEACHER
// ==========================================
class Teacher : public Person {
private:
    string subject;

public:
    Teacher() : Person() {
        subject = "";
    }
    Teacher(int tId, string n, string sub) : Person(n, tId) {
        subject = sub;
    }

    string getSubject() { return subject; }

    // Overriding the Virtual Function (Week 10)
    void displayDetails() override {
        cout << left << setw(10) << id 
             << setw(25) << name 
             << "Subject: " << subject << endl;
    }
};

// ==========================================
// 4. DEPARTMENT CLASS (Aggregation & Operator Overloading)
// ==========================================
class Department {
private:
    string deptName;
    int studentCount;
    
    // AGGREGATION (Week 12): Department HAS A Student array pointer
    Student* students[50]; 

public:
    Department() {
        deptName = "General";
        studentCount = 0;
    }
    
    Department(string name) {
        deptName = name;
        studentCount = 0;
    }

    string getDeptName() { return deptName; }
    int getStudentCount() { return studentCount; }

    // Aggregation Connector Logic
    void linkStudent(Student* s) {
        if (studentCount < 50) {
            students[studentCount] = s;
            studentCount++;
        }
    }

    void displayDeptDocs() {
        cout << "\nDepartment: " << deptName << " | Active Count: " << studentCount << endl;
        if(studentCount == 0) {
            cout << "(No active records mapped to this cluster)\n";
            return;
        }
        
        cout << "---------------------------------------------\n";
        cout << left << setw(10) << "Roll No" << setw(25) << "Name" << setw(5) << "GPA" << endl;
        cout << "---------------------------------------------\n";
        for (int i = 0; i < studentCount; i++) {
            // Polymorphic structure evaluation
            students[i]->displayDetails();
        }
        cout << "---------------------------------------------\n";
    }

    // OPERATOR OVERLOADING (Week 11): Overloading '+' to accumulate strength
    int operator + (const Department& d) {
        return this->studentCount + d.studentCount;
    }

    // Friend declarations (Week 11)
    friend void grandReport(Department& d1, Department& d2);
    friend class AcademicAudit;
};

// ==========================================
// 5. FRIEND FUNCTION IMPLEMENTATION
// ==========================================
void grandReport(Department& d1, Department& d2) {
    cout << "\n============== FRIEND FUNCTION SYSTEM AUDIT ==============\n";
    cout << "Accessing non-public operational metrics directly:\n";
    cout << "-> Class Cluster [" << d1.deptName << "] Strength: " << d1.studentCount << " nodes.\n";
    cout << "-> Class Cluster [" << d2.deptName << "] Strength: " << d2.studentCount << " nodes.\n";
    cout << "-> Operator Overloaded Evaluation Total: " << (d1 + d2) << " records checked.\n";
    cout << "==========================================================\n";
}

// ==========================================
// 6. FRIEND CLASS IMPLEMENTATION
// ==========================================
class AcademicAudit {
public:
    void verifyDepartmentIntegrity(Department& d) {
        cout << "\n[AUDIT] Extracting values from private structures for: " << d.deptName << endl;
        float totalGPA = 0;
        if (d.studentCount == 0) {
            cout << "[AUDIT STATUS] Idle state. No parameters loaded.\n";
            return;
        }
        for (int i = 0; i < d.studentCount; i++) {
            totalGPA += d.students[i]->getGPA();
        }
        cout << "[AUDIT STATUS] Operational stability confirmed. Target Avg: " 
             << (totalGPA / d.studentCount) << endl;
    }
};

// ==========================================
// 7. UNIVERSITY CLASS (Composition & Master System)
// ==========================================
class UniversitySystem {
private:
    string recordFile;
    const static int MAX_LIMIT = 100;
    
    // COMPOSITION (Week 12): Lifetime controlled internally
    Department csDept;
    Department seDept;

    // Runtime Dynamic Memory allocation mimic using physical allocation
    Student temporaryStorage[MAX_LIMIT];
    int totalSystemStudents;

public:
    UniversitySystem() : csDept("Computer Science"), seDept("Software Engineering") {
        recordFile = "master_university_data.txt";
        totalSystemStudents = 0;
        loadDataFromFile();
    }

    // 1. ADD / WRITE FUNCTION
    void addStudentToSystem() {
        int r, choice;
        string n, dept;
        float g;

        cout << "\n--- Enroll New Academic Unit ---\n";
        cout << "Enter Roll Number (ID): "; cin >> r;
        cin.ignore();
        cout << "Enter Name: "; getline(cin, n);
        cout << "Enter Calculated GPA: "; cin >> g;
        cout << "Select Department (1 for CS, 2 for SE): "; cin >> choice;

        if (choice == 1) dept = "CS";
        else dept = "SE";

        ofstream outFile(recordFile, ios::app);
        if (outFile.is_open()) {
            outFile << r << "," << n << "," << g << "," << dept << "\n";
            outFile.close();
            cout << "\nData successfully written to persistence pipeline.\n";
            loadDataFromFile(); // Hot reload memory structures
        } else {
            cout << "Critical I/O Failure: Cannot open storage pipeline.\n";
        }
    }

    // 2. READ & SYNC FUNCTION (Composition + Aggregation Wiring)
    void loadDataFromFile() {
        ifstream inFile(recordFile);
        if (!inFile.is_open()) return;

        totalSystemStudents = 0;
        csDept = Department("Computer Science");
        seDept = Department("Software Engineering");

        string rStr, n, gStr, dStr;
        while (getline(inFile, rStr, ',') && 
               getline(inFile, n, ',') && 
               getline(inFile, gStr, ',') && 
               getline(inFile, dStr)) {
            
            int r = stoi(rStr);
            float g = stof(gStr);

            if(totalSystemStudents < MAX_LIMIT) {
                temporaryStorage[totalSystemStudents] = Student(r, n, g);
                
                if (dStr == "CS") {
                    csDept.linkStudent(&temporaryStorage[totalSystemStudents]);
                } else if (dStr == "SE") {
                    seDept.linkStudent(&temporaryStorage[totalSystemStudents]);
                }
                totalSystemStudents++;
            }
        }
        inFile.close();
    }

    // 3. UPDATE FUNCTION (1st Sem Arrays + 2nd Sem File System Interface)
    void updateStudentRecord() {
        ifstream inFile(recordFile);
        if (!inFile.is_open()) {
            cout << "\nDatabase empty. Aborting process.\n";
            return;
        }

        int targetRoll;
        cout << "\nEnter Roll Number to Modify: "; cin >> targetRoll;

        int rollArr[MAX_LIMIT];
        string nameArr[MAX_LIMIT];
        float gpaArr[MAX_LIMIT];
        string deptArr[MAX_LIMIT];
        
        string rStr, n, gStr, dStr;
        int localCount = 0;
        bool matchingFlag = false;

        while (getline(inFile, rStr, ',') && getline(inFile, n, ',') && 
               getline(inFile, gStr, ',') && getline(inFile, dStr)) {
            
            int r = stoi(rStr);
            if (r == targetRoll) {
                matchingFlag = true;
                rollArr[localCount] = r;
                cin.ignore();
                cout << "\nRecord matched! Enter replacement variables:\n";
                cout << "New Name: "; getline(cin, nameArr[localCount]);
                cout << "New GPA: "; cin >> gpaArr[localCount];
                cout << "New Department Designation (CS/SE): "; cin >> deptArr[localCount];
            } else {
                rollArr[localCount] = r;
                nameArr[localCount] = n;
                gpaArr[localCount] = stof(gStr);
                deptArr[localCount] = dStr;
            }
            localCount++;
        }
        inFile.close();

        if (!matchingFlag) {
            cout << "Roll number pattern match unverified.\n";
            return;
        }

        // Flush and rebuild stream configuration
        ofstream outFile(recordFile, ios::trunc); 
        for (int i = 0; i < localCount; i++) {
            outFile << rollArr[i] << "," << nameArr[i] << "," << gpaArr[i] << "," << deptArr[i] << "\n";
        }
        outFile.close();
        cout << "\nData state synchronized and written successfully.\n";
        loadDataFromFile();
    }

    // 4. VIEW LOGISTICS
    void viewUniversityBlueprints() {
        cout << "\n=========================================\n";
        cout << "       CAMPUS STRUCTURAL DIAGRAMS        \n";
        cout << "=========================================\n";
        csDept.displayDeptDocs();
        seDept.displayDeptDocs();
    }

    // 5. RUN PERFORMANCE LOGS
    void deployAuditProtocols() {
        grandReport(csDept, seDept);
        AcademicAudit auditor;
        auditor.verifyDepartmentIntegrity(csDept);
        auditor.verifyDepartmentIntegrity(seDept);
    }
};

// ==========================================
// 8. MAIN MANAGEMENT FLOW CONTROL
// ==========================================
int main() {
    UniversitySystem lahoreUni; 
    int choice;

    do {
        cout << "\n==================================================\n";
        cout << "   ENTERPRISE CAMPUS RESOURCE MANAGEMENT SYSTEM   \n";
        cout << "==================================================\n";
        cout << "1. Create Student Entity File Record\n";
        cout << "2. Print Structural Map Metrics (Aggregation Layout)\n";
        cout << "3. Modify Existing Entity Index (Database Update)\n";
        cout << "4. Invoke Runtime Friends & Overloading Sub-routines\n";
        cout << "5. System Safemode Termination\n";
        cout << "--------------------------------------------------\n";
        cout << "Request Selector Menu (1-5): ";
        cin >> choice;

        switch (choice) {
            case 1: lahoreUni.addStudentToSystem(); break;
            case 2: lahoreUni.viewUniversityBlueprints(); break;
            case 3: lahoreUni.updateStudentRecord(); break;
            case 4: lahoreUni.deployAuditProtocols(); break;
            case 5: cout << "\nSystem flushing runtime states... Done.\n"; break;
            default: cout << "\nInvalid request stream handler error.\n";
        }
    } while (choice != 5);

    return 0;
}
