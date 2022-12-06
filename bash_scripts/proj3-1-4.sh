#!/bin/bash
make clean
rm output/*
make debug

printf "\nFIFO REPLACEMENT 4 FRAMES RUN:\n\n"
./proj3 1 4 sample_input/input_1
./proj3 1 4 sample_input/input_2
./proj3 1 4 sample_input/input_3
./proj3 1 4 sample_input/input_4
./proj3 1 4 sample_input/input_5
./proj3 1 4 sample_input/input_6
./proj3 1 4 sample_input/input_7
./proj3 1 4 sample_input/input_8
./proj3 1 4 sample_input/input_9
./proj3 1 4 sample_input/input_10
./proj3 1 4 sample_input/input_11
./proj3 1 4 sample_input/input_12

printf "\nFIFO REPLACEMENT 4 FRAMES OUTPUTS:\n\n"
diff -qs output/result-1-4-input_1 sample_output/result-1-4-input_1
diff -qs output/result-1-4-input_2 sample_output/result-1-4-input_2
diff -qs output/result-1-4-input_3 sample_output/result-1-4-input_3
diff -qs output/result-1-4-input_4 sample_output/result-1-4-input_4
diff -qs output/result-1-4-input_5 sample_output/result-1-4-input_5
diff -qs output/result-1-4-input_6 sample_output/result-1-4-input_6
diff -qs output/result-1-4-input_7 sample_output/result-1-4-input_7
diff -qs output/result-1-4-input_8 sample_output/result-1-4-input_8
diff -qs output/result-1-4-input_9 sample_output/result-1-4-input_9
diff -qs output/result-1-4-input_10 sample_output/result-1-4-input_10
diff -qs output/result-1-4-input_11 sample_output/result-1-4-input_11
diff -qs output/result-1-4-input_12 sample_output/result-1-4-input_12