from time import time

from table import *


def mrv():
    roz = []
    not_ready = [(INF, x) for x in req]
    tb = Table(teachers, groups)
    while len(roz) < len(req):
        for i in range(len(not_ready)):
            _, val = not_ready[i]
            not_ready[i] = (tb.ok_count(val), val)

        not_ready.sort()
        # print(not_ready)

        pos = tb.get_first(not_ready[0][1])
        if pos == None:
            print("Failed to do something")

            q = tb.pop()
            not_ready.append([INF, q])
            continue

        tb.add(pos)
        roz.append(pos)
        not_ready = not_ready[1:]

    paint(roz)


def step_evr():
    roz = []
    not_ready = [(INF, x) for x in req]
    tb = Table(teachers, groups)

    def count(ti):
        return -len([1 for _, x in not_ready if
                     x[GROUP_ID] in ti[GROUP_ID] or ti[GROUP_ID] in x[GROUP_ID] or x[TEACHER_ID] == ti[TEACHER_ID]])

    while len(roz) < len(req):
        for i in range(len(not_ready)):
            _, val = not_ready[i]
            not_ready[i] = (count(val), val)

        not_ready.sort()
        #  print(not_ready)

        pos = tb.get_first(not_ready[0][1])
        if pos == None:
            print("Failed to do something")

            q = tb.pop()
            not_ready.append([INF, q])
            continue

        tb.add(pos)
        roz.append(pos)
        not_ready = not_ready[1:]

    paint(roz)


def lcv():
    roz = []
    not_ready = [(INF, x) for x in req]
    tb = Table(teachers, groups)

    while len(roz) < len(req):
        for i in range(len(not_ready)):
            total_ok = 0
            for j in range(len(not_ready)):
                if i == j:
                    continue
                total_ok += tb.ok_count(not_ready[j][1])

            tp = tb.get_first(not_ready[i][1])
            roz.append(not_ready[i][1])
            tb.add(tp)

            for j in range(len(not_ready)):
                if i == j:
                    continue
                total_ok -= tb.ok_count(not_ready[j][1])

            tb.pop()
            roz = roz[:-1]

            _, val = not_ready[i]
            not_ready[i] = (total_ok, val)

        not_ready.sort()
        # print(not_ready)

        pos = tb.get_first(not_ready[0][1])
        if pos == None:
            print("Failed to add value")

            q = tb.pop()
            not_ready.append([INF, q])
            continue

        tb.add(pos)
        roz.append(pos)
        not_ready = not_ready[1:]

    paint(roz)


def forward_check(cands_key, cands_val, roz):
    if len(cands_key) == 0:
        paint(roz)
        return True

    for i in range(len(cands_key)):
        for j in range(len(cands_val[i])):
            key_cpy = cands_key.copy()
            val_cpy = cands_val.copy()
            del key_cpy[i]
            del val_cpy[i]
            q1, q2 = cands_val[i][j].split(' ')
            q2 = int(q2)
            qq = cands_key[i].copy()
            qq.extend([q1, q2])
            qq = tuple(qq)

            def inters_gr(gr1, gr2):
                m1 = gr1.split(':')
                m2 = gr2.split(':')
                if m1[0] != m2[0]:
                    return False
                if len(m1) == len(m2) and len(m2) == 2 and m1[1] != m2[1]:
                    return False
                return True

            def check(key, val):
                #  print("key,val =", key, val, "orig = ", cands_key[i], cands_val[i][j], cands_val[i][j] == val)
                res = (key[0] != cands_key[i][0] and not inters_gr(key[1], cands_key[i][1]))
                #  print(res)
                return res

            key_cpy, val_cpy = list(key_cpy), list(val_cpy)
            for i1 in range(len(val_cpy)):
                val_cpy[i1] = [y for y in val_cpy[i1] if (y != cands_val[i][j] or check(key_cpy[i1], y))]

            roz.append(qq)
            if forward_check(key_cpy, val_cpy, roz):
                return True
            roz.pop()

    return False


def forward_checking():
    cands = []
    pos = 0
    req_cpy = req.copy()
    val = []
    for i in req:
        val.append([])
        for day in days:
            for nom in range(maxPairCount):
                val[pos].append(convert(day, nom))
        pos += 1

    forward_check(req_cpy, val, [])


def constraint_prop():
    forward_checking()


if __name__ == "__main__":
    func = [mrv, step_evr, lcv, forward_checking, constraint_prop]

    print(f"req_cnt = {len(req)}, teacher_cnt = {len(teachers)}, groups_cnt = {len(groups)}")

    for x in func:
        t = time()
        x()
        print(f"func = {x.__name__}, time = {(time() - t) * 1000.:.3f}ms")
