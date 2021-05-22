#include <iostream>

using namespace std;


template<typename T>
class Node {
public:
    T data;
    int deleted;

    Node(const T &data, int deleted = 0) : data(data), deleted(deleted) {};
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

    HashTable(const HashTable<T> &table) {
        bufferSize = table.bufferSize;
        size = table.bufferSize;
        p = table.p;
        nodes = new Node<T> *[bufferSize];

        for (int i = 0; i < bufferSize; ++i) {
            if (table.nodes[i] != nullptr) {
                nodes[i] = new Node<T>(table.nodes[i]->data, table.nodes[i]->deleted);
            } else {
                nodes[i] = nullptr;
            }
        }

    }

    ~HashTable() {
        for (int i = 0; i < bufferSize; ++i)
            if (nodes[i] != nullptr)
                delete nodes[i];
        delete[] nodes;
    }

    HashTable<T> &operator=(const HashTable<T> &table) {
        for (int i = 0; i < bufferSize; ++i)
            if (nodes[i] != nullptr)
                delete nodes[i];
        delete[] nodes;

        bufferSize = table.bufferSize;
        size = table.bufferSize;
        p = table.p;
        nodes = new Node<T> *[bufferSize];

        for (int i = 0; i < bufferSize; ++i) {
            if (table.nodes[i] != nullptr) {
                nodes[i] = new Node<T>(table.nodes[i]->data, table.nodes[i]->deleted);
            } else {
                nodes[i] = nullptr;
            }
        }

        return *this;
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

    friend ostream &operator<<(ostream &out, const HashTable<T> &table) {
        if (table.size == 0) {
            out << "No elements in hashtable\n";
            return out;
        }

        for (int i = 0; i < table.bufferSize; ++i) {
            if (table.nodes[i] != nullptr && !table.nodes[i]->deleted) {
                out << table.nodes[i]->data << " ";
            }
        }
        out << "\n";
        return out;
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
    HashTable<string> table = HashTable<string>(8, p_string);
    table.insert("Token 0");
    HashTable<string> table2 = HashTable<string>(table);
    table.insert("Token 1");
    table2.insert("Token 2");
    HashTable<string> table3 = HashTable<string>(32, p_string);
    table3.insert("HAHA");
    cout << "table:  " << table;
    cout << "table2:   " << table2;
    cout << "table3:   " << table3;
    table3 = table;
    cout << "table3:   " << table3;
    return 0;
}
