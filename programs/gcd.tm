; Input: a string of 0's and 1's, eg '1111110111'
; Calculate greatest common divisor

; the finite set of states
#Q = {q0,q1,q2,q3,q4,q5,q6,q7,q8,q9,q10,q11,q12,q13,ok1,ok2,ok3}

; the finite set of input symbols
#S = {0,1}

; the complete set of tape symbols
#G = {0,1,_}

; the start state
#q0 = q0

; the blank symbol
#B = _

; the set of final states
#F = {ok1}

; the number of tapes
#N = 2 

; the transition functions

; State q0: begin
q0 1_ _1 rr q0
q0 0_ __ rl q1

; State q1:将两个数分割到两个纸带上之后
q1 11 11 rl q1
q1 _1 _1 lr q2
q1 1_ 1_ lr q3
q1 __ __ lr ok1

; State q2:第一条带比较短
q2 11 11 lr q2
q2 __ __ rl q4 

; State q3:第二条带比较短
q3 11 11 lr q3
q3 __ __ rl q5

; State q4:将q2的带头恢复到两端准备开始计算
q4 11 1_ rl q4
q4 _1 _1 l* q6
q4 1_ 1_ lr q8
q4 __ __ l* ok1

; State q5:将q3的带头恢复到两端准备开始计算
q5 11 _1 rl q5
q5 1_ 1_ *r q7
q5 _1 _1 lr q11
q5 __ __ *r ok2

; State q6:q4将除数减完一轮后带头转向继续
q6 11 1_ ll q6
q6 _1 _1 r* q4
q6 1_ 1_ rr q9
q5 __ __ r* ok1

; State q7:q5将除数减完一轮后带头转向继续
q7 11 _1 rr q7
q7 1_ 1_ *l q4
q7 _1 _1 ll q12
q7 __ __ *l ok3

; State q8:q4状态中除数将最后剩下的余数也减完后进入的修正状态，恢复余数
q8 1_ 11 lr q8
q8 __ __ rl q1

; State q9:q6状态中除数将最后剩下的余数也减完后进入的修正状态，恢复余数
q9 1_ 11 rr q9
q9 __ __ ll q10

; State q10:q9完成余数修正后进行带头的转向恢复为q1时带头的状态（第一条带带头在最左端，第二条带带头在最右端）
q10 11 11 l* q10
q10 _1 _1 r* q1

; State q11:q5状态中除数将最后剩下的余数也减完后进入的修正状态，恢复余数
q11 _1 11 lr q11
q11 __ __ rl q1

; State q12:q7状态中除数将最后剩下的余数也减完后进入的修正状态，恢复余数
q12 _1 11 ll q12
q12 __ __ rr q13

; State q13:q12完成余数修正后进行带头的转向恢复为q1时带头的状态（第一条带带头在最左端，第二条带带头在最右端）
q13 11 11 *r q13
q13 1_ 1_ *l q1

; State ok1:接受状态，此时得到结果且结果在第一条带上

; State ok2:得到结果且结果在第二条带上，最开始带头在最左端，将结果转移到第一条带上
ok2 _1 1_ rr ok2
ok2 __ __ ** ok1

; State ok3:得到结果且结果在第二条带上，最开始带头在最右端，将结果转移到第一根带上
ok3 _1 1_ ll ok3
ok3 __ __ ** ok1