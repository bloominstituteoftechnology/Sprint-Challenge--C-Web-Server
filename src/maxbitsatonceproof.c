#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// 4096

int main() {
    int i = 0;
    while(1) {
        int res = pow(2, i);
        printf("2 ** %d = %d\n", i, res);
        if(res == 4096) {
            printf("PROOF: 2 ** %d is 4096, so it's %d bytes long", i, i);
            break;
        }
        if(i >= 30) {
            break;
        }
        i++;
    }
    return 0;
}