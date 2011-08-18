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

            test3();

            test4();

            test5();
        }

        class ImmutableType
        {
            public readonly int i = 100;
            public ImmutableType(int _i)
            {
                i = _i;
            }
        };
        private static void test2()
        {
            //(new ImmutableType()).i = 999; // 无法对只读的字段赋值(构造函数或变量初始值指定项中除外)
            Console.WriteLine((new ImmutableType(888)).i);
        }

        delegate void LazyInitialization();
        class ClassWithoutSideEffects
        {
            string _value;
            public ClassWithoutSideEffects(Func<string> remoteInitialize)
            {
                Initialize = delegate()
                {
                    this._value = remoteInitialize();
                    Console.WriteLine("-LazyInitialization-");
                    Initialize = delegate() { };
                };
            }
            protected LazyInitialization Initialize;
            public string GetMeAValue()
            {
                Initialize();
                return _value;
            }
        }
        private static void test3()
        {
            ClassWithoutSideEffects c = new ClassWithoutSideEffects(() => "hello,world");
            Console.WriteLine("after construction");
            Console.WriteLine("GetMeAValue:" + c.GetMeAValue());
            Console.WriteLine("GetMeAValue:" + c.GetMeAValue());
            Func<string, Func<string>> lazyString = (stringToRetrieve) => () =>
            {
                return stringToRetrieve;
            };
            {
                // 不能在此范围内声明名为“c”的局部变量，因为这样会使“c”具有不同的含义，而它已在“父级或当前”范围中表示其他内容了
                ClassWithoutSideEffects c2 = new ClassWithoutSideEffects(lazyString("hello,world2"));
                Console.WriteLine("after construction");
                Console.WriteLine("GetMeAValue:" + c2.GetMeAValue());
            }
        }

        delegate void Counter(int iterations);
        private static void test4()
        {
            Counter RecursiveCount = null; // have to break to 2 lines
            RecursiveCount = ((iterations) =>
             {
                 if (iterations > 0)
                 {
                     RecursiveCount(iterations - 1);
                 }
                 Console.WriteLine("Curr count( " + iterations + ")");
             });
            RecursiveCount(10);
        }
        delegate void Counter2(Counter2 c, int iter);
        private static void test5()
        {
            Counter2 RecursiveCount = null; // have to break to 2 lines
            RecursiveCount = ((r2, iterations) =>
            {
                if (iterations > 0)
                {
                    r2(r2, iterations - 1);
                }
                Console.WriteLine("Curr count( " + iterations + ")");
            });
            RecursiveCount(RecursiveCount, 10);
        }


        private static void test1()
        {
            Func<string, Func<string, string>> curry =
                a => b => a + ":" + b;
            Console.WriteLine(curry("hello")("world"));            

        }
    }
}
