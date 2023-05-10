package org.example;

import java.util.Map;

public class DivWorker implements Runnable
{
    private Tasks tasks;
    private Resource resource;
    private String data;
    public DivWorker(Tasks tasks, Resource resource)
    {
        this.tasks = tasks;
        this.resource = resource;
    }

    @Override
    public void run()
    {
        while(true)
        {
            try
            {

                int number_to_check = tasks.get_task();

                boolean is_prime = true;
                for(int i = 2; i <= number_to_check - 1; i++)
                {
                    Thread.sleep(500);
                    if(number_to_check % i == 0)
                    {
                        is_prime = false;
                        break;
                    }
                }

                resource.send_result(number_to_check, is_prime);

                //System.out.println("Worker(" + Thread.currentThread().getId() + ") -> " + number + " | " + divisor + " -> " + is_divisible);
            }
            catch (InterruptedException e)
            {
                return;
            }
        }
    }
}
