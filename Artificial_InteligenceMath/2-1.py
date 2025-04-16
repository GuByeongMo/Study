from sympy import symbols, Eq, simplify, solve

x, y = symbols('x y')

def line_equation_from_two_points(x1, y1, x2, y2):
    m = (y2 - y1) / (x2 - x1)
    eq = Eq(y - y1, m * (x - x1))  # 점-기울기 공식
    return simplify(eq)

print("2-1:", line_equation_from_two_points(1, -2, 3, 0))

print("2-2:", line_equation_from_two_points(3, 5, 0, -4))