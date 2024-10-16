import os

os.system(":> tests/test_result.out")                     #clear result file
for test_number in range (1, 11):
  os.system("echo \"TEST " + str(test_number) + ": \"  >> tests/test_result.out")
  os.system("./bin/FIFO < tests/test_data/test_" + str(test_number) + ".in >> tests/test_result.out")
  os.system("./bin/IDL < tests/test_data/test_" + str(test_number) + ".in >> tests/test_result.out")
  os.system("echo \" \n \" >> tests/test_result.out")
