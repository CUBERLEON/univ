from req import *


def convert(*args):
    res = ''
    for x in args:
        res = res + ' ' + str(x)
    return res[1:]


class Table():
    def __init__(self, teach, gr):
        self.roz = []
        self.teach = dict()
        self.group = dict()
        self.large = dict()
        self.small = dict()

        self.teacher_count = dict()
        self.group_count = dict()

        for te in teach:
            self.teacher_count[te] = 0

        for gr in groups:
            self.group_count[gr] = 0

        for da in days:
            for pa in range(maxPairCount):
                for te in teach:
                    self.teach[convert(te, da, pa)] = False
                for gr in groups:
                    self.group[convert(gr, da, pa)] = [False, False]

                self.large[convert(da, pa)] = 0
                self.small[convert(da, pa)] = 0

    def add(self, ti):
        self.roz.append(ti)
        self.teacher_count[ti[TEACHER_ID]] += 1

        c_gr = ti[GROUP_ID]
        if len(c_gr.split(":")) > 1:
            c_gr, sub_id = c_gr.split(":")
            self.small[convert(ti[DAY_ID], ti[NOM_ID])] += 1
            self.group[convert(c_gr, ti[DAY_ID], ti[NOM_ID])] = [sub_id == '1', sub_id == '2']
        else:
            self.group[convert(ti[GROUP_ID], ti[DAY_ID], ti[NOM_ID])] = [True, True]
            self.large[convert(ti[DAY_ID], ti[NOM_ID])] += 1

        self.teach[convert(ti[TEACHER_ID], ti[DAY_ID], ti[NOM_ID])] = True

    def pop(self):
        ti = self.roz[-1]
        self.roz = self.roz[:-1]
        c_gr = ti[GROUP_ID]
        if len(c_gr.split(":")) > 1:
            c_gr, sub_id = c_gr.split(":")
            self.small[convert(ti[DAY_ID], ti[NOM_ID])] -= 1
            old = self.group[convert(c_gr, ti[DAY_ID], ti[NOM_ID])]
            self.group[convert(c_gr, ti[DAY_ID], ti[NOM_ID])] = [sub_id == '2' and old[0], sub_id == '1' and old[1]]
        else:
            self.group[convert(ti[GROUP_ID], ti[DAY_ID], ti[NOM_ID])] = [False, False]
            self.large[convert(ti[DAY_ID], ti[NOM_ID])] -= 1

        self.teach[convert(ti[TEACHER_ID], ti[DAY_ID], ti[NOM_ID])] = False
        return (ti[0], ti[1])

    def is_ok(self, ti):

        c_gr = ti[GROUP_ID]

        da, pa, te = ti[DAY_ID], ti[NOM_ID], ti[TEACHER_ID]

        if len(c_gr.split(":")) > 1:
            c_gr, sub_id = c_gr.split(":")
            sm = self.small[convert(da, pa)]
            la = self.large[convert(da, pa)]
            if sm + la >= SMALL_COUNT + LARGE_COUNT:
                return False

            if self.teach[convert(te, da, pa)]:
                return False

            if self.group[convert(c_gr, da, pa)][int(sub_id) - 1]:
                return False
        else:
            la = self.large[convert(da, pa)]
            if la >= LARGE_COUNT:
                return False

            if self.teach[convert(te, da, pa)]:
                return False

            if self.group[convert(c_gr, da, pa)] != [False, False]:
                return False

        return True

    def ok_count(self, ti):
        res = 0
        for day in days:
            for nom in range(maxPairCount):
                tmp = (ti[0], ti[1], day, nom)
                if self.is_ok(tmp):
                    res += 1
        return res

    def get_first(self, ti):
        for day in days:
            for nom in range(maxPairCount):
                tmp = (ti[0], ti[1], day, nom)
                if self.is_ok(tmp):
                    return tmp
        return None
