#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <string>
using namespace std;

struct Book {
    string id;
    string title;
    string author;
    int year;
};

const string DATA_FILE = "books.dat";

void saveRecords(const vector<Book>& records) {
    ofstream out(DATA_FILE, ios::binary | ios::trunc);
    if (!out) {
        cerr << "Error opening file for writing.\n";
        return;
    }
    size_t size = records.size();
    out.write(reinterpret_cast<const char*>(&size), sizeof(size));
    for (const auto& book : records) {
        size_t len;
        len = book.id.size(); out.write(reinterpret_cast<const char*>(&len), sizeof(len)); out.write(book.id.c_str(), len);
        len = book.title.size(); out.write(reinterpret_cast<const char*>(&len), sizeof(len)); out.write(book.title.c_str(), len);
        len = book.author.size(); out.write(reinterpret_cast<const char*>(&len), sizeof(len)); out.write(book.author.c_str(), len);
        out.write(reinterpret_cast<const char*>(&book.year), sizeof(book.year));
    }
    out.close();
}

vector<Book> loadRecords() {
    vector<Book> records;
    ifstream in(DATA_FILE, ios::binary);
    if (!in) return records; // No file yet
    size_t size;
    in.read(reinterpret_cast<char*>(&size), sizeof(size));
    for (size_t i = 0; i < size; ++i) {
        Book book;
        size_t len;
        char buffer[1024];

        in.read(reinterpret_cast<char*>(&len), sizeof(len)); in.read(buffer, len); book.id.assign(buffer, len);
        in.read(reinterpret_cast<char*>(&len), sizeof(len)); in.read(buffer, len); book.title.assign(buffer, len);
        in.read(reinterpret_cast<char*>(&len), sizeof(len)); in.read(buffer, len); book.author.assign(buffer, len);
        in.read(reinterpret_cast<char*>(&book.year), sizeof(book.year));

        records.push_back(book);
    }
    in.close();
    return records;
}

void addRecord() {
    vector<Book> records = loadRecords();
    Book book;
    cout << "Enter ID: "; cin >> ws; getline(cin, book.id);
    for (const auto& b : records)
        if (b.id == book.id) {
            cout << "ID already exists.\n"; return;
        }
    cout << "Enter Title: "; getline(cin, book.title);
    cout << "Enter Author: "; getline(cin, book.author);
    cout << "Enter Year: "; cin >> book.year;
    records.push_back(book);
    saveRecords(records);
    cout << "Book added.\n";
}

void displayAllRecords() {
    vector<Book> records = loadRecords();
    if (records.empty()) {
        cout << "No records found.\n"; return;
    }
    cout << left << setw(10) << "ID" << setw(30) << "Title" << setw(25) << "Author" << "Year\n";
    for (const auto& book : records) {
        cout << left << setw(10) << book.id << setw(30) << book.title << setw(25) << book.author << book.year << '\n';
    }
}

void searchRecordByID() {
    string id;
    cout << "Enter ID to search: "; cin >> ws; getline(cin, id);
    vector<Book> records = loadRecords();
    for (const auto& book : records) {
        if (book.id == id) {
            cout << "Found: " << book.title << " by " << book.author << " (" << book.year << ")\n";
            return;
        }
    }
    cout << "Record not found.\n";
}

void deleteRecord() {
    string id;
    cout << "Enter ID to delete: "; cin >> ws; getline(cin, id);
    vector<Book> records = loadRecords();
    auto it = remove_if(records.begin(), records.end(), [&](const Book& b){ return b.id == id; });
    if (it == records.end()) {
        cout << "Record not found.\n";
    } else {
        records.erase(it, records.end());
        saveRecords(records);
        cout << "Record deleted.\n";
    }
}

void updateRecord() {
    string id;
    cout << "Enter ID to update: "; cin >> ws; getline(cin, id);
    vector<Book> records = loadRecords();
    for (auto& book : records) {
        if (book.id == id) {
            cout << "Editing " << book.title << " by " << book.author << " (" << book.year << ")\n";
            string input;
            cout << "New Title (blank to keep): "; getline(cin, input); if (!input.empty()) book.title = input;
            cout << "New Author (blank to keep): "; getline(cin, input); if (!input.empty()) book.author = input;
            cout << "New Year (0 to keep): "; int newYear; cin >> newYear; if (newYear != 0) book.year = newYear;
            saveRecords(records);
            cout << "Record updated.\n";
            return;
        }
    }
    cout << "Record not found.\n";
}

void sortRecords() {
    vector<Book> records = loadRecords();
    if (records.empty()) {
        cout << "No records to sort.\n"; return;
    }
    int choice;
    cout << "Sort by (1) Title or (2) Year? ";
    cin >> choice;
    if (choice == 1) {
        sort(records.begin(), records.end(), [](const Book& a, const Book& b) {
            return a.title < b.title;
        });
    } else if (choice == 2) {
        sort(records.begin(), records.end(), [](const Book& a, const Book& b) {
            return a.year < b.year;
        });
    } else {
        cout << "Invalid choice.\n"; return;
    }
    for (const auto& book : records) {
        cout << book.id << " | " << book.title << " | " << book.author << " | " << book.year << '\n';
    }
}

void exportToCSV() {
    vector<Book> records = loadRecords();
    ofstream out("books.csv");
    if (!out) {
        cerr << "Error creating CSV file.\n"; return;
    }
    out << "ID,Title,Author,Year\n";
    for (const auto& book : records) {
        out << '"' << book.id << "\",\"" << book.title << "\",\"" << book.author << "\"," << book.year << "\n";
    }
    cout << "Exported to books.csv\n";
}

void menu() {
    while (true) {
        cout << "\n--- Book Record System ---\n";
        cout << "1. Add Record\n2. Display All Records\n3. Search by ID\n4. Delete Record\n";
        cout << "5. Update Record\n6. Sort Records\n7. Export to CSV\n8. Exit\n";
        cout << "Choose an option: ";
        int choice; cin >> choice;
        switch (choice) {
            case 1: addRecord(); break;
            case 2: displayAllRecords(); break;
            case 3: searchRecordByID(); break;
            case 4: deleteRecord(); break;
            case 5: updateRecord(); break;
            case 6: sortRecords(); break;
            case 7: exportToCSV(); break;
            case 8: cout << "Exiting...\n"; return;
            default: cout << "Invalid choice.\n";
        }
    }
}

int main() {
    menu();
    return 0;
}
//
// Created by X on 13. 7. 2025.
//
