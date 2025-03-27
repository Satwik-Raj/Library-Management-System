#ifndef CLASSES_H
#define CLASSES_H
#include <iostream>
#include <map>
#include <string>
#include <cassert>
#include <fstream>
#include <vector>
#include <span>
#include <thread>
#include <chrono>
using namespace std;

extern int days;
void daysPassed(int);
void daysPassedX();
class Library;
bool welcome();
void toUpperCase(string &);
void clearFile(const string&);
void clearAllFiles();
bool isIntegerString(const string& str);
void save_to_file(string,string,string);

class User;
class Student;
class Faculty;
class Librarian;
class Book;
class Account;
class Library{
   friend class User;
   friend class Student;
   friend class Faculty;
   friend class Librarian;
   friend class Account;
   private:
      static Library* library;
      string name;
      int year;
      vector<pair<pair<string,string>,int> >global_borrowed;
      map<Book*,int> booksInLibrary;
      map<const User*,string> UsersRole;
      // map<const Student*,bool> Students;
      // map<const Faculty*,bool> Faculties;
      // map<const Librarian*,bool> Librarians;
   public:
      map<Book*,int> getBooksInLibrary();
      vector<pair<pair<string,string>,int> > getBorrowed();
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
      void loadBorrowed();
      void saveBorrowed();
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
      void printTransactions();
      void printCurrentlyBorrowed();
      Library(string name,int year);//load files pending
      ~Library();//save to files pending
};

class User{
   protected:
      string UID,name;
      int age;
   public:
      Account* account;
      string getName() const;
      int getAge() const;
      string getUID() const;
      // void saveUser(ofstream &out);
      // void loadUser(ifstream &in);
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
      void removeBook(Book*);//updateBook?
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

class Book{
   // friend class Faculty;
   friend class Library;
   friend class Librarian;
   friend class Student;
   friend class User;
   int ISBN,year,copies;
   string title,author,lib_name,publisher,status;
public:
   int borrowedBooks;
   queue<User*> reservations;
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

class Account{
   friend class Student;
   friend class Faculty;
   private:
      map<Book,int> borrowDates;//old date will be replaced with new ones 'BAD' CHECK
      int maxOverdueDays=0;
      int fineAmount=0;
      int balance = 0;
      void viewFine() const;
      void payFine();
      void payFine(int);
      int getFineAmount()const;
   public:
      User* user;
      void update_fine();
      map<Book,int> checkOverdueDays();
      void accountBorrow(const Book&);
      void accountReturn(const Book&);
      Account(const User*);
      ~Account();
      void addToBalance(int);
      // void printFineList();//doesn't include which fine is paid....Lengthy
      void simulateFinePayment();
};
#endif
