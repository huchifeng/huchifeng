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
    public delegate void ProcessValue(int value);
    // delegate Func<....., TResult> // TResult cannot void

    public static class Extensions
    {//扩展方法必须在顶级静态类中定义；Extensions 是嵌套类
        public static void MyIterate(this IList<int> collection, ProcessValue cb
            )
        {
            foreach (int element in collection)
            {
                cb(element);
            }
        }
    }
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

    struct MyValueType
    {
        public int a; //=100; 结构中不能有实例字段初始值

    }
    class MyReferenceType
    {
        public int a = 200;
        ~MyReferenceType()
        {   // 只有类类型才能包含析构函数
            Console.WriteLine("~MyReferenceType");
        }
    }
    partial class MyPartial
    {
        partial void f();
        public void g()
        {
            f();
            f();
        }
    }
    class ClassWithProperty
    {
        public string a { get; set; }
        public string Name
        {
            get
            {
                return "ok";
            }
        }
    }
    class Program
    {

        static void Main(string[] args)
        {
            test1();
            test2();
            test3();
            test4();
            test5();
            test6();
            test7();

            
            TestDelegate.go();
            test8_anonynmous_delegate();

            test_destroy();

            test_stream();

            test_load_assembly();
            test_thread();

            test_nullable();

            (new MyPartial()).g();

            ClassWithProperty cp = new ClassWithProperty();
            cp.a = "999";
            Console.WriteLine(cp.a+":"+cp.Name);

            Console.WriteLine("all done");
        }

        private static void test_nullable()
        {
            MyValueType? v = null; // make nullable
            System.Nullable<MyValueType> v2 = new MyValueType();
            Console.WriteLine("nullable:" + v);
            Console.WriteLine("nullable:" + v2.Value.a);
        }

        class ThreadedTask
        {
            public void MethodToRun()
            {
                Console.WriteLine("from a class");
            }
        }
        private static void test_thread()
        {
            List<int> elements = new List<int>();
            elements.Add(10);
            elements.Add(20);

            Thread thread1 = new Thread(
            delegate()
            {
                lock (elements) // 如果不lock输出会交错
                {
                    foreach (int item in elements)
                    {
                        Console.WriteLine("Thread1: Item (" + item + ")");
                        Thread.Sleep(100);
                    }
                } 
                while (true)
                    Console.WriteLine("hello there, Thread1");
            }
            );
            Thread thread2 = new Thread(
            () =>
            {
                lock (elements)
                {
                    foreach (int item in elements)
                    {
                        Console.WriteLine("Thread2: Item (" + item + ")");
                        Thread.Sleep(100);
                    }
                }
                Console.WriteLine("Well then goodbye, Thread2");
            });
            thread2.Start();
            thread1.Start();
            new Thread(new ThreadStart(new ThreadedTask().MethodToRun)).Start();
            if(!thread1.Join(500))
                thread1.Abort(); // 
            thread2.Join();
        }

        private static void test_load_assembly()
        {
            string path = @"C:\_my2011\huchifeng\csharp-console-tester\ClassLibrary1\bin\Debug\ClassLibrary1.dll";
            //System.Reflection.Assembly asm = System.Reflection.Assembly.Load(path); // 需要强名称 public token?
            System.Reflection.Assembly asm = System.Reflection.Assembly.LoadFile(path);
            Console.WriteLine("load assembly:" + path + "," + asm);
            object obj = asm.CreateInstance("ClassLibrary1.Class1");
            Console.WriteLine("obj:" + obj);
        }

        private static void test_stream()
        {
            System.IO.Stream writer = Console.OpenStandardOutput();
            byte[] bytes = System.Text.Encoding.Default.GetBytes("System.IO.Stream 中文\n"); //.UTF8
            writer.Write(bytes, 0, bytes.Length);
            // TextWriter Console.Out
        }

        private static void test_destroy()
        {

            MyValueType v1 = new MyValueType();
            for (int i = 0; i < 2; i++)
            {
                MyReferenceType r1 = new MyReferenceType(); // 析构在函数调用返回之后，垃圾回收。与C++不同
                Console.WriteLine(i);
                goto DONE;
            }
        DONE: return;
        }

        class TestDelegate
        {
            static int _maxValue;
            static void ProcessMaximumValue(int value)
            {
                if (value > _maxValue)
                {
                    _maxValue = value;
                }
            }
            public static void go()
            {
                List<int> lst = new List<int> { 1, 2, 3, 4, 5, 6 };
                _maxValue = int.MinValue;
                lst.MyIterate(new ProcessValue(ProcessMaximumValue));
                Console.WriteLine("Maximum value is (" + _maxValue + ")");
            }
        }
        private static void test8_anonynmous_delegate()
        {
            List<int> lst = new List<int> { 1, 2, 3, 4 };
            int runningTotal = 0;
            lst.ForEach( // Iterate ?
            delegate(int value)
            {
                runningTotal += value;
            });
            Console.WriteLine("Running total is (" + runningTotal + ")");

            runningTotal = 1;
            lst.MyIterate(
                new ProcessValue( // can be omitted
                    delegate(int value)
                    {
                        runningTotal *= value;
                    }
                )
            );
            lst.MyIterate(
                (value) =>
                {
                    runningTotal *= value;
                }
            );
            Console.WriteLine("Running total is (" + runningTotal + ")");
        }

        private static void test7()
        {
            try
            {
                string s = null;
                s.Trim();
            }
            catch (Exception e)
            {
                Console.WriteLine(e);
            }
            finally
            {
                Console.WriteLine("finally ............");
            }
            try
            {
                throw new NotSupportedException("There is no code");
            }
            catch (NotSupportedException e)
            {
                Console.WriteLine(e);
            }
        }

        private static void test6()
        {
            int value;
            if (int.TryParse("1", out value))
            {
                Console.WriteLine("parse ok:" + value);
            }
            if (!int.TryParse("1a", out value))
            {
                Console.WriteLine("parse fail:" + value); // reset to 0
            }
            value = int.Parse("100", NumberStyles.HexNumber);
            Console.WriteLine("parse ok:" + value);
            CultureInfo info = Thread.CurrentThread.CurrentCulture;
            Console.WriteLine("Culture (" + info.EnglishName + ")"); // Culture (Chinese (Simplified, PRC))
        }

        private static void test5()
        {

            const string MSG = "   -CONST- ";
            Console.WriteLine(MSG.Trim());
            Console.WriteLine(MSG + "," + MSG.GetHashCode());
            Console.WriteLine("IsValueType:" + MSG.GetType().IsValueType); // false
            // value type: numbers, copy
            // ref type:
            //Console.WriteLine("bool.IsValueType:" + bool.IsValueType); // bool”并不包含“IsValueType”的定义
            //Console.WriteLine("Boolean.IsValueType:" + Boolean.IsValueType); // bool”并不包含“IsValueType”的定义
            Console.WriteLine("bool.IsValueType:" + (true).GetType().IsValueType); // True
            Console.WriteLine(2.GetHashCode()); // "2"

        }

        private static void test1()
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
            Console.WriteLine(Int64.MinValue + "," + Int64.MaxValue + "," + long.MaxValue + "," + ulong.MaxValue);

            Console.WriteLine((float)0xffffffff); //4.294967E+09
            Console.WriteLine((double)0xffffffff); // 4294967295

            Console.WriteLine((bool)(1 == 1)); // True
            Console.WriteLine((char)49); // '1'

            Console.WriteLine(sizeof(long)); // 8
            Console.WriteLine(sizeof(decimal)); // 16
        }

        private static void test4()
        {
            int[,] a2 = new int[,] { { 1, 2, 3 }, { 4, 5, 6 } };
            int[,] a22 = { { 1, 2, 3, 4 }, { 5, 6, 7, 8 } }; // 简写形式
            Console.WriteLine(a2); // System.Int32[,]
            int[][] a3 = new int[][] { new int[] { 1 }, new int[] { 1, 2, 3 } };
            int[][] a4 = new int[2][] { new int[] { 1 }, new int[] { 1, 2, 3 } };
            Console.WriteLine(a3); // System.Int32[][]
        }

        private static void test3()
        {
            string s = null;
            Console.WriteLine(s is object); // False
            s = "hello";
            Console.WriteLine(s is object); // True
            //Console.WriteLine((int)100 is object); // 警告 CS0183: 给定表达式始终为所提供的(“object”)类型
            //Console.WriteLine(null is object); //  警告 CS0184: 给定表达式始终不是所提供的(“object”)类型
        }

        private static void test2()
        {
            string s = "hello";
            string t = s;
            string u = string.Copy(s);
            Console.WriteLine(s + (s == t) + (s == u) + ((object)s == (object)t) + ((object)s == (object)u));
        }
    }
}
