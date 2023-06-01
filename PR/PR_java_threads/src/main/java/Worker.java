import java.util.Random;

public class Worker implements Runnable
{
    int id;
    Resource resource;

    private static final Object lock = new Object();

    Worker(Resource resource, int id)
    {
        this.resource = resource;
        this.id = id;
    }


    @Override
    public void run()
    {
        Random rand = new Random();

        while (true)
        {
            int random_number = rand.nextInt(resource.MAX_NUM);
            boolean success = resource.input_number(random_number);

            synchronized (lock)
            {
                System.out.print(id + " -> ");

                if (success)
                {
                    System.out.println(random_number);
                }
                else
                {
                    System.out.println("Already exists: " + random_number);
                }
            }

            try
            {
                Thread.sleep(50, 0);
            }
            catch (InterruptedException e)
            {
                throw new RuntimeException(e);
            }
        }

    }
}
