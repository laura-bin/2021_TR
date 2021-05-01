for i in {0..99}
do
    sudo ./build/reader_writer 50 19 10 -20 10
    sudo ./build/reader_writer 50 -20 10 19 10
    sudo ./build/reader_writer 50 0 10 19 10
    sudo ./build/reader_writer 50 0 10 0 10
done > /dev/null
