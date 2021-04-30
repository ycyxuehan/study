package main

/*
// #cgo LDFLAGS: -L . -lcalculator -lstdc++
// #cgo CFLAGS: -I ./
#include "c.h"
*/
import "C"
import "fmt"

func main(){
	a := 8
	b := 5
	fmt.Printf("a + b = %d", C.add(C.int(a), C.int(b)))
}