#ifndef TEST_DEFINE2_H
#define TEST_DEFINE2_H

#include <utility>
#include <iostream>
#include <vector>
#include <bits/stdc++.h>
#include <cmath>

using namespace std;

// forward declarations
class unit;
class bs;
class ue;
class ServiceProvider;

// (weighted) distance between two units
double weightedDistance(const unit& u1, const unit& u2, double sigma = 1);
// between two units
double distance(const unit& u1, const unit& u2);

class ServiceProvider {
public:
    int spID;
    // service prices for UEs to use
    vector<double> prices;
    // all BSs
    const vector<bs*> BSs;
    // profit after allocation
    double profit;
public:

};

class unit {
public:
    // position of this unit on the grid
    pair<int, int> location;
    // SP of this unit
    ServiceProvider* mysp;

    // indicate the unit is set on the grid
    bool set;
    bool isBs;
};



class service {
public:
    int serviceType;
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
    // get the unit price of this service on a bs requested by an sp
    double getPrice(const ue& u) const;
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

    double band;
    // BSs thats cover this ue and provides the service requested
    unordered_set<bs*> coverBSs;
    // services requested by this ue
    request req;
    // the bs that finally handles req
    bs* targetBs;
    // the service that finally handles req
    service* targetService;
public:
    // ue to bs preference, lower is more prefered
    double bsPreference(const bs& BS) const;
    // estimate latency
    double serviceLatency(const service& s) const;
    bs* findBestBS();
};

class bs : public unit {
public:
    // total radio resource
    double totalBan;
    // remained radio resource
    double remBan;
    // services provided by this bs
    unordered_map<int, service> services;
public:
    // bs to ue preference, lower is more prefered
    // TODO: modify this
    double uePreference(const ue& UE);
    //void operator = (const bs &BS);
    bool provideService(int service_type) const {
        auto it = services.find(service_type);
        return it != services.end();
    }
};

class World {
public:
    // a virtual remote_cloud
    bs remote_cloud;
    vector<bs> BSs;
    vector<ue> UEs;
    vector<ServiceProvider> SPs;
    // result of the match problem
    // each ue has one request assigned to a bs; if no bs, assigned to the remote cloud
    // unordered_map<ue*, bs*> result;
public:
    World();
    void maxProfit();
    void printSPProfit();
    void printTotalLatency();
};

#endif
