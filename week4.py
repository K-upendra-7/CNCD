grammar = {}
firstSet = {}
followSet = {}
table = {}

def FIRST(ch):
    if ch not in grammar:
        return {ch}
    return firstSet[ch]

def FOLLOW(ch):
    return followSet[ch]

def firstOfString(s):
    if not s or s == '#':
        return {'#'}

    result = set()

    for ch in s:
        temp = FIRST(ch)
        result.update(temp - {'#'})

        if '#' not in temp:
            return result

    result.add('#')
    return result

def calculateFirst():
    changed = True

    while changed:
        changed = False

        for nt, productions in grammar.items():
            for p in productions:
                temp = firstOfString(p)

                if not temp.issubset(firstSet[nt]):
                    firstSet[nt].update(temp)
                    changed = True

def calculateFollow():
    followSet[start].add('$')
    changed = True

    while changed:
        changed = False

        for lhs, productions in grammar.items():
            for p in productions:
                for i, ch in enumerate(p):
                    if ch not in grammar:
                        continue

                    temp = firstOfString(p[i + 1:])
                    old = len(followSet[ch])

                    followSet[ch].update(temp - {'#'})

                    if '#' in temp:
                        followSet[ch].update(followSet[lhs])

                    if len(followSet[ch]) > old:
                        changed = True

n = int(input("Enter number of productions: "))
print("Enter productions (E->TR):")

for i in range(n):
    s = input().replace(" ", "")
    lhs, rhs = s.split("->")

    if i == 0:
        start = lhs

    grammar.setdefault(lhs, []).extend(rhs.split('|'))

for nt in grammar:
    firstSet[nt] = set()
    followSet[nt] = set()

calculateFirst()
calculateFollow()

print("\nFIRST SETS")
for nt in sorted(grammar):
    print(f"FIRST({nt}) = {{ {' '.join(sorted(FIRST(nt)))} }}")

print("\nFOLLOW SETS")
for nt in sorted(grammar):
    print(f"FOLLOW({nt}) = {{ {' '.join(sorted(FOLLOW(nt)))} }}")

terminals = {'$'}

for nt, productions in grammar.items():
    for p in productions:
        temp = firstOfString(p)

        for t in temp - {'#'}:
            if (nt, t) in table:
                print("Grammar is not LL(1)")
                exit()

            table[nt, t] = p
            terminals.add(t)

        if '#' in temp:
            for t in FOLLOW(nt):
                if (nt, t) in table:
                    print("Grammar is not LL(1)")
                    exit()

                table[nt, t] = p
                terminals.add(t)

print("\nPREDICTIVE PARSING TABLE")
print(f"{'NT':<10}", end="")

for t in sorted(terminals):
    print(f"{t:<10}", end="")
print()

for nt in sorted(grammar):
    print(f"{nt:<10}", end="")

    for t in sorted(terminals):
        print(f"{table.get((nt, t), '-'):<10}", end="")
    print()

s = input("\nEnter input string: ") + '$'
stack = ['$', start]
i = 0
accepted = False

print("\nPREDICTIVE PARSER TRACE TABLE")
print(f"{'STACK':<20}{'INPUT':<20}ACTION")

while stack:
    currentStack = ''.join(reversed(stack))
    remaining = s[i:]
    top = stack[-1]
    current = s[i] if i < len(s) else None

    print(f"{currentStack:<20}{remaining:<20}", end="")

    if top == current == '$':
        print("ACCEPT")
        accepted = True
        break

    if top == current:
        print(f"Match {current}")
        stack.pop()
        i += 1

    elif top in grammar:
        if (top, current) not in table:
            print("ERROR")
            break

        p = table[top, current]
        print(f"{top}->{p}")
        stack.pop()

        if p != '#':
            stack.extend(reversed(p))

    else:
        print("ERROR")
        break

print("\nString Accepted" if accepted else "\nString Rejected")
