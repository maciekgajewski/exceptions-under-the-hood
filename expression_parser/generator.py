#!/usr/bin/env python3
# Generates random expressions
# prints them out in format: EXPRESSION=VALUE

import random

NUM = 100
MAX_DEPTH = 10


def rand_parent() -> bool:
    return random.randint(0, 10) > 5


def rand_just_number() -> bool:
    return random.randint(0, 10) < 7


def expr_no_parent(depth: int):
    just_number = rand_just_number()
    if depth > 1 and just_number or depth >= MAX_DEPTH:
        return str(random.randint(0, 1000))
    else:
        operator = random.choice(['+', '-'])
        return generate_expression(depth+1) + operator + generate_expression(depth+1)


def generate_expression(depth: int) -> str:
    ph = rand_parent()
    if ph:
        return '({})'.format(expr_no_parent(depth))
    else:
        return expr_no_parent(depth)


for i in range(NUM):
    expression = generate_expression(0)
    value = eval(expression)
    assert isinstance(value, int)
    print('{}={}'.format(expression, value))
