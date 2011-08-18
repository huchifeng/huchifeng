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
        static void Main(string[] args)
        { 
            test1();
            test2();
        }

        private static void test2()
        {
            XDocument loaded = XDocument.Parse("<r><b><a a1=\"1\"><firstName>ff</firstName></a></b></r>");
            // Query the data and write out a subset of contacts
            foreach( var i in (from c in loaded.Descendants("a")
                    where (int)c.Attribute("a1") < 4
                    select (string)c.Element("firstName"))){
                Console.WriteLine(i);
            }
        }

        private static void test1()
        {

            int[] ai = { 1, 2, 3, 4, 5 };
            // from i in ai where i>2 select i; // 不能编译
            // var query = from i in ai where i>2 select i;
            // from -- where -- select
            Console.WriteLine("query:" + (from i in ai where i > 2 select i).Count());
            Console.WriteLine("query:" + (from i in (from i in ai where i > 2 select i * i) where i < 25 select i).Count());
            Console.WriteLine("query max:" + ai.Where((i) => i < 5).Where((i) => (i > 3)).Max());
            Console.WriteLine("query sum:" + ai.Where((i) => i < 5).Select((i) => i * i).Sum());
            foreach (var i in ((new int[] { 1, 3, 5 }).Select((i) => i * i)))
                Console.WriteLine(i);
            foreach (var i in ((new int[] { 1, 3, 5 }).SelectMany((i) => new int[] { i, i * i })))
                Console.WriteLine(i);
            // 匿名类
            foreach (var i in ((new int[] { 1, 3, 5 }).Select((i, index) => new { a = i, aa = i * i, index = index })))
                Console.WriteLine(i.a + ":" + i.aa + ";" + i.index);
            // JOIN
            foreach (var i in (from i in (new int[] { 1, 2 }) from j in (new int[] { 30, 40 }) select new { i, j }))
            {
                Console.WriteLine(i.i + ":" + i.j);
            }
            foreach (var i in (from i in (new string[] { "aa", "bbb" }) orderby i.Length descending select i + "," + i.Length))
                Console.WriteLine(i);
            foreach (var i in (from i in (new string[] { "aa", "bb", "bbb" }) group i by i.Length into gg select new { k = gg.Key, gg }))
            {
                Console.WriteLine(i.k + ":" + i.gg.Key + "," + i.gg.GetType().FullName);
                foreach (var j in i.gg)
                {
                    Console.WriteLine(j);
                }
            }
            foreach (var i in ((new string[] { "aa", "bbb" }).Union(new string[] { "aaa", "bbb" })))
            {
                Console.WriteLine("union:" + i);
            }

        }
    }
}
