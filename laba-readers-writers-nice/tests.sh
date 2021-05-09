for i in {1..50}
do
    ./build/reader_writer 60 1 5 1 20
    # ./build/reader_writer 60 19 5 -20 20
    # ./build/reader_writer 60 -20 5 19 20
    # ./build/reader_writer 60 0 5 0 20
done > /dev/null
