import jakarta.persistence.*;

import java.util.List;
import java.util.Scanner;

// for now input commands like this: add/remove [enter] [param1] [param2] ... [enter]

public class Main {

    public static void add_to_db(EntityManager beer_manager)
    {
        // TODO: make this function take a String as a parameter that is the whole String line from the console, for example: "Piwo mocne 20 Warka"
        Scanner scanner = new Scanner(System.in);
        System.out.println("(Piwo name cena browar) / (Browar name wartosc)");
        String object_type = scanner.next();
        if(object_type.equals("Piwo"))
        {
            String name = scanner.next();
            long cena = scanner.nextInt();
            String browar_name = scanner.next();

            Browar browar = beer_manager.find(Browar.class, browar_name);

            if(browar == null)
            {
                System.out.println("Wrong browar!");
                return;
            }

            Piwo piwo = new Piwo(name, cena, browar);

            browar.getPiwa().add(piwo);

            beer_manager.getTransaction().begin();
            beer_manager.persist(piwo);
            beer_manager.getTransaction().commit();
        }
        else // Browar
        {
            String name = scanner.next();
            long wartosc = scanner.nextInt();

            Browar browar = beer_manager.find(Browar.class, name);

            if(browar != null)
            {
                System.out.println("Browar already exists!");
                return;
            }

            beer_manager.getTransaction().begin();
            beer_manager.persist(new Browar(name, wartosc));
            beer_manager.getTransaction().commit();
        }
    }

    public static void remove_from_db(EntityManager beer_manager)
    {
        Scanner scanner = new Scanner(System.in);

        System.out.println("(Piwo) / (Browar)");
        String object_type = scanner.next();
        if(object_type.equals("Piwo"))
        {
            String name = scanner.next();

            Piwo piwo = beer_manager.find(Piwo.class, name);

            if(piwo == null)
            {
                System.out.println("This piwo doesn't exist!");
                return;
            }

            Browar browar = piwo.getBrowar();

            beer_manager.getTransaction().begin();
            browar.getPiwa().remove(piwo); // cascade delete
            beer_manager.remove(piwo);
            beer_manager.getTransaction().commit();
        }
        else // Browar
        {
            String name = scanner.next();

            Browar browar = beer_manager.find(Browar.class, name);

            if(browar == null)
            {
                System.out.println("This browar doesn't exist!");
                return;
            }

            List<Piwo> piwa = browar.getPiwa();

            beer_manager.getTransaction().begin();
            for(int j = 0; j < piwa.size(); j++) // cascade delete
            {
                beer_manager.remove(piwa.get(j));
            }
            beer_manager.remove(browar);
            beer_manager.getTransaction().commit();
        }
    }

    public static void show_db(EntityManager beer_manager)
    {
        String query_all_browar = "SELECT b FROM Browar b";
        Query query_b = beer_manager.createQuery(query_all_browar, Browar.class);
        List<Browar> browar_all = query_b.getResultList();

        String query_all_piwo = "SELECT p FROM Piwo p";
        Query query_p = beer_manager.createQuery(query_all_piwo, Piwo.class);
        List<Piwo> piwo_all = query_p.getResultList();

        System.out.print("Browary: ");
        System.out.println(browar_all);

        System.out.print("Piwa: ");
        System.out.println(piwo_all);
    }

    public static void main(String[] args) {
        EntityManagerFactory factory = Persistence.createEntityManagerFactory("default");
        EntityManager beer_manager = factory.createEntityManager();

        Scanner scanner = new Scanner(System.in);

        for(int i = 0; i < 1000; i++)
        {
            String input = scanner.next();
            if(input.equals("add"))
            {
               add_to_db(beer_manager);
            }
            else if(input.equals("remove"))
            {
                remove_from_db(beer_manager);
            }
            else if(input.equals("show"))
            {
               show_db(beer_manager);
            }
            else if(input.equals("query"))
            {

            }
            else
            {
                System.out.println("Wrong command!");
            }
        }

        beer_manager.close();
    }
}
