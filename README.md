# Task1
Record Management System

# Book Record System (C++ CLI)

A simple, menu-driven C++ application to manage book records. The system uses binary file storage to persist data and supports basic CRUD operations, sorting, and exporting to CSV.

---

## Features

- Add new book records
- Display all stored books
- Search for a book by ID
- Update a book’s details
- Delete a book record
- Sort records by title or publication year
- Export records to a CSV file
- Input validation and basic error handling

---

## Book Structure

Each book record contains:

- `ID` (string): Unique identifier
- `Title` (string)
- `Author` (string)
- `Year` (int): Year of publication

---

## How to Compile

### Using `g++`:

```bash
g++ -o book_system main.cpp
./book_system
