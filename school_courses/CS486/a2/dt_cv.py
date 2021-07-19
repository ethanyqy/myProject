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
    
    features = dt_global.feature_names[:-1]
    train_accuracy = list()
    validation_accuracy = list()
    
    listoftree = list()
    trainingList = list()
    validationList = list()
    
    for i in range(len(folds)):
        
        validation = folds[i]
        
        training = list()
        
        for j in range(len(folds)):
            if j != i:
                training = training + folds[j]
                
        trainingList.append(training)
        
        validationList.append(validation)
        
        current_dt = learn_dt(training,features)
        
        listoftree.append(current_dt)
        
        
    for p in value_list:
        
        tmp_train, tmp_va = list(), list()
    
        for i in range(len(folds)):
        
            current_train = get_prediction_accuracy(listoftree[i], trainingList[i], max_depth= p)
            current_va = get_prediction_accuracy(listoftree[i], validationList[i], max_depth = p)
            
            tmp_train.append(current_train)
            tmp_va.append(current_va)
            #print(current_train)
            #print(f"test is  {test_train}")
        
        avg_train = sum(tmp_train)/len(tmp_train)
        avg_va = sum(tmp_va)/len(tmp_va)
        
        train_accuracy.append(avg_train)
        validation_accuracy.append(avg_va)
        
        
    return train_accuracy, validation_accuracy
        
        
        
    
    
    




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
    
    features = dt_global.feature_names[:-1]
    train_accuracy = list()
    validation_accuracy = list()
    
    listoftree = list()
    trainingList = list()
    validationList = list()
    
    for i in range(len(folds)):
        
        validation = folds[i]
        
        training = list()
        
        for j in range(len(folds)):
            if j != i:
                training = training + folds[j]
                
        trainingList.append(training)
        
        validationList.append(validation)
        
        current_dt = learn_dt(training,features)
        
        listoftree.append(current_dt)
        
        
    for p in value_list:
        
        tmp_train, tmp_va = list(), list()
    
        for i in range(len(folds)):
            
            current_train = get_prediction_accuracy(listoftree[i], trainingList[i], min_num_examples= p)
            current_va = get_prediction_accuracy(listoftree[i], validationList[i], min_num_examples = p)
            
            tmp_train.append(current_train)
            tmp_va.append(current_va)
            
        
        avg_train = sum(tmp_train)/len(tmp_train)
        avg_va = sum(tmp_va)/len(tmp_va)
        
        train_accuracy.append(avg_train)
        validation_accuracy.append(avg_va)
        
        
    return train_accuracy, validation_accuracy

    


data = read_data("data.csv")
tenfolds = preprocess(data)
input_feature_names = dt_global.feature_names[:-1]
start = timer()
#tree = learn_dt(data, input_feature_names)

#acc = get_prediction_accuracy(tree, data)
#print(acc)
#t = list(range(0,30))
t = list(range(0,301,20))

ta, va = cv_post_prune(tenfolds, t)

#post_prune(tree, 20)
#print("training")
#print(ta)
#print("Validation")
#print(va)

print(f"Time is {timer() - start}")


"""
data = read_data("mytest.csv")
training_accuracy, validation_accuracy = cv_pre_prune([data[1:3], data[3:5]], [0, 1])

print(training_accuracy)
print(validation_accuracy)
"""