# Library Management System (LMS)

## Introduction
The **Library Management System (LMS)** is a C++ application built using Object-Oriented Programming (OOP) principles to manage library operations such as book borrowing, returning, reserving, user management, fine payments, and more. The system supports different types of users (Students, Faculty, Librarians) and maintains persistence through file handling.

## Features
- **User Authentication**: Users can enter their User ID to access the system.
- **User Management**:
  - Add new users (Students, Faculty, Librarians)
  - Edit user details (Name, Age)
  - Remove users
- **Book Management**:
  - Add new books
  - Remove books
  - View available books
- **Borrowing & Returning**:
  - Borrow a book
  - Return a book
  - Reserve a book if unavailable (Queue-based system for reservations)
- **Fine Management**:
  - View outstanding fines
  - Pay fines using balance or additional payment
- **Data Persistence**:
  - Save and load books and users from files **(books.txt,users.txt)**
  - Saves each and every transaction happening in the library **(transactions.txt)**
  - Saves the event of borrowing and returning in order **(bookCirculation.txt)**
  - Saves currently borrowed books which are yet to be returned **(listOfCurrentlyBorrowed.txt)**
  - Clear all records if required

## Disclaimer
- **You might see some 🔴 error messages displayed when you run the program**
- **They are meant to be shown. Reason**:
  - You can't add the same user or book if it is already in the system
  - When you load the data at the beginning of the program, you load all the users and books into the database of the library.
  - Now, when Filch tries to add those users and books again, those error messages are printed to just confirm the fact that loadState() worked correctly 
- **Having those ADD statements are useful if we are not loading the books and users, and won't give any error then**

## File Structure
```
📂 C++ Assignment  
│── 📂 Library Records  
│   ├── 📄 books.txt                   # Persistent storage for books  
│   ├── 📄 users.txt                   # Persistent storage for users  
│   ├── 📄 transactions.txt            # Records of payments made  
│   ├── 📄 bookCirculation.txt         # Lists borrowed and returned books
│   ├── 📄 listOfCurrentlyBorrowed.txt # Tracks curently borrowed books
│  
├── 📄 Library.cpp            # Library class  
├── 📄 LMS.cpp                # Entry point of the application
├── 📄 Account.cpp            # Manages user transactions  
├── 📄 Book.cpp               # Manages book details & reservations  
├── 📄 User.cpp               # Base class for users  
├── 📄 Student.cpp            # Derived class for students  
├── 📄 Faculty.cpp            # Derived class for faculty members  
├── 📄 Librarian.cpp          # Derived class for librarians  
│  
├── 📄 Makefile               # Compilation instructions  
├── 📄 README.md              # Project documentation  
└── 📄 classes.h              # Header file for class definitions  

```

## Installation & Compilation
### **Prerequisites**
- C++20 or later
- GNU Make (for compiling using `Makefile`)

### **Compilation**
Run the following command to build the program:
```sh
make
```

### **Running the Application**
```sh
./library
```

## Usage
1. Run the program.
2. Enter your User ID or type `help` for assistance.
3. Navigate through the menu options for various operations.
4. type `exit` to exit the program and auto-save data.

## Example User Interface
```
=======================================================
📚✨  WELCOME TO THE HOGWARTS LIBRARY (ESTD.2005)  ✨📚
=======================================================

1. Do you want to start Interactive Interface for a user
                    OR
2. Do you want to execute the test commands?
1
🔹 Please Enter your User ID
🔹 Type 'help' if you don't know your ID
🔹 Type 'exit' to exit
->S1

Opening the Menu for Harry Potter......


📚 Menu for the Student Harry Potter 📚
1. Borrow Book
2. Return Book
3. Reserve Book
4. Edit Profile
5. Fine Management
6. Future
7. Exit
Enter choice(1-7): 6
Current Day Count = 0
How many days in the future do you want to go?
Days: 12


⚡⚡TIME TRAVEL IN PROGRESS⚡⚡

TIME TRAVEL SUCCESSFUL⏳⏩⌛
Now Day Count = 12

📚 Menu for the Student Harry Potter 📚
1. Borrow Book
2. Return Book
3. Reserve Book
4. Edit Profile
5. Fine Management
6. Future
7. Exit
Enter choice(1-7):
```


## File Handling
### **Saving Users**
```cpp
void Library::saveUsers() {
   ofstream file("LibraryRecords/users.txt");
   if (!file) {
      cout << "🔴 Error opening file for writing!" << endl;
      return;
  }
   for (auto &[user,_] : UsersRole) {
      file<<user->getUID()<<" "<<user->getAge()<<endl<<user->getName()<<endl;
   }//name in new line as it could have multiple words
   file.close();
   cout<<"Saved the Users\n";
}
```

### **Loading Users**
```cpp
void Library::loadUsers() {
   ifstream file("LibraryRecords/users.txt");
   if (!file) {
      cout << "🔴 Error opening file for reading!" << endl;
      return;
  }
   User* user;
   string uid,n;int age;
   while (file >> uid >> age) {
      file.ignore();// V.IMP-->Ignore the newline left by previous extraction
      getline(file,n);
      user = findUserByUID(uid);
      if (!user){
         switch(uid[0]){
            case 'S': addUser(new Student(uid, n, age));break;
            case 'F': addUser(new Faculty(uid, n, age));break;
            case 'L': addUser(new Librarian(uid, n, age));break;
         }
      }else addUser(user);
   }
   file.close();
   cout<<"Loaded the Users\n";
}
```

## Class Structure
### **User (Base Class)**
```cpp
class User{
   protected:
      string UID,name;
      int age;
   public:
      Account* account;
      string getName() const;
      int getAge() const;
      string getUID() const;
      bool operator<(const User& other) const;
      virtual void borrowBook(Book& book) = 0;
      virtual void returnBook(Book& book) = 0;
      virtual void reserveBook(Book& book) = 0;
      User(string,string,int);
      User(const User&);
      virtual void menu()=0;
      void editProfile();
      virtual void borrowBook()=0;
      virtual void returnBook()=0;
      virtual void reserveBook()=0;
      virtual ~User();
};
```

### **Derived Classes**
```cpp
class Student : public User{
   int borrowedBooks=0;
   public:
      const int maxBooks = 3;
      const int borrowingPeriod = 15;
      Student(string UID,string name,int age);
      Student(const Student&);
      void borrowBook(Book& book) override;
      void returnBook(Book& book) override;
      void reserveBook(Book& book) override;
      void borrowBook()override;
      void returnBook()override;
      void reserveBook()override;
      void menu()override;
};

class Faculty : public User{
   int borrowedBooks=0;
   public:
      const int maxBooks = 5;
      const int borrowingPeriod = 30;
      Faculty(string UID,string name,int age);
      Faculty(const Faculty&);
      void borrowBook(Book& book) override;
      void returnBook(Book& book) override;
      void reserveBook(Book& book) override;
      void borrowBook()override;
      void returnBook()override;
      void reserveBook()override;
      void menu()override;
};

class Librarian : public User{
   private:
      Library* library;
   public:
      Librarian(string UID, string name, int age);
      Librarian(const Librarian&);
      ~Librarian();
      string getRole(const User&);
      void addBook(Book*);
      void addCopies(Book*,int);
      void removeCopies(Book*,int);
      void removeBook(Book*);
      void changeLibraryName(string);
      void addUser(User*);
      void removeUser(User*);
      void borrowBook(Book&) override;
      void returnBook(Book&) override;
      void reserveBook(Book&) override;
      void getRole();
      void addUser();
      void borrowBook()override;
      void returnBook()override;
      void reserveBook()override;
      void removeUser();
      void addBook();
      void removeBook();
      void menu()override;
};
```

### **Book Class**
```cpp
class Book{
   int ISBN,year,copies,borrowedBooks;
   queue<User*> reservations;
   string title,author,lib_name,publisher,status;
public:
   void updateStatus();
   void saveBook(ofstream &out);
   void loadBook(ifstream &in);
   string getStatus() const;
   string getTitle() const;
   int getISBN() const;
   int getCopies() const;
   bool operator<(const Book& other) const;
   Book(int ISBN,string title,string author,string publisher,int year,int copies);
   Book(const Book&);
};
```
### **Account Class**
```cpp
class Account{
      map<Book,int> borrowDates;
      int maxOverdueDays=0;
      int fineAmount=0;
      int balance = 0;
      void viewFine() const;
      void payFine();
      void payFine(int);
      int getFineAmount()const;
   public:
      User* user;
      map<Book,int> checkOverdueDays();
      void accountBorrow(const Book&);
      void accountReturn(const Book&);
      Account(const User*);
      ~Account();
      void addToBalance(int);
      void simulateFinePayment();
};
```

### **Library Class (Basic Attributes and functions)**
```cpp
class Library{
   private:
      static Library* library;
      string name;
      int year;
      map<Book*,int> booksInLibrary;
      map<const User*,string> UsersRole;
   public:
      map<Book*,int> getBooksInLibrary();
      void startSimulation();
      static Library* getInstance();
      static void setInstance(Library* lib);
      map<const User*,string> getUserRoles();
      void addUser(User*);
      void addBook(Book*);
      void loadUsers();
      void saveUsers();
      void loadBooks();
      void saveBooks();
      User* findUserByUID(string);
      Book* findBookByISBN(int);
      void addLibrarian(Librarian*);
      void removeLibrarian(Librarian*);
      void addStudent(Student*);
      void removeStudent(Student*);
      void addFaculty(Faculty*);
      void removeFaculty(Faculty*);
      string getName();
      int getYear();
      void loadState();
      void saveState();
      void printUserList();
      void printBookList();
      Library(string name,int year);//load files pending
      ~Library();//save to files pending
};
```

## Future Enhancements
- GUI-based interface
- Online book catalog
- Barcode scanner integration
- AI-powered book recommendations

---

✨ **Made with ❤️ by Satwik Raj Wadhwa for efficient library management!** 🚀