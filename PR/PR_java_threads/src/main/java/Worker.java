import java.util.Random;

public class Worker implements Runnable
{
    int id;
    Resource resource;
    int limit;

    Worker(Resource resource, int limit)
    {
        this.resource = resource;
        this.limit = limit;
    }


    @Override
    public void run()
    {
        Random rand = new Random();

        while (true)
        {
            int random_number = rand.nextInt(limit);
            boolean success = resource.input_number(random_number);
            if (success)
            {
                System.out.println(random_number);
            }
            else
            {
                System.out.println("Already exists: ", Integer.parseInt(random_number));
            }
        }

    }
}
