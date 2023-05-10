package org.example;

import java.util.HashMap;

public class Resource
{
    HashMap<Integer, Boolean> primes;

    Resource()
    {
        primes = new HashMap<>();
    }

    public synchronized void send_result(int number, boolean is_divisible)
    {
        primes.put(number, is_divisible);

        //System.out.println("new result");

        System.out.println(number + " -> " + primes.get(number));

        notify();
    }


    public synchronized void get_latest_result(int number_to_check) throws InterruptedException
    {
        wait();

        System.out.println(number_to_check + " -> " + primes.get(number_to_check));
    }
}
