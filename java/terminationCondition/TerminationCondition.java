//TerminationCondition.java
// Using finalize() to detect a object that
// hasn't been properly cleaned up
// import java.*;

class Book {
    boolean checkedOut = false;
    Book(boolean checkOut){
        checkedOut = checkOut;
    }
    void CheckIn(){
        checkedOut = false;
    }
    @Override
    protected void finalize() throws Throwable {
        if(checkedOut){
            System.out.println("Error: checked out");
        }
    }
}


public class TerminationCondition {
    public static void main(String[] args) {
        Book novel = new Book(true);
        novel.CheckIn();
        new Book(true);
        System.gc();
        try{
            Thread.sleep (1);
        }catch(InterruptedException ie){}
    }
}