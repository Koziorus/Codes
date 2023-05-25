import jakarta.persistence.Entity;
import jakarta.persistence.Id;
import jakarta.persistence.OneToMany;
import java.util.*;


@Entity
public class Browar {
    @Id
    private String name;

    public Browar()
    {

    }

    public String getName() {
        return name;
    }

    public List<Piwo> getPiwa()
    {
        return this.piwa;
    }
    private long wartosc;

    @OneToMany
    private List<Piwo> piwa;

    @Override
    public String toString()
    {
        String ret = "Browar{" +
                "name='" + name + '\'' +
                ", wartosc=" + wartosc +
                ", piwa=[";

        for(int i = 0; i < piwa.size(); i++)
        {
            if(i != 0)
            {
                ret += ", ";
            }
            ret += piwa.get(i).getName();
        }

        ret += "]}";

        return ret;
    }

    public Browar(String name, long wartosc) {
        this.name = name;
        this.wartosc = wartosc;
        this.piwa = new ArrayList<>();
    }
}
