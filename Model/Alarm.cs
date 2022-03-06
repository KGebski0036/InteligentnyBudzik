using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BigBenTen.Model
{
    abstract class Alarm
    {
        string Name { get; set; }
        bool Enabled { get; set; }
    }
}
