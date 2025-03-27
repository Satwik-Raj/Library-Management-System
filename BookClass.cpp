#include "classes.h"

//Book Functions' Implementations
string Book::getStatus() const {
   return this->status;
}
string Book::getTitle() const {
   return this->title;
}
int Book::getCopies()const{
   return this->copies;
}
int Book::getISBN()const{
   return this->ISBN;
}
void Book::saveBook(ofstream &out){
   out<<ISBN<<" "<<copies<<" "<<borrowedBooks<<" "<<reservations.size()<<endl<<publisher<<endl<<title<<endl<<status<<endl<<author<<endl;
   // cout << "Size of reservations: " << reservations.size() << endl;
   while (!reservations.empty()) {
      User* user = reservations.front();
      reservations.pop();
      if (user) {
         // cout << "User UID: " << user->getUID() << " Name: " << user->getName() << " Age: " << user->getAge() << endl;
         out << user->getUID() << " " << user->getAge() << endl << user->getName() << endl;
      } else {
         cout << "Error: Invalid user in reservations!" << endl;
      }
   }
}
void Book::loadBook(ifstream &in) {
   string uid, name,role;
   int age,res;
   in>>ISBN>>copies>>borrowedBooks>>res;

   in.ignore(); // V.IMP-->Ignore the newline left by previous extraction

   getline(in, publisher);//IMP as they can have multiple words---->(AWESOME)
   getline(in, title);
   getline(in, status);
   getline(in, author);
   Library* library=Library::getInstance();
   User* user;
   // cout<<res;
   while (res--) {
      in >> uid >> age;
      getline(in, name);
      user = library->findUserByUID(uid);
      switch(uid[0]){
         case 'S':
         if (!user) library->addStudent(new Student(uid, name, age));
         else library->addStudent(dynamic_cast<Student*>(user));break;
         case 'F':
         if (!user)library->addFaculty(new Faculty(uid, name, age));
         else library->addFaculty(dynamic_cast<Faculty*>(user));break;
         case 'L':
         if (!user)library->addLibrarian(new Librarian(uid, name, age));
         else library->addLibrarian(dynamic_cast<Librarian*>(user));break;
      }
      reservations.push(user);
  }
}
void Book::updateStatus(){
   // cout<<"copies: "<<this->copies<<", borrowed: "<<this->borrowedBooks<<"\n";
   assert(this->copies>=this->borrowedBooks);
   if(this->copies > this->borrowedBooks){
      this->status = "available";
   }else if(reservations.size()==0){
      this->status = "borrowed";
   }else{
      this->status = to_string(reservations.size())+" Reservations Pending";
   }
}
// 2nd const is necessary for calling it on a const Book1, else it will give error
// it ensures "this" is not modified inside
bool Book::operator<(const Book& other) const {
   return this->ISBN < other.ISBN;
}
Book::Book(int ISBN,string title,string author,string publisher,int year,int copies){
   // cout<<"Book constructor"<<endl;
   this->copies = copies;
   this->ISBN=ISBN;
   this->title=title;
   this->author=author;
   this->year=year;
   this->status="available";
   this->publisher=publisher;
}
Book::Book(const Book& other){
   this->copies = other.copies;
   this->ISBN=other.ISBN;
   this->title=other.title;
   this->author=other.author;
   this->year=other.year;
   this->status=other.status;
   this->publisher=other.publisher;
}

// #include <set>

// class Library {
// private:
//     std::set<int> uniqueISBNs;  // Track inserted ISBNs
// public:
//     bool addBook(Book* book) {
//         int isbn = book->getISBN();
//         if (uniqueISBNs.count(isbn)) {
//             cout << "❌ Book with ISBN " << isbn << " already exists!\n";
//             return false;  // Reject duplicate
//         }
//         uniqueISBNs.insert(isbn);  // Insert new ISBN
//         booksInLibrary[book] += book->getCopies();
//         return true;
//     }
// };
