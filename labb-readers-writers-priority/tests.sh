for i in {1..50}
do
    sudo ./build/reader_writer 60 10 5 20 20
    sudo ./build/reader_writer 60 20 5 10 20
    sudo ./build/reader_writer 60 10 5 10 20
done > /dev/null
