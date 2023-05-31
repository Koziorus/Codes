package org.example;

import org.junit.Rule;
import org.junit.Test;
import org.junit.rules.ExpectedException;

import java.util.Optional;

import static org.junit.Assert.*;

public class TestMages
{
    @Rule
    public final ExpectedException exception = ExpectedException.none();

    @Test
    public void repo_delete_bad()
    {
        MageRepository repository = new MageRepository();
        repository.save(new Mage("A", 10));

        exception.expect(IllegalArgumentException.class);
        repository.delete("B");
    }

    @Test
    public void repo_find_bad()
    {
        MageRepository repository = new MageRepository();
        repository.save(new Mage("A", 10));

        Optional<Mage> mage = repository.find("B");
        assertTrue(mage.isEmpty());
    }

    @Test
    public void repo_find_good()
    {
        MageRepository repository = new MageRepository();
        repository.save(new Mage("A", 10));

        Optional<Mage> mage = repository.find("A");
        assertFalse(mage.isEmpty());
    }

    @Test
    public void repo_save_bad()
    {
        MageRepository repository = new MageRepository();
        repository.save(new Mage("A", 10));

        exception.expect(IllegalArgumentException.class);
        repository.save(new Mage("A", 20));
    }

    @Test
    public void contr_delete_good()
    {
        MageController mage_controller = new MageController(new MageRepository());
        mage_controller.save("A", "10");

        String result = mage_controller.delete("A");

        assertEquals("done", result);
    }

    @Test
    public void contr_delete_bad()
    {
        MageController mage_controller = new MageController(new MageRepository());
        mage_controller.save("A", "10");

        String result = mage_controller.delete("B");

        assertEquals("not found", result);
    }

    @Test
    public void contr_find_good()
    {
        MageController mage_controller = new MageController(new MageRepository());
        mage_controller.save("A", "10");

        String result = mage_controller.find("A");

        Mage mage = new Mage("A", 10);
        assertEquals(mage.toString(), result);
    }

    @Test
    public void contr_find_bad()
    {
        MageController mage_controller = new MageController(new MageRepository());
        mage_controller.save("A", "10");

        String result = mage_controller.find("B");

        assertEquals("not found", result);
    }

    @Test
    public void contr_save_good()
    {
        MageController mage_controller = new MageController(new MageRepository());
        mage_controller.save("A", "10");

        String result = mage_controller.save("B", "20");

        assertEquals("done", result);
    }

    @Test
    public void contr_save_bad()
    {
        MageController mage_controller = new MageController(new MageRepository());
        mage_controller.save("A", "10");

        String result = mage_controller.save("A", "20");

        assertEquals("bad request", result);
    }
}
