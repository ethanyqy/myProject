# version 1.1
from typing import List

import dt_global
from dt_core import *


def cv_pre_prune(folds: List, value_list: List[float]) -> (List[float], List[float]):
    """
    Determines the best parameter value for pre-pruning via cross validation.

    Returns two lists: the training accuracy list and the validation accuracy list.

    :param folds: folds for cross validation
    :type folds: List[List[List[Any]]]
    :param value_list: a list of parameter values
    :type value_list: List[float]
    :return: the training accuracy list and the validation accuracy list
    :rtype: List[float], List[float]
    """

    training_examples = []
    validation_examples = []
    dts = []
    for i in range(len(folds)):
        training_set = []
        for j in range(len(folds)):
            if j != i:
                training_set += folds[j]
        training_examples.append(training_set)
        validation_examples.append(folds[i])
        dts.append(learn_dt(training_set, dt_global.feature_names[:-1]))

    training_accuracies = []
    validation_accuracies = []
    for p in value_list:
        training_accuracy = []
        validation_accuracy = []
        for i in range(len(folds)):
            training_accuracy.append(get_prediction_accuracy(dts[i], training_examples[i], max_depth=p))
            validation_accuracy.append(get_prediction_accuracy(dts[i], validation_examples[i], max_depth=p))
        training_accuracies.append(sum(training_accuracy) / len(training_accuracy))
        validation_accuracies.append(sum(validation_accuracy) / len(training_accuracy))

    return training_accuracies, validation_accuracies


def cv_post_prune(folds: List, value_list: List[float]) -> (List[float], List[float]):
    """
    Determines the best parameter value for post-pruning via cross validation.

    Returns two lists: the training accuracy list and the validation accuracy list.

    :param folds: folds for cross validation
    :type folds: List[List[List[Any]]]
    :param value_list: a list of parameter values
    :type value_list: List[float]
    :return: the training accuracy list and the validation accuracy list
    :rtype: List[float], List[float]
    """

    training_examples = []
    validation_examples = []
    dts = []
    for i in range(len(folds)):
        training_set = []
        for j in range(len(folds)):
            if j != i:
                training_set += folds[j]
        training_examples.append(training_set)
        validation_examples.append(folds[i])
        dts.append(learn_dt(training_set, dt_global.feature_names[:-1]))

    training_accuracies = []
    validation_accuracies = []
    for p in value_list:
        training_accuracy = []
        validation_accuracy = []
        for i in range(len(folds)):
            training_accuracy.append(get_prediction_accuracy(dts[i], training_examples[i], min_num_examples=p))
            validation_accuracy.append(get_prediction_accuracy(dts[i], validation_examples[i], min_num_examples=p))
        training_accuracies.append(sum(training_accuracy) / len(training_accuracy))
        validation_accuracies.append(sum(validation_accuracy) / len(training_accuracy))

    return training_accuracies, validation_accuracies