#ifndef __TOL_H__
#define __TOL_H__

#include "Ssd.h"
#include "dram.h"

#define DRAM 0
#define SSD 1
#define HDD 2

class Storage
{
	friend class Run;
	Ssd *s;
	uint8_t *d;
	size_t srcSeek;
	size_t blockSize;
	Storage(Ssd *storage, uint8_t *d, size_t bs, size_t ss);
	~Storage();
};

class Run
{
	friend class TOL;
	Storage source;
	size_t runSize;
	size_t head;
	size_t tail;
public:
	Run(Ssd *_s, uint8_t *_d, size_t bs, size_t ss, size_t rs);
	~Run();
	int getNext(size_t s = source.blockSize, size_t offset);// SSD/HDD to DRAM
	int setNext(size_t s = source.blockSize, size_t offset);// DRAM to SSD/HDD
	uint8_t *getBuf();										// Get DRAM buffer
	size_t getSize();
};

class Node
{
	friend class TOL;
	int index;
	uint8_t *key;
	int ofc;
	Node *lft, *rht, *pnt;
};


class TOL
{
	size_t numOfRun;
	Node **nodeList;
	Run **runList;
	Run *output
public:
	TOL(size_t nor, Run **rl, Run *o);
};


// Leaf: Should have info about where to get next part of run
// node: index(which run this key belong to), key, ofc, left child, right child,
// parent
#endif
