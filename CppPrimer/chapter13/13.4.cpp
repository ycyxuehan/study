#include <string>
#include <set>

using namespace std;

class Folder
{
    friend class Message;
private:
    string name;
    set<Message *> messages;

public:
    void addMsg(Message *);
    void remMsg(Message *);
    Folder(const Folder&);
    explicit Folder(const string &str="default"):name(str){}
    Folder &operator=(const Folder&);
    ~Folder();    
};

Folder::Folder(const Folder& f):name(f.name)
{

}

class Message
{
    friend class Folder;
    friend void swap(Message &, Message &);

public:
    explicit Message(const string &str = "") : contents(str) {}
    Message(const Message &);
    Message &operator=(const Message &);
    ~Message();
    void save(Folder &);
    void remove(Folder &);
    void move_Folders(Message *m);
    Message(Message &&m):contents(std::move(m.contents)){move_Folders(&m);}
    Message& Message::operator=(Message &&rhs);
private:
    string contents;
    set<Folder *> folders;
    void add_to_Folders(const Message &);
    void remove_from_Folders();
};

void Message::save(Folder &f)
{
    folders.insert(&f);
    f.addMsg(this);
}

void Message::remove(Folder &f)
{
    folders.erase(&f);
    f.remMsg(this);
}

void Message::add_to_Folders(const Message &m)
{
    for (auto f : m.folders)
        f->addMsg(this);
}

Message::Message(const Message &m) : contents(m.contents), folders(m.folders)
{
    add_to_Folders(m);
}

void Message::remove_from_Folders()
{
    for (auto f : folders)
        f->remMsg(this);
}

Message::~Message()
{
    remove_from_Folders();
}

Message &Message::operator=(const Message &rhs)
{
    //通过先删除指针再插入它们来处理自赋值情况
    remove_from_Folders();
    contents = rhs.contents;
    folders = rhs.folders;
    add_to_Folders(rhs);
    return *this;
}

void swap(Message &lhs, Message &rhs)
{
    using std::swap;
    for (auto f : lhs.folders)
        f->remMsg(&lhs);
    for (auto f : rhs.folders)
        f->remMsg(&rhs);
    swap(lhs.folders, rhs.folders);
    swap(lhs.contents, rhs.contents);
    for (auto f : lhs.folders)
        f->addMsg(&lhs);
    for (auto f : rhs.folders)
        f->addMsg(&rhs);
}

void Message::move_Folders(Message *m)
{
    folders = std::move(m->folders);
    for(auto f:folders){
        f->remMsg(m);
        f->addMsg(this);
    }
    m->folders.clear();
}

Message& Message::operator=(Message &&rhs)
{
    if(this != &rhs)
    {
        remove_from_Folders();
        contents = std::move(rhs.contents);
        move_Folders(&rhs);
    }
    return *this;
}