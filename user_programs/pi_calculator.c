
#include "syscalls.h"
char modname[] __attribute__((section(".modname"), aligned(4))) = "PI digits";

// Factorial function
long double factorial(int n) {
    if (n == 0 || n == 1) return 1;
    long double result = 1;
    for (int i = 2; i <= n; i++) {
        result *= i;
    }
    return result;
}
long double powl(double long x, int n){
    for(int i = 0; i < n; i++){
        x*=x;
    }
    return x;
}
// Compute pi using the Chudnovsky algorithm
long double compute_pi(int terms) {
    const long double C = 442698670.666;
    long double sum = 0;

    for (int k = 0; k < terms; k++) {
        long double numerator = factorial(6 * k) * (545140134 * k + 13591409);
        long double denominator = factorial(3 * k) * powl(factorial(k), 3) * powl(640320, 3 * k);
        sum += numerator / denominator;
    }

    return C / sum; // Final computation of pi
}

int main() {
    int terms = 1;
    while(1){
        long double pi = compute_pi(terms);
    }
    return 0;
}
