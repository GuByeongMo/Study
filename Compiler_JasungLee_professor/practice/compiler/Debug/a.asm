LVALUE 매출액
PUSH 100
:=
LVALUE 매출원가
PUSH 50
:=
LVALUE 관리비
PUSH 10
:=
LVALUE 판매비
PUSH 20
:=
LVALUE 비용
RVALUE 매출원가
RVALUE 관리비
+
RVALUE 판매비
+
:=
LVALUE profit
RVALUE 매출액
RVALUE 비용
-
:=
HALT
$ -- END OF EXECUTION CODE AND START OF VAR DEFINITIONS --
DW 매출액
DW 매출원가
DW 관리비
DW 판매비
DW 비용
DW profit
END
