#include "classes.h"

//Librarian Functions' Implemenatations
Librarian::Librarian(string UID,string name,int age):User(UID,name,age){
   library = Library::getInstance();
   //it doesn't work as you won't find the address "this" in the list
   // if (library->UsersRole.find(this) != library->UsersRole.end() && library->UsersRole[this] == "Librarian"){}
   //you have to do the following
   if(!library->findUserByUID(UID)){
      library->UsersRole[this]="Librarian";
      // library->Librarians[this]=true;
      cout<<"New Librarian added to the library\nName: "<<name<<"\nAge: "<<age<<"\n";
   }
}
Librarian::Librarian(const Librarian& other) : User(other) {
   // cout<<"Librarian CC called.\n";//for debugging
   library = other.library;
}

string Librarian::getRole(const User& user){
   return Library::getInstance()->UsersRole[&user];
}
void Librarian::addBook(Book* book){
   library->addBook(book);
}
void Librarian::addCopies(Book* book,int number){
   if(number>1)
      cout<<number<<" copies of \""<<book->title<<"\" are added to the library.\n";
   else if(number==1)
      cout<<"A copy of  \""<<book->title<<"\" is added to the library.\n";
   else cout<<"ERROR: can't add a negative number of copies\n";//maybe put removeCopies here CHECK
   library->booksInLibrary[book]+=number;
}
void Librarian::removeCopies(Book* book,int number){
   if(number>book->copies) cout<<"Can't remove more copies than total.\n";
   else{
      if(number>1)
         cout<<number<<" copies of \""<<book->title<<"\" are removed from the library.\n";
      else if(number==1)
         cout<<"A copy of  \""<<book->title<<"\" is removed from the library.\n";
      else cout<<"ERROR: can't remove a negative number of copies\n";//maybe put addCopies here CHECK
      library->booksInLibrary[book]-=number;
      book->copies-=number;
   }
}
void Librarian::removeBook(Book* book){
   if(library->booksInLibrary[book]){
      library->booksInLibrary[book]=0;
      if(book->copies>1)
         cout<<book->copies<<" copies of a book with title \""<<book->title<<"\" are removed from the library.\n";
      else
         cout<<"Book with the title \""<<book->title<<"\" is removed from the library.\n";
   }else cout<<"🔴 ERROR: Can't remove a book which is not in the library.\n";
}
void Librarian::changeLibraryName(string name){
   cout<<"The name of "<<library->name<<" is being changed to "<<name<<"\n";
   library->name = name;
}
void Librarian::addUser(User* user){
   library->addUser(user);
}
void Librarian::removeUser(User* user){
   if (dynamic_cast<Student *>(user)){
      if(library->UsersRole[user]=="Student"){
         Student *student = dynamic_cast<Student *>(user);
         Library::getInstance()->removeStudent(student);
         cout<<"This Student is being removed from the library.\n"
         "Name: "<<student->getName()<<"\nAge: "<<student->getAge()<<"\n";
      }else cout<<"🔴 ERROR: Can't remove the student, NOT FOUND IN THE DATABASE\n";
   }
   else if (dynamic_cast<Faculty *>(user)){
      if(library->UsersRole[user]=="Faculty"){
         Faculty *faculty = dynamic_cast<Faculty *>(user);
         Library::getInstance()->removeFaculty(faculty);
         cout<<"This Faculty is being removed from the library.\n"
         "Name: "<<faculty->getName()<<"\nAge: "<<faculty->getAge()<<"\n";
      }else cout<<"🔴 ERROR: Can't remove the faculty, NOT FOUND IN THE DATABASE\n";
   }
   else if (dynamic_cast<Librarian *>(user)){
      if(library->UsersRole[user]=="Librarian"){
         Librarian *librarian = dynamic_cast<Librarian *>(user);
         Library::getInstance()->removeLibrarian(librarian);
         cout<<"This Librarian is being removed from the library.\n"
         "Name: "<<librarian->getName()<<"\nAge: "<<librarian->getAge()<<"\n";
      }else cout<<"🔴 ERROR: Can't remove the librarian, NOT FOUND IN THE DATABASE\n";
   }
   else cout<<"🔴 ERROR: Can't determine the role of the user";
}
void Librarian::borrowBook(Book& book){
   cout<<"Borrow Request for "<<book.title<<" denied.\n"
   "As a Librarian, you are not allowed to borrow.\n";
}
void Librarian::returnBook(Book& book){
   cout<<"Return Request for "<<book.title<<" denied.\n"
   "You have not officially borrowed any book, So you can't return.\n";
}
void Librarian::reserveBook(Book& book){
   cout<<"Reserve Request for "<<book.title<<" denied.\n"
   "As a Librarian, you are not allowed to reserve.\n";
}
void Librarian::borrowBook(){
   cout<<"Borrow Request denied.\nAs a Librarian, you are not allowed to borrow.\n";
}
void Librarian::returnBook(){
   cout<<"Return Request denied.\nYou have not officially borrowed any book, So you can't return.\n";
}
void Librarian::reserveBook(){
   cout<<"Reserve Request denied.\nAs a Librarian, you are not allowed to reserve.\n";
}
Librarian::~Librarian(){
   library = nullptr;
}
void Librarian::addUser() {
   string UID, name;
   int age;
   cout << "Enter UID: ";
   cin >> UID;
   cout << "Enter Name: ";
   cin.ignore();// for \n from before
   getline(cin,name);
   cout << "Enter Age: ";
   while(true){
      cin >> age;
      if (cin.fail()) {
         cin.clear();          //***BEST APPROACH FOR ERROR HANDLING TILL DATE----YAY
         cin.ignore(numeric_limits<streamsize>::max(), '\n');
         cout << "\nInvalid input. Please enter a valid age:\n";
      }else break;
   }
   switch (UID[0]){
   case 'S':
      addUser(new Student(UID, name, age));
      break;
   case 'F':
      addUser(new Faculty(UID, name, age));
      break;
   case 'L':
      addUser(new Librarian(UID, name, age));
      break;
   }
   cout << "User added successfully!\n";
}
void Librarian::removeUser() {
   string UID;
   cout << "Enter UID to remove: ";
   cin >> UID;
   Library* library = Library::getInstance();
   if(library->findUserByUID(UID)){
      removeUser(library->findUserByUID(UID));
      cout << "User removed successfully!\n";
   }
   cout << "User not found!\n";
}
void Librarian::addBook() {
   string title, author,publisher;
   int ISBN,year,copies;
   cout << "Enter ISBN: ";
   while(true){
      cin >> ISBN;
      if (cin.fail()) {
         cin.clear();          //***BEST APPROACH FOR ERROR HANDLING TILL DATE----YAY
         cin.ignore(numeric_limits<streamsize>::max(), '\n');
         cout << "\nInvalid input. Please enter a valid ISBN:\n";
      }else break;
   }
   cin.ignore();// Removes the leftover '\n'
   cout << "Enter Title: ";
   getline(cin, title);
   cout << "Enter Author: ";
   getline(cin, author);
   cout << "Enter Publisher: ";
   getline(cin, publisher);
   cout << "Enter Year Published: ";
   while(true){
      cin >> year;
      if (cin.fail()) {
         cin.clear();          //***BEST APPROACH FOR ERROR HANDLING TILL DATE----YAY
         cin.ignore(numeric_limits<streamsize>::max(), '\n');
         cout << "\nInvalid input. Please enter a valid year:\n";
      }else break;
   }
   cout << "Enter no. of copies of the book: ";
   while(true){
      cin >> copies;
      if (cin.fail()) {
         cin.clear();          //***BEST APPROACH FOR ERROR HANDLING TILL DATE----YAY
         cin.ignore(numeric_limits<streamsize>::max(), '\n');
         cout << "\nInvalid input. Please enter a valid no. of copies:\n";
      }else break;
   }
   addBook(new Book(ISBN,title,author,publisher,year,copies));
   cout << "Book added successfully!\n";
}
void Librarian::removeBook() {
   int ISBN;
   cout << "Enter ISBN to remove: ";
   while(true){
      cin >> ISBN;
      if (cin.fail()) {
         cin.clear();          //***BEST APPROACH FOR ERROR HANDLING TILL DATE----YAY
         cin.ignore(numeric_limits<streamsize>::max(), '\n');
         cout << "\nInvalid input. Please enter a valid ISBN:\n";
      }else break;
   }
   Library* library = Library::getInstance();
   if(library->findBookByISBN(ISBN)){
      removeBook(library->findBookByISBN(ISBN));
      cout << "Book removed successfully!\n";
   }else cout << "🔴 Book not found!\n";   
}
void Librarian::getRole(){
   string UID;
   cout << "Enter the UID: ";
   cin >> UID;
   if(!library->findUserByUID(UID)){
      cout<<"🔴 User not found 🔴\n";return;
   }
   switch (UID[0]){
      case 'S':
         cout<<"The role of this user is \"Student\"\n";
         break;
      case 'F':
         cout<<"The role of this user is \"Faculty\"\n";
         break;
      case 'L':
         cout<<"The role of this user is \"Librarian\"\n";
         break;
   }
}
void Librarian::menu() {
   int choice;
   while (true) {
      cout << "\n📚 Menu for the Librarian "<< name <<" 📚\n"
      "1. Add User\n2. Remove User\n3. Add Book\n4. Remove Book\n"
      "5. Get The Role of a User\n6. Print Users List\n7. Print Books List\n"
      "8. Print Transaction History\n9. Print Currently borrowed books\n10. Edit Profile\n"
      "11. Future\n12. Exit\nEnter choice(1-12): ";
      while(true){
         cin >> choice;
         if (cin.fail()) {
            cin.clear();          //***BEST APPROACH FOR ERROR HANDLING TILL DATE----YAY
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\nInvalid input. Please enter a valid choice(1-12):\n";
         }else break;
      }
      switch (choice) {
         case 1: addUser(); break;
         case 2: removeUser(); break;
         case 3: addBook(); break;
         case 4: removeBook(); break;
         case 5: getRole(); break;
         case 6: library->printUserList(); break;
         case 7: library->printBookList(); break;
         case 8: library->printTransactions(); break;
         case 9: library->printCurrentlyBorrowed(); break;
         case 10: editProfile(); break;
         case 11: daysPassedX(); break;
         case 12: return;
         default: cout << "\nInvalid choice!\n\n";
      }
   }
}