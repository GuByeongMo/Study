from sympy import symbols, Eq, simplify, solve

def line_equation_from_point_slope(x1, y1, m):
    eq = Eq(y - y1, m * (x - x1))
    return simplify(eq)

print("2-3:", line_equation_from_point_slope(5, 4, 3))

print("2-4:", line_equation_from_point_slope(3, 2, -2))