using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BigBenTen.Modele
{
    internal abstract class Alarm
    {
        string Nazwa { get; set; }
        bool Enable { get; set; }
    }
}
