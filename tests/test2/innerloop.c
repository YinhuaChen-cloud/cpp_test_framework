#include <stdio.h>
#include <stdbool.h>

void func1() {
    // 10
    int numLoop = 3;
    bool toggle = true;

    // 11
    for(int i = 0; i < numLoop; i++) {
        if(toggle) {
            printf("in func1, i = %d, toggle = %d\n", i, toggle);
            toggle = false;
        }
        else {
            printf("in func1, i = %d, toggle = %d\n", i, toggle);
            toggle = true;
        }
    }
    // 21

}

void func2() {
    int numLoop = 3;
    bool toggle = true;

    for(int i = 0; i < numLoop; i++) {
        if(toggle) {
            printf("in func2, i = %d, toggle = %d\n", i, toggle);
            toggle = false;
        }
        else {
            printf("in func2, i = %d, toggle = %d\n", i, toggle);
            toggle = true;
        }
    }

}

