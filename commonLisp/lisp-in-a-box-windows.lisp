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

(+ 
  (+ 1 2 3)
   99
)
(print "hello, world")
(defun hello-world ()
  (print "my hello world!!!"))

