#ifndef TEST_DEFINE2_H
#define TEST_DEFINE2_H

#include <utility>
#include <iostream>
#include <vector>
#include <bits/stdc++.h>
#include <cmath>

using namespace std;

// forward declaration
class unit;
class bs;
class ue;
class ServiceProvider;

const int WIDTH = 100;
const int LENGTH = 100;
const int TASK = 1;
const int UENUM = 20;
const int BSNUM = 10;

// the constant b in the price equation
const int CON_B_PRICE = 1;
// the constant theta in the price equation
const double DIFF_PRICE = 1.5;
// the constant IOTA in the price equation
const double IOTA_PRICE = 1.5;
// cost for SP to pay for BS computation
const double SERVICE_PRICE = 1.0;
// cost for SP to pay for cloud computation
const double REMOTE_CLOUD_PRICE = 5.0;
// constant mko
const double MKO = 0.5;
// constant mk, price for UE to pay for SP's service
const double MK = 2.0;

// the constant rho in the price of v_ui
const int RHO = 5;
const int BANDUP = 3;
const double COVERDIS = 100;
// for knapsack problem
const double capacity = 500;

double weightedDistance(const unit& u1, const unit& u2, double sigma = 1);

double ue2bsPreference(const ue& UE, const bs& BS);


class ServiceProvider {
public:
    int spID;
    // service prices for UEs to use
    vector<double> prices;
public:

};

class unit {
public:
    // position of this unit on the grid
    pair<int,int> location;
    // SP of this unit
    ServiceProvider* mysp;

    // indicate the unit is set on the grid
    bool set;
    bool isBs;
};



class service {
public:
    int serviceID;
    // For bs, it is the total number of the resource
    int res;
    // For bs, it is the remaining number of the resource
    int resRem;
    // It is the final price that the bs would collect from a ue
    // for a sprcific service
    double price = 0;
    // how fast a bs can process this service
    int computationPower;

    // UEs that require this service
    vector<ue*> candidates;

    // the bs that this service is located on
    bs* mybs;

public:
    service(int res = 0, int resRem = 0, double price = 1);
    // get the unit price of this service on a bs requested by an sp
    double getPrice(const ue& u);
};

class request {
public:
    // amount of work needed to process this request
    int workload;
    // type of service
    int serviceType;
    int resourceRequested;
    // radio resource needed for this request
    double radioNeeded;
    // the ue that issues this request
    ue* myue;
};


class ue : public unit {
public:
    // if the task of ue has been all clear;
    bool clear = false;
    int index;
    // int ueCom;    
    // bool set = true;
    // bool isBs = false;
    double band;
    // service* reqService;
    // BSs that cover this ue
    vector<bs*> coverBSs;

    // services requested by this ue
    request req;
    // the bs that finally handles req
    bs* targetBs;
    ue(ServiceProvider* sp, int band = 10, int row = 0, int col = 0);
};

class bs : public unit {
public:
    int ServiceNum;
    // int bsCom;    //the company of this bs
    // bool set = true;
    // bool isBs = true;
    double totalBan;
    double remBan;
    // service* totalService;
    // services provided by this bs
    unordered_map<int, service> services;
    int reqUe;
    // ue* ueList;
    vector<ue*> ueList;
    // the list of the tmp U for the bs in the original method
    vector<ue> tmpList;    
    // bool* serUe = new bool[UENUM];    //if the bs has served the specific ue
    unordered_set<ue*> servedUEs;
    bs(ServiceProvider* sp, int totalBan = 100, int row = 0, int col = 0);
    //void operator = (const bs &BS);
};

#endif
