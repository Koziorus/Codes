public class Main
{
    public static void main(String[] args)
    {
        int num_of_workers = 2;

        Resource resource = new Resource(1000);

        for (int i = 0; i < num_of_workers; i++)
        {
            (new Thread(new Worker(resource, i))).start();
        }


    }
}
