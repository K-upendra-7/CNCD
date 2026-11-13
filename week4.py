
def first(ch):
    if ch not in grammar:
        return {ch}

    result = set()

    for p in grammar[ch]:
        if p == '#':
            result.add('#')
            continue

        for c in p:
            f = first(c)
            result.update(f - {'#'})
            if '#' not in f:
                break
        else:
            result.add('#')

    return result


def follow(ch):
    result = {'$'} if ch == start else set()

    for lhs in grammar:
        for p in grammar[lhs]:
            for i, c in enumerate(p):
                if c == ch:
                    suffix = p[i + 1:]

                    if suffix:
                        for s in suffix:
                            f = first(s)
                            result.update(f - {'#'})
                            if '#' not in f:
                                break
                        else:
                            if lhs != ch:
                                result.update(follow(lhs))
                    elif lhs != ch:
                        result.update(follow(lhs))

    return result


grammar = {}
n = int(input("Enter number of productions: "))

for _ in range(n):
    lhs, rhs = input().replace(" ", "").split("->")
    grammar.setdefault(lhs, []).extend(rhs.split('|'))

start = next(iter(grammar))
table = {}
terminals = set()

for nt in grammar:
    for p in grammar[nt]:
        f = set()

        if p == '#':
            f.add('#')
        else:
            for c in p:
                temp = first(c)
                f.update(temp - {'#'})
                if '#' not in temp:
                    break
            else:
                f.add('#')

        for t in f - {'#'}:
            if (nt, t) in table:
                print("Grammar is not LL(1)")
                exit()
            table[nt, t] = p
            terminals.add(t)

        if '#' in f:
            for t in follow(nt):
                if (nt, t) in table:
                    print("Grammar is not LL(1)")
                    exit()
                table[nt, t] = p
                terminals.add(t)

terminals.add('$')

print("\nPREDICTIVE PARSING TABLE")
print("NT\t" + "\t".join(sorted(terminals)))

for nt in grammar:
    print(nt, end="\t")
    for t in sorted(terminals):
        print(table.get((nt, t), "-"), end="\t")
    print()

s = input("\nEnter input string: ") + '$'
stack = ['$', start]
i = 0

while stack:
    top = stack[-1]
    current = s[i]

    if top == current == '$':
        print("String Accepted")
        break

    if top == current:
        stack.pop()
        i += 1

    elif top in grammar:
        if (top, current) not in table:
            print("String Rejected")
            break

        p = table[top, current]
        stack.pop()

        if p != '#':
            stack.extend(reversed(p))

    else:
        print("String Rejected")
        break
