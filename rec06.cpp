/*
  Nathanael Leyton
  Recitation 06
  CS2124
 
  Focus: Dynamic arrays and copy control
 */

#include <string>
#include <iostream>
using namespace std;

class Position {
    friend ostream& operator<<(ostream& os, const Position& rhs) {
        os << '[' << rhs.title << ", " << rhs.salary << ']';
        return os;
    }
    private:
        string title;
        double salary;
    public:
        Position(const string& aTitle, double aSalary)
                : title(aTitle), salary(aSalary) {}
        const string& getTitle() const { return title; }
        double getSalary() const { return salary; }
        void changeSalaryTo(double d) { salary = d; }
};

class Entry {
    friend ostream& operator<<(ostream& os, const Entry& rhs) {
        os << rhs.name << ' ' << rhs.room
           << ' ' << rhs.phone << ", " << *rhs.pos;
        return os;
    }
    private:
        string name;
        unsigned room;
        unsigned phone;
        Position* pos;
    public:
        Entry(const string& name, unsigned room, unsigned phone, Position& position)
                : name(name), room(room), phone(phone), pos(&position) {
        }
        const string& getName() const { return name; }
        const unsigned& getPhone() const { return phone; }
};

class Directory {
    // Overload output operator
    friend ostream& operator<<(ostream& os, const Directory& rhs){
        for (size_t i = 0; i < rhs.size; i++){
            os << *rhs.entries[i] << endl;
        }
        return os;
    }
    private:
        Entry** entries;
        size_t size;
        size_t capacity;
    public:
        ~Directory() {
            for (size_t i = 0; i < size; i++){
                delete entries[i];
            }
            delete[] entries;
        }
        Directory ():size(0), capacity(4) { entries = new Entry*[capacity]; }
        Directory(const Directory& aDirectory) : size(aDirectory.size), capacity(aDirectory.capacity){
            entries = new Entry*[aDirectory.capacity];
            for (size_t i = 0; i < aDirectory.size; i++){
                Entry* aEntry = new Entry(*aDirectory.entries[i]);
                entries[i] = aEntry;
            }
        }
        Directory& operator=(Directory& rhs){
            // Compare addresses to make sure there not the same.
            if (this != &rhs){
                for (size_t i = 0; i < size; i++){
                    delete entries[i];
                }
                delete[] entries;
                entries = new Entry*[rhs.capacity];
                size = rhs.size;
                capacity = rhs.capacity;
                for (size_t i = 0; i < rhs.size; i++){
                    Entry* aEntry = new Entry(*rhs.entries[i]);
                    entries[i] = aEntry;
                }
            }
            return *this;
        }
        void add(const string& name, unsigned room, unsigned ph, Position& pos) {
            if (size == capacity)	{
                Entry** oldEntries = entries;
                entries = new Entry*[2 * capacity];
                for (size_t i = 0; i < size; i++){
                    entries[i] = oldEntries[i];
                }
                delete[] oldEntries;
            }
            entries[size] = new Entry(name, room, ph, pos);
            ++size;
        }

        unsigned operator[](const string& name) const {
            for (size_t i = 0; i < size; i++){
                if (entries[i]->getName() == name){
                    return entries[i]->getPhone();
                }
            }
            return 55555;
        }
};
void doNothing(Directory dir) { cout << dir << endl; }
int main() {

    // Note that the Postion objects are NOT on the heap.
    Position boss("Boss", 3141.59);
    Position pointyHair("Pointy Hair", 271.83);
    Position techie("Techie", 14142.13);
    Position peon("Peonissimo", 34.79);

    // Create a Directory
    Directory d;
    d.add("Marilyn", 123, 4567, boss);
    cout << d << endl;

    Directory d2 = d;	// What function is being used??
    d2.add("Gallagher", 111, 2222, techie);
    d2.add("Carmack", 314, 1592, techie);
    cout << d << endl;

    cout << "Calling doNothing\n";
    doNothing(d2);
    cout << "Back from doNothing\n";

    Directory d3;
    d3 = d2;

    // Should display 1592
    cout << d2["Carmack"] << endl;

}