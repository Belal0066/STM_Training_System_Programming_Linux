#include <stdio.h>

int is_prime(int num);

int main() {
    int num;
    
    printf("Enter a number: ");
    scanf("%d", &num);

    
    if(is_prime(num)) {
        printf("%d is a prime number.\n", num);
    } else {
        printf("%d is not a prime number.\n", num);
    }

    return 0;
}

