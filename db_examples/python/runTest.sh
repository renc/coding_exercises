#!/bin/bash

# comments 
echo "Hello World!"
conda info --envs
source activate py36q
python --version
source deactivate
date >> runTest_log

