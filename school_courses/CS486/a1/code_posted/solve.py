from board import *
import copy


def a_star(init_board, hfn):
    """
    Run the A_star search algorithm given an initial board and a heuristic function.

    If the function finds a goal state, it returns a list of states representing
    the path from the initial state to the goal state in order and the cost of
    the solution found.
    Otherwise, it returns am empty list and -1.

    :param init_board: The initial starting board.
    :type init_board: Board
    :param hfn: The heuristic function.
    :type hfn: Heuristic
    :return: (the path to goal state, solution cost)
    :rtype: List[State], int
    """

    # init the start state
    start_state = State(init_board, hfn, hfn(init_board), 0)

    # create frontier and explored
    frontier = [start_state]
    explored = []
    expand = 0
    while len(frontier) > 0:
        current_state = frontier.pop()
        # check if explored
        if current_state.id not in explored:
            expand += 1
            explored.append(current_state.id)
            if is_goal(current_state):
                # output the number of nodes expanded
                print("The number of nodes that " + str(hfn).split()[1] + " expands is " + str(expand))
                return get_path(current_state), current_state.depth
            frontier += get_successors(current_state)
            frontier.sort(key=lambda x: (x.f, x.id, x.parent.id), reverse=True)
    return [], -1


def dfs(init_board):
    """
    Run the DFS algorithm given an initial board.

    If the function finds a goal state, it returns a list of states representing
    the path from the initial state to the goal state in order and the cost of
    the solution found.
    Otherwise, it returns am empty list and -1.

    :param init_board: The initial board.
    :type init_board: Board
    :return: (the path to goal state, solution cost)
    :rtype: List[State], int
    """

    # init the start state
    start_state = State(init_board, zero_heuristic, 0, 0)

    # create frontier and explored
    frontier = [start_state]
    explored = []
    while len(frontier) > 0:
        current_state = frontier.pop()
        # check if explored
        if current_state.id not in explored:
            explored.append(current_state.id)
            if is_goal(current_state):
                return get_path(current_state), current_state.depth
            successors = get_successors(current_state)
            frontier += sorted(successors, key=lambda x: x.id, reverse=True)
    return [], -1


def get_successors(state):
    """
    Return a list containing the successor states of the given state.
    The states in the list may be in any arbitrary order.

    :param state: The current state.
    :type state: State
    :return: The list of successor states.
    :rtype: List[State]
    """

    # get the info of cars of the current state
    cars = state.board.cars
    size = state.board.size
    grid = state.board.grid

    successors = []

    # check every car if it is movable and create successors
    for i in range(len(cars)):
        if cars[i].orientation == "v":
            x = cars[i].fix_coord
            y = cars[i].var_coord
            length = cars[i] .length

            # upwards
            j = 1
            while y - j >= 0:
                if grid[y-j][x] == '.':
                    new_cars = copy.deepcopy(cars)
                    new_cars[i].set_coord(y - j)
                    new_board = Board(state.board.name, state.board.size, new_cars)
                    successors.append(State(new_board, state.hfn,
                                            state.hfn(new_board) + state.depth + 1,
                                            state.depth + 1, state))
                else:
                    break
                j += 1

            # downwards
            j = 1
            while y + length + j <= size:
                if grid[y + length - 1 + j][x] == '.':
                    new_cars = copy.deepcopy(cars)
                    new_cars[i].set_coord(y + j)
                    new_board = Board(state.board.name, state.board.size, new_cars)
                    successors.append(State(new_board, state.hfn,
                                            state.hfn(new_board) + state.depth + 1,
                                            state.depth + 1, state))
                else:
                    break
                j += 1
        else:
            y = cars[i].fix_coord
            x = cars[i].var_coord
            length = cars[i].length

            # leftwards
            j = 1
            while x - j >= 0:
                if grid[y][x - j] == '.':
                    new_cars = copy.deepcopy(cars)
                    new_cars[i].set_coord(x - j)
                    new_board = Board(state.board.name, state.board.size, new_cars)
                    successors.append(State(new_board, state.hfn,
                                            state.hfn(new_board) + state.depth + 1,
                                            state.depth + 1, state))
                else:
                    break
                j += 1

            # rightwards
            j = 1
            while x + length + j <= size:
                if grid[y][x + length + j - 1] == '.':
                    new_cars = copy.deepcopy(cars)
                    new_cars[i].set_coord(x + j)
                    new_board = Board(state.board.name, state.board.size, new_cars)
                    successors.append(State(new_board, state.hfn,
                                            state.hfn(new_board) + state.depth + 1,
                                            state.depth + 1, state))
                else:
                    break
                j += 1

    return successors


def is_goal(state):
    """
    Returns True if the state is the goal state and False otherwise.

    :param state: the current state.
    :type state: State
    :return: True or False
    :rtype: bool
    """

    # get the info of cars of the current state
    cars = state.board.cars

    # check if the goal car is at goal place: (2, 4)
    for car in cars:
        if car.is_goal:
            if car.fix_coord == 2 and car.var_coord == 4:
                return True
            else:
                return False


def get_path(state):
    """
    Return a list of states containing the nodes on the path
    from the initial state to the given state in order.

    :param state: The current state.
    :type state: State
    :return: The path.
    :rtype: List[State]
    """
    path = []
    current_state = state
    while current_state.parent is not None:
        path = [current_state] + path
        current_state = current_state.parent

    return [current_state] + path


def blocking_heuristic(board):
    """
    Returns the heuristic value for the given board
    based on the Blocking Heuristic function.

    Blocking heuristic returns zero at any goal board,
    and returns one plus the number of cars directly
    blocking the goal car in all other states.

    :param board: The current board.
    :type board: Board
    :return: The heuristic value.
    :rtype: int
    """

    # check if the goal car is at goal place: (2, 4)
    for car in board.cars:
        if car.is_goal:
            if car.var_coord == 4:
                return 0

    end = 0
    b = 0
    for point in board.grid[2]:
        if point == '>':
            end = 1
        if end:
            if point == '|' or point == '^' or point == 'v':
                b += 1
    return b + 1


def advanced_heuristic(board):
    """
    Returns the heuristic value for the given board
    based on the Blocking Heuristic function.

    Blocking heuristic returns zero at any goal board,
    and returns one plus the number of cars directly
    blocking the goal car, and plus one if among those
    cars blocking the goal car, there exists one car
    that is blocked by cars on both sides, for all
    other states.


    :param board: The current board.
    :type board: Board
    :return: The heuristic value.
    :rtype: int
    """

    # get the info of cars of the current state
    cars = board.cars

    # check if the goal car is at goal place: (2, 4)
    for car in cars:
        if car.is_goal:
            if car.fix_coord == 2 and car.var_coord == 4:
                return 0

    blocking = -1
    blocking2 = 0
    points = board.grid[2]
    for i in range(len(points)):
        if points[i] == '>':
            blocking += 1
        if blocking != -1:
            if points[i] == '|' or points[i] == '^' or points[i] == 'v':
                blocking += 1
                # additional case: directly blocking car is length 1
                if points[i] == 'v':
                    if board.grid[1][i] != '|' and board.grid[1][i] != '^' and board.grid[1][i] != '.' and board.grid[3][i] != '.':
                        blocking2 = 1
                        continue
                y_up = 2
                while board.grid[y_up][i] != '^' and y_up > 0:
                    y_up -= 1
                y_down = 2
                while board.grid[y_down][i] != 'v':
                    y_down += 1
                # directly blocking car's head and tail are at top and bottom
                if y_up == 0 and y_down == 5:
                    blocking2 = 1
                # directly blocking car's head is at top
                elif y_up == 0 and board.grid[y_down + 1][i] != '.':
                    blocking2 = 1
                # directly blocking car's tail is at bottom
                elif board.grid[y_up - 1][i] != '.' and y_down == 5:
                    blocking2 = 1
                elif board.grid[y_up - 1][i] != '.' and board.grid[y_down + 1][i] != '.':
                    blocking2 = 1
                else:
                    blocking2 = 0
    return blocking + blocking2 + 1
