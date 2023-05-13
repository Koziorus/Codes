package org.example;

import java.io.*;
import java.util.Scanner;

public class Main
{
    public static void main(String[] args) throws InterruptedException
    {
        Resource resource = new Resource();
        Tasks tasks = new Tasks();

        int n_workers = Integer.parseInt(args[0]);

        DivWorker[] div_workers = new DivWorker[n_workers];

        for(int i = 0; i <n_workers; i++)
        {
            div_workers[i] = new DivWorker(tasks, resource);
            div_workers[i].start();
        }

        boolean read_from_file = true;

        Scanner scanner = new Scanner(System.in);

        try {
            File file = new File("input.txt");
            scanner = new Scanner(file);
        }
        catch(FileNotFoundException e) {

        }

        while(true)
        {
            if(!read_from_file)
            {
                scanner = new Scanner(System.in);
            }

            String input = scanner.next();

            try
            {
                int number = Integer.parseInt(input);
                tasks.create_new_task(number);
                //System.out.println("Task created");
            }
            catch(NumberFormatException e)
            {
                System.out.println("exiting...");
                if(input.equals("exit"))
                {
                    for(int i = 0; i <n_workers; i++)
                    {
                        div_workers[i].stop_worker();
                        //div_workers[i].interrupt();
                    }

                    synchronized (tasks)
                    {
                        tasks.notifyAll();
                    }

                    for(int i = 0; i <n_workers; i++)
                    {
                        div_workers[i].join();
                    }

                    return;
                }
            }

            if(read_from_file && !scanner.hasNext())
            {
                read_from_file = false;
            }

            //resource.get_latest_result(check_number);
        }
    }
}