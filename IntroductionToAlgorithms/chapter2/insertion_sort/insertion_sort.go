package main

import (
	"fmt"
)

func insertion_sort(ai []int){
	for i, item := range ai {
		if i == 0 {
			continue
		}
		j := i - 1
		for j>=0 && ai[j] > item{
			
			ai[j + 1] = ai[j]
			j--
		}
		ai[j+1]=item
	}
}
func main(){
	ai := []int{3, 2, 5, 1, 9, 4}
	insertion_sort(ai)
	fmt.Println(ai)
}