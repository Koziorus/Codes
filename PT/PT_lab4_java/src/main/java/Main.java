import jakarta.persistence.*;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.List;
import java.util.Scanner;


public class Main {

    public static void add_to_db(EntityManager beer_manager, Scanner scanner)
    {
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

    public static void remove_from_db(EntityManager beer_manager, Scanner scanner)
    {
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

    public static void query_db(EntityManager beer_manager, Scanner scanner)
    {
        System.out.println("1. Wszystkie browary, które posiadają piwa tańsze niż ... zł");
        System.out.println("2. Wszystkie piwa z ceną niższą niż ... zł");
        System.out.println("3. Wszystkie piwa z browaru ... z ceną większą niż ... zł");

        String query_choice = scanner.next();
        if(query_choice.equals("1"))
        {
            int cena = scanner.nextInt();
            String query_str = "SELECT DISTINCT p.browar FROM Piwo p WHERE p.cena < " + Integer.toString(cena);
            Query query = beer_manager.createQuery(query_str, Piwo.class);
            List<Piwo> piwa = query.getResultList();
            System.out.println(piwa);
        }
        else if(query_choice.equals("2"))
        {
            int cena = scanner.nextInt();
            String query_str = "SELECT p FROM Piwo p WHERE p.cena < " + Integer.toString(cena);
            Query query = beer_manager.createQuery(query_str, Piwo.class);
            List<Piwo> piwa = query.getResultList();
            System.out.println(piwa);
        }
        else if(query_choice.equals("3"))
        {
            String browar = scanner.next();
            int cena = scanner.nextInt();
            String query_str = "SELECT p FROM Piwo p WHERE p.cena > " + Integer.toString(cena) + " AND p.browar = '" + browar + "'";
            Query query = beer_manager.createQuery(query_str, Piwo.class);
            List<Piwo> piwa = query.getResultList();
            System.out.println(piwa);
        }
    }

    public static void main(String[] args) throws FileNotFoundException {
        EntityManagerFactory factory = Persistence.createEntityManagerFactory("default");
        EntityManager beer_manager = factory.createEntityManager();

        File file = new File("input.txt");
        Scanner scanner = new Scanner(file);

        for(int i = 0; i < 1000; i++)
        {
            if(!scanner.hasNext())
            {
                scanner.close();
                scanner = new Scanner(System.in);
            }

            String input = scanner.next();
            if(input.equals("add"))
            {
               add_to_db(beer_manager, scanner);
            }
            else if(input.equals("remove"))
            {
                remove_from_db(beer_manager, scanner);
            }
            else if(input.equals("show"))
            {
               show_db(beer_manager);
            }
            else if(input.equals("query"))
            {
                query_db(beer_manager, scanner);
            }
            else
            {
                System.out.println("Wrong command!");
            }
        }

        beer_manager.close();
    }
}
