#include "classes.h"

//Library Functions' Implementations

Library* Library::library = nullptr;
User* Library::findUserByUID(string uid) {
   for (auto &[user,role] : UsersRole) {
       if (user->getUID() == uid) {
           return const_cast<User*>(user);
       }
   }
   return nullptr;
}
Book* Library::findBookByISBN(int ISBN) {
   for (auto &[book,copies] : booksInLibrary) {
       if (book->getISBN() == ISBN) {
           return const_cast<Book*>(book);
       }
   }
   return nullptr;
}
map<Book*,int> Library::getBooksInLibrary(){
   return booksInLibrary;
}
vector<pair<pair<string,string>,int> > Library::getBorrowed(){
   return global_borrowed;
}
void Library::printUserList(){
   for(auto &[user,role] : getUserRoles()) {
      cout << "User: " << user->getName() << ", Role: " << role << endl;
   }
}
void Library::printBookList(){
   for(auto &[book,c] : getBooksInLibrary()) {
      cout << "Book: " << book->getTitle() << ", Copies: " << c << endl;
   }
}
void Library::printTransactions(){
   ifstream file("LibraryRecords/transactions.txt");
   if (!file) {
      cout << "🔴 Error opening file for reading!" << endl;
      return;
   }
   string str;
   while (getline(file, str)) {
      cout<<"1. "<<str<<endl;
   }
   file.close();
   cout<<"This indicates the end of the list.\n";
}
void Library::printCurrentlyBorrowed(){
   if(global_borrowed.empty()){
      cout<<"No Book is currently borrowed\n";return;
   }
   // cout<<global_borrowed.size();
   for(auto& x : global_borrowed){
      cout<<x.first.second<<" borrowed a copy of "<<x.first.first<<" on Day Count: "<<x.second<<"\n";
   }
}
void Library::loadState(){
   this->loadBooks();
   this->loadUsers();
   this->loadBorrowed();
}
void Library::saveState(){
   this->saveBooks();
   this->saveUsers();
   this->saveBorrowed();
}
string Library::getName(){
   return this->name;
}
int Library::getYear(){
   return this->year;
}
Library* Library::getInstance() {
   return library;
}
void Library::setInstance(Library* lib) {
   library = lib;
}
map<const User*,string> Library::getUserRoles(){
   return UsersRole;
}
void Library::addLibrarian(Librarian* librarian){
   library->UsersRole[librarian]="Librarian";
   // library->Librarians[librarian] = true;
}
void Library::removeLibrarian(Librarian* librarian){
   library->UsersRole.erase(librarian);
   // library->Librarians.erase(librarian);
}
void Library::addStudent(Student* student){
   library->UsersRole[student]="Student";
   // library->Students[student] = true;
}
void Library::removeStudent(Student* student){
   library->UsersRole.erase(student);
   // library->Students.erase(student);
}
void Library::addFaculty(Faculty* faculty){
   library->UsersRole[faculty]="Faculty";
   // library->Faculties[faculty] = true;
}
void Library::removeFaculty(Faculty* faculty){
   library->UsersRole.erase(faculty);
   // library->Faculties.erase(faculty);
}
Library::Library(string name,int year){
   // cout<<"Welcome to "<<name<<" (ESTD."<<year<<")\n";
   this->name = name;
   this->year = year;
}
Library::~Library(){
   //prints multiple times(when we delete library pointer for Librarian class)
   //but I removed the dynamic pointer, replaced it with a normal one.
   // cout<<this->name<<" will remain closed until further notice."<<"\n";
}
void Library::addUser(User* user){
   if(!findUserByUID(user->getUID())){
      if (dynamic_cast<Student *>(user)){
         Student *student = dynamic_cast<Student *>(user);
         addStudent(student);
         cout<<"New Student added to the library.\n"
         "Name: "<<student->getName()<<"\nAge: "<<student->getAge()<<"\n";
      }
      else if (dynamic_cast<Faculty *>(user)){
         Faculty *faculty = dynamic_cast<Faculty *>(user);
         addFaculty(faculty);
         cout<<"New Faculty added to the library.\n"
         "Name: "<<faculty->getName()<<"\nAge: "<<faculty->getAge()<<"\n";
      }
      else if (dynamic_cast<Librarian *>(user)){
         Librarian *librarian = dynamic_cast<Librarian *>(user);
         addLibrarian(librarian);
         cout<<"New Librarian added to the library.\n"
         "Name: "<<librarian->getName()<<"\nAge: "<<librarian->getAge()<<"\n";
      }
      else cerr<<"🔴 ERROR: Can't add user because the role of the user can't be determined.\n";
   }else cerr<<"🔴 ERROR: Can't add user because it has already been added.\n";
}
void Library::addBook(Book* book){
   if(!findBookByISBN(book->getISBN())){
      booksInLibrary[book]=book->copies;
      if(book->copies>1)
         cout<<book->copies<<" copies of a book with title \""<<book->title<<"\" are added to the library.\n";
      else
         cout<<"New book with the title \""<<book->title<<"\" is added to the library.\n";
   }else cerr<<"🔴 Error: Can't add the book, when it's already added.\nTry adding Copies.\n";
}
void Library::loadBorrowed(){
   ifstream file("LibraryRecords/listOfCurrentlyBorrowed.txt");
   if (!file) {
      cout << "🔴 Error opening file for reading!" << endl;
      return;
   }
   global_borrowed.clear();
   string first, second;
   int value;
   while (getline(file, first) && getline(file, second) && (file >> value)) {
      file.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore trailing newline after the integer
      global_borrowed.push_back({{first, second}, value});
   }

   file.close();
   cout << "✅ Currently borrowed books loaded successfully!\n";
}
void Library::saveBorrowed(){
   ofstream file("LibraryRecords/listOfCurrentlyBorrowed.txt");
   if (!file) {
       cerr << "🔴 Error opening file for writing!" << endl;
       return;
   }

   for (const auto& entry : global_borrowed) {
       file << entry.first.first << endl << entry.first.second << endl << entry.second << endl;
   }

   file.close();
   cout << "✅ Currently borrowed books saved successfully!\n";
}
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
   cout<<"✅ Loaded the Users\n";
}

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
   cout<<"✅ Saved the Users\n";
}

void Library::loadBooks() {
   ifstream file("LibraryRecords/books.txt");
   if (!file) {
       cout << "🔴 Error opening file for reading!" << endl;
       return;
   }
   file>>days;
   file.ignore();
   while (file.peek() != EOF) {//**this is not moving the pointer forward**
      Book* bookp = new Book(0, "", "", "", 0, 0);
      bookp->loadBook(file);
      addBook(bookp);
      booksInLibrary[bookp] += bookp->getCopies();
   }
   file.close();
   cout << "✅ Loaded the Books\n";
}

// void Library::loadBooks() {
//    ifstream file("LibraryRecords/books.txt");
//    if (!file) {
//        cout << "🔴 Error opening file for reading!" << endl;
//        return;
//    }
//    string line;
//    while (getline(file, line)) {//ERROR -- Title is the alchemist, it reads title as the and author as alchemist
//       // cout<<line<<endl;
//       Book* bookp = new Book(0, "", "", "", 0, 0);
//       file.seekg(-line.length(), ios::cur);  // IMPORTANT---Move the file pointer back for line re-reading
//       bookp->loadBook(file);
//       // cout<<bookp->getTitle()<<endl;
//       booksInLibrary[bookp] += bookp->getCopies();
//       // cout<<booksInLibrary[bookp]<<endl;
//    }
//    file.close();
//    cout << "Loaded the Books\n";
// }


void Library::saveBooks() {
   ofstream file("LibraryRecords/books.txt");
   if (!file) {
       cout << "🔴 Error opening file for writing!" << endl;
       return;
   }
   file<<days<<endl;
   for (auto& [book,_] : booksInLibrary) {
      book->saveBook(file);
   }
   file.close();
   cout<<"✅ Saved the Books\n";
}
void Library::startSimulation(){
   string UID;
   while(true){
      cout<<"🔹 Please Enter your User ID\n"
            "🔹 Type 'help' if you don't know your ID\n"
            "🔹 Type 'exit' to exit\n*Please note that the User ID is case sensitive.\n->";
      while(true){
         cin >> UID;
         if (cin.fail()) {
            cin.clear();          //***BEST APPROACH FOR ERROR HANDLING TILL DATE----YAY
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\nInvalid input. Please enter a valid UID:\n";
         }else break;
      }
      Library* library = Library::getInstance();
      if(library->findUserByUID(UID)){
         User* user = library->findUserByUID(UID);
         cout<<"\nOpening the Menu for "<<user->getName()<<"......\n\n";
         this_thread::sleep_for(chrono::seconds(3));
         user->menu();
      }else if(UID == "help"){
         cout<<"💡 Your User ID is assigned when you register. If you are a student"
         ", it starts with 'S'. Faculty IDs start with 'F', and librarians have 'L'.\n\n";
      }
      else if(UID=="exit"){
         cout<<"Exiting Interactive Interface\n";
         break;
      }
      else{
         cout<<"🔴 Invalid User ID!!\nPlease note you must be added beforehand by a Librarian.\n";
      }
   }
}