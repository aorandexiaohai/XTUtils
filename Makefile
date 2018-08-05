header_files = *.h
google_test_install_dir=/home/wzh/source_code_manage/googletestinstall
google_benchmark_install_dir=/home/wzh/source_code_manage/googlebenchmarkinstall/include
google_benchmark_lib_dir=/home/wzh/source_code_manage/googlebenchmarkinstall/lib
test : test.cpp ${header_files}
	g++ -o test -I ${google_test_install_dir}/include test.cpp -std=c++17  -lgtest_main -lgtest -lpthread -Wfatal-errors -w -I ${google_benchmark_install_dir} -L ${google_benchmark_lib_dir} -lbenchmark_main -lbenchmark
