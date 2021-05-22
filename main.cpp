#include <iostream>

using namespace std;

template<typename T>
class Node {
public:
    T data;
    int deleted;

    Node(const T &data) : data(data), deleted(0) {};
};

template<typename T>
class HashTable {
    int bufferSize;
    int size;
    Node<T> **nodes;

    int (*p)(const T &x);

public:
    HashTable(int bufferSize = 4, int (*p)(const T &x) = nullptr) :
            bufferSize(bufferSize), p(p) {
        size = 0;
        nodes = new Node<T> *[bufferSize];

        for (int i = 0; i < bufferSize; ++i) {
            nodes[i] = nullptr;
        }
    };

    ~HashTable() {
        for (int i = 0; i < bufferSize; ++i)
            if (nodes[i] != nullptr)
                delete nodes[i];
        delete[] nodes;
    }


    int hash(const T &key) {
        return (*p)(key) % bufferSize;
    }

    bool contains(const T &key) {
        int hashed = hash(key);
        int i = 0;
        while (nodes[hashed] != nullptr && i < bufferSize) {
            if (!nodes[hashed]->deleted && nodes[hashed]->data == key)
                return true;
            i++;
            hashed += (i * i + i) / 2;
            //hashed += i;
            hashed %= bufferSize;
        }
        return false;
    }

    bool insert(const T &key) {
        if (contains(key))
            return false;

        if (3 * bufferSize <= 4 * (size + 1))
            rehash();

        int hashed = hash(key);
        int i = 0;

        while (i < bufferSize) {
            if (nodes[hashed] == nullptr) {
                nodes[hashed] = new Node<T>(key);
                size++;
                return true;
            } else if (nodes[hashed]->deleted) {
                nodes[hashed]->data = key;
                nodes[hashed]->deleted = false;
                size++;
                return true;
            }
            i++;
            hashed += (i * i + i) / 2;
            //hashed += i;
            hashed %= bufferSize;
        }
        return false;
    }

    bool remove(const T &key) {
        if (!contains(key))
            return false;

        int hashed = hash(key);
        int i = 0;

        while (i < bufferSize) {
            if (!nodes[hashed]->deleted && nodes[hashed]->data == key) {
                nodes[hashed]->deleted = true;
                size--;
                return true;
            }
            i++;
            hashed += (i * i + i) / 2;
            //hashed += i;
            hashed %= bufferSize;
        }
        return false;
    }


    void rehash() {
        Node<T> **tmp = nodes;
        bufferSize *= 2;
        size = 0;
        nodes = new Node<T> *[bufferSize];

        for (int i = 0; i < bufferSize; i++)
            nodes[i] = nullptr;

        for (int i = 0; i < bufferSize / 2; i++)
            if (tmp[i] != nullptr) {
                if (!tmp[i]->deleted)
                    insert(tmp[i]->data);
                delete tmp[i];
            }

        delete[] tmp;
    }

    void print() {
        if (size == 0) {
            cout << "No elements in hashtable\n";
            return;
        }

        for (int i = 0; i < bufferSize; ++i) {
            if (nodes[i] != nullptr && !nodes[i]->deleted) {
                cout << nodes[i]->data << " ";
            }
        }
        cout << "\n";
    }

};

int HashCntString(const string &key) {
    int hashed = 0;
    for (char c : key) {
        hashed += 31 * c;
        //hashed %= bufferSize;
    }
    return hashed;
}

int HashCntInt(const int &key) {
    return key;
}

int (*p_string)(const string &s) = &HashCntString;

int (*p_int)(const int &key) = &HashCntInt;


int main() {
    HashTable<string> my_table = HashTable<string>(8, p_string);
    my_table.insert("Hello");
    my_table.insert("my name is");
    my_table.insert("Islam");
    my_table.print();
    my_table.remove("Iskan");
    my_table.remove("Islam");
    my_table.remove("Islam");
    my_table.print();
    HashTable<int> my_table2 = HashTable<int>(8, p_int);
    my_table2.insert(5);
    my_table2.insert(5);
    my_table2.insert(2);
    my_table2.print();
    my_table2.remove(5);
    my_table2.print();
    return 0;
}
