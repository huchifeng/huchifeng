; download from http://common-lisp.net/project/lispbox/
; lispbox is an IDE, a pre-configured packaging of 
;     the Emacs editing environment, 
;     SLIME (The Superior Lisp Interaction Mode for Emacs), 
;     the Quicklisp library manager, 
;     and the Clozure Common Lisp compiler.

; C-h ?    
; C-h t    ; tutorial
; C-x C-s ; save
; M-W C-y ;内部复制粘贴
; C-x u   ;Undo
; C-z 切换到其他程序
; C-x C-c 结束

; clipboard-kill-region <cut>, <f20>, <menu-bar> <edit> <cut>.
; 描述 工具栏按钮，菜单？

; SLIME 菜单
; C-x C-e 执行光标之前的表达式，光标不能在单词中间,除非是数字之类可以截断
; C-c C-c ; compile code, e.g. the "defun"
; C-c C-z ; enter CL-USER, then call the func just compiled
;    出错时选择 q 或者 4 退出 debug 模式
; C-c C-y 执行光标所在的函数名
; C-c C-p 执行前面的表达式，单独窗口输出
; C-c C-r 执行选中区域

(+ 
  (+ 1 2 3)
   99
)
(print "hello, world")
(defun hello-world ()
  (print "my hello world!!!"))

(+ (parse-integer "123") 100)
(+ (parse-integer (read-line *query-io*)) 100)


(with-standard-io-syntax (print '("hello" "world") t))
; GCL 下 with-standard-io-syntax 未定义？ lispbox OK
(with-standard-io-syntax (print '((:hello "world")) t))

; setf 赋值
(setf xxx 123)
(+ xxx  xxx)

#'evenp ; #' 取函数
#'print  ; compiled function PRINT
(evenp 126)

(remove-if-not #'evenp '(1 2 3 5 6 7 8)) ; 保留偶数
(remove-if #'evenp '(1 2 3 5 6 7 8)) ; 删除偶数
(remove-if-not #'(lambda (x)(> x 3 )) '(1 2 3 5 6 7 8)) 

(defun high-order-f (f)
  (remove-if-not f '(1 2 3 4 5)))
(high-order-f #'(lambda (x)(= (mod x 2) 1)))
(defun return-f (y)
  #'(lambda (x) (< x y)))
(high-order-f (return-f 3))

(defun foo (ff &key a (b nil b-is-nil)(c nil c-is-nil))
  (list ff a b c b-is-nil c-is-nil))
(foo 100 :a 1 :c nil) 
; b 将对应 nil; 可以区分出明确传nil与省略得到的nil
(funcall #'foo 100 :a 99)

(defun foo ( a b c)(list a b c))
(foo 1 2) ; 运行时错误，参数太少


(mapcar #'(lambda (x)(* x x)) '(1 2 3)) ; 得到到 ( 1 4 9 )

(when t (+ 1 1)(+ 2 3)) ; 如果t，则连续执行，返回最后一个
(when t (setf r nil)(push 1 r)(push 2 r) r); 得到 r list
r ; 仍然可用

(when t (setf r nil)(push (list :a 1) r)(push (list :b 2) r) r)

(when t (setf r nil)(setf (getf r :aa) 100) r); 设置了 :aa

(defun f () 1 2 3)
(f) ; 得到最后一个表达式 3

(defmacro macro1 (expr)(reverse expr))
(macro1 (1 2 +)) ; 反过来然后执行
(macroexpand-1 '(macro1 ( 1 2 + ))) ; 只执行宏
(reverse '(1 2 3));
#'reverse ; compiled function; 在编译期可以用运行期的函数

'(1 2 (+ 3 4))
`(1 2 ,(+ 3 4)) ; 逗号开头的被执行； 例如用于作为webserver输出html时嵌入变量

`(and ,@(list 1 2) 3 4)  ;  拼接

(defun f(a b &rest the-rest) (list a b the-rest)); 收集剩余参数为列表
(f 1 2 3 4 5 )  ; 得到 (1 2 (3 4 5))

(defun xxx()
"document string ?"
1
)
(xxx )