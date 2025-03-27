#include "classes.h"

//Student Functions' Implemenatations
Student::Student(string UID,string name,int age):User(UID,name,age){
   // a Librarian must add the student
   // lib.UsersRole[*this]="Student";
   // lib.Students[*this]=true;
   // cout<<"New student added to the server\nName: "<<name<<"\nAge: "<<age<<"\n";
}
Student::Student(const Student& other):User(other){
   // cout<<"Student CC called\n";
}

void Student::borrowBook(Book& book){//override wasn't working
   if(borrowedBooks<maxBooks&&account->checkOverdueDays().empty()){
      if(book.status=="available"){
         cout<<"The book is available.\nInitiating the Borrowing sequence for "<<name<<".\n";
         cout<<"A copy of "<<book.title<<" is being issued to "<<this->name<<"\n";
         borrowedBooks++;
         book.borrowedBooks++;
         book.updateStatus();
         account->accountBorrow(book);

         Library::getInstance()->global_borrowed.push_back(make_pair(make_pair(book.getTitle(),name),days));
         string x = name+" borrowed "+book.getTitle()+" on Day Count: "+to_string(days);
         save_to_file(x,"LibraryRecords/bookCirculation.txt","Book Borrowed");

      }else{
         cout<<"Borrow Request for "<<book.title<<" denied.\n"
         "All the copies for "<<book.title<<" have been issued.\n"
         <<"\nPlease choose another related book or reserve it and wait till one returns.\n";
         cout<<"Do you want to reserve it?\n";
         string str="Hi";
         cin>>str;
         transform(str.begin(), str.end(), str.begin(), ::tolower);
         while(str!="y"&&str!="yes"&&str!="yeah"&&str!="no"&&str!="n"&&str!="nah"){
            cout<<"Didn't quite catch that.(Y/N)?";
            cin>>str;
            transform(str.begin(), str.end(), str.begin(), ::tolower);
         }
         if(str=="y"||str=="yes"||str=="yeah") reserveBook(book);
      }
   }else{
      cout<<"Borrow Request for "<<book.title<<" denied.\n";
      if(borrowedBooks>=maxBooks&&!account->checkOverdueDays().empty())
         cout<<"You've reached the limit of borrowed books.\n"
         "Please return a book and pay all the unpaid fines to get your borrowing rights back.\n";
      else if(borrowedBooks>=maxBooks)
         cout<<"You've reached the limit of borrowed books.\n"
         "Please return a book to get your borrowing rights back.\n";
      else if(!account->checkOverdueDays().empty())
         cout<<"You have unpaid fines.\nBorrowing is not allowed.\n"
         "Please pay all the unpaid fines to get your borrowing rights back.\n";
   }
}
void Student::returnBook(Book& book){
   auto& b = Library::getInstance()->global_borrowed;
   bool x = find_if(b.begin(), b.end(),[this,&book](const auto& entry) {
               return entry.first.second == name&&entry.first.first==book.getTitle();
            })!= b.end();
   if(x){
      cout<<"Initiating the Returning Sequence for "<<name<<".\n";
      borrowedBooks--;
      book.borrowedBooks--;
      book.updateStatus();
      account->accountReturn(book);

      string x = name+" returned "+book.getTitle()+" on Day Count: "+to_string(days);
      save_to_file(x,"LibraryRecords/bookCirculation.txt","Book Returned");
      auto& b = Library::getInstance()->global_borrowed;

      b.erase(remove_if(b.begin(), b.end(), [&](const auto& entry) {
         return entry.first.first == book.getTitle() && entry.first.second == name;  // Ignore 'days'
      }), b.end());


      if(account->checkOverdueDays()[book]){
         if(account->checkOverdueDays()[book]==1)
         cout<<"You are late by "<<account->checkOverdueDays()[book]<<
         " day and are expected to deposit Rs. "<<account->checkOverdueDays()[book]*10<<"\n";
         else
         cout<<"You are late by "<<account->checkOverdueDays()[book]<<
         " days and are expected to deposit Rs. "<<account->checkOverdueDays()[book]*10<<"\n";
      }
      if(!book.reservations.empty()){
         cout<<"\nNotification for "<<book.reservations.front()->getName()<<" :-\n"
         "\nThe Book you reserved is available now\nSubmitting your Borrow Request\n\n";
         book.reservations.front()->borrowBook(book);
         book.reservations.pop();//WHAT IF THE STUDENT HAD 3 BOOKS, can't borrow. How to move to 2nd guy inn line? CHECK
      }
   }else cout<<"ERROR: You didn't borrow it, How could you possibly return it?\n";
}
void Student::reserveBook(Book& book){
   if(borrowedBooks<maxBooks){
      if(book.status=="available"){
         cout<<book.title<<" is available.\nYou can borrow it right now\n"
         "Do you want to borrow it?\n";
         string str="Hi";
         cin>>str;
         std::transform(str.begin(), str.end(), str.begin(), ::tolower);
         while(str!="y"&&str!="yes"&&str!="yeah"&&str!="no"&&str!="n"&&str!="nah"){
            cout<<"Didn't quite catch that.(Y/N)?\n";
            cin>>str;
            std::transform(str.begin(), str.end(), str.begin(), ::tolower);
         }
         if(str=="y"||str=="yes"||str=="yeah") borrowBook(book);
      }else{
         cout << "Initiating the Reserving Sequence for "<<name<<".\n"
         <<book.title<<" is being reserved for "<<this->name<<"\n";
         book.reservations.push(this);
         book.updateStatus();
         // account->accountReserve(book);
      }
   }else cout<<"You've reached the limit of borrowed books.\n"
   "Please return a book to get your reserving rights back.\n";
}
void Student::borrowBook(){
   int ISBN;
   cout << "Enter the ISBN of the book you want to borrow: ";
   while(true){
      cin >> ISBN;
      if (cin.fail()) {
         cin.clear();          //***BEST APPROACH FOR ERROR HANDLING TILL DATE----YAY
         cin.ignore(numeric_limits<streamsize>::max(), '\n');
         cout << "\nInvalid input. Please enter a valid ISBN:\n";
      }else break;
   }
   Library* library = Library::getInstance();
   Book* book = library->findBookByISBN(ISBN);
   if(!book){
      cout<<"No book found using that ISBN. Try again with another ISBN.\n";
      return;
   }
   // cout<<"borrow start: "<<book->getCopies()<<" "<<book->getTitle()<<"\n";
   borrowBook(*book);
}
void Student::returnBook(){
   int ISBN;
   cout << "Enter the ISBN of the book you want to return: ";
   while(true){
      cin >> ISBN;
      if (cin.fail()) {
         cin.clear();          //***BEST APPROACH FOR ERROR HANDLING TILL DATE----YAY
         cin.ignore(numeric_limits<streamsize>::max(), '\n');
         cout << "\nInvalid input. Please enter a valid ISBN:\n";
      }else break;
   }
   Library* library = Library::getInstance();
   Book* book = library->findBookByISBN(ISBN);
   if(!book){
      cout<<"No book found using that ISBN. Try again with another ISBN.\n";
      return;
   }
   returnBook(*book);
}
void Student::reserveBook(){
   int ISBN;
   cout << "Enter the ISBN of the book you want to reserve: ";
   while(true){
      cin >> ISBN;
      if (cin.fail()) {
         cin.clear();          //***BEST APPROACH FOR ERROR HANDLING TILL DATE----YAY
         cin.ignore(numeric_limits<streamsize>::max(), '\n');
         cout << "\nInvalid input. Please enter a valid ISBN:\n";
      }else break;
   }
   Library* library = Library::getInstance();
   Book* book = library->findBookByISBN(ISBN);
   if(!book){
      cout<<"No book found using that ISBN. Try again with another ISBN.\n";
      return;
   }
   reserveBook(*book);
}
void Student::menu() {
   int choice;
   while (true) {
      cout << "\n📚 Menu for the Student "<< name <<" 📚\n"
      "1. Borrow Book\n2. Return Book\n3. Reserve Book\n4. Edit Profile\n"
      "5. Fine Management\n6. Future\n7. Exit\nEnter choice(1-7): ";
      cin >> choice;
      if (cin.fail()) {
         cin.clear();          //***BEST APPROACH FOR ERROR HANDLING TILL DATE----YAY
         cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Discard invalid input
         cout << "\nInvalid input. Please enter a valid choice (1-7).\n";
         continue;
      }
      switch (choice) {
         case 1: borrowBook(); break;
         case 2: returnBook(); break;
         case 3: reserveBook(); break;
         case 4: editProfile(); break;
         case 5: account->simulateFinePayment(); break;
         case 6: daysPassedX(); break;
         case 7: return;
         default: cout << "\nInvalid choice!\n\n";
      }
   }
}