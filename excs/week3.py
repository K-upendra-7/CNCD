def first(symbol):
    if symbol not in grammar:
        return {symbol}

    if symbol in visited:
        return set()

    visited.add(symbol)
    result = set()

    for production in grammar[symbol]:
        if production == '#':
            result.add('#')
            continue

        for ch in production:
            temp = first(ch)
            result.update(temp - {'#'})

            if '#' not in temp:
                break
        else:
            result.add('#')

    visited.remove(symbol)
    return result


def follow(symbol):
    result = set()

    if symbol == start:
        result.add('$')

    for lhs in grammar:
        for production in grammar[lhs]:
            for i, ch in enumerate(production):
                if ch == symbol:
                    if i + 1 < len(production):
                        next_first = set()
                        for s in production[i + 1:]:
                            temp = first(s)
                            next_first.update(temp - {'#'})
                            if '#' not in temp:
                                break
                        else:
                            result.update(follow(lhs))

                        result.update(next_first)
                    elif lhs != symbol:
                        result.update(follow(lhs))

    return result


grammar = {}
n = int(input("Enter number of productions: "))

for _ in range(n):
    lhs, rhs = input().replace(" ", "").split("->")
    grammar.setdefault(lhs, []).extend(rhs.split('|'))

start = next(iter(grammar))

print("First: ")
for nt in grammar:
    visited = set()
    print("FIRST(" + nt + ") =", first(nt))

print("Follow: ")
for nt in grammar:
    print("FOLLOW(" + nt + ") =", follow(nt))
