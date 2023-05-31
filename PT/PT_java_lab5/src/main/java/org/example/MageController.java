package org.example;

import java.util.Optional;

public class MageController
{
    private MageRepository repository;

    public MageController(MageRepository repository)
    {
        this.repository = repository;
    }

    public String find(String name)
    {
        Optional<Mage> mage = repository.find(name);
        if(mage.isEmpty())
        {
            return "not found";
        }
        else
        {
            return mage.stream().findFirst().get().toString();
        }
    }

    public String delete(String name)
    {
        try
        {
            repository.delete(name);
            return "done";
        }
        catch (IllegalArgumentException e)
        {
            return "not found";
        }
    }

    public String save(String name, String level)
    {
        try
        {
            repository.save(new Mage(name, Integer.parseInt(level)));
            return "done";
        }
        catch (IllegalArgumentException e)
        {
            return "bad request";
        }
    }
}
