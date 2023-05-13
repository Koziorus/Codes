package org.example;

import java.util.Comparator;

public class MageComparator implements Comparator<Mage>
{
    @Override
    public int compare(Mage mage1, Mage mage2) {
        if (mage1.getLevel() != mage2.getLevel())
        {
            return (mage1.getLevel() < mage2.getLevel() ? -1 : 1);
        }
        else if (!mage1.getName().equals(mage2.getName()))
        {
            return mage1.getName().compareTo(mage2.getName());
        }
        else if (mage1.getPower() != mage2.getPower())
        {
            return (mage1.getPower() < mage2.getPower() ? -1 : 1);
        }
        else
        {
            return 0;
        }

    }
}
