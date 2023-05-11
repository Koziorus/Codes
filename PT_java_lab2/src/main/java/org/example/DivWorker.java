package org.example;

import java.util.LinkedList;
import java.util.Map;

public class DivWorker extends Thread
{
    private Tasks tasks;
    private Resource resource;
    private String data;
    private boolean exit;
    public DivWorker(Tasks tasks, Resource resource)
    {
        this.tasks = tasks;
        this.resource = resource;
        this.exit = false;
    }

    public void stop_worker()
    {
        this.exit = true;
    }

    @Override
    public void run()
    {
        while(!exit)
        {
            try
            {
                //Thread.sleep(1000);
                int number = tasks.get_task();
                if(number == -1)
                {
                    return;
                }

                LinkedList<Long> divisors = new LinkedList<>();
                if(number != 0)
                {
                    for(long i = 1; i <= number; i++)
                    {
                        //Thread.sleep(500);
                        if(number % i == 0)
                        {
                            divisors.add(i);
                        }
                    }
                }

                resource.send_result(number, divisors);

                //System.out.println("Worker(" + Thread.currentThread().getId() + ") -> " + number + " | " + divisor + " -> " + is_divisible);
            }
            catch (InterruptedException e)
            {
                return;
            }
        }
    }
}
