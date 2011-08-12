(* 12345678901234567890 1234567890)
(+ 1010 -100)
(- 1 11111111111111111111111111111111111)
(/ 200 9)
(+  1 2 3 4 5)
(1+ 99)
(1- 99)
(= 9 (+ 1 8)) ; T 代表 true
(equal 9 (+ 4 5))
(/= 1 1)  ; NIL 代表 false null
(< 1 2)
(<= 1 1)
(> 1 2)
(>= 1 1)
; 没有 ~; 可能是 autolisp 的
'( 1 2 (+ 1 3) ) ; 不计算
"hello, world"
(write-line "1,2, 3") ; 运行过程中输出，最后自动打印返回值
(print "print")
(format t "hello, world")

(defun hello-world ()
   (format t "hello, world")) ; 输出大写
(hello-WORLD)

"标识符不能使用 ()\"|"
"可以用 _中文+-*/%{}[]!@#$^&?<>~' "
"大小写不分"

(load "loaded.lisp")
(hello-world-1)



(load (compile-file "loaded.lisp"))
(hello-world-1)
"compile-file 得到 .o "

(bye)