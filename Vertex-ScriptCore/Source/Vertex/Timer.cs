using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Vertex
{
    [Serializable]
    public class Timer
    {
        private float seconds = 0f;
        public Timer() { Reset(); }
        public Timer(float seconds) { Reset(seconds); }

        public float GetSeconds() { return seconds; }
        public void SetSeconds(float seconds)
        {
            this.seconds = seconds;
        }

        public void Reset(float seconds = 0.0f)
        {
            this.seconds = seconds;
        }

        public void Update(float ts)
        {
            seconds += ts;
        }
        


    }
}
