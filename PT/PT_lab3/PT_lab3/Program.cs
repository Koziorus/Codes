using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection.Metadata;
using System.Text;
using System.Threading.Tasks;
using System.Xml;
using System.Xml.Linq;
using System.Xml.Serialization;
using System.Xml.XPath;

namespace PT_lab3
{
    internal class Program
    {

        private static void createXMLFromLinq(List<Car> myCars)
        {
            IEnumerable<XElement> nodes = from c in myCars
                                          select new
                                          {
                                              car = new XElement("car",
                                                        new XElement("model",
                                                        new XElement(c.model)),
                                                        new XElement("year",c.year),
                                                        new XElement("motor",
                                                            new XElement("displacement",c.engine.displacement),
                                                            new XElement("horsePower", c.engine.horsePower)))
                                          });

            XElement rootNode = new XElement("cars", nodes);

            rootNode.Save("CarsFromLinq.xml");
        }

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

            Console.WriteLine();

            string carsCollectionFilePath = "CarsCollection.xml";

            XmlSerializer x = new XmlSerializer(myCars.GetType(), new XmlRootAttribute("cars"));

            FileStream writer = File.OpenWrite(carsCollectionFilePath);

            x.Serialize(writer, myCars);

            writer.Close();


            XElement rootNode = XElement.Load(carsCollectionFilePath);
            double avgHP = (double)rootNode.XPathEvaluate("sum(/car/motor[@model!=\"TDI\"]/horsePower) div count(/car/motor[@model!=\"TDI\"]/horsePower)");

            Console.WriteLine(avgHP);

            Console.WriteLine();

            XmlNamespaceManager nsManager = new XmlNamespaceManager(new NameTable());
            nsManager.AddNamespace("xsi", "http://www.w3.org/2001/XMLSchema-instance");
            nsManager.AddNamespace("xsd", "http://www.w3.org/2001/XMLSchema");

            IEnumerable<XElement> models = rootNode.XPathSelectElements("car/model[not(. = preceding::model)]");

            foreach(XElement model in models)
            {
                Console.WriteLine(model.Value);
            }

            createXMLFromLinq(myCars);
        }

    }
}