import os

os.system(":> tests/test_result.out")                     #clear result file
for test_number in range (1, 11):
  os.system("./bin/cache < tests/test_data/test_" + str(test_number) + ".in >> tests/test_result.out")
