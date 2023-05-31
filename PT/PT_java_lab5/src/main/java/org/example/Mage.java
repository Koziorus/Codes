package org.example;

public class Mage {
    private String name;
    private int level;

    @Override
    public String toString()
    {
        return "Mage{" + "name='" + name + '\'' + ", level=" + level + '}';
    }

    public Mage(String name, int level)
    {
        this.name = name;
        this.level = level;
    }

    public String getName()
    {
        return name;
    }

    public int getLevel()
    {
        return level;
    }
}

