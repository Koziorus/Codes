package org.example;

import java.util.Objects;
import java.util.Set;

public class Mage implements Comparable<Mage>
{
    private String name;
    private int level;
    private double power;
    private Set<Mage> apprentices;

    public Mage(String name, int level, double power, Set<Mage> apprentices) {
        this.name = name;
        this.level = level;
        this.power = power;
        this.apprentices = apprentices;
    }

    @Override
    public String toString()
    {
        return "Mage{name='" + name + '\'' +
                ", level=" + level +
                ", power=" + power + '}';
    }

    @Override
    public int hashCode() {
        return Objects.hash(name, level, power, apprentices);
    }



    @Override
    public boolean equals(Object obj)
    {
        if (this == obj)
            return true;

        if (obj == null)
            return false;

        if (this.getClass() != obj.getClass())
            return false;

        Mage other = (Mage) obj;
        if (!this.name.equals(other.name))
            return false;

        if (this.level != other.level)
            return false;

        if (this.power != other.power)
            return false;

        if (!this.apprentices.equals(other.apprentices))
            return false;

        return true;
    }

    @Override
    public int compareTo(Mage other) {
        // asc
        // 0 -> ==
        // < 0 -> <
        // > 0 -> >

        if (this.name.compareTo(other.name) != 0)
        {
            return this.name.compareTo(other.name); // sort by name
        }
        else if (this.level != other.level)
        {
            return (this.level < other.level ? -1 : 1); // sort by level
        }
        else if (this.power != other.power)
        {
            return (this.power < other.power ? -1 : 1); // sort by power
        }
        else
        {
            return 0; // equal stats
        }
    }
}
