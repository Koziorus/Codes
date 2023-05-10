package org.example;

import java.util.Scanner;

public class Main
{
    public static void main(String[] args) throws InterruptedException
    {
        Resource resource = new Resource();
        Tasks tasks = new Tasks();

        int n_workers = 5;

        DivWorker[] div_workers = new DivWorker[n_workers];
        Thread[] threads = new Thread[n_workers];

        for(int i = 0; i <n_workers; i++)
        {
            div_workers[i] = new DivWorker(tasks, resource);
            threads[i] = new Thread(div_workers[i]);
            threads[i].start();
        }

        while(true)
        {
            Scanner scanner = new Scanner(System.in);
            String input = scanner.next();

            try
            {
                int number = Integer.parseInt(input);
                tasks.create_new_task(number);
                System.out.println("Task created");
            }
            catch(NumberFormatException e)
            {
                System.out.println("ending");
                if(input.equals("END"))
                {
                    for(int i = 0; i <n_workers; i++)
                    {
                        threads[i].interrupt(); // tell the thread to close
                        threads[i].join(); // wait until it closes
                    }

                    return;
                }
            }

            //resource.get_latest_result(check_number);
        }
    }
}