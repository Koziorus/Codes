using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Serialization;

namespace PT_lab3
{
    internal class Program
    {
        static void Main(string[] args)
        {
            List<Car> myCars = new List<Car>()
            {
                new Car("E250", new Engine(1.8, 204, "CGI"), 2009),
                new Car("E350", new Engine(3.5, 292, "CGI"), 2009),
                new Car("A6", new Engine(2.5, 187, "FSI"), 2012),
                new Car("A6", new Engine(2.8, 220, "FSI"), 2012),
                new Car("A6", new Engine(3.0, 295, "TFSI"), 2012),
                new Car("A6", new Engine(2.0, 175, "TDI"), 2011),
                new Car("A6", new Engine(3.0, 309, "TDI"), 2011),
                new Car("S6", new Engine(4.0, 414, "TFSI"), 2012),
                new Car("S8", new Engine(4.0, 513, "TFSI"), 2012)
            };

            var engines = from c in myCars
                               where c.model == "A6"
                               select new
                               {
                                   engineType = (c.engine.model == "TDI" ? "diesel" : "petrol"),
                                   hppl = c.engine.horsePower / c.engine.displacement
                               };

            var groupedEngines = from e in engines
                                 group e by e.engineType into g
                                 select new
                                 {
                                     g.Key,
                                     avgHppl = g.Average(x => x.hppl)
                                 };

            foreach(var g in groupedEngines)
            {
                Console.WriteLine($"{g.Key.ToString()}: {g.avgHppl.ToString()}");
            }

            XmlSerializer x = new XmlSerializer(myCars.GetType(), new XmlRootAttribute("cars"));

            x.Serialize(File.OpenWrite("C:\\Users\\Tomek\\Desktop\\PT_lab3 file\\CarsCollection.xml"), myCars);

            
        }

    }
}