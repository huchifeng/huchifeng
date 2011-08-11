(list :a :b :c)
"输出:A :B :C; 不能 (list a b c); 未定义"

(getf (list :a 100 :b 200) :b))
"plist - property list"

(defun make-cd (title artist rating ripped)
 ( list :title title :artist artist :rating rating :ripped ripped))

(make-cd 1 2 3 4)

(defvar *db* nil)
"全局变量命名约定"

(defun add-record (cd) (push cd *db*))

(add-record (make-cd 1 2 3 4))
(add-record (make-cd :a :b :c :d))

*db*

(defun dump-db ()
  (dolist (cd *db*)
    (format t "~{~a:~10t~a~%~}~%" cd)))
"format调用, t代表stdout, nil 则是字符串作为返回值 "
" ~r 英语数字表示法！"
"~a 字符串"
"~10t 空白对齐"
"~%换行"
"~{ ~} 枚举 list 中 key value"

(dump-db)

(defun dump-db ()
    (format t "~{~{~a:~10t~a~%~}~%~}" *db*))
"等效，省略了 do-list"

(dump-db)

*query-io*

(defun prompt-read (prompt)
  (format *query-io* "~a: " prompt)
  (force-output *query-io*)
  (read-line *query-io*))

(prompt-read "input a cd")

(or (parse-integer "123abc" :junk-allowed t) 0)
(or (parse-integer "abc" :junk-allowed t) 0)
; (or (parse-integer "abc") 0) 因为无法解析，导致异常

;( 注释()不必配对

'(
  "以下需要交互执行"
  (y-or-n-p "Ripped [y/n]: ")
  (loop (format t "hello")
     (if (y-or-n-p "done? [y/n]: ") (return)))
)

(defun save-db (filename)
 (with-open-file (out filename :direction :output :if-exists :supersede)
     (print *db* out)))

;(with-standard-io-syntax (print *db* t))
; with-standard-io-syntax 未定义？

; print 的输出可以读回

(save-db "tmp.txt")

(setf *db* nil)
*db*

(defun load-db (filename)
  (with-open-file (in filename)
  (setf *db* (read in))
  )
  t)

(load-db "tmp.txt")

*db*

(bye)


