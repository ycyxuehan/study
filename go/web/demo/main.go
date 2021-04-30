package main

import (
	"io/ioutil"
	"log"
	"net/http"
)

func echo(w http.ResponseWriter, r *http.Request){
	msg, err := ioutil.ReadAll(r.Body)
	defer r.Body.Close()
	if err != nil {
		w.Write([]byte("echo error"))
		return
	}
	writeLen, err := w.Write(msg)
	if err != nil || writeLen != len(msg){
		log.Println(err, "write len:", writeLen)
	}
}



func main(){
	http.HandleFunc("/", echo)
	err := http.ListenAndServe(":8080", nil)
	if err != nil {
		log.Fatal(err)
	}
}