import java.util.ArrayList;

public class Resource
{
    int MAX_NUM;

    ArrayList<Integer> number_array = new ArrayList<>();

    public Resource(int MAX_NUM)
    {
        this.MAX_NUM = MAX_NUM;
    }

    public synchronized boolean input_number(int number)
    {
        if (number_array.contains(number))
        {
            return false;
        }
        else
        {
            number_array.add(number);
            return true;
        }
    }
}