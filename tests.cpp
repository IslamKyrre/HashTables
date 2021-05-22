#include "hashtables.cpp"

int HashCntString(const string &key) {
    int hashed = 0;
    for (char c : key) {
        hashed += 31 * c;
    }
    return hashed;
}

int HashCntInt(const int &key) {
    return key;
}


int (*p_string)(const string &s) = &HashCntString;

int (*p_int)(const int &key) = &HashCntInt;

int main() {
    try{
        HashTable<int> LOL = HashTable<int> (-3, p_int);
    } catch (const HashTable<int>::hash_table_error &er) {
        if (er == HashTable<int>::EINVARG) {
            cout << "Test is working \n";
        }
    }
    /*HashTable<string> table = HashTable<string>(8, p_string);
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
    cout << "table3:   " << table3;*/
    return 0;
}
