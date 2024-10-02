import os

for test_number in range (1, 11):
  os.system("./bin/cache < tests/test_data/test_" + str(test_number) + ".in >> tests/test_result.out")

# from subprocess import call
# call(["./bin/cache", "<", "tests/test_data/test_1.in"])

# import subprocess
# subprocess.run(["./bin/cache", "<", "tests/test_data/test_1.in"])