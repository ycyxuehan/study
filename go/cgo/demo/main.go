package main

/*
#include <stdio.h>

void print(int v){
	// std::cout<<v<<std::endl;
	printf("printint: %d\n", v);
}
*/
import "C"

func main(){
	v := 42
	C.print(C.int(v))
}