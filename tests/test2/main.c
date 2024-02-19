#include <stdio.h>
#include <stdbool.h>

extern void func1();
extern void func2();

int main() {
    int numLoop = 10;
    bool toggle = true;

    for(int i = 0; i < numLoop; i++) {
        if(toggle) {
            func1();
            toggle = false;
        }
        else {
            func2();
            toggle = true;
        }
    }

    return 0;
}

