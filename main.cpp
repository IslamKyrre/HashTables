#include <iostream>

using namespace std;

class Node {
public:
    string data;
    int deleted;

    Node(const string &data) : data(data), deleted(0) {};
};

class HashTable {
    int bufferSize;
    int size;
    Node **nodes;

public:
    HashTable(int bufferSize = 4) : bufferSize(bufferSize) {
        size = 0;
        nodes = new Node *[bufferSize];

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

    int hash(const string &key) {
        int hashed = 0;
        for (char c : key) {
            hashed += 31 * c;
            hashed %= bufferSize;
        }
        return hashed;
    }

    bool contains(const string& key) {
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

    bool insert(const string &key) {
        if (contains(key))
            return false;

        if (3 * bufferSize <= 4 * (size + 1))
            rehash();

        int hashed = hash(key);
        int i = 0;

        while (i < bufferSize) {
            if (nodes[hashed] == nullptr) {
                nodes[hashed] = new Node(key);
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

    bool remove(const string& key) {
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
        Node **tmp = nodes;
        bufferSize *= 2;
        size = 0;
        nodes = new Node *[bufferSize];

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

    void print(){
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


int main() {
    HashTable my_table = HashTable();
    my_table.insert("Hello");
    my_table.insert("my name is");
    my_table.insert("Islam");
    my_table.print();
    my_table.remove("Iskan");
    my_table.remove("Islam");
    my_table.remove("Islam");
    my_table.print();
    return 0;
}
