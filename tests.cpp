#include "hashtables.cpp"

int HashString(const string &key) {
    int hashed = 0;
    for (char c : key) {
        hashed += 31 * c;
    }
    return hashed;
}

int HashInt(const int &key) {
    return key;
}


int (*p_string)(const string &s) = &HashString;

int (*p_int)(const int &key) = &HashInt;


int test_cnt = 1;

void passed() {
    cout << "TEST " << test_cnt++ << " PASSED\n";
}

void failed() {
    cout << "TEST " << test_cnt++ << " FAILED\n";
}


int main() {

    HashTable<int> *InvArg = nullptr;
    HashTable<int> *A, *A1, *A2;
    HashTable<string> *B;
    try {
        InvArg = new HashTable<int>(p_int, 36);
    } catch (const HashTable<int>::hash_table_error &er) {
        if (er == HashTable<int>::EINVARG) {
            passed();
        } else {
            failed();
        }
    } catch (...) {
        failed();
    }


    try {
        A = new HashTable<int>(p_int, 4);
    } catch (...) {
        failed();
    }
    passed();

    cout << "A: " << *A;

    try {
        B = new HashTable<string>(p_string);
    } catch (...) {
        failed();
    }
    passed();

    try {
        for (int i = 0; i < 10; ++i) {
            A->insert(i);
        }
    } catch (...) {
        failed();
    }
    passed();

    cout << "A: " << *A;

    try {
        A->insert(8);
    } catch (const HashTable<int>::hash_table_error &err) {
        if (err == HashTable<int>::EINSERT) passed();
        else failed();
    } catch (...) {
        failed();
    }

    try {
        A->remove(999);
    } catch (const HashTable<int>::hash_table_error &err) {
        if (err == HashTable<int>::EREMOVE) passed();
        else failed();
    } catch (...) {
        failed();
    }

    try {
        A->remove(6);
    } catch (...) {
        failed();
    }
    passed();

    try {
        A->insert(6);
    } catch (...) {
        failed();
    }
    passed();

    try {
        A->insert(70);
    } catch (...) {
        failed();
    }
    passed();

    try {
        A->remove(6);
    } catch (...) {
        failed();
    }
    passed();

    try {
        A->remove(70);
    } catch (...) {
        failed();
    }
    passed();






    try {
        A1 = new HashTable<int>(*A);
    } catch (...) {
        failed();
    }
    passed();

    cout << "A1: " << *A1;

    try {
        A2 = new HashTable<int>(p_int);
    } catch (...) {
        failed();
    }
    passed();

    try {
        A->insert(11);
    } catch (...) {
        failed();
    }
    passed();



    cout << "A: " << *A;

    try {
        *A2 = *A;
    } catch (...) {
        failed();
    }
    passed();

    cout << "A2: " << *A2;

    try {
        B->insert("Hello");
        B->insert("My");
        B->insert("World");
    } catch (...) {
        failed();
    }
    passed();

    cout << "B: " << *B;

    try {
        B->remove("My");
    } catch (...) {
        failed();
    }
    passed();

    cout << "B: " << *B;


    delete A;
    delete A1;
    delete A2;
    delete B;

    return 0;
}