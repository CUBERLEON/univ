from req import *

def evaluate(x):
    # for teacher
    total = 0

    for teacher in teachers:
        x1 = [xval for xval in x if xval[0] == teacher]
        for day in days:
            x2 = [xval for xval in x1 if xval[2] == day]
            x2.sort(key=lambda x: x[3])
            for i in range(1, len(x2)):
                if x2[i][3] == x2[i - 1][3]:
                    return INF

            if len(x2):
                total += TEACHER_MUL * (x2[-1][3] - x2[0][3] + 1) ** 2 + TEACHER_DAY
                if x2[-1][3] > maxPairCount:
                    return INF

    for group in groups:
        x1 = [xval for xval in x if group in xval[1]]
        for day in days:
            x2 = [xval for xval in x1 if xval[2] == day]
            x2.sort(key=lambda x: x[3])
            for i in range(1, len(x2)):
                if x2[i][3] == x2[i - 1][3] and (
                        x2[i][3] == group or x2[i - 1][3] == group or x2[i][3] == x2[i - 1][3]):
                    return INF

            if len(x2):
                total += GROUP_MUL * (x2[-1][3] - x2[0][3] + 1) ** 2 + GROUP_DAY
                if x2[-1][3] > maxPairCount:
                    return INF

    for day in days:
        for nom in range(maxPairCount):
            x1 = [val for val in x if val[NOM_ID] == nom and val[DAY_ID] == day and ':' not in val[GROUP_ID]]
            if len(x1) > LARGE_COUNT:
                return INF
            x1 = [val for val in x if val[NOM_ID] == nom and val[DAY_ID] == day]
            if len(x1) > len(aud):
                return INF

    return total


def get_rand(req):
    result = []
    for x in req:
        day, pair = days[randint(0, len(days) - 1)], randint(1, maxPairCount)
        tmp = copy(x)
        tmp.extend([day, pair])
        result.append(tmp)

        while evaluate(result) >= INF:
            result.pop(-1)
            day, pair = days[randint(0, len(days) - 1)], randint(0, maxPairCount - 1)
            tmp = copy(x)
            tmp.extend([day, pair])
            result.append(tmp)
    return result


def mutate(v1, v2):
    res = []
    for x1, x2 in zip(v1, v2):
        if randint(0, 10) <= 5:
            res.append(x1)
        else:
            res.append(x2)
    return res


def print_res(x):
    for y in x:
        print(y, evaluate(y))






if __name__ == "__main__":
    cands = []
    count = 100
    for q in range(10):
        for i in range(count):
            z = (get_rand(req))
            cands.append(z)
        # print(z, evaluate(z))

        for x in cands[:count]:
            for y in cands[:count]:
                if x == y:
                    continue
                z = mutate(x, y)
                cands.append(z)

        cands = [x for x in cands if len(x) >= len(req)]
        cands.sort(key=lambda x: evaluate(x))
        cands = cands[:count]
        # print_res(cands)
        # print("\n\n")

    print_res(cands)
    paint(cands[0])
