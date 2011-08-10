; PMT = P*R*(1+R)^N/((1+R)^N - 1)
; where
; P   = principal (mortgage)
; R   = interest rate for each period (expressed as a decimal)
; N   = number of periods (generally number of months for a mortgage)
; PMT = payment for each period (generally monthly payments for a mortgage) 

; ##########################################

  .486
  .model flat, stdcall
  option casemap :none   ; case sensitive

; ##########################################

  include \masm32\include\windows.inc
  include \masm32\include\user32.inc
  include \masm32\include\kernel32.inc
  include \masm32\include\Fpu.inc

  includelib \masm32\lib\user32.lib
  includelib \masm32\lib\kernel32.lib
  includelib \masm32\lib\Fpu.lib

; ##########################################

.data

mortgage      dt    0 ; 10 字节浮点
monthly_rate  dt    0
months        dt    0
temp_real     dt    0
loan_inp      db    "1000000",0
rate_inp      db    "8.9",0
year_inp      db    "25",0
result        db    25 dup(0)

.code

start:

;convert loan principal input to REAL10 and store it

    invoke FpuAtoFL, ADDR loan_inp, ADDR mortgage, DEST_MEM

;convert yearly interest (%) rate input to REAL10 and store it as a monthly fractional rate

    invoke FpuAtoFL,ADDR rate_inp, 0, DEST_FPU
    invoke FpuDiv, 0, 1200, ADDR monthly_rate, SRC1_FPU or SRC2_DIMM

;convert term of loan (years) input to REAL10 and store it as months

    invoke FpuAtoFL, ADDR year_inp, 0, DEST_FPU
    invoke FpuMul, 0, 12, ADDR months, SRC1_FPU or SRC2_DIMM

;compute the (1+R) portion, keeping it on the FPU, and then the (1+R)N term
;and storing it as a temporary REAL10 value

    invoke FpuAdd, ADDR monthly_rate, 1, 0, SRC1_REAL or SRC2_DIMM or DEST_FPU
    invoke FpuXexpY, 0, ADDR months, ADDR temp_real, SRC1_FPU or SRC2_REAL
	; DEST_MEM 是缺省的，不需要指定; temp_real = (1+R)^N

;compute the denominator of the equation and leave it on the FPU

    invoke FpuSub, ADDR temp_real, 1, 0, SRC1_REAL or SRC2_DIMM or DEST_FPU

;divide the (1+R)N term by that denominator and leave on the FPU

    invoke FpuDiv, ADDR temp_real, 0, 0, SRC1_REAL or SRC2_FPU or DEST_FPU
	; SRC2_FPU 表示操作数已经在 FPU 上，上一次指定了 DEST_FPU

;multiply by the other terms and leave answer on the FPU

    invoke FpuMul, 0, ADDR monthly_rate, 0, SRC1_FPU or SRC2_REAL or DEST_FPU
    invoke FpuMul, 0, ADDR mortgage, 0, SRC1_FPU or SRC2_REAL or DEST_FPU

;convert result to alphanumeric with 2 decimals and display

    invoke FpuFLtoA, 0, 2, ADDR result, SRC1_FPU or SRC2_DIMM ;  %.2G ?
    invoke MessageBox, 0, ADDR result, 0, MB_OK

    invoke ExitProcess, 0

end start

; ########################################## 
