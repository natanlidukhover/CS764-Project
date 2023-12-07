#include "Filter.h"
#include "Iterator.h"
#include "Scan.h"
#include "Sort.h"
#include "Table.h"
#include "Dram.h"
#include "defs.h"
#include <iostream>
#include "Ssd.h"
#include <fstream>
#include "tol.h"

using namespace std;
int main1(int argc, char* argv[]) {
	size_t nor, dramRunSize, rowSize, numRecords, blockSize;
	uint8_t *dataPtr;

	// all constants
	numRecords = 4;
	rowSize = 5;
	blockSize = 5;
	dramRunSize = blockSize;
	nor = (numRecords * rowSize)/dramRunSize;

	outTrace.open("o.txt",  std::ios_base::out );
    TRACE(true);

    // Print the values
    cout << "numRecords " << numRecords << std::endl;
    cout << "Row size " << rowSize << std::endl;
	cout << "blockSize " << blockSize << std::endl;
    cout << "Size of runs " << dramRunSize << std::endl;
    cout << "Number of runs " << nor << std::endl;

	dataPtr = (uint8_t *)malloc(dramRunSize * (nor + 1));

	Ssd ssd("./input/testData.bin", (size_t) numRecords * rowSize, blockSize);
	Ssd ossd("./output/testData.bin", (size_t) numRecords * rowSize, blockSize);
	ScanIterator * const sc_it = new ScanIterator(new ScanPlan (numRecords * rowSize, blockSize));
	vector<int> numbers = sc_it->run();

	for (size_t i = 0; i < numRecords; i++) {
		for (size_t j = 0; j < rowSize; j++) {
			cout << (int)numbers[i * rowSize + j] << " ";
		}
		cout << endl;
	}
	cout << endl;

	Run **runs;
	runs = (Run **)malloc(sizeof(Run *) * nor);

	for (size_t i = 0; i < nor; i++) {
		runs[i] = new Run(&ssd, &(dataPtr[dramRunSize * i]), blockSize, ((numRecords * rowSize)/nor) * i, numRecords * rowSize, numRecords * rowSize, dramRunSize, rowSize, 0);
	}
	Run outputRun(&ossd, &(dataPtr[dramRunSize * nor]), blockSize, 0, 0, numRecords * rowSize, dramRunSize, rowSize, 0);

	int ret = SUCCESS;
	//for (size_t i = 0; i < nor; i++) {
	//	cout << "Run: " << i << endl;
	//	for (size_t j = 0; j < numRecords/nor; j++) {
	//		uint8_t *ptr;
	//		if ((ret = runs[i]->getNext(&ptr))) {
	//			cout << "Run get next error " << ret << endl;
	//			break;
	//		}
	//		for (size_t k = 0; k < rowSize; k++) {
	//			cout << (int)(ptr[k]) << " ";
	//		}
	//		cout << endl;
	//	}
	//}
	//cout << endl;

	ETable t(numRecords, rowSize, rowSize, 0);
	TOL tol(nor, runs, &outputRun, t);
	tol.print();
	tol.pass();
	tol.print();

	free(runs);
	free(dataPtr);
}
//    // Parse command-line arguments
//    for (int i = 1; i < argc; i += 2) {
//        if (strcmp(argv[i], "-n") == 0) {
//            nor = std::stoi(argv[i + 1]);
//        } else if (strcmp(argv[i], "-s") == 0) {
//            dramRunSize = std::stoi(argv[i + 1]);
//		} else if (strcmp(argv[i], "-r") == 0) {
//			rowSize = std::stoi(argv[i + 1]);
//		}
//    }