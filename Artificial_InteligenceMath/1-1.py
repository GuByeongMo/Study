from sympy import symbols, Eq, solve

x, y = symbols("x y")
ex1 = Eq(2*x + 3*y, 6)
ex2 = Eq(3*x + 2*y, 12)
answer = solve((ex1, ex2), (x, y))
print(answer)
