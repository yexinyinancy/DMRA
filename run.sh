for test_file in $(ls -v ./${1}/input_paper_order_ue*.txt); do
    # echo $test_file
    ./dmra < $test_file | awk 'NR == 2 {print $4}'
done