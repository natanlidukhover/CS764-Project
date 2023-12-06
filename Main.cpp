#include "Filter.h"
#include "Iterator.h"
#include "Scan.h"
#include "Sort.h"
#include "tol.h"
#include "Table.h"
#include "Dram.h"
#include "defs.h"
#include <iostream>
#include "Ssd.h"
#include <fstream>
#include <math.h>
#define cout outTrace
#define KB (size_t) 1024
#define MB 1024 * KB
#define GB 1024 * MB

using namespace std;
std::ofstream  outTrace;

const size_t ssdBlockSize = 10 * KB;
const size_t hddBlockSize = 1 * MB;
const size_t cacheLimit = 1 * MB;
const size_t dramLimit = 100 * MB;
const size_t ssdLimit = 10 * GB;
const size_t ssdMaxRunCount = 100;
const size_t hddMaxRunCount = 97;
const size_t ssdMaxInputBufferSize = ssdMaxRunCount * ssdBlockSize;
const size_t hddMaxInputBufferSize = hddMaxRunCount * hddBlockSize;
const size_t cacheRunSize = cacheLimit - ssdBlockSize;
const size_t ssdOutputBufferSize = ssdBlockSize;
const size_t hddOutputBufferSize = hddBlockSize;
size_t dramToHddInputBufferSize = dramLimit - hddBlockSize;
size_t totalDataSize;
Ssd* inputHdd;
Ssd* outputSsd = new Ssd("./output/ssd.bin", 10 * GB, 10 * KB);
Ssd* outputHdd = new Ssd("./output/hdd.bin", 900 * GB, 1 * MB);

void fillSsdToHdd(size_t inputSeekCounter, size_t bytesToFill) {
    // Create constants
    uint8_t* dramToHddInputBuffer;
    uint8_t* hddOutputBuffer;
    const size_t ssdRunSize = dramLimit - hddBlockSize;
    const size_t dramToSsdInputBufferSize = ssdRunSize;
    const size_t minDramToHddInputBufferSize = dramLimit - hddOutputBufferSize - ssdMaxInputBufferSize;
    const size_t ssdMaxDataSize = bytesToFill - minDramToHddInputBufferSize;
    const size_t numberOfSsdRuns = (ssdMaxDataSize / ssdRunSize) + (ssdMaxDataSize % ssdRunSize != 0); // Round up
    
    dramToHddInputBufferSize = dramLimit - hddOutputBufferSize - (numberOfSsdRuns * ssdBlockSize);
    const size_t ssdActualDataSize = bytesToFill - dramToHddInputBufferSize;

    // Create buffers
    uint8_t* ssdOutputBuffer = (uint8_t*) dram.getSpace(ssdOutputBufferSize);
    uint8_t* dramToSsdInputBuffer = (uint8_t*) dram.getSpace(dramToSsdInputBufferSize);

    // Read input into DRAM-to-SSD input buffer
    size_t seekCounter = inputSeekCounter;
    for (unsigned i = 0; i < numberOfSsdRuns; i++) {
        while (seekCounter < ssdActualDataSize) {
            if (inputHdd->readData(dramToSsdInputBuffer, seekCounter) <= 0) {
                break;
            }
            seekCounter += hddBlockSize;
            // If end of run reached
            if (seekCounter % ssdRunSize == 0) {
                break;
            }
        }
        // Generate cache-size runs over the input buffer

        // Merge these runs using Tree-of-Losers and output to SSD using ssdOutputBuffer
    }
    // Free buffers
    dram.freeSpace(dramToSsdInputBuffer, dramToSsdInputBufferSize);
    dram.freeSpace(ssdOutputBuffer, ssdOutputBufferSize);

    // Create new buffers
    dramToHddInputBuffer = (uint8_t*) dram.getSpace(dramToHddInputBufferSize);
    uint8_t** ssdInputBuffers = new uint8_t*[numberOfSsdRuns];
    for (unsigned i = 0; i < numberOfSsdRuns; i++) {
        ssdInputBuffers[i] = (uint8_t*) dram.getSpace(ssdBlockSize);
    }
    hddOutputBuffer = (uint8_t*) dram.getSpace(hddOutputBufferSize);

    // Read input into DRAM-to-HDD input buffer
    while (seekCounter < totalDataSize) {
        if (inputHdd->readData(dramToHddInputBuffer, seekCounter) <= 0) {
            break;
        }
        seekCounter += hddBlockSize;
    }
    // Generate cache-size runs over the input buffer

    // Merge these runs with the SSD input buffers using Tree-of-Losers and output to HDD using hddOutputBuffer

    // Free buffers
    dram.freeSpace(hddOutputBuffer, hddOutputBufferSize);
    for (unsigned i = 0; i < numberOfSsdRuns; i++) {
        (uint8_t*) dram.freeSpace(ssdInputBuffers[i], ssdBlockSize);
    }
    //free(ssdInputBuffers);
    dram.freeSpace(dramToHddInputBuffer, dramToHddInputBufferSize);
}

void fillSsdToSsd(size_t inputSeekCounter, size_t bytesToFill) {
    // Create constants
    uint8_t* dramToHddInputBuffer;
    uint8_t* hddOutputBuffer;
    const size_t ssdRunSize = dramLimit - hddBlockSize;
    const size_t dramToSsdInputBufferSize = ssdRunSize;
    const size_t minDramToHddInputBufferSize = dramLimit - ssdOutputBufferSize - ssdMaxInputBufferSize;
    const size_t ssdMaxDataSize = bytesToFill - minDramToHddInputBufferSize;
    const size_t numberOfSsdRuns = (ssdMaxDataSize / ssdRunSize) + (ssdMaxDataSize % ssdRunSize != 0); // Round up
    
    dramToHddInputBufferSize = dramLimit - ssdOutputBufferSize - (numberOfSsdRuns * ssdBlockSize);
    const size_t ssdActualDataSize = bytesToFill - dramToHddInputBufferSize;

    // Create buffers
    uint8_t* ssdOutputBuffer = (uint8_t*) dram.getSpace(ssdOutputBufferSize);
    uint8_t* dramToSsdInputBuffer = (uint8_t*) dram.getSpace(dramToSsdInputBufferSize);

    // Read input into DRAM-to-SSD input buffer
    size_t seekCounter = inputSeekCounter;
    for (unsigned i = 0; i < numberOfSsdRuns; i++) {
        while (seekCounter < ssdActualDataSize) {
            if (inputHdd->readData(dramToSsdInputBuffer, seekCounter) <= 0) {
                break;
            }
            seekCounter += hddBlockSize;
            // If end of run reached
            if (seekCounter % ssdRunSize == 0) {
                break;
            }
        }
        // Generate cache-size runs over the input buffer

        // Merge these runs using Tree-of-Losers and output to SSD using ssdOutputBuffer
    }
    // Free buffers
    dram.freeSpace(dramToSsdInputBuffer, dramToSsdInputBufferSize);
    dram.freeSpace(ssdOutputBuffer, ssdOutputBufferSize);

    // Create new buffers
    dramToHddInputBuffer = (uint8_t*) dram.getSpace(dramToHddInputBufferSize);
    uint8_t** ssdInputBuffers = new uint8_t*[numberOfSsdRuns];
    for (unsigned i = 0; i < numberOfSsdRuns; i++) {
        ssdInputBuffers[i] = (uint8_t*) dram.getSpace(ssdBlockSize);
    }
    hddOutputBuffer = (uint8_t*) dram.getSpace(hddOutputBufferSize);

    // Read input into DRAM-to-HDD input buffer
    while (seekCounter < totalDataSize) {
        if (inputHdd->readData(dramToHddInputBuffer, seekCounter) <= 0) {
            break;
        }
        seekCounter += hddBlockSize;
    }
    // Generate cache-size runs over the input buffer

    // Merge these runs with the SSD input buffers using Tree-of-Losers and output to HDD using hddOutputBuffer

    // Free buffers
    dram.freeSpace(hddOutputBuffer, hddOutputBufferSize);
    for (unsigned i = 0; i < numberOfSsdRuns; i++) {
        (uint8_t*) dram.freeSpace(ssdInputBuffers[i], ssdBlockSize);
    }
    //free(ssdInputBuffers);
    dram.freeSpace(dramToHddInputBuffer, dramToHddInputBufferSize);
}

int main_buggy(int argc, char* argv[]) {

    // Default values
   	size_t number_of_records = 12;							// Number of rows
	size_t row_size = 3; 									// Size of each row in bytes
    size_t totalDataSize = number_of_records * row_size;   	// Total amount of data in bytes
    size_t blockSize = 3;
    std::string o_filename="o.txt";							// Output file name
    inputHdd = new Ssd("./input/testData.bin", totalDataSize, 1 * MB);

    // Parse command-line arguments
    for (int i = 1; i < argc; i += 2) {
        if (strcmp(argv[i], "-c") == 0) {
            number_of_records = std::stoi(argv[i + 1]);
        } else if (strcmp(argv[i], "-s") == 0) {
            row_size = std::stoi(argv[i + 1]);
        } else if (strcmp(argv[i], "-o") == 0) {
            o_filename = argv[i + 1];
        } 
		
    }
	outTrace.open(o_filename,  std::ios_base::out );
    TRACE(true);

 	// Print the values
    cout << "Total number of records " << number_of_records << std::endl;
    cout << "Size of one record" << row_size << std::endl;
    cout << "Output_filename: " << o_filename << std::endl;
    Ssd *  unsorted_hdd = new Ssd("./input/testData.bin",(size_t)number_of_records*row_size, 3);
    Ssd *  sorted_hdd = new Ssd("./output/testData.bin",(size_t)number_of_records*row_size, 3);

	ScanIterator * const sc_it = new ScanIterator(new ScanPlan (number_of_records));
	vector<int> numbers = sc_it->run();
	
	//Table tmp(number_of_records, 3, row_size/3);
	uint8_t *data = (uint8_t *)dram.getSpace(1 * pow(10, 6));
    //Run initRun(unsorted_hdd, data, (size_t)(1 * 1e6), (size_t)0, number_of_records * row_size, number_of_records * row_size, (size_t)1e6, row_size, (size_t)0);
    //uint8_t *row;
	for (size_t i = 0; i < number_of_records * row_size; i += pow(10, 6)) {
		unsorted_hdd->readData(data + i, i);
	}
	for (size_t i = 0; i < number_of_records; i++) {
        //initRun.getNext(&row);
		for (size_t j = 0; j < row_size; j++) {
            //tmp[i][j] = row[j];
			cout << data[i * row_size + j];
			cout << " ";
		}
		cout << "\n";
	}
	cout << "\n";

	quickSort(data, (int) number_of_records, (int)row_size);
	verifySortedRuns(data, (int)number_of_records,(int) row_size);

	cout << "Sorted table" << "\n";
	for (size_t i = 0; i < number_of_records; i++) {
		for (size_t j = 0; j < row_size; j++) {
			cout << (int)data[i * row_size + j] << " ";
		}
		cout << "\n";
	}
	//for hdd blockSize is given by bandwidth * latency = 100*0.01
	
	size_t offset = 0;
	for(size_t i = 0; i < (number_of_records * row_size)/blockSize; i++)
	{
		sorted_hdd->writeData(static_cast<const void*>(data + i * blockSize),offset + i*(blockSize));
	}

    /**
     * Case 1: < 100 MB
     * Case 2: < 10 GB
     * Case 3: >= 10 GB
    */
    // Create buffers
    uint8_t* dramToHddInputBuffer;
    uint8_t* hddOutputBuffer;

    if (totalDataSize < dramLimit) { // Case 1: DRAM->HDD (exclusive with case 2/3)
        // Create buffers
        dramToHddInputBuffer = (uint8_t*) dram.getSpace(dramToHddInputBufferSize);
        hddOutputBuffer = (uint8_t*) dram.getSpace(hddBlockSize);

        // Read input into DRAM-to-HDD input buffer
        size_t seekCounter = 0;
        while (seekCounter < totalDataSize) {
            if (inputHdd->readData(dramToHddInputBuffer, seekCounter) <= 0) {
                break;
            }
            seekCounter += hddBlockSize;
        }
        // Generate cache-size runs over the input buffer

        // Merge these runs using Tree-of-Losers and output to HDD using hddOutputBuffer

        // Free buffers
        dram.freeSpace(hddOutputBuffer, hddBlockSize);
        dram.freeSpace(dramToHddInputBuffer, dramToHddInputBufferSize);
    } else if (totalDataSize < ssdLimit) { // Case 2: DRAM->SSD, SSD->HDD
        fillSsdToHdd(0, totalDataSize);
    } else { // Case 3: DRAM->SSD, SSD->HDD, HDD->HDD
        // Create constants
        const size_t hddRunSize = ssdLimit - hddBlockSize;
        const size_t minDramToHddInputBufferSize = dramLimit - hddOutputBufferSize - ssdMaxInputBufferSize - hddMaxInputBufferSize;
        const size_t hddMaxDataSize = totalDataSize - ssdMaxInputBufferSize - minDramToHddInputBufferSize;
        const size_t numberOfHddRuns = (hddMaxDataSize / hddRunSize) + (hddMaxDataSize % hddRunSize != 0); // Round up

        dramToHddInputBufferSize = dramLimit - hddOutputBufferSize - ssdMaxInputBufferSize - (numberOfHddRuns * hddBlockSize);
        //const size_t hddActualDataSize = totalDataSize - dramToHddInputBufferSize - ssdLimit;

        size_t seekCounter = 0;
        for (unsigned i = 0; i < numberOfHddRuns; i++) {
            fillSsdToHdd(seekCounter, hddRunSize);
            seekCounter += hddRunSize;
            // Generate cache-size runs over the input buffer

            // Merge these runs using Tree-of-Losers and output to SSD using ssdOutputBuffer
        }
        fillSsdToSsd(seekCounter, ssdLimit);
        seekCounter += ssdLimit;

        // Create buffers
        dramToHddInputBuffer = (uint8_t*) dram.getSpace(dramToHddInputBufferSize);
        uint8_t** ssdInputBuffers = new uint8_t*[ssdMaxRunCount];
        for (unsigned i = 0; i < ssdMaxRunCount; i++) {
            ssdInputBuffers[i] = (uint8_t*) dram.getSpace(ssdBlockSize);
        }
        uint8_t** hddInputBuffers = new uint8_t*[numberOfHddRuns];
        for (unsigned i = 0; i < numberOfHddRuns; i++) {
            hddInputBuffers[i] = (uint8_t*) dram.getSpace(hddBlockSize);
        }
        uint8_t* hddOutputBuffer = (uint8_t*) dram.getSpace(hddOutputBufferSize);

        // Read input into DRAM-to-HDD input buffer
        while (seekCounter < totalDataSize) {
            if (inputHdd->readData(dramToHddInputBuffer, seekCounter) <= 0) {
                break;
            }
            seekCounter += hddBlockSize;
        }
        // Generate cache-size runs over the input buffer

        // Merge these runs with the SSD and HDD input buffers using Tree-of-Losers and output to HDD using hddOutputBuffer

        // Free buffers
        dram.freeSpace(hddOutputBuffer, hddOutputBufferSize);
        for (unsigned i = 0; i < numberOfHddRuns; i++) {
            (uint8_t*) dram.freeSpace(hddInputBuffers[i], hddBlockSize);
        }
        //free(hddInputBuffers);
        for (unsigned i = 0; i < ssdMaxRunCount; i++) {
            (uint8_t*) dram.freeSpace(ssdInputBuffers[i], ssdBlockSize);
        }
        //free(ssdInputBuffers);
        dram.freeSpace(dramToHddInputBuffer, dramToHddInputBufferSize);
    }
    return 0;
}  // main
