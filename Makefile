# Makefile for Deliver Eats
# Author: Yiquan Sun
# Created: 24/12/2025

CXX = g++
CXXFLAGS = -std=c++14 -Wall -Wextra -g

TARGET = deliver_eats
TEST_TARGET = test_suite

all: $(TARGET)

$(TARGET): DispatchSystem.o CourierBase.o
	$(CXX) $(CXXFLAGS) -o $(TARGET) DispatchSystem.o CourierBase.o

DispatchSystem.o: DispatchSystem.cpp CourierBase.h GlobalSettings.h
	$(CXX) $(CXXFLAGS) -c DispatchSystem.cpp

CourierBase.o: CourierBase.cpp CourierBase.h GlobalSettings.h
	$(CXX) $(CXXFLAGS) -c CourierBase.cpp

# Task 4.3 Requirements
check: $(TEST_TARGET)
	./$(TEST_TARGET)

$(TEST_TARGET): tests.o CourierBase.o
	$(CXX) $(CXXFLAGS) -o $(TEST_TARGET) tests.o CourierBase.o

tests.o: tests.cpp CourierBase.h GlobalSettings.h catch.hpp
	$(CXX) $(CXXFLAGS) -c tests.cpp

clean:
	rm -f *.o $(TARGET) $(TEST_TARGET) *.dat