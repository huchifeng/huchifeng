using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace csharp_console_tester
{
    class A
    {
        public int i = 100;
        public static Random r = new Random();
        public double j = r.NextDouble(); // 错误 CS0236: 字段初始值无法引用非静态字段、方法或属性“csharp_console_tester.A.r”
        public enum color
        {
            R, G, B
        }
    }
    class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("hello, world"); // System.Console
            A a = new A();
            Console.WriteLine(a.i + "," + a.j);
            Console.WriteLine("i={1}, j={0}", a.j, a.i);
            Console.WriteLine("string:{0}, int:{1}", A.color.G, (int)A.color.G); // default as string

            Console.WriteLine((sbyte)(511 + 0 * a.i)); // -1
            Console.WriteLine((byte)(511 + 0 * a.i)); // 255
            Console.WriteLine((short)(65535 * 2 + 0 * a.i)); // -2
            Console.WriteLine((ushort)(65535 * 2 + 0 * a.i)); // 65534
            Console.WriteLine((int)(0xffffffff + 0 * a.i)); // -1
            Console.WriteLine((uint)(0xffffffff + 0 * a.i)); // 4294967295
            Console.WriteLine((long)(0xffffffffffffffff + (ulong)0 * (ulong)a.i)); // -1
            Console.WriteLine((ulong)(0xffffffffffffffff + (ulong)0 * (ulong)a.i)); // 18446744073709551615
            Console.WriteLine(Int64.MinValue + "," + Int64.MaxValue + "," + ulong.MaxValue);

            Console.WriteLine((float)0xffffffff); //4.294967E+09
            Console.WriteLine((double)0xffffffff); // 4294967295

            Console.WriteLine((bool)(1 == 1)); // True
            Console.WriteLine((char)49); // '1'

            Console.WriteLine(sizeof(long)); // 8
            Console.WriteLine(sizeof(decimal)); // 16

            {
                string s = "hello";
                string t = s;
                string u = string.Copy(s);
                Console.WriteLine(s + (s == t) + (s == u) + ((object)s == (object)t) + ((object)s == (object)u));
            }
            {
                string s = null;
                Console.WriteLine(s is object); // False
                s = "hello";
                Console.WriteLine(s is object); // True
                //Console.WriteLine((int)100 is object); // 警告 CS0183: 给定表达式始终为所提供的(“object”)类型
                //Console.WriteLine(null is object); //  警告 CS0184: 给定表达式始终不是所提供的(“object”)类型
            }
            {
                int[,] a2 = new int[,] { { 1, 2, 3 }, { 4, 5, 6 } };
                int [,] a22 = {{1,2,3,4}, {5,6,7,8}}; // 简写形式
                Console.WriteLine(a2); // System.Int32[,]
                int[][] a3 = new int[][] { new int[]{1}, new int[]{1,2,3}};
                int[][] a4 = new int[2][] { new int[] { 1 }, new int[] { 1, 2, 3 } };
                Console.WriteLine(a3); // System.Int32[][]
            }
        }
    }
}
