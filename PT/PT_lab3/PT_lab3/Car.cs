using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Serialization;

namespace PT_lab3
{
    [XmlType(TypeName = "car")]
    public class Car
    {
        public string model;
        public int year;

        [XmlElement("motor")]
        public Engine engine;

        public Car(string model, Engine engine, int year)
        {
            this.model = model;
            this.engine = engine;
            this.year = year;
        }
        public Car() { }
    }
}
