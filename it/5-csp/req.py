maxPairCount = 4
INF = 100000000
TEACHER_DAY = 20
GROUP_DAY = 100
TEACHER_MUL = 1
GROUP_MUL = 5
teachers = ['Ivanov', 'Petrov', 'Sidorov', 'Vasya', 'Petya', 'Dima']
groups = ['Inf1', 'Inf2', 'MI3', 'MI4']

days = ['Monday', 'Tuesday', 'Wednesday','Thusday',  'Friday']

req = [['Ivanov', 'Inf1'],
       ['Petrov', 'MI4'],
       ['Sidorov', 'MI4:1'],
       ['Petrov', 'MI4:2'],
       ['Ivanov', 'MI4'],
       ['Sidorov', 'MI4'],
       ['Vasya', 'MI4:1'],
       ['Vasya', 'MI4:2'],

       ['Ivanov', 'Inf2'],
       ['Petrov', 'Inf2'],
       ['Sidorov', 'Inf2:1'],
       ['Petrov', 'Inf2:2'],
       ['Ivanov', 'Inf2'],
       ['Sidorov', 'Inf2'],
       ['Vasya', 'Inf2:1'],
       ['Vasya', 'Inf2:2'],

       ['Ivanov', 'MI4'],
       ['Sidorov', 'MI4'],
       ['Vasya', 'MI4:1'],
       ['Vasya', 'MI4:2'],
       ['Ivanov', 'MI4'],
       ['Sidorov', 'MI4'],
       ['Vasya', 'MI4:1'],
       ['Vasya', 'MI4:2'],
       ['Ivanov', 'MI4'],
       ['Sidorov', 'MI4'],
       ['Vasya', 'MI4:1'],
       ['Vasya', 'MI4:2'],
       ['Ivanov', 'MI3'],
       ['Sidorov', 'MI3'],
       ['Petya', 'MI3:1'],
       ['Vasya', 'MI3:2'],
       ['Ivanov', 'MI3'],
       ['Sidorov', 'MI3'],
       ['Petya', 'MI3:1'],
       ['Vasya', 'MI3:2'],
       ['Ivanov', 'MI3'],
       ['Sidorov', 'MI3'],
       ['Petya', 'MI3:1'],
       ['Vasya', 'MI3:2'],
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
SMALL_COUNT = len(list(x for x in aud if x[1] == 'S'))

# format (teacher, group, day, pair_nom)
TEACHER_ID = 0
GROUP_ID = 1
DAY_ID = 2
NOM_ID = 3


def paint(x):
    qlen = 25
    print(x)
    return
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
