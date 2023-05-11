package org.example;

import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;
import java.util.HashMap;
import java.util.LinkedList;

public class Resource
{
    HashMap<Integer, LinkedList<Long>> divisors_for_number;

    Resource()
    {
        divisors_for_number = new HashMap<>();
    }

    public synchronized void send_result(int number, LinkedList<Long> divisors)
    {
        divisors_for_number.put(number, divisors);

        //System.out.println("new result");

        System.out.println(number + " -> " + divisors.toString());

        String text = Integer.toString(number) + ": ";
        for(int i = 0; i < divisors.size(); i++)
        {
            if(i != 0)
            {
                text = text + ", ";
            }

            text = text + Long.toString(divisors.get(i));
        }

        try
        {
            write_to_file(text, "output.txt");
        }
        catch(IOException ex)
        {
            System.out.println("error");
        }
    }


    public synchronized void write_to_file(String text, String file_name) throws IOException {
        BufferedWriter writer = new BufferedWriter(new FileWriter(file_name, true));

        writer.append(text);
        writer.append('\n');

        writer.close();
    }

}
