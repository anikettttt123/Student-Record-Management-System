#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

// Student struct to store student information
struct Student {
    string name;
    int rollNumber;
    vector<pair<string, double>> courseGrades; // Course name and grade
    vector<bool> attendance; // Attendance record for each course
};

// Vector to store all student records
vector<Student> students;

// Function prototypes
void addStudent();
void displayStudentRecords();
void updateStudentRecord();
void deleteStudentRecord();
void saveToFile();
void loadFromFile();
double calculateGPA(const Student& student);

int main() {
    int choice;
    char cont;

    do {
        cout << "\n\t\t\tStudent Record Management System\n";
        cout << "\t\t\t----------------------------------\n";
        cout << "\n\t1. Add Student Record";
        cout << "\n\t2. Display Student Records";
        cout << "\n\t3. Update Student Record";
        cout << "\n\t4. Delete Student Record";
        cout << "\n\t5. Save Records to File";
        cout << "\n\t6. Load Records from File";
        cout << "\n\t7. Exit";
        cout << "\n\n\tEnter your choice (1-7): ";
        cin >> choice;

        switch (choice) {
            case 1:
                addStudent();
                break;
            case 2:
                displayStudentRecords();
                break;
            case 3:
                updateStudentRecord();
                break;
            case 4:
                deleteStudentRecord();
                break;
            case 5:
                saveToFile();
                break;
            case 6:
                loadFromFile();
                break;
            case 7:
                cout << "\n\tThank you for using the system.\n";
                break;
            default:
                cout << "\n\tInvalid choice! Try again.\n";
        }

        cout << "\n\tDo you want to continue? (y/n): ";
        cin >> cont;
    } while (cont == 'y' || cont == 'Y');

    return 0;
}

// Function to add a new student record
void addStudent() {
    Student newStudent;

    cout << "\nEnter student name: ";
    cin.ignore();
    getline(cin, newStudent.name);

    cout << "Enter roll number: ";
    cin >> newStudent.rollNumber;

    int numCourses;
    cout << "Enter the number of courses: ";
    cin >> numCourses;

    for (int i = 0; i < numCourses; i++) {
        string courseName;
        double grade;
        bool attendance;

        cout << "Enter course " << i + 1 << " name: ";
        cin.ignore();
        getline(cin, courseName);

        cout << "Enter grade for " << courseName << ": ";
        cin >> grade;

        cout << "Enter attendance for " << courseName << " (1 for present, 0 for absent): ";
        cin >> attendance;

        newStudent.courseGrades.push_back(make_pair(courseName, grade));
        newStudent.attendance.push_back(attendance);
    }

    students.push_back(newStudent);
    cout << "\nStudent record added successfully!\n";
}

// Function to display all student records
void displayStudentRecords() {
    if (students.empty()) {
        cout << "\nNo student records found.\n";
        return;
    }

    cout << "\n\t\t\tStudent Records\n";
    cout << "\t\t\t----------------\n";

    for (const auto& student : students) {
        cout << "\nName: " << student.name << "\nRoll Number: " << student.rollNumber << "\n\n";
        cout << "Courses and Grades:\n";
        cout << left << setw(20) << "Course" << setw(10) << "Grade" << setw(10) << "Attendance" << "\n";

        for (int i = 0; i < student.courseGrades.size(); i++) {
            cout << left << setw(20) << student.courseGrades[i].first
                 << setw(10) << student.courseGrades[i].second
                 << setw(10) << (student.attendance[i] ? "Present" : "Absent") << "\n";
        }

        cout << "\nGPA: " << fixed << setprecision(2) << calculateGPA(student) << "\n\n";
    }
}

// Function to update a student record
void updateStudentRecord() {
    int rollNumber;
    cout << "\nEnter roll number of the student to update: ";
    cin >> rollNumber;

    auto it = find_if(students.begin(), students.end(), [rollNumber](const Student& student) {
        return student.rollNumber == rollNumber;
    });

    if (it == students.end()) {
        cout << "\nStudent record not found.\n";
        return;
    }

    Student& student = *it;

    cout << "\nEnter new student name (or press Enter to keep the current name): ";
    cin.ignore();
    string newName;
    getline(cin, newName);
    if (!newName.empty()) {
        student.name = newName;
    }

    int numCourses;
    cout << "Enter the number of courses (or 0 to keep the current courses): ";
    cin >> numCourses;

    if (numCourses > 0) {
        student.courseGrades.clear();
        student.attendance.clear();

        for (int i = 0; i < numCourses; i++) {
            string courseName;
            double grade;
            bool attendance;

            cout << "Enter course " << i + 1 << " name: ";
            cin.ignore();
            getline(cin, courseName);

            cout << "Enter grade for " << courseName << ": ";
            cin >> grade;

            cout << "Enter attendance for " << courseName << " (1 for present, 0 for absent): ";
            cin >> attendance;

            student.courseGrades.push_back(make_pair(courseName, grade));
            student.attendance.push_back(attendance);
        }
    }

    cout << "\nStudent record updated successfully!\n";
}

// Function to delete a student record
void deleteStudentRecord() {
    int rollNumber;
    cout << "\nEnter roll number of the student to delete: ";
    cin >> rollNumber;

    auto it = find_if(students.begin(), students.end(), [rollNumber](const Student& student) {
        return student.rollNumber == rollNumber;
    });

    if (it == students.end()) {
        cout << "\nStudent record not found.\n";
        return;
    }

    students.erase(it);
    cout << "\nStudent record deleted successfully!\n";
}

// Function to save student records to a file
void saveToFile() {
    ofstream file("student_records.txt");

    if (!file) {
        cout << "\nError opening file for writing.\n";
        return;
    }

    for (const auto& student : students) {
        file << student.name << "," << student.rollNumber << ",";

        for (int i = 0; i < student.courseGrades.size(); i++) {
            file << student.courseGrades[i].first << "," << student.courseGrades[i].second << ","
                 << student.attendance[i];

            if (i < student.courseGrades.size() - 1) {
                file << "|";
            }
        }

        file << "\n";
    }

    file.close();
    cout << "\nStudent records saved to file successfully!\n";
}

// Function to load student records from a file
void loadFromFile() {
    ifstream file("student_records.txt");

    if (!file) {
        cout << "\nError opening file for reading.\n";
        return;
    }

    students.clear();

    string line;
    while (getline
