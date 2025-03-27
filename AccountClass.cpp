#include "classes.h"

//Account Functions' Implementations
map<Book,int> Account::checkOverdueDays(){
   map<Book,int> books;
   int borrowingPeriod;
   if (dynamic_cast<Student *>(user)) {
      borrowingPeriod = 15;
      for(auto &[book,date] : borrowDates){
         int borrow_days = days - date;
         if(borrow_days>borrowingPeriod){
               books[book]=(borrow_days-borrowingPeriod);
         }
      }
   } else if (dynamic_cast<Faculty *>(user)) {
      borrowingPeriod = 30;
      for(auto &[book,date] : borrowDates){
         int borrow_days = days - date;
         if(borrow_days>borrowingPeriod){
            books[book]=(borrow_days-borrowingPeriod);
            if(books[book]>maxOverdueDays)
               maxOverdueDays = books[book];
         }
      }
   }else cout<<"ERROR: Can check overdues only for a Faculty or a Student\n";
   return books;
}
void Account::update_fine(){
   for(auto &[_,it] : checkOverdueDays()){
      fineAmount+=it*10;
   }
}
void Account::accountBorrow(const Book& book){
   borrowDates[book]=days;
}
void Account::accountReturn(const Book& book){
   fineAmount+=checkOverdueDays()[*Library::getInstance()->findBookByISBN(book.getISBN())]*10;
}
Account::Account(const User* userp){
   user = const_cast<User*>(userp);
   //ISSUE CHECK(MULTIPLE PRINTS)------->>>>RESOLVED YAY!!!
   // cout<<"Account is being created for "<<userp->getName()<<"\n";
}
Account::~Account(){
   // delete user;
}

// void Account::printFineList(){
//    for(auto &[book,day] : this->checkOverdueDays()) {
//       cout << "Book: " << book.getTitle() << ", Fine: Rs. " << day*10 << "\n";
//    }
// }
void Account::addToBalance(int amount){
   if(amount<=0) cout << "Invalid payment amount!\n";
   else{
      string x = user->getName()+" added "+to_string(amount)+" to their wallet\n";
      save_to_file(x,"LibraryRecords/transactions.txt","Wallet Transaction");
      balance+=amount;
      cout<<"Remaining balance: Rs. "<<balance<<"\n";
   }
}
void Account::viewFine() const {
   cout << user->getName() << " has an outstanding fine of Rs. " << fineAmount << "\n";
}
void Account::payFine(){
   if(balance==0)cout<<"Balance remaining: Rs. 0\nTransaction aborted!";
   else if (balance >= fineAmount) {
      cout << "Fine fully paid using balance! Remaining balance: Rs. " << (balance-fineAmount) << "\n";
      balance -= fineAmount;
      string x = user->getName()+" used their balance to pay the total fine of Rs. "+\
      to_string(fineAmount)+"\nThe remaining balance is Rs. "+to_string(balance)+"\n";
      save_to_file(x,"LibraryRecords/transactions.txt","Full Fine Payment using balance only");
      fineAmount = 0;
   } else {
      fineAmount -= balance;
      cout << "Partial payment using balance successful! Remaining fine: Rs. " << fineAmount << "\n";
      string x = user->getName()+" used all of their balance(Rs. "+to_string(balance)\
      +") to pay their fine partially.\nThe remaining fine is Rs. "+to_string(fineAmount)+"\n";
      save_to_file(x,"LibraryRecords/transactions.txt","Partial Fine Payment using balance only");
      balance=0;
   }
}
void Account::payFine(int amount) {
   if (amount <= 0) {
      cout << "Invalid payment amount!\n";
      return;
   }
   if (amount+balance >= fineAmount) {
      cout << "Fine fully paid! Remaining balance: Rs. " << (amount-fineAmount+balance) << "\n";
      balance += (amount - fineAmount);
      string x;
      if(amount<fineAmount)
      x = user->getName()+" paid Rs. "+to_string(amount)+" and used their balance to pay their fine of Rs. "+\
      to_string(fineAmount)+"\nThe remaining balance is Rs. "+to_string(balance)+"\n";
      else x = user->getName()+" paid Rs. "+to_string(amount)+" to pay their fine of Rs. "+\
      to_string(fineAmount)+"\nThe remaining balance is Rs. "+to_string(balance)+"\n";
      save_to_file(x,"LibraryRecords/transactions.txt","Full Fine Payment");
      fineAmount = 0;
      
   } else {
      fineAmount -= (amount+balance);
      cout << "Partial payment successful! Remaining fine: Rs. " << fineAmount << "\n"
      "You have no remaining balance.\n";
      cout << "Partial payment using balance successful! Remaining fine: Rs. " << fineAmount << "\n";
      string x = user->getName()+" paid Rs. "+to_string(amount)+" and used all of their balance(Rs. "+to_string(balance)\
      +") to pay their fine partially.\nThe remaining fine is Rs. "+to_string(fineAmount)+"\n";
      save_to_file(x,"LibraryRecords/transactions.txt","Partial Fine Payment");
      balance=0;
   }
}
int Account::getFineAmount() const {
   return fineAmount;
}
void Account::simulateFinePayment() {
   Library* library = Library::getInstance();
   if(library->UsersRole[user]!="Student"){
      cout<<"Fine Payment is only for Students\n";
      return;
   }
   while (true) {
      cout << "\n=== Library Fine Payment System ===\n"
      "1. View Fine\n2. Pay Fine\n3. Add to Wallet\n4. Exit\nEnter choice(1/2/3/4): ";
      int choice;
      cin >> choice;
      if (cin.fail()) {
         cin.clear();          //***BEST APPROACH FOR ERROR HANDLING TILL DATE----YAY
         cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Discard invalid input
         cout << "\nInvalid input. Please enter a valid choice (1/2/3/4).\n";
         continue;
     }
      if(choice!=1&&choice!=2&&choice!=3&&choice!=4){
         cout<<"Invalid Choice!\n";continue;
      }
      if (choice == 4) break;
      if(choice==3){
         cout << "Enter amount to pay: ";
         int amount;
         cin >> amount;
         if (cin.fail()) {
            cin.clear();          //***BEST APPROACH FOR ERROR HANDLING TILL DATE----YAY
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Discard invalid input
            cout << "\nInvalid input. Please enter a valid amount.\n";
            continue;
         }
         this->addToBalance(amount);
         continue;
      }
      this->viewFine();
      if(choice==2){
         if (this->getFineAmount() == 0) {
            cout << "No outstanding fines!\n";
            return;
         }
         cout<<"Do you want to:\n"
            "1.Pay the fine using only your balance, without giving any additional money?\n"
            "2.Pay the fine using both your balance and additional money?\nEnter choice(1/2): ";
         while(true){
            cin >> choice;
            if (cin.fail()) {
               cin.clear();          //***BEST APPROACH FOR ERROR HANDLING TILL DATE----YAY
               cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Discard invalid input
               cout << "\nInvalid input. Please enter a valid choice (1/2).\n";
               continue;
            }
            if(choice==1||choice==2)break;
            else cout<<"Invalid Choice\nEnter choice(1/2): ";
         }
         if(choice==1) this->payFine();
         else if(choice==2){
            cout << "Enter amount to pay: ";
            int amount;
            cin >> amount;
            if (cin.fail()) {
               cin.clear();          //***BEST APPROACH FOR ERROR HANDLING TILL DATE----YAY
               cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Discard invalid input
               cout << "\nInvalid input. Please enter a valid amount.\n";
               continue;
            }
            this->payFine(amount);
         }
      }
   }
}