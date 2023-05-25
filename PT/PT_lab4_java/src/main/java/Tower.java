import jakarta.persistence.Id;
import jakarta.persistence.OneToMany;

import java.util.List;

public class Tower {
    @Id
    private String name;
    private int height;
    @OneToMany
    private List<Mage> mages;
}
