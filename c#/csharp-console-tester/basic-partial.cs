using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Globalization; // NumberStyles
using System.Threading;

/*

 * Common Interme-diate Language (CIL)
 * 
 * 
 */
namespace csharp_console_tester
{
    partial class MyPartial{
        partial void f()
        {// partial method, must private void;
            Console.WriteLine("partial f");
        }
    }
}
