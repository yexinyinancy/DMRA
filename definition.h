#ifndef TEST_DEFINE2_H
#define TEST_DEFINE2_H
#include <utility>
#include <iostream>
#include <vector>
using namespace std;
const int WIDTH = 100;
const int LENGTH = 100;
const int TASK = 1;
const int UENUM = 20;
const int BSNUM = 10;
class unit{
public:
    pair<int,int> location; //表示该单元在grid上的位置
    int com;    //表示该单元所属的sp单位
	bool set;	//indicate the unit is setted on the grid;
	bool isBs;
};
class service{
public:
    int res;    //For bs, it is the total number of the resource
    //For ue, it is the total number requested
    int resRem;//For bs, it is the remaining number of the resource
    //For ue, it is not needed.
    int price = 0;  //It is the final price that the bs would collect from a ue
    //for a sprcific service
    service(int res = 0, int resRem = 0);
};
class bs;
class ue : public unit{
public:
	bool clear = false;	//if the task of ue has been all clear;
	int index;
	int ueCom;	//the company of this ue
	bool set = true;
	bool isBs = false;
    double band;
    service* reqService;
    vector<bs> coverBs;
    bs* targetBs;
    ue(int ueCom = 0, int band = 10, int row=0, int col=0);
};
class bs : public unit{
public:
	int ServiceNum;
	int bsCom;	//the company of this bs
	bool set = true;
	bool isBs = true;
    double totalBan;
    double remBan;
    service* totalService;
	int reqUe;
    ue* ueList;
	vector<ue> tmpList;	//the list of the tmp U for the bs in the original method
	bool* serUe = new bool[UENUM];	//if the bs has served the specific ue
    bs(int bsCom = 0, int totalBan = 100, int remBan = 100, int row=0, int col=0);
	//void operator = (const bs &BS);
};
#endif
