# Copyright 2012-2021 Crytek GmbH / Crytek Group. All rights reserved.

'''
This example script reads a given file and outputs it to stdout

@argument name="Open Text File", type="fileopen", default="test.txt", extension="txt"
'''

import sys

if (len(sys.argv) > 1):
    for line in open(sys.argv[1], 'r'):
        print(line)
else:
	print("Invalid arguments")