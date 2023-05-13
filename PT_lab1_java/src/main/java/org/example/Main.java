package org.example;

import java.util.Collection;
import java.util.HashSet;
import java.util.Set;
import java.util.TreeSet;

public class Main
{
    static void printRecursively(Mage mage, int depth)
    {
        for (int i = 0; i < depth; i++)
        {
            System.out.print("-");
        }

        System.out.println(mage.toString());

        for (Mage apprentice : mage.getApprentices())
        {
            printRecursively(apprentice, depth + 1);
        }
    }

    static int mode;

    public static void main(String[] args)
    {
        Set<Mage> recursiveMages = null;

        if (args.length == 1)
        {
            if (args[0].equals("natural_sort"))
            {
                System.out.println("Natural sorting");
                mode = 1;
                recursiveMages = new TreeSet<>();
            }
            else if (args[0].equals("alter_sort"))
            {
                System.out.println("Alternative sorting");
                mode = 2;
                recursiveMages = new TreeSet<>(new MageComparator());
            }
            else if (args[0].equals("no_sort"))
            {
                System.out.println("No sorting");
                mode = 0;
                recursiveMages = new HashSet<>();
            }
        }
        else
        {
            System.out.println("No sorting");
            mode = 0;
            recursiveMages = new HashSet<>();
        }

        Set<Mage> appren2 = (mode == 0 ? new HashSet<>() : (mode == 1 ? new TreeSet<>() : new TreeSet<>(new MageComparator())));
        appren2.add(new Mage("D", 10, 130, new HashSet<>()));
        appren2.add(new Mage("C", 12, 100, new HashSet<>()));

        Set<Mage> appren3 = (mode == 0 ? new HashSet<>() : (mode == 1 ? new TreeSet<>() : new TreeSet<>(new MageComparator())));
        appren3.add(new Mage("E", 5, 233, new HashSet<>()));

        Set<Mage> appren5 = (mode == 0 ? new HashSet<>() : (mode == 1 ? new TreeSet<>() : new TreeSet<>(new MageComparator())));
        appren5.add(new Mage("L", 3, 100, new HashSet<>()));
        appren5.add(new Mage("B", 19, 400, new HashSet<>()));

        Set<Mage> appren1 = (mode == 0 ? new HashSet<>() : (mode == 1 ? new TreeSet<>() : new TreeSet<>(new MageComparator())));
        appren1.add(new Mage("B", 66, 666, appren2));
        appren1.add(new Mage("Z", 80, 800, appren3));

        Set<Mage> appren4 = (mode == 0 ? new HashSet<>() : (mode == 1 ? new TreeSet<>() : new TreeSet<>(new MageComparator())));
        appren4.add(new Mage("V", 20, 298, appren5));

        Set<Mage> appren6 = (mode == 0 ? new HashSet<>() : (mode == 1 ? new TreeSet<>() : new TreeSet<>(new MageComparator())));
        appren6.add(new Mage("A", 110, 1200, appren1));
        appren6.add(new Mage("K", 90, 1500, appren4));

        recursiveMages.add(new Mage("O", 500, 9001, appren6));

        printRecursively(recursiveMages.iterator().next(), 1);
    }
}