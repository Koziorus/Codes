import jakarta.persistence.Id;
import jakarta.persistence.ManyToOne;

public class Mage {
    @Id
    private String name;
    private int level;
    @ManyToOne
    private Tower tower;
}
