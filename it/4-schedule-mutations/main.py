from copy import copy
from random import randint

maxPairCount = 4
INF = 100000000
TEACHER_DAY = 20
GROUP_DAY = 100
TEACHER_MUL = 1
GROUP_MUL = 5
teachers = ['Ivanov', 'Petrov', 'Sidorov', 'Vasya', 'Petya', 'Dima']
groups = ['Inf1', 'Inf2', 'MI3', 'MI4']

days = ['Monday', 'Tuesday', 'Wednesday', 'Thusday', 'Friday']

req = [['Ivanov', 'Inf1'],
       ['Petrov', 'MI4'],
       ['Sidorov', 'MI4:1'],
       ['Petrov', 'MI4:2'],
       ['Ivanov', 'MI4'],
       ['Sidorov', 'MI4'],
       ['Vasya', 'MI4:1'],
       ['Vasya', 'MI4:2'],
       ['Ivanov', 'MI3'],
       ['Sidorov', 'MI3'],
       ['Petya', 'MI3:1'],
       ['Vasya', 'MI3:2'],
       ['Sidorov', 'MI3'],
       ['Sidorov', 'MI3'],
       ['Ivanov', 'Inf2'],
       ['Sidorov', 'Inf2'],
       ['Petya', 'Inf2'],
       ['Vasya', 'Inf2'],
       ['Sidorov', 'Inf2:1'],
       ['Sidorov', 'Inf2:2'],
       ['Dima', 'Inf1'],
       ['Dima', 'Inf1'],
       ['Dima', 'Inf1'],
       ['Dima', 'Inf1'],
       ['Dima', 'Inf1'],
       ['Dima', 'Inf1'],
       ['Dima', 'Inf1:2'],
       ['Dima', 'Inf1:1'],
       ]

aud = [
    [304, 'S'],
    [305, 'S'],
    [306, 'S'],
    [307, 'S'],
    [41, 'L'],
    [42, 'L'],
       [43, 'L'],
    #   [44, 'L']
]
LARGE_COUNT = len(list(x for x in aud if x[1] == 'L'))

# format (teacher, group, day, pair_nom)
TEACHER_ID = 0
GROUP_ID = 1
DAY_ID = 2
NOM_ID = 3


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


def paint(x):
    qlen = 25

    qres = [[None for y in range(2 * len(days) * maxPairCount)] for x in range(len(groups))]
    for i in range(len(days)):
        day = days[i]

        x1 = [xval for xval in x if xval[2] == day]
        x1.sort(key=lambda x: x[NOM_ID])
        for nom in range(0, maxPairCount):
            x2 = [val for val in x1 if val[NOM_ID] == nom]
            id = 0
            used = [False for x in range(len(aud))]

            for group in groups:
                x3 = [val for val in x2 if group in val[GROUP_ID]]
                # x3 = [x[TEACHER_ID] for x in x3]

                qres[id][(i * maxPairCount + nom) * 2] = ' ' * qlen
                qres[id][(i * maxPairCount + nom) * 2 + 1] = ' ' * qlen

                for val in x3:
                    au = -1
                    if ':' in val[GROUP_ID]:
                        for ii in range(len(aud)):
                            if not used[ii] and aud[ii][1] == 'S':
                                au = ii
                    if au == -1:
                        for ii in range(len(aud)):
                            if not used[ii] and aud[ii][1] == 'L':
                                au = ii

                    used[au] = True
                    cc = val[TEACHER_ID] + '  ' + val[GROUP_ID] + ' ' + str(aud[au][0]) + "(" + aud[au][1] + ")"
                    if val[GROUP_ID][-1:] == '2':
                        qres[id][(i * maxPairCount + nom) * 2 + 1] = cc + ' ' * (qlen - len(cc))
                    else:
                        qres[id][(i * maxPairCount + nom) * 2] = cc + ' ' * (qlen - len(cc))
                id += 1
    for j in range(2 * len(days) * maxPairCount):
        if j % (2 * maxPairCount) == 0:
            q = "#" * (qlen + 1)
            print(q * (len(groups)))
        elif j % 2 == 0:
            q = "-" * (qlen + 1)
            print(q * (len(groups)))
        for i in range(len(groups)):
            print(qres[i][j], end='|')
        print()


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
