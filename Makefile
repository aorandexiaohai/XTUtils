header_files = *.h
google_test_install_dir=/home/wzh/source_code_manage/googletestinstall
test : test.cpp ${header_files}
	g++ -o test -I ${google_test_install_dir}/include test.cpp -std=c++17  -lgtest_main -lgtest -lpthread -Wfatal-errors -w
