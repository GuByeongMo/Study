from sympy import symbols, factor

# 변수 선언
x, y = symbols('x y')

# 1. 3xy - 6y^2 - x + 2y
expr1 = 3*x*y - 6*y**2 - x + 2*y
factored1 = factor(expr1)

# 2. 9x^2 - 4y^2 + 16y - 16
expr2 = 9*x**2 - 4*y**2 + 16*y - 16
factored2 = factor(expr2)

# 3. x^2 + xy - x - 2y - 2
expr3 = x**2 + x*y - x - 2*y - 2
factored3 = factor(expr3)

# 결과 출력
print("1:", factored1)
print("2:", factored2)
print("3:", factored3)
