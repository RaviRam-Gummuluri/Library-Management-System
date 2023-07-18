#include <ctime>
#include <iostream>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <dos.h>
#include <bits/stdc++.h>

using namespace std;

void gotoxy(int x, int y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

int seats = 200;
struct BookNode {
    int id;
    string title;
    string author;
    int year;
    int quantity;
    float rating;
    int cnt;
    BookNode* left;
    BookNode* right;
    BookNode(int id, string title, string author, int year, int quantity, float rating)
        : id(id), title(title), author(author), year(year), quantity(quantity), rating(rating),cnt(1),
          left(nullptr), right(nullptr) {}
};

queue<int> complaints_queue;
queue<int> adv;

struct AdminNode {
    int id;
    int pass;
    string name;
    AdminNode* left;
    AdminNode* right;
    AdminNode(int id, int pass, string name)
        : id(id), pass(pass), name(name),left(nullptr), right(nullptr){}
};

struct UserNode {
    int id;
    int pass;
    string name;
    int book1;
    int book2;
    bool ostat;
    int flag;
    int num;
    string cstat;
    UserNode* left;
    UserNode* right;
    string t1;
    string t2;
    UserNode(int id, int pass, string name, int num, int book1, int book2 )
        : id(id), pass(pass), name(name),book1(book1),book2(book2),ostat(false),flag(0),num(num),cstat("\0"),left(nullptr), right(nullptr), t1("\0"),t2("\0"){}
};

std::map<int , int> users;
std::map<int , struct UserNode*> userinfo;
std::map<int , int> admins;
std::map<int , struct AdminNode*> Admininfo;
std::map<int , string> complaints;
std::map<int , string> advance_bookings;
std::map<string , int> bookt;

class BookBST {
private:
    BookNode* root;
    void insert(BookNode*& node, int id, string title, string author, int year, int quantity, float rating) {
        if (node == nullptr) {
            node = new BookNode(id, title, author, year, quantity,rating);
        } else if (id < node->id) {
            insert(node->left, id, title, author, year, quantity,rating);
        } else if (id > node->id) {
            insert(node->right, id, title, author, year, quantity,rating);
        }

    }
     void InOrderTraversal(BookNode* node) {
        if (node == nullptr) {
            return;
        }
        InOrderTraversal(node->left);
        cout << "Book ID  : " << node->id << endl;
        cout << "Title    : " << node->title << endl;
        cout << "Author   : " << node->author << endl;
        cout << "Year     : " << node->year << endl;
        cout << "Quantity : " << node->quantity << endl;
        cout << "Rating   : " << node->rating << endl << endl << endl;
        InOrderTraversal(node->right);
    }
    BookNode* Delete(BookNode* node, int id) {
        if (node == nullptr) {
            return nullptr;
        }
        if (id < node->id) {
            node->left = Delete(node->left, id);
        }
        else if (id > node->id) {
            node->right = Delete(node->right, id);
        }
        else
        {
            if (node->left == nullptr && node->right == nullptr) {
                delete node;
                return nullptr;
            }
            else if (node->left == nullptr) {
                BookNode* right = node->right;
                delete node;
                return right;
            }
            else if (node->right == nullptr) {
                BookNode* left = node->left;
                delete node;
                return left;
            }
            else {
                BookNode* min_node = FindMinNode(node->right);
                node->id = min_node->id;
                node->right = Delete(node->right, min_node->id);
            }
        }
        return node;
    }

    BookNode* FindMinNode(BookNode* node) {
        while (node->left != nullptr) {
            node = node->left;
        }
        return node;
    }
    void saveBooksToFile(BookNode* node) {
        if (node == nullptr) return;
        saveBooksToFile(node->left);
        ofstream booksFile("books.csv", ios::app);
        booksFile << node->id << "," << node->title << "," << node->author << ","
                  << node->year << "," << node->quantity << "," << node->rating<< endl;
        booksFile.close();

        saveBooksToFile(node->right);
    }

public:

    BookBST() : root(nullptr) {}
    void insert(int id, string title, string author, int year, int quantity, float rating) {
        insert(root, id, title, author, year, quantity,rating);
    }
    void saveBooks() {
        saveBooksToFile(root);
    }
    void Delete(int id) {
        root = Delete(root, id);
    }
    void InOrderTraversal() {
        InOrderTraversal(root);
    }
    BookNode* Search(int id) {
        BookNode* current = root;
        while (current != nullptr) {
            if (id == current->id) {
                return current;
            } else if (id < current->id) {
                current = current->left;
            } else {
                current = current->right;
            }
        }
        return nullptr;
    }
};


class UserBST {
private:
    UserNode* root;
    void insert(UserNode*& node, int id, int pass, string name, int num, int book1, int book2) {
        if (node == nullptr) {
            node = new UserNode(id, pass, name, num, book1, book2);
        } else if (id < node->id) {
            insert(node->left, id, pass,name, num, book1, book2);
        } else if (id > node->id) {
            insert(node->right, id, pass, name, num, book1, book2);
        }

    }
    void saveUsersToFile(UserNode* node) {
        if (node == nullptr) return;
        saveUsersToFile(node->left);
        ofstream usersFile("users.csv", ios::app);
        usersFile << node->id << "," << node->pass << "," << node->name<< "," << node->num << "," << node->book1 << "," << node->book2<< endl;
        usersFile.close();
        saveUsersToFile(node->right);
    }

public:
    UserBST() : root(nullptr) {}
    void insert(int id, int pass, string name, int num, int book1, int book2) {
        insert(root, id, pass, name, num, book1, book2);
    }
    void saveUsers() {
        saveUsersToFile(root);
    }

    UserNode* Search(int id) {
        UserNode* current = root;
        while (current != nullptr) {
            if (id == current->id) {
                return current;
            } else if (id < current->id) {
                current = current->left;
            } else {
                current = current->right;
            }
        }
        return nullptr;
    }
};

class AdminBST {
private:
    AdminNode* root;
    void insert(AdminNode*& node, int id, int pass, string name) {
        if (node == nullptr) {
            node = new AdminNode(id, pass, name);
        } else if (id < node->id) {
            insert(node->left, id, pass,name);
        } else if (id > node->id) {
            insert(node->right, id, pass, name);
        }

    }
    void saveAdminsToFile(AdminNode* node) {
        if (node == nullptr) return;

        saveAdminsToFile(node->left);

        ofstream AdminsFile("admins.csv", ios::app);
        AdminsFile << node->id << "," << node->pass << "," << node->name<< endl;
        AdminsFile.close();
        saveAdminsToFile(node->right);
    }

public:
    AdminBST() : root(nullptr) {}
    void insert(int id, int pass, string name) {
        insert(root, id, pass, name);
    }
    void saveAdmins() {
        saveAdminsToFile(root);
    }

    AdminNode* Search(int id) {
        AdminNode* current = root;
        while (current != nullptr) {
            if (id == current->id) {
                return current;
            } else if (id < current->id) {
                current = current->left;
            } else {
                current = current->right;
            }
        }
        return nullptr;
    }
};


BookBST book;
UserBST user;
AdminBST admin;

void addBook()
{
    cout << "Enter book ID: ";
    int id;
    cin >> id;
    BookNode* book_node=book.Search(id);
    if(book.Search(id)!=nullptr)
    {
        cout << "book already exists."<<endl;
        (book_node->quantity)+=1;
        cout << "So, quantity has been updated."<<endl;
        return;
    }
    cout << "Enter book title: ";
    string title;
    cin >> title;

    cout << "Enter book author: ";
    string author;
    cin >> author;

    cout << "Enter book year: ";
    int year;
    cin >> year;

    cout << "Enter book quantity: ";
    int quantity;
    cin >> quantity;

    cout << "Enter book rating (scale of 5) :";
    int rating;
    cin >> rating;

    book.insert(id, title, author, year, quantity, rating);
    cout << "Book added successfully !" <<endl;
}

void addUser()
{
    cout << "Enter ID: ";
    int id;
    cin >> id;
    if(user.Search(id)!=nullptr)
    {
        cout << "User with ID already exists."<<endl;
        return;
    }

    cout << "Set a PIN: ";
    int pin;
    cin >> pin;

    cout << "Enter name: ";
    char name[100];
    fflush(stdin);
    cin.getline (name,100);

    user.insert(id, pin, name, 0, 0, 0);
    users[id]=pin;
    userinfo[id]=user.Search(id);
    cout << "User added successfully !" <<endl;
}

void delBook(){
    int id;
    cout<<"Enter the book-ID to be removed : ";
    cin>>id;
    if (book.Search(id)==nullptr)
    {
        cout << "The book with id "<<id << " doesn't exist." <<endl;
        return;
    }
    book.Delete(id);
    cout << "The book with id "<<id << " deleted successfully !" <<endl;
}

string book_time_stamp(){
    time_t t ;
	struct tm *tmp ;
	char MY_TIME[20];
	time( &t );
	tmp = localtime( &t );
	strftime(MY_TIME, sizeof(MY_TIME), "%x - %I:%M%p", tmp);
    return MY_TIME;
}

void issuebook(int id){
    int id_req;
    cout<<"Enter the book id: ";
    cin>>id_req;
    if (book.Search(id_req)==nullptr||book.Search(id_req)->quantity==0)
    {
        cout << "requested book ID is unavailable, Please try later..." <<endl;
        return;
    }
    if (userinfo[id]->book1==id_req || userinfo[id]->book2==id_req)
    {
        string t;
        if(userinfo[id]->book1==id_req)
            t=userinfo[id]->t1;
        else
            t=userinfo[id]->t2;
        cout << "You have already issued this book ";
        if (t!="\0")
            cout << "on "<<t;
        cout << endl;
        return;
    }
    if((userinfo[id]->num) >= 2){
        cout<<"You have already issued 2 books. Can't issue more. "<<endl;
    }
    else{

        if(userinfo[id]->num==0)
        {
            userinfo[id]->book1 = book.Search(id_req)->id;
            cout << "book with id "<<id_req<< " issued successfully"<<endl;
            cout << "You have issued 1 book as of now." << endl;
            userinfo[id]->t1 = book_time_stamp();

            cout << "Issued on: " << userinfo[id]->t1<< endl;
            book.Search(id_req)->quantity--;
            userinfo[id]->num++;
        }
        else
        {
            userinfo[id]->book2 = book.Search(id_req)->id;
            cout << "book with id "<<id_req<< " issued successfully"<<endl;
            cout << "You have issued 2 books as of now." << endl;

            userinfo[id]->t2 = book_time_stamp();
            cout << "Issued on: "<< userinfo[id]->t2 << endl;
            book.Search(id_req)->quantity--;
            userinfo[id]->num++;
        }
    }
}

void viewComplaints()
{
    if(complaints_queue.empty())
    {
        cout<<"No complaints/issues to be resolved as of now.."<<endl<<endl;
        return;
    }
    cout << "ID       "<<"Issue/Complaint raised"<<endl;
    queue<int> dup_complaints_queue=complaints_queue;
    while(!dup_complaints_queue.empty())
    {
        cout <<dup_complaints_queue.front()<< "    ";
        cout<<complaints[dup_complaints_queue.front()]<<endl;
        dup_complaints_queue.pop();
    }
}

void viewAdvance()
{
    if(adv.empty())
    {
        cout << "No advance bookings to view as of now.."<<endl<<endl;
        return;
    }
    queue<int> dup_advance_queue=adv;
    while(!dup_advance_queue.empty())
    {
        cout << endl << dup_advance_queue.front()<< "    ";
        cout<<advance_bookings[dup_advance_queue.front()]<<endl;;
        dup_advance_queue.pop();
    }
}
void view_issues(int id){
    switch(userinfo[id]->num){
        case 0:
            cout<<"No books issued as of now."<<endl;
            break;
        case 1:
            cout<<"Only one book is issued as of now."<<endl;

            cout<<"Book 1 details are: "<<endl;
            cout<<"Book Title  : "<<book.Search(userinfo[id]->book1)->title<<endl;
            cout<<"Book Author : "<<book.Search(userinfo[id]->book1)->author<<endl;
            cout<<"Book ID     : "<<book.Search(userinfo[id]->book1)->id<<endl<<endl<<endl;

            break;
        case 2:
            cout<<"Two books are issued as of now. "<<endl;

            cout<<"Book 1 details are: "<<endl;
            cout<<"Book Title  : "<<book.Search(userinfo[id]->book1)->title<<endl;
            cout<<"Book Author : "<<book.Search(userinfo[id]->book1)->author<<endl;
            cout<<"Book ID     : "<<book.Search(userinfo[id]->book1)->id<<endl<<endl;

            cout<<"Book 2 details are: "<<endl;
            cout<<"Book Title  : "<<book.Search(userinfo[id]->book2)->title<<endl;
            cout<<"Book Author : "<<book.Search(userinfo[id]->book2)->author<<endl;
            cout<<"Book ID     : "<<book.Search(userinfo[id]->book2)->id<<endl<<endl<<endl;
            break;
    }
}
void complaint(int id){
    char cmp[100];
    fflush(stdin);
    cout << "Enter the complaint (less than 100 characters) : ";
    cin.getline (cmp,100);
    complaints_queue.push(id);
    complaints[id]=cmp;
    cout<<"Sure! We will Work on Your Complaint."<<endl;
}
void start()
{
    system("cls");
    gotoxy(35,11);
    cout<<"LIBRARY";
    gotoxy(35,14);
    cout<<"MANAGEMENT";
    gotoxy(35,17);
    cout<<"SYSTEM";
    getch();
}

void rating(int uid)
{
    float r;
    int id;
    cout << "Enter book ID : " ;
    cin >> id;
    if(book.Search(id)==nullptr)
    {
        cout << "The book with ID " <<id<<" doesn't exist..."<<endl;
        return;
    }
    cout << "Rate the book !\n ";
    cin >> r;
    float ri = book.Search(id)->rating;
    int c = book.Search(id)->cnt;
    book.Search(id)->rating = (ri * c + r)/(c+1);
    book.Search(id)->cnt++;
    cout << "Fun fact ! "<<c+1<<" users rated this book including you.. and ts rating stands at "<<book.Search(id)->rating<<" now."<<endl;
}

void advance_booking(int id)
{

    if(seats){
        seats--;
        cout << "You have been allotted a seat. Report in 5 minutes orelse it would be deallotted." <<endl;
        userinfo[id]->ostat=true;
        adv.push(id);
        advance_bookings[id]=book_time_stamp();
    }
    else{
        cout<<"No vacant seats..."<<endl;
    }
}

int main()
{
    cout << "loading requirements ...."<<endl;
    ifstream booksFile("books.csv");
    string line;
    while (getline(booksFile, line)) {
        stringstream lineStream(line);
        string cell;

        int id;
        string title;
        string author;
        int year;
        int quantity;
        float rating;

        getline(lineStream, cell, ',');
        id = stoi(cell);
        getline(lineStream, title, ',');
        getline(lineStream, author, ',');
        getline(lineStream, cell, ',');
        year = stoi(cell);
        getline(lineStream, cell, ',');
        quantity = stoi(cell);
        getline(lineStream, cell, ',');
        rating = stof(cell);
        book.insert(id, title, author, year, quantity, rating);
        bookt[title]=id;
    }
    booksFile.close();

    ifstream usersFile("users.csv");
    string uline;
    while (getline(usersFile, uline)) {
        stringstream lineStream(uline);
        string cell;

        int id;
        int pass;
        string name;
        int num;
        int book1;
        int book2;

        getline(lineStream, cell, ',');
        id = stoi(cell);
        getline(lineStream, cell, ',');
        pass = stoi(cell);
        getline(lineStream, name, ',');
        getline(lineStream, cell, ',');
        num = stoi(cell);
        getline(lineStream, cell, ',');
        book1 = stoi(cell);
        getline(lineStream, cell, ',');
        book2 = stoi(cell);
        users[id]=pass;
        user.insert(id, pass, name, num, book1, book2);
        userinfo[id]=user.Search(id);
    }
    usersFile.close();

    ifstream adminsFile("admins.csv");
    string aline;
    while (getline(adminsFile, aline)) {
        stringstream lineStream(aline);
        string cell;
        int id;
        int pass;
        string name;

        getline(lineStream, cell, ',');
        id = stoi(cell);
        getline(lineStream, cell, ',');
        pass = stoi(cell);
        getline(lineStream, name, ',');
        admins[id]=pass;
        admin.insert(id, pass, name);
        Admininfo[id]=admin.Search(id);
    }
    adminsFile.close();
    users[18]=2003;
    int pc=0;
    system("cls");
    Sleep(200);
    start();
    while (pc!=3)
    {
        fflush(stdin);
        getchar();
        system("cls");
        cout << "1. Admin Login" << endl;
        cout << "2. User Login" << endl;
        cout << "3. Exit" << endl;
        cout << "Enter your choice: ";
        cin>>pc;
        system("cls");
        switch(pc)
        {
        case 1 :
            cout << "Enter your ID: ";
            int id;
            cin >> id;

            if(admins.find(id)==admins.end())
            {
                cout << "Admin with such ID doesn't exist, try again..." <<endl;
                break;
            }

            cout << "Enter your password: ";
            int apass;
            cin >> apass;
            if (admins[id]!=apass)
            {
                cout << "Invalid ID-Pass." <<endl;
                break;
            }

            if (admins.find(id) != admins.end() && admins[id] == apass)
            {
                cout << "Welcome, " <<Admininfo[id]->name<<"!"<< endl;
                int sc=10;
                while (sc!=9)
                {
                    cout << "0. Register new User"<<endl;
                    cout << "1. Add Book" << endl;
                    cout << "2. Delete Book" << endl;
                    cout << "3. View All Books" << endl;
                    cout << "4. view complaints/issues" << endl;
                    cout << "5. Offline access" << endl;
                    cout << "6. View user details." << endl;
                    cout << "7. Collect book from user"<<endl;
                    cout << "8. search book by name"<<endl;
                    cout << "9. Logout" << endl<<endl;
                    cout << "Enter your choice: ";
                    cin>>sc;
                    system("cls");
                    switch(sc)
                    {
                    case 0:
                        addUser();
                        break;
                    case 1:
                        addBook();
                        break;
                    case 2:
                        delBook();
                        break;
                    case 3:
                        book.InOrderTraversal();
                        break;
                    case 4:
                        viewComplaints();
                        if(!complaints_queue.empty()){
                            cout<<"\nWould you like to resolve the issue?"<<endl;
                            cout<<"enter 1 to resolve the issue : ";
                            int c5;
                            cin>>c5;
                            if(c5==1)
                            {
                                cout<<"ID of student whose complaint is to be resolved is "<<complaints_queue.front()<<endl;
                                int id;
                                id=complaints_queue.front();
                                char res[100];
                                cout << "enter the feedback : ";
                                fflush(stdin);
                                cin.getline (res,100);
                                userinfo[id]->cstat=res;
                                complaints_queue.pop();
                            }
                        }
                        break;
                    case 5:
                        cout << "1. student login" << endl;
                        cout << "2. student logout" << endl;
                        cout << "3. View advance bookings" <<endl;
                        cout << "Enter a choice : ";
                        int c6;
                        cin>>c6;
                        if(c6==1)
                        {
                            int id6;
                            cout<<"Enter ID of student : ";
                            cin>>id6;
                            userinfo[id6]->ostat=true;
                            cout << "seat alloated successfully for student-ID "<<id6<<endl;
                            seats--;
                            cout << "Remaining seats are "<<seats<<endl;
                        }
                        else if(c6==2)
                        {
                            int id6;
                            cout<<"Enter ID of student : ";
                            cin>>id6;
                            if(userinfo[id6]->ostat){
                                userinfo[id6]->ostat=false;
                                cout << "seat deallocated successfully for student-ID "<<id6<<endl;
                                seats++;
                                cout << "Remaining seats are "<<seats<<endl;
                            }
                            else{
                                cout<<"This user did not log in to offline library."<<endl;
                            }
                        }
                        else if(c6==3)
                        {
                            viewAdvance();
                            if(!adv.empty()){
                                cout << "Enter 1 to invalidate the booking of "<<adv.front()<<" (Press any key to continue)"<<endl;
                                int i;
                                cin>>i;
                                if(i==1)
                                {
                                    advance_bookings.erase(adv.front());
                                    userinfo[adv.front()]->ostat=false;
                                    seats++;
                                    adv.pop();
                                }
                            }
                        }
                        else
                            cout<<"Invalid Input"<<endl;
                        break;
                    case 6:
                        cout << "enter the user ID whose details to see - ";
                        int cid;
                        cin>>cid;
                        if(users.find(cid)==users.end())
                        {
                            cout << "User with such ID doesn't exist, try again..." <<endl;
                            break;
                        }
                        cout << "Details of user with ID " << cid << " are :"<<endl<<endl;
                        cout << "Name - " << userinfo[cid]->name<<endl;;
                        cout << "no. of books issued - "<<userinfo[cid]->num<<endl;
                        if(userinfo[cid]->num==1)
                            cout<<"issued book ID is "<< userinfo[cid]->book1<<endl<<endl<<endl;
                        else if(userinfo[cid]->num==2)
                            cout << "issued book IDs are "<<userinfo[cid]->book1<<" and "<<userinfo[cid]->book2<<endl<<endl<<endl;
                    case 7:
                        cout << "Enter student ID - ";
                        int rid;
                        cin>>rid;
                        int bid;
                        if(users.find(rid)==users.end())
                        {
                            cout << "User with such ID doesn't exist, try again..." <<endl;
                            break;
                        }
                        else if(userinfo[rid]->num==0){
                            cout<<"He has't issued any books yet.."<<endl;
                            break;
                        }
                        else if(userinfo[rid]->num==1){
                            cout << "Book with ID "<<userinfo[rid]->book1<< " returned sucessfully.";
                            userinfo[rid]->book1=0;
                            userinfo[rid]->num-=1;
                            book.Search(bid)->quantity++;
                            cout << "Catalogue updated successfuly..."<<endl;
                            break;
                        }
                        cout << "he has books with IDs "<<userinfo[rid]->book1<<" and "<<userinfo[rid]->book2<< " with him."<<endl;
                        cout << "enter book ID to be returned - ";
                        cin>>bid;
                        if(userinfo[rid]->book1!=bid&&userinfo[rid]->book2!=bid)
                        {
                            cout << "Book with such ID doesnt exist with him.";
                            break;
                        }
                        else if(userinfo[rid]->book1==bid)
                        {
                            cout << "Book with ID "<<userinfo[rid]->book1<< " returned sucessfully.";
                            userinfo[rid]->book1=0;
                            userinfo[rid]->num--;
                            book.Search(bid)->quantity++;
                            cout << "Catalogue updated successfuly..."<<endl;
                            break;
                        }
                        else
                        {
                            cout << "Book with ID "<<userinfo[rid]->book1<< " returned sucessfully.";
                            userinfo[rid]->book2=0;
                            userinfo[rid]->num--;
                            book.Search(bid)->quantity++;
                            cout << "Catalogue updated successfuly..."<<endl;
                            break;
                        }
                    case 8:
                        cout << "Enter the book title to search : " ;
                        char title[100];
                        fflush(stdin);
                        cin.getline(title,100);
                        if(bookt.find(title) == bookt.end())
                        {
                            cout << "Book with such title doesn't exist." <<endl<<endl;
                            break;
                        }
                        cout << "The ID of book with title "<<title<<" is " <<bookt[title]<<endl<<endl;
                    case 9:
                        cout<<"Admin logged out!"<<endl;
                        break;
                    default:
                        cout<<"Invalid Input !"<<endl;
                    }
                }
                break;
            }
        case 2 :
            cout << "Enter your ID: ";
            int uid;
            cin >> uid;
            if(users.find(uid)==users.end())
            {
                cout << "User with such ID doesn't exist, try again..." <<endl;
                break;
            }
            cout << "Enter your password: ";
            int upass;
            cin >> upass;
            if (users[uid]!=upass)
            {
                cout << "Invalid ID-Pass." <<endl;
                break;
            }

            if (users.find(uid) != users.end() && users[uid] == upass)
            {
                cout << "Welcome, " <<userinfo[uid]->name<<"!"<< endl;
                int sc=0;

                while(sc!=8)
                {
                    cout << endl;
                    cout << "1. Issue Book" << endl;
                    cout << "2. View Issued books and status" << endl;
                    cout << "3. View All Books" << endl;
                    cout << "4. submit complaint/feedback" << endl;
                    cout << "5. View Complaint Status" << endl;
                    cout << "6. Rate a book" <<endl;
                    cout << "7. Offline access" << endl;
                    cout << "8. Logout" << endl << endl;
                    cout << "Enter your choice: ";
                    cin>>sc;
                    system("cls");
                    switch(sc)
                    {
                    case 1 :
                        if (userinfo[uid]->num>2)
                        {
                            cout<<"You can have atmost 2 books at a time. Return a book to order another."<<endl;
                            break;
                        }
                        issuebook(uid);
                        break;
                    case 2 :
                        view_issues(uid);
                        break;
                    case 3 :
                        book.InOrderTraversal();
                        break;
                    case 4 :
                        complaint(uid);
                        break;
                    case 5 :
                        if (complaints.find(uid) == complaints.end())
                        {
                            cout<< "You didn't raise any issues as of now." <<endl;
                            break;
                        }
                        cout<<"Your complaint was - "<<complaints[uid]<<endl;
                        if (userinfo[uid]->cstat=="\0")
                        {
                            cout << "Your issue is not yet resolved.It would be addressed soon." << endl << endl;
                            break;
                        }
                        cout<<"Your issue is resolved, and feedback recieved is - "<<userinfo[uid]->cstat<<endl;
                        break;
                    case 6:
                        rating(uid);
                        break;
                    case 7 :
                        int c7;
                        cout<<"1. Check seat vacancies"<<endl;
                        cout<<"2. Advance seat booking"<<endl;
                        cout<<"Enter the choice : ";
                        cin>>c7;
                        if(c7==1)
                            cout<<"There are "<<seats<<" vacant in library now."<<endl;
                        else if(c7==2)
                        {
                            if(userinfo[uid]->ostat)
                            {
                                cout << "You already have a seat allotted in library." <<endl;
                                cout << "If not reported, report offline immediately to keep it allotted, orelse it would be dellotted."<<endl;
                            }
                            advance_booking(uid);
                        }
                        else
                            cout<<"Invalid Input"<<endl;
                        break;
                    case 8 :
                        cout<<"User logged out!"<<endl;
                        break;
                    default :
                        cout<<"Invalid Input !"<<endl;
                    }
                }
                break;
            }
        }
    }
    cout << "exited from application successfully !"<<endl;
    system("cls");
    cout<<"Saving changes ...."<<endl;
    Sleep(3000);
    ofstream bookFile("books.csv", ios::trunc);
    bookFile.close();
    ofstream userFile("users.csv", ios::trunc);
    userFile.close();
    ofstream adminFile("admins.csv", ios::trunc);
    adminFile.close();
    book.saveBooks();
    user.saveUsers();
    admin.saveAdmins();
    system("cls");
    cout << "Application closed..." <<endl;
    return 0;
}


