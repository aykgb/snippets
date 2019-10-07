import java.io.*;

public class EmployeeType001{
    
    // salary is a static private variable;
    private static double salary;
    // DEPATMENT is a static vriable;
    private static final String DEPARTMENT = "Development";
    
    public static void main(String args[]) {
	salary = 1000;
	System.out.println(DEPARTMENT + "average salary:" + salary);
    }
}
