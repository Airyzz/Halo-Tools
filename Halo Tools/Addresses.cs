using Airyz;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace HaloTools
{
    public static class Addresses
    {
        public static IntPtr fov;

        public static void ReadAllAddresses(AiryzMemory memory)
        {
            IntPtr BASE_ADDR = memory.GetBaseAddress("halo3.dll");

            fov = IntPtr.Add(BASE_ADDR, 0xB773684);
        }
    }
}
