using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Globalization; // NumberStyles
using System.Threading;
using System.Xml.Linq;

/*

 * Common Interme-diate Language (CIL)
 * 
 * 
 */
namespace csharp_console_tester
{

    class Program
    {
        interface IC
        {
            void f();
        };
        class C1 : IC
        {
            public void f()
            {
                Console.WriteLine("f");
            }
        }
        class A<C> : IC where C :  class, IC,new() // new() 约束必须是指定的最后一个约束 // “class”或“struct”约束必须在其他任何约束之前
        // “C”是一个类型形参，无法从它进行派生
        {
            //C c1 = new C(); // 变量类型“C”没有 new() 约束，因此无法创建该类型的实例
            C c1;
            public A()
            {
                c1 = new C();
            }
            //public string i = C.Name; // “C”是一个“类型形参”，这在给定的上下文中无效
            public void f()
            {
                c1.f();
                Console.WriteLine("c1.f");
            }
        };
        static void Main(string[] args)
        {
            (new A<C1>()).f();

        }
    }
}
