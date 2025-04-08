from sympy import symbols, Eq, simplify, solve

x, y = symbols('x y')

def line_equation_from_two_points(x1, y1, x2, y2):
    m = (y2 - y1) / (x2 - x1)
    eq = Eq(y - y1, m * (x - x1))  # 점-기울기 공식
    return simplify(eq)

def line_equation_from_point_slope(x1, y1, m):
    eq = Eq(y - y1, m * (x - x1))
    return simplify(eq)