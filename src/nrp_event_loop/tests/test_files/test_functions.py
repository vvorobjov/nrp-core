def test_function(i_1, i_2, i_3):
    return [i_1, i_2, i_3]


def test_function_2(i_1):
    return [i_1]


def test_wrong_return_type(i_1):
    return "not a list"


def test_less_elements(i_1):
    return []


def test_more_elements(i_1):
    return [1, 2]


def test_broken_function(i_1):
    raise Exception("Broken function called")

