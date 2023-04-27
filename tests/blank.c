int main() {
    int i;
    int j;
    int x;
    x = 1;
    if (0) {
        j = 2;
    }
    else {
        j = 10;
    }
    i = 3;
    return x;
}

void main2() {
    int i;
    int j;
    while (0) {
        i = 3;
    }
    j = 2;
    i = 5;
}

void main3() {
    int i;
    int j;
    int k;
    for (i = 0; i < 5; i = i + 1) {
        j = j + 1;
    }
    k = 5;
}
