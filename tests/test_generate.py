import random
count_tests = 10

for i in range(0, count_tests) :
    file_name = "tests/test_data/test_" + f'{i+1}' + ".in"
    file = open(file_name, 'w')

    size_of_cache  = random.randint(10, 20)
    count_of_elems = random.randint(10000, 20000)
    test_str = str(count_of_elems) + " " + str(size_of_cache) + "\n"
    
    for j in range(count_of_elems) :
      test_str += str(random.randint(1, 2)) + " "

    file.write(test_str)
    file.close()

    print("test ", i + 1, " generated")