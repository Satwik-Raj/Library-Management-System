#include "classes.h"

//User Functions' Implementations

string User::getName() const {
   return this->name;
}
string User::getUID() const {
   return this->UID;
}
void User::editProfile() {
   cout << "Edit Name (Current: " << name << "): ";
   if (cin.peek() == '\n') cin.ignore();//AWESOME :)
   getline(cin,name);
   cout << "Edit Age (Current: " << age << "): ";
   while(true){
      cin >> age;
      if (cin.fail()) {
         cin.clear();          //***BEST APPROACH FOR ERROR HANDLING TILL DATE----YAY
         cin.ignore(numeric_limits<streamsize>::max(), '\n');
         cout << "\nInvalid input. Please enter a valid age:\n";
      }else break;
   }
}
int User::getAge() const {
   return this->age;
}
// 2nd const is necessary for calling it on a const Book1, else it will give error
// it ensures "this" is not modified inside
bool User::operator<(const User& other) const {
   return this->UID < other.UID;
}
// void User::saveUser(ofstream &out) {
//    out << UID << " " << name << " " << age << endl;
// }

// void User::loadUser(ifstream &in) {
//    in >> UID >> name >> age;
// }
// void User::borrowBook(Book& book){
//    cout<<"Borrow Request for "<<book.title<<" denied.\n"
//    "User trying to borrow (maybe some issue) CHECK.\n";
// }
// void User::returnBook(Book& book){
//    cout<<"Return Request for "<<book.title<<" denied.\n"
//    "User trying to return (maybe some issue) CHECK.\n";
// }
// void User::reserveBook(Book& book){
//    cout<<"Reserve Request for "<<book.title<<" denied.\n"
//    "User trying to reserve (maybe some issue) CHECK.\n";
// }
User::User(string UID,string name,int age){
   this->UID = UID;
   this->name = name;
   this->age = age;
   this->account = new Account(this);
   // cout<<"Constructor of user class\n";
}
User::User(const User& user){
   this->UID = user.UID;
   this->name = user.name;
   this->age = user.age;
   this->account = new Account(&(user));
}
User::~User(){
   // cout<<"Destructor of user class called\n";
   delete account;
   account = nullptr;
}
