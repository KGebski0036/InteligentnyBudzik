using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BigBenTen.Modele
{
    internal class NormalAlarm : Alarm
    {
        public TimeSpan TimeToWakeUp { get; set; }
    }
}
