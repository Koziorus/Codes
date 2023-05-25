import jakarta.persistence.Entity;
import jakarta.persistence.Id;
import jakarta.persistence.ManyToMany;
import jakarta.persistence.ManyToOne;

@Entity
public class Piwo {
    @Id
    private String name;

    private long cena;

    public Piwo()
    {

    }

    public Browar getBrowar()
    {
        return browar;
    }

    @Override
    public String toString()
    {
        return "Piwo{" +
                "name='" + name + '\'' +
                ", cena=" + cena +
                ", browar=" + browar.getName() +
                '}';
    }

    public String getName()
    {
        return name;
    }

    @ManyToOne
    private Browar browar;

    public Piwo(String name, long cena, Browar browar) {
        this.name = name;
        this.cena = cena;
        this.browar = browar;
    }
}
