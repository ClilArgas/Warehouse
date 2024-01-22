all: compile link clean

clean:
	rm -f ./bin/*

compile:
	g++ -c -o ./bin/file1.o ./src/main.cpp
	g++ -c -o ./bin/file2.o ./src/Warehouse.cpp
	g++ -c -o ./bin/file3.o ./src/Customer.cpp
	g++ -c -o ./bin/file4.o ./src/Volunteer.cpp
	g++ -c -o ./bin/file5.o ./src/collector.cpp
	g++ -c -o ./bin/file6.o ./src/Order.cpp
	g++ -c -o ./bin/file7.o ./src/driver.cpp
	g++ -c -o ./bin/file8.o ./src/LimitedCollector.cpp
	g++ -c -o ./bin/file9.o ./src/LimitedDriver.cpp

link:
	g++ -o ./warehouse ./bin/*.o