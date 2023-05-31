package org.example;

import javax.lang.model.type.NullType;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Optional;

public class MageRepository
{
    private Collection<Mage> collection;

    public MageRepository()
    {
        this.collection = new ArrayList<>();
    }

    public Optional<Mage> find(String name)
    {
        for (Mage mage : collection)
        {
            if (mage.getName().equals(name))
            {
                return Optional.of(mage);
            }
        }

        return Optional.empty();
    }

    public void delete(String name) throws IllegalArgumentException
    {
        for (Mage mage : collection)
        {
            if (mage.getName().equals(name))
            {
                collection.remove(mage);
                return;
            }
        }

        throw new IllegalArgumentException();
    }

    public void save(Mage new_mage) throws IllegalArgumentException
    {
        for (Mage mage : collection)
        {
            if (mage.getName().equals(new_mage.getName()))
            {
                throw new IllegalArgumentException();
            }
        }

        collection.add(new_mage);
    }
}
