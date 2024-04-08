// #include <iostream>
// using namespace std;


int isEqual(int i1, int i2) {
    return i1 == i2;
}

int bocchiShutUp(int flag, int seq[], int size) {
    int hash[30], i;
    for (i = 11; i <= 26; ++i) hash[i] = 0;
    for (i = 0; i < size; ++i)
        if (seq[i] / 10 == flag) hash[seq[i]]++;
    int m = -1;
    for (i = 11; i <= 26; ++i)
        if (hash[i] > m) m = hash[i];
    int ret = -1;
    for (i = 11; i <= 26; ++i)
        if (isEqual(hash[i], m)) {
            if (ret != -1) return 10;
            ret = i;
        }
    return ret;
}

// int main() {
//     int a[10] = {13,14,13,11,21,13,21,22};
//     // cout << bocchiShutUp(2, a, 8) << endl;
//     return 0;
// }



