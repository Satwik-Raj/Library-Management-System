#include "classes.h"

//Faculty Functions' Implemenatations
Faculty::Faculty(string UID,string name,int age):User(UID,name,age){
   // a Librarian must add the faculty
   // lib.UsersRole[*this]="Faculty";
   // lib.Faculties[*this]=true;
   // cout<<"New Faculty added to the server\nName: "<<name<<"\nAge: "<<age<<"\n";
}
Faculty::Faculty(const Faculty& other) : User(other) {
   cout<<"Copy constructor called\n";
}
void Faculty::borrowBook(Book& book){
   if(borrowedBooks<maxBooks){
      account->checkOverdueDays();// for getting maxOverdueDays
      if(account->maxOverdueDays<=60){
         if(book.getStatus()=="available"){
            cout<<"The book is available.\nInitiating the Borrowing sequence for "<<name<<".\n";
            cout<<"A copy of "<<book.getTitle()<<" is being issued to "<<this->name<<"\n";
            borrowedBooks++;
            book.borrowedBooks++;
            book.updateStatus();
            account->borrowDates[book]=days;

            Library::getInstance()->global_borrowed.push_back(make_pair(make_pair(book.getTitle(),name),days));
            string x = name+" borrowed "+book.getTitle()+" on Day Count: "+to_string(days);
            save_to_file(x,"LibraryRecords/bookCirculation.txt","Book Borrowed");

         }else{
            cout<<"Borrow Request for "<<book.getTitle()<<" Unsuccessful.\n"
            "All the copies for "<<book.getTitle()<<" have been issued.\n"
            "Please choose another related book or reserve it and wait till one returns.\n"
            "Do you want to reserve it?\n";
            string str="Hi";
            cin>>str;
            std::transform(str.begin(), str.end(), str.begin(), ::tolower);
            while(str!="y"&&str!="yes"&&str!="yeah"&&str!="no"&&str!="n"&&str!="nah"){
               cout<<"Didn't quite catch that.(Y/N)?";
               cin>>str;
               std::transform(str.begin(), str.end(), str.begin(), ::tolower);
            }
            if(str=="y"||str=="yes"||str=="yeah") reserveBook(book);
         }
      }else cout<<"Borrow Request for "<<book.getTitle()<<" denied.\n"
         "You have an Overdue book for more than 60 days\n"
         "Please return it in order to restore your borrowing rights.\n";
   }else cout<<"Borrow Request for "<<book.getTitle()<<" denied.\n"
      "You've reached the limit of borrowed books.\n"
      "Please return a book to get your borrowing rights back.\n";
}
void Faculty::returnBook(Book& book){
   auto& b = Library::getInstance()->global_borrowed;
   bool x = find_if(b.begin(), b.end(),[this,&book](const auto& entry) {
                          return entry.first.second == name&&entry.first.first==book.getTitle();
                      })!= b.end();
   if(x){
      map<Book,int> overdue = account->checkOverdueDays();
      cout<<"Initiating the Returning Sequence for "<<name<<".\n";
      borrowedBooks--;
      book.borrowedBooks--;
      book.updateStatus();

      string x = name+" returned "+book.getTitle()+" on Day Count: "+to_string(days);
      save_to_file(x,"LibraryRecords/bookCirculation.txt","Book Returned");
      auto& b = Library::getInstance()->global_borrowed;

      b.erase(remove_if(b.begin(), b.end(), [&](const auto& entry) {
         return entry.first.first == book.getTitle() && entry.first.second == name;  // Ignore 'days'
      }), b.end());

      if(overdue[book]){
         cout<<"You are late by "<<overdue[book]<<" days\n";
      }
      if(!book.reservations.empty()){
         cout<<"Notification for "<<book.reservations.front()->getName()<<" :-\n"
         "\nThe Book you reserved is now available\nSubmitting your Borrow Request\n\n";
         book.reservations.front()->borrowBook(book);
         book.reservations.pop();//WHAT IF THE STUDENT HAD 3 BOOKS, can't issue. How to move to 2nd guy inn line? CHECK
      }
   }else cout<<"Return Request for "<<book.getTitle()<<" denied.\n"
      "ERROR: You didn't borrow it, How could you possibly return it?\n";
}
void Faculty::reserveBook(Book& book){
   if(borrowedBooks<maxBooks){
      if(book.getStatus()=="available"){
         cout<<book.getTitle()<<" is available.\nYou can borrow it right now\n"
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
         <<book.getTitle()<<" is being reserved for "<<this->name<<"\n";
         book.reservations.push(this);
         book.updateStatus();
         // account->accountReserve(book);
      }
   }else cout<<"Reserve Request for "<<book.getTitle()<<" denied.\n"
      "You've reached the limit of borrowed books.\n"
      "Please return a book to get your reserving rights back.\n";
}
void Faculty::borrowBook(){
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
   borrowBook(*book);
}
void Faculty::returnBook(){
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
void Faculty::reserveBook(){
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
void Faculty::menu() {
   int choice;
   while (true) {
      cout << "\n📚 Menu for the Faculty "<< name <<" 📚\n"
      "1. Borrow Book\n2. Return Book\n3. Reserve Book\n4. Edit Profile\n5. Future\n6. Exit\n"
      "Enter choice(1-6): ";
      while(true){
         cin >> choice;
         if (cin.fail()) {
            cin.clear();          //***BEST APPROACH FOR ERROR HANDLING TILL DATE----YAY
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\nInvalid input. Please enter a valid choice(1-6):\n";
         }else break;
      }
      switch (choice) {
         case 1: borrowBook(); break;
         case 2: returnBook(); break;
         case 3: reserveBook(); break;
         case 4: editProfile(); break;
         case 5: daysPassedX(); break;
         case 6: return;
         default: cout << "\nInvalid choice!\n\n";
      }
   }
}