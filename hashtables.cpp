#include <iostream>
#include <vector>

using namespace std;


template<typename T>
class Node {
public:
    T data;
    int deleted;

    Node(const T &data, int deleted = 0) : data(data), deleted(deleted) {};
};

template <typename T>
int check_arg(T arg) {
    if (arg < 0) return 0;
    while (arg > 1) {
        if (arg % 2) return 0;
        arg /= 2;
    }
    return 1;
}



template<typename T>
class HashTable {
    int bufferSize;
    int size;
    Node<T> **nodes;

    int (*p)(const T &x);

public:
    enum hash_table_error {
        EINVARG = 1,
        EINSERT,
        EREMOVE
    };

    HashTable(int (*p)(const T &x), int bufferSize = 128) :
            bufferSize(bufferSize), p(p) {
        if (!check_arg(bufferSize)) {
            throw EINVARG;
        }


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
            hashed %= bufferSize;
        }
        return false;
    }

    void insert(const T &key) {
        if (contains(key)) {
            throw EINSERT;
        }

        if (3 * bufferSize <= 4 * (size + 1))
            rehash();

        int hashed = hash(key);
        int i = 0;

        while (i < bufferSize) {
            if (nodes[hashed] == nullptr) {
                nodes[hashed] = new Node<T>(key);
                size++;
                return;
            } else if (nodes[hashed]->deleted) {
                nodes[hashed]->data = key;
                nodes[hashed]->deleted = false;
                size++;
                return;
            }
            i++;
            hashed += (i * i + i) / 2;
            hashed %= bufferSize;

        }
    }

    void remove(const T &key) {
        if (!contains(key))
            throw EREMOVE;

        int hashed = hash(key);
        int i = 0;

        while (i < bufferSize) {
            if (!nodes[hashed]->deleted && nodes[hashed]->data == key) {
                nodes[hashed]->deleted = true;
                size--;
                return;
            }
            i++;
            hashed += (i * i + i) / 2;
            hashed %= bufferSize;
        }
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

};
