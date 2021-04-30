#include "calculator.h"
Calculator cal = Calculator();
int add(int a, int b){
    return cal.add(a, b);
}
int multiply(int a, int b){
    return cal.multiply(a, b);
}
int divide(int a, int b){
    return cal.divide(a, b);
}
int subtract(int a, int b){
    return cal.subtract(a, b);
}
int mod(int a, int b){
    return cal.mod(a, b);
}