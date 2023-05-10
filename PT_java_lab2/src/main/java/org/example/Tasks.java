package org.example;

import java.security.KeyPair;
import java.util.ArrayList;
import java.util.LinkedList;
import java.util.Map;
import java.util.Queue;

public class Tasks
{
    LinkedList<Integer> numbers_to_check;

    Tasks()
    {
        numbers_to_check = new LinkedList<>();
    }
    // TODO: try it without a queue and use the example of producer and a consumer from the lecture and see if it can still work like a queue

    public synchronized void create_new_task(int number)
    {
        numbers_to_check.add(number);
        notify();
    }

    public synchronized int get_task() throws InterruptedException
    {
        wait(); // waits for someone to notify

        return numbers_to_check.removeFirst();
    }

}
