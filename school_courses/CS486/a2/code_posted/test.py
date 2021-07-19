import dt_global
from dt_provided import *
from dt_core import *
from dt_cv import *

if __name__ == "__main__":
    examples = read_data("data.csv")
    folds = preprocess(examples)
    features = dt_global.feature_names[:-1]

    full_tree = learn_dt(examples, features)
    print("height of full_tree:")
    print(full_tree.height)
    print("depth of full_tree:")
    print(full_tree.height-1)

    max_depth = list(range(0, 30, 1))
    min_examples = list(range(0, 300, 20))
    t, v = cv_pre_prune(folds, max_depth)
    print(t)
    print(v)
    max_so_far = v[0]
    max_i = 0
    for i in range(len(v)):
        if v[i] > max_so_far:
            max_so_far = v[i]
            max_i = i
    print("the best value of maximum depth for pre prune is:")
    print(max_depth[max_i])

    t, v = cv_post_prune(folds, min_examples)
    print(t)
    print(v)
    max_so_far = v[0]
    max_i = 0
    for i in range(len(v)):
        if v[i] > max_so_far:
            max_so_far = v[i]
            max_i = i
    print("the best value of minimum examples for post prune is:")
    print(min_examples[max_i])