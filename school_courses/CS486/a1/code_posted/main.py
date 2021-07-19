from solve import *


def sort_hash(los):
    """
    Sort the given list of states in decreasing order based on hash id, State.id

    :param los: List[State]
    :return: List[State]
    """

    state_dict = {}
    for i in los:
        state_dict[i.id] = i

    los_sorted = sorted(state_dict, reverse=True)

    for i in range(len(los_sorted)):
        los_sorted[i] = state_dict[los_sorted[i]]

    return los_sorted


def sort_f(los):
    """
    Sort the given list of states in decreasing order based on f value, hash id, (parent hash id)
    :param los: List[State]
    :return: List[State]
    """
    los_sorted = sorted(los, key=lambda x: (x.f, x.id, x.parent.id))

    return los_sorted


if __name__ == "__main__":
    boards = from_file("jams_posted.txt")

    # # test1: get_successors
    # test_board = boards[0]
    # s1 = State(boards[0], zero_heuristic, 0, 0)
    # for s in get_successors(s1):
    #     s.board.display()
    #

    # # test2: dfs
    # test_board = boards[0]
    # l = dfs(test_board)[0]
    #
    # for s in l[0:3]:
    #     s.board.display()
    #     # print(s.id)
    # print(dfs(test_board)[1])

    # test3: 40 boards dfs and A*
    # for b in boards:
    #     # print(dfs(b)[1])  # max recursion on 14th
    #     print((a_star(b, blocking_heuristic))[1] == (a_star(b, advanced_heuristic))[1])

    # # check 14th
    # b = boards[13]
    # l = dfs(b)[0]
    # print(dfs(b)[1])
    # for s in l[1:10]:
    #     s.board.display()

    # # many states:
    # los = []
    # for i in range(len(boards)):
    #     los.append(State(boards[i], zero_heuristic, i, 0))
    # for s in los[1:5]:
    #     print(s.f, s.id)
    # los = sort_f(los[1:5])
    # for s in los[1:5]:
    #     print(s.f, s.id)

    # # test4: A*
    # test_board = boards[0]
    # # # # print(test_board.grid)
    # l = a_star(test_board, advanced_heuristic)
    # ll = a_star(test_board, blocking_heuristic)
    # for s in l[0]:
    #     print(s.id)
    #     s.board.display()
    # # for s in ll[0]:
    # #     s.board.display()
    # print(l[1])
    # # print(l[1], ll[1])

    # # test5: block_h
    # for b in boards:
    # #     b.display()

    # compare blocking_h with advance_h
    for i in range(len(boards)):
        b = boards[i]
        print("For Jam-" + str(i+1) + ":")
        p1 = a_star(b, blocking_heuristic)
        p2 = a_star(b, advanced_heuristic)
        print()