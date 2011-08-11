; download from http://common-lisp.net/project/lispbox/
; lispbox is an IDE, a pre-configured packaging of 
;     the Emacs editing environment, 
;     SLIME (The Superior Lisp Interaction Mode for Emacs), 
;     the Quicklisp library manager, 
;     and the Clozure Common Lisp compiler.

; C-h ?
; C-h t ; tutorial
; C-x C-s ; save

; C-c C-c ; compile code, e.g. the "defun"
; C-c C-z ; enter CL-USER, then call the func just compiled
; 出错时选择 q 或者 4 退出 debug 模式

; SLIME 菜单
; C-c C-y 执行光标所在的函数名
; C-x C-e 光标之前的表达式，光标不能在单词中间,除非是数字之类可以截断
; C-c C-p 执行前面的表达式，单独窗口输出
; C-c C-r 执行选中区域

(+ 
  (+ 1 2 3)
   99
)
(print "hello, world")
(defun hello-world ()
  (print "my hello world!!!"))

(read-line *query-io*)


