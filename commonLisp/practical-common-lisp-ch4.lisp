； 只有部分 s-expression 是合法的 lisp form
； ("foo" 1 2) 不是; (foo 1 2 ) 才是

123       ; the integer one hundred twenty-three
3/7       ; the ratio three-sevenths
1.0       ; the floating-point number one in default precision
1.0e0     ; another way to write the same floating-point number
1.0d0     ; the floating-point number one in "double" precision
1.0e-4    ; the floating-point equivalent to one-ten-thousandth
+42       ; the integer forty-two
-42       ; the integer negative forty-two
-1/4      ; the ratio negative one-quarter
-2/8      ; another way to write negative one-quarter
246/2     ; another way to write the integer one hundred twenty-three
; 内部存储 2/8 已经简化为 1/4

"foo"     ; the string containing the characters f, o, and o.
"fo\o"    ; the same string
"fo\\o"   ; the string containing the characters f, o, \, and o.
"fo\"o"   ; the string containing the characters f, o, ", and o.

(defun \fo\o () t) ; 被 escape 就不会自动转大写 |fOo|
(defun |fooABC| () t) ; 同escape

x             ; the symbol X
()            ; the empty list; 即 NIL
(1 2 3)       ; a list of three numbers
("foo" "bar") ; a list of two strings
(x y z)       ; a list of three symbols
(x 1 "foo")   ; a list of a symbol, a number, and a string
(+ (* 2 3) 4) ; a list of a symbol, a list, and a number.

*x*
PI ;
NIL nil
:abc ; 自动得到 :ABC; 作为 keyword

(if t (format t "yes") (format t "no"))
; if 是特别 operator; 一般function 无法实现短路
; quote 也是 operator
; let 也是 operator

(let ((x 10)) x)

(dolist (x '(1 2)) (print x))
; (x '(1 2)) 不是合法 form, 说明 dolist 是 macro

't    ; 加 ' 与不加等价
'nil
'()

(eq 2 (+ 1 1)) ; T, 但与实现有关； 对象同一性比较
(eq 1000000 (* 1000 1000)); T
(eq 1 1.0) ; NIL
(eq '(1 2) '(1 2)) ; NIL
(eql 2 (+ 1 1)) ; 应该保证T，对数字和字母要特别处理；比eq更松; 提倡总是eql代替eq
(eql 1 1.0) ; NIL
(eql '(1 2) '(1 2)); NIL
(equal '(1 2) '(1 2) ) ; T, 比eql更松
(equal "hello" "HELLO") ; NIL
(equal 1 1.0); NIL
(equalp "hello" "HELLO"); T; 比 equalp 更松
(equalp 1 1.0);  T
(equalp '(1 2) '(1.0 2)); T


;;;; 文件级注释
;;; 段落注释
;; 函数内块注释
; 行尾注释

(list :A 100)
'(:A 100) ; 相同


