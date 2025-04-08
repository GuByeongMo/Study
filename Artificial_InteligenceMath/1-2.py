from sympy import symbols, Eq, solve

x, y, z = symbols("x y z")
ex1 = Eq(x + y + 2*z, 9)
ex2 = Eq(2*x + 4*y - 3*z, 1)
ex3 = Eq(3*x + 6*y - 5*z,0)
answer = solve((ex1, ex2, ex3), (x, y, z))
print(answer)
