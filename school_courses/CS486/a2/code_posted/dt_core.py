# version 1.1
import math
from typing import List

import anytree
from anytree import Node

import dt_global 

tol = 1e-5


def get_feature_index(feature: str) -> int:
    """
    Given a feature, returns the index of this feature in the example

    :param feature: a feature name
    :type feature: str
    :return: an integer indicating the index of given feature in the example
    :rtype: int
    """

    features = dt_global.feature_names
    for i in range(len(features)):
        if features[i] == feature:
            return i


def entropy(examples: List) -> float:
    """
    calculate the entropy of given examples

    :param examples: a list of examples
    :return: a float representing the entropy of given examples
    """

    label = dt_global.label_index
    labels = []
    counts = {}
    for example in examples:
        if not example[label] in labels:
            labels.append(example[label])
            counts[example[label]] = 1
        else:
            counts[example[label]] += 1

    e = 0
    n = len(examples)
    for label in labels:
        e += counts[label]/n * math.log(counts[label]/n, 2)
    return -1 * e


def majority(examples: List):
    """

    :param examples: list of examples
    :return: majority labels in the given examples
    """

    label = dt_global.label_index
    labels = []
    count = {}
    for example in examples:
        if not example[label] in labels:
            labels.append(example[label])
            count[example[label]] = 1
        else:
            count[example[label]] += 1
    labels.sort()
    m = labels[0]
    for label in labels:
        if count[label] > count[m]:
            m = label
    return m


def get_splits(examples: List, feature: str) -> List[float]:
    """
    Given some examples and a feature, returns a list of potential split point values for the feature.
    
    :param examples: a set of examples
    :type examples: List[List[Any]]
    :param feature: a feature
    :type feature: str
    :return: a list of potential split point values 
    :rtype: List[float]
    """ 

    index = get_feature_index(feature)
    label = dt_global.label_index
    splits = []
    examples_sorted = sorted(examples, key=lambda x: x[index])

    current_value = "0"
    current_labels = []
    past_value = "0"
    past_labels = []
    i = 0
    while i < len(examples_sorted):
        if current_value == "0":
            current_value = examples_sorted[i][index]
        if math.isclose(current_value, examples_sorted[i][index], abs_tol=tol):
            if not examples_sorted[i][label] in current_labels:
                current_labels.append(examples_sorted[i][label])
            if past_value != "0":
                if examples_sorted[i][label] != past_labels[0]:
                    splits.append(1 / 2 * (past_value + current_value))
                    past_value = "0"
        else:
            past_value, past_labels = current_value, current_labels
            current_value = examples_sorted[i][index]
            current_labels = [examples_sorted[i][label]]
            if len(past_labels) > 1:
                splits.append(1 / 2 * (past_value + current_value))
                past_value = "0"
            else:
                if examples_sorted[i][label] != past_labels[0]:
                    splits.append(1 / 2 * (past_value + current_value))
                    past_value = "0"
        i += 1
    return splits


def choose_feature_split(examples: List, features: List[str]) -> (str, float):
    """
    Given some examples and some features,
    returns a feature and a split point value with the max expected information gain.

    If there are no valid split points for the remaining features, return None and -1.

    Tie breaking rules:
    (1) With multiple split points, choose the one with the smallest value. 
    (2) With multiple features with the same info gain, choose the first feature in the list.

    :param examples: a set of examples
    :type examples: List[List[Any]]    
    :param features: a set of features
    :type features: List[str]
    :return: the best feature and the best split value
    :rtype: str, float
    """

    best_feature = None
    best_value = -1
    max_info_gain = -1
    e0 = entropy(examples)
    for feature in features:
        splits = get_splits(examples, feature)
        index = get_feature_index(feature)
        examples_sorted = sorted(examples, key=lambda x: x[index])
        if splits:
            for split in splits:
                i = 0
                while examples_sorted[i][index] < split or math.isclose(examples_sorted[i][index], split, abs_tol=tol):
                    i += 1
                left = examples_sorted[0:i]
                right = examples_sorted[i:]
                n = len(examples)
                e = len(left)/n * entropy(left) + len(right)/n * entropy(right)
                if e0 - e > max_info_gain and not math.isclose(e0-e, max_info_gain, abs_tol=tol):
                    best_feature = feature
                    best_value = split
                    max_info_gain = e0 - e
    return best_feature, best_value


def split_examples(examples: List, feature: str, split: float) -> (List, List):
    """
    Given some examples, a feature, and a split point,
    splits examples into two lists and return the two lists of examples.

    The first list of examples have their feature value <= split point.
    The second list of examples have their feature value > split point.

    :param examples: a set of examples
    :type examples: List[List[Any]]
    :param feature: a feature
    :type feature: str
    :param split: the split point
    :type split: float
    :return: two lists of examples split by the feature split
    :rtype: List[List[Any]], List[List[Any]]
    """

    index = get_feature_index(feature)
    examples_sorted = sorted(examples, key=lambda x: x[index])
    i = 0
    while i < len(examples_sorted):
        if examples_sorted[i][index] < split or math.isclose(examples_sorted[i][index], split, abs_tol=tol):
            i += 1
        else:
            break
    left = examples_sorted[0:i]
    right = examples_sorted[i:]
    return left, right


def split_node(cur_node: Node, examples: List, features: List[str], max_depth=math.inf):
    """
    Given a tree with cur_node as the root, some examples, some features, and the max depth,
    grows a tree to classify the examples using the features by using binary splits.

    If cur_node is at max_depth, makes cur_node a leaf node with majority decision and return.

    This function is recursive.

    :param cur_node: current node
    :type cur_node: Node
    :param examples: a set of examples
    :type examples: List[List[Any]]
    :param features: a set of features
    :type features: List[str]
    :param max_depth: the maximum depth of the tree
    :type max_depth: int
    """ 
    label = dt_global.label_index
    if not examples:
        cur_node.decision = cur_node.parent.majority
        cur_node.name = cur_node.name + "x"
    elif not features or cur_node.depth == max_depth:
        cur_node.decision = majority(examples)
        cur_node.name = cur_node.name + "x"
    else:
        feature, split = choose_feature_split(examples, features)
        if feature is None:
            cur_node.decision = examples[0][label]
            cur_node.name = cur_node.name + "x"
        else:
            left, right = split_examples(examples, feature, split)
            cur_node.feature = feature
            cur_node.split = split
            cur_node.majority = majority(examples)
            cur_node.num = len(examples)
            left_node = Node(name=cur_node.name+"0", parent=cur_node)
            right_node = Node(name=cur_node.name+"1", parent=cur_node)
            cur_node.children = [left_node, right_node]
            split_node(left_node, left, features, max_depth)
            split_node(right_node, right, features, max_depth)


def learn_dt(examples: List, features: List[str], max_depth=math.inf) -> Node:
    """
    Given some examples, some features, and the max depth,
    creates the root of a decision tree, and
    calls split_node to grow the tree to classify the examples using the features, and
    returns the root node.

    This function is a wrapper for split_node.

    Tie breaking rule:
    If there is a tie for majority voting, always return the label with the smallest value.

    :param examples: a set of examples
    :type examples: List[List[Any]]
    :param features: a set of features
    :type features: List[str]
    :param max_depth: the max depth of the tree
    :type max_depth: int, default math.inf
    :return: the root of the tree
    :rtype: Node
    """

    root = Node(name="r", parent=None)
    split_node(root, examples, features, max_depth)
    return root


def predict(cur_node: Node, example, max_depth=math.inf, min_num_examples=0) -> int:
    """
    Given a tree with cur_node as its root, an example, and optionally a max depth,
    returns a prediction for the example based on the tree.

    If max_depth is provided and we haven't reached a leaf node at the max depth, 
    return the majority decision at this node.

    If min_num_examples is provided and the number of examples at the node is less than min_num_examples, 
    return the majority decision at this node.
    
    This function is recursive.

    Tie breaking rule:
    If there is a tie for majority voting, always return the label with the smallest value.

    :param cur_node: cur_node of a decision tree
    :type cur_node: Node
    :param example: one example
    :type example: List[Any]
    :param max_depth: the max depth
    :type max_depth: int, default math.inf
    :param min_num_examples: the minimum number of examples at a node
    :type min_num_examples: int, default 0
    :return: the decision for the given example
    :rtype: int
    """ 

    if cur_node.is_leaf:
        return cur_node.decision
    elif cur_node.depth == max_depth or cur_node.num < min_num_examples:
        return cur_node.majority
    else:
        index = get_feature_index(cur_node.feature)
        split = cur_node.split
        if example[index] < split or math.isclose(example[index], split, abs_tol=tol):
            return predict(cur_node.children[0], example, max_depth, min_num_examples)
        else:
            return predict(cur_node.children[1], example, max_depth, min_num_examples)


def get_prediction_accuracy(cur_node: Node, examples: List, max_depth=math.inf, min_num_examples=0) -> float:
    """
    Given a tree with cur_node as the root, some examples, 
    and optionally the max depth or the min_num_examples, 
    returns the accuracy by predicting the examples using the tree.

    The tree may be pruned by max_depth or min_num_examples.

    :param cur_node: cur_node of the decision tree
    :type cur_node: Node
    :param examples: the set of examples. 
    :type examples: List[List[Any]]
    :param max_depth: the max depth
    :type max_depth: int, default math.inf
    :param min_num_examples: the minimum number of examples at a node
    :type min_num_examples: int, default 0
    :return: the prediction accuracy for the examples based on the cur_node
    :rtype: float
    """ 

    label = dt_global.label_index
    hit = 0
    for example in examples:
        if predict(cur_node, example, max_depth, min_num_examples) == example[label]:
            hit += 1
    return hit/len(examples)


def post_prune(cur_node: Node, min_num_examples: float):
    """
    Given a tree with cur_node as the root, and the minimum number of examples,
    post prunes the tree using the minimum number of examples criterion.

    This function is recursive.

    Let leaf parents denote all the nodes that only have leaf nodes as its descendants. 
    Go through all the leaf parents.
    If the number of examples at a leaf parent is smaller than the pre-defined value,
    convert the leaf parent into a leaf node.
    Repeat until the number of examples at every leaf parent is greater than
    or equal to the pre-defined value of the minimum number of examples.

    :param cur_node: the current node
    :type cur_node: Node
    :param min_num_examples: the minimum number of examples
    :type min_num_examples: float
    """

    if not cur_node.is_leaf:
        if cur_node.children[0].is_leaf and cur_node.children[1].is_leaf:
            if cur_node.num < min_num_examples:
                cur_node.children = []
                cur_node.name += "x"
                cur_node.decision = cur_node.majority
                del cur_node.majority, cur_node.feature, cur_node.split, cur_node.num
        else:
            if not cur_node.children[0].is_leaf:
                post_prune(cur_node.children[0], min_num_examples)
            if not cur_node.children[1].is_leaf:
                post_prune(cur_node.children[1], min_num_examples)
            if cur_node.children[0].is_leaf and cur_node.children[1].is_leaf:
                if cur_node.num < min_num_examples:
                    cur_node.children = []
                    cur_node.name += "x"
                    cur_node.decision = cur_node.majority
                    del cur_node.majority, cur_node.feature, cur_node.split, cur_node.num