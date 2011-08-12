; common lisp 函数与变量名字空间独立; lisp-2
(when t (setq double 202)(defun double (x)(* 2 x))(double double))

; scheme 属于 lisp-1, 统一名字空间


(setq x 1)
(setq x (+ x 1))


