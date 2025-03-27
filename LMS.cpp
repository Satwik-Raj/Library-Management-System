#include "classes.h"

int days = 0;

int main() {
   Library Lib("The Hogwarts Library",2005);
   Library::setInstance(&Lib);
   Lib.loadState();//doesn't load transactions, will print transactions directly from the file

   // clearAllFiles();//for testing, not recommended once the library is official

   //comment out the Lib.loadState() if you want to clear files and start over

   cout<<"\n\n";
   cout<<"Current Day Count: "<<days;
   cout<<"\n\n";
   Lib.printUserList();
   cout<<"\n\n";
   Lib.printBookList();
   cout<<"\n\n";

   cout<<"Now the previous state is loaded, so when Filch tries to add these users and books again\n"
   "It will print an error message that they already exist in the database, no need to add again :)\n\n";

   Student Harry("S1","Harry Potter",19);
   Student Hermione("S2","Hermoine Granger",19);
   Student Ron("S3","Ronald Weasley",19);
   Student Draco("S4","Draco Malfoy",19);
   Student Neville("S5","Neville Longbottom",19);
   //Added Ginny Weasley with UID S6 using the Menu for Filch---(visible in the list of users loaded)
   Faculty DumbleDore("F1","Albus Percival Wolfric Brian DumbleDore",110);
   Faculty Snape("F2","Severus Snape",31);
   Faculty McGonagall("F3","Minerva Mcgonagall",127);
   Librarian Filch("L1","Argus Filch",40);// *IMP ERROR RESOLVED-> ISSUE = COPY CONSTRUCTOR NOT EXPLICITLY DEFINED*
   Book b1(1,"To Kill a Mockingbird","J.B. Lippincott & Co.","Harper Lee",1960,5);
   Book b2(2,"1984","Secker & Warburg","George Orwell",1949,5);
   Book b3(3,"Pride and Prejudice","T. Egerton","Jane Austen",1813,5);
   Book b4(4,"The Great Gatsby","Charles Scribner's Sons","F. Scott Fitzgerald",1925,5);
   Book b5(5,"Moby-Dick","Harper & Brothers","Herman Melville",1851,5);
   Book b6(6,"War and Peace","The Russian Messenger","Leo Tolstoy",1869,5);
   Book b7(7,"The Catcher in the Rye","Little, Brown and Company","J.D. Salinger",1951,5);
   Book b8(8,"One Hundred Years of Solitude","Editorial Sudamericana","Gabriel García Márquez",1967,5);
   Book b9(9,"The Lord of the Rings","George Allen & Unwin","J.R.R. Tolkien",1954,5);
   Book b10(10,"The Alchemist","Rocco","Paulo Coelho",1988,5);
   Filch.addBook(&b1);
   Filch.addBook(&b2);
   Filch.addBook(&b3);
   Filch.addBook(&b4);
   Filch.addBook(&b5);
   Filch.addBook(&b6);
   Filch.addBook(&b7);
   Filch.addBook(&b8);
   Filch.addBook(&b9);
   Filch.addBook(&b10);
   Filch.addUser(&Harry);
   Filch.addUser(&Hermione);
   Filch.addUser(&Ron);
   Filch.addUser(&Draco);
   Filch.addUser(&Neville);
   Filch.addUser(&DumbleDore);
   Filch.addUser(&Snape);
   Filch.addUser(&McGonagall);

   if(Lib.findBookByISBN(1))
   cout<<Lib.findBookByISBN(1)->getTitle()<<"\n";
   else cout<<"nullptr1\n";
   if(Lib.findUserByUID("S1"))
   cout<<Lib.findUserByUID("S1")->getName()<<"\n";
   else cout<<"nullptr2\n";

   if(welcome()){ //take the User ID for Interactive Interface out of S1 to S6, F1 to F3, L1
      //welcome function asks if you want Interactive Interface or you want to test these commands
      Lib.printUserList();
      Lib.printBookList();
      Harry.borrowBook(b1);
      Harry.borrowBook(b1);
      Harry.borrowBook(b1);
      Hermione.borrowBook(b1);
      Hermione.borrowBook(b1);
      cout<<"\n\n"<<b1.getStatus()<<"\n\n";
      Hermione.borrowBook(b1);
      cout<<"\n\n"<<b1.getStatus()<<"\n\n";
      daysPassed(31);
      Harry.returnBook(b1);
      Harry.account->simulateFinePayment();
      User* user = &Harry;
      user->menu();
      user = &DumbleDore;
      user->menu();
      user = &Filch;
      user->menu();
   }
   Lib.saveState();
   cout<<Lib.getName()<<" will remain closed until further notice.\n";
   return 0;
}
void daysPassed(int x){
   days+=x;
}
void daysPassedX(){
   string x;
   cout<<"Current Day Count = "<<days<<"\n";
   while(true){
      cout<<"How many days in the future do you want to go?\nDays: ";
      cin>>x;
      if(!isIntegerString(x)){
         cout<<"Invalid Input\n";
      }else break;
   }
   int y = stoi(x);
   days+=y;
   cout<<"\n\n⚡⚡TIME TRAVEL IN PROGRESS⚡⚡\n\n";
   this_thread::sleep_for(chrono::seconds(3));
   cout<<"TIME TRAVEL SUCCESSFUL⏳⏩⌛\nNow Day Count = "<<days<<"\n";
}
void save_to_file(string x,string file_path,string event){
   ofstream file(file_path,ios::app);
   if (!file) {
      cerr << "🔴 Error opening file for writing!" << endl;
      return;
   }
   string y = event;
   toUpperCase(event);
   file<<event<<" :\n"<<x<<endl;
   file.close();
   cout<<"Saved the event: \""<<y<<"\".\n";
}
bool welcome() {
   Library* lib = Library::getInstance();
   string y = lib->getName();
   toUpperCase(y);
   cout << "\n\n=======================================================\n";
   cout << "📚✨  WELCOME TO "<<y<<" (ESTD."<<lib->getYear()<<")  ✨📚\n";
   cout << "=======================================================\n\n";
   while(true){
      cout<<"1. Do you want to start Interactive Interface for a user\n"
      "                    OR\n"
      "2. Do you want to execute the test commands?\n";
      int choice;
      // if(cin.peek()=="\n")cin.ignore();//doesn't work as peek looks for int, can't be == "\n"
      cin>>choice;
      if (cin.fail()) {
         cin.clear();          //***BEST APPROACH FOR ERROR HANDLING TILL DATE----YAY
         cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Discard invalid input
         cout << "\nInvalid input. Please enter a valid choice (1 or 2).\n";
         continue;
     }
      switch (choice){
      case 1:
         lib->startSimulation();
         return false;
      case 2:
         cout<<"\nInitiating the chain of commands\n";
         return true;
      default:
         cout<<"\nInvalid Choice:(\n";
      }
   }
}
void toUpperCase(string &str) {
   transform(str.begin(), str.end(), str.begin(), ::toupper);
}
void clearAllFiles(){
   clearFile("LibraryRecords/books.txt");
   clearFile("LibraryRecords/users.txt");
   clearFile("LibraryRecords/bookCirculation.txt");
   clearFile("LibraryRecords/transactions.txt");
   clearFile("LibraryRecords/listOfCurrentlyBorrowed.txt");
}
void clearFile(const string &filename) {
   std::ofstream file(filename,ios::trunc);
   if (!file) {
      cerr << "🔴 Error: Could not open file " << filename << " for clearing!" << endl;
      return;
   }
   file.close();
   cout << "File '" << filename << "' has been cleared!" << endl;
}
bool isIntegerString(const string& str) {
   if (str.empty()) return false;
   for (char ch : str) {
       if (!isdigit(ch)) return false;
   }
   return true;
}
      //AMENDMENTS PENDING
      //Right Now, Fines only appear once the book is returned
      //If first one in reservation queue can't borrow, move to second, cancel the reservation.
      //Only member of library can borrow/return (though unnecessary)
      //Do ACCOUNT for multiple copies of same book borrowed by same person