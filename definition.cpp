#include "definition.h"
#include <cassert>

/*********************  constants  *************************/

const int WIDTH = 100;
const int LENGTH = 100;
const int TASK = 1;
const int UENUM = 20;
const int BSNUM = 10;

const double COVERDIS = 300;

// 10 types of services are provided
const int NUM_SERVICE_TYPES = 10;

// the constant b in the price equation
const int CON_B_PRICE = 1;
// the constant SIGMA in the price equation
const double SIGMA_PRICE = COVERDIS;
// the constant IOTA in the price equation
const double IOTA_PRICE = 2.0;
// cost for SP to pay for BS computation
const double SERVICE_PRICE = 1.0;
// cost for SP to pay for cloud computation
const double REMOTE_CLOUD_PRICE = 15.0;
// constant mko
const double MKO = 5.0;
// constant mk, price for UE to pay for SP's service
const double MK = 2.0;

// the constant rho in the price of v_ui
const int RHO = 5;
const int BANDUP = 3;

// for knapsack problem
const double capacity = 500;


/*********************  service methods  *************************/

double service::getPrice(const ue& u) const {
    double iota = 1.0;
    if (this->mybs->mysp && u.mysp != this->mybs->mysp) {
        iota = IOTA_PRICE;
    }
    double p = 0.0;
    if (this->mybs->mysp) {
        p = this->price * (iota + weightedDistance(u, *mybs, SIGMA_PRICE));
    } else {
        p = this->price * iota;
    }
    return p;
}

/*********************  ue methods  *************************/

double ue::bsPreference(const bs& BS) const {
    auto service_it = BS.services.find(this->req.serviceType);
    if (service_it == BS.services.end()) {
        return 999999999.0;
    }
    const service& s = service_it->second;
    double p = s.getPrice(*this) + RHO / (s.resRem + BS.remBan);
    // double p = s.getPrice(*this) - (s.resRem + BS.remBan);
    // double p = -(this->mysp->prices[this->req.serviceType] - s.getPrice(*this)) * this->req.resourceRequested + RHO / (s.resRem + BS.remBan);

    return p;
}

bs* ue::findBestBS() {
    bs* target = nullptr;
    double min_pref = 999999999.0;
    for (auto BS : coverBSs) {
        double tp = this->bsPreference(*BS);
        if (tp < min_pref) {
            min_pref = tp;
            target = BS;
        }
    }
    return target;
}

double ue::serviceLatency(const service& s) const {
    // cout << s.serviceType << " " << this->req.serviceType << endl;
    assert(s.serviceType == this->req.serviceType);
    double distance_delay = 2.0 * distance(*(s.mybs), *this) / 1000.0;
    double computation_delay = this->req.workload * 1.0 / (this->req.resourceRequested * s.computationPower);
    return distance_delay + computation_delay;
}

/*********************  bs methods  *************************/

// bs::bs(ServiceProvider* sp, int totalBan, int row, int col) {
//     this->mysp = sp;
//     this->totalBan = totalBan;
//     this->remBan = totalBan;
//     this->location = make_pair(row, col);
// }

double bs::uePreference(const ue& UE) {
    double p = 10 * UE.serviceLatency(this->services[UE.req.serviceType]) -(UE.mysp->prices[UE.req.serviceType] - this->services[UE.req.serviceType].getPrice(UE)) * UE.req.resourceRequested + UE.req.radioNeeded + UE.req.resourceRequested;
    return p;
    // return UE.serviceLatency(this->services[UE.req.serviceType]) + UE.req.radioNeeded + UE.req.resourceRequested;
}

/*********************  World methods  *************************/

World::World() {
    int service_num;
    cin >> service_num;

    // init remote_cloud
    this->remote_cloud.mysp = nullptr;
    for (int i = 0; i < service_num; i++) {
        service s;
        s.mybs = &(this->remote_cloud);
        s.computationPower = 10;
        s.price = REMOTE_CLOUD_PRICE;
        s.serviceType = i;
        this->remote_cloud.services[i] = s;
        assert(i == this->remote_cloud.services[i].serviceType);
    }
    this->remote_cloud.location.first = 5000;
    this->remote_cloud.location.second = 5000;

    // init SPs
    int sp_num;
    cin >> sp_num;
    this->SPs.resize(sp_num);
    for (ServiceProvider& sp : this->SPs) {
        cin >> sp.spID;
        sp.prices.resize(service_num);
        for (double& p : sp.prices) {
            cin >> p;
        }
    }

    // init BSs
    int bs_num;
    cin >> bs_num;
    this->BSs.resize(bs_num);
    for (bs& BS : this->BSs) {
        int bs_service_num;
        int bs_spid;
        cin >> BS.location.first >> BS.location.second >> BS.totalBan >> bs_service_num >> bs_spid;
        BS.remBan = BS.totalBan;
        BS.mysp = &(this->SPs[bs_spid]);
        for (int i = 0; i < bs_service_num; i++) {
            int service_type;
            service s;
            cin >> service_type >> s.res >> s.price >> s.computationPower;
            s.resRem = s.res;
            s.mybs = &BS;
            s.serviceType = service_type;
            BS.services[service_type] = s;
        }
    }

    // init UEs
    int ue_num;
    cin >> ue_num;
    this->UEs.resize(ue_num);
    for (ue& UE : this->UEs) {
        int ue_spid;
        cin >> UE.location.first >> UE.location.second >> ue_spid
            >> UE.req.serviceType >> UE.req.workload
            >> UE.req.resourceRequested >> UE.req.radioNeeded;
        UE.mysp = &(this->SPs[ue_spid]);
        UE.req.myue = &UE;
        for (bs& BS : this->BSs) {
            if (distance(BS, UE) <= COVERDIS && BS.provideService(UE.req.serviceType)) {
                UE.coverBSs.insert(&BS);
            }
        }
    }
}

void World::maxProfit() {
    bool req_sent;
    do {
        req_sent = false;
        for (ue& UE : this->UEs) {
            while (!UE.coverBSs.empty() && UE.targetBs == nullptr) {
                bs* target_bs = UE.findBestBS();
                assert(target_bs != nullptr);
                if (
                    target_bs->services[UE.req.serviceType].resRem >= UE.req.resourceRequested &&
                    target_bs->remBan >= UE.req.radioNeeded
                ) {
                    // send service request to target bs
                    target_bs->services[UE.req.serviceType].candidates.push_back(&UE);
                    assert(target_bs->services[UE.req.serviceType].serviceType == UE.req.serviceType);
                    req_sent = true;
                    break;
                } else {
                    UE.coverBSs.erase(target_bs);
                }
            }
        }

        for (bs& BS : this->BSs) {
            vector<ue*> target_UEs;
            for (auto it = BS.services.begin(); it != BS.services.end(); it++) {
                auto& candidates = it->second.candidates;
                if (candidates.empty()) {
                    continue;
                }
                // U1 is the set of UEs with the same SP as BS's SP
                // U2 is the set of UEs with different SP from BS's SP
                vector<ue*> U1, U2;
                for (ue* cand : candidates) {
                    if (cand->mysp == BS.mysp) {
                        U1.push_back(cand);
                    } else {
                        U2.push_back(cand);
                    }
                }
                candidates.clear();
                assert(!U1.empty() || !U2.empty());
                auto cmp = [&BS](ue* u1, ue* u2) -> bool {
                    bool a = u1->coverBSs.size() < u2->coverBSs.size();
                    bool b = u1->coverBSs.size() == u2->coverBSs.size();
                    bool c = BS.uePreference(*u1) < BS.uePreference(*u2);
                    return a || (b && c);
                };
                ue* target_UE = nullptr;
                if (!U1.empty()) {
                    sort(U1.begin(), U1.end(), cmp);
                    target_UE = U1.front();
                } else {
                    sort(U2.begin(), U2.end(), cmp);
                    target_UE = U2.front();
                }
                assert(target_UE != nullptr);
                target_UEs.push_back(target_UE);
            }
            double w = 0;
            for (ue* tUE : target_UEs) {
                w += tUE->req.radioNeeded;
            }
            if (w > BS.remBan) {
                cerr << "Shouldn't be here" << endl;
                assert(false);
            }
            assert(w <= BS.remBan);
            BS.remBan -= w;
            for (ue* tUE : target_UEs) {
                assert(BS.services[tUE->req.serviceType].resRem >= tUE->req.resourceRequested);
                BS.services[tUE->req.serviceType].resRem -= tUE->req.resourceRequested;
                tUE->targetBs = &BS;
                tUE->targetService = &(BS.services[tUE->req.serviceType]);
                // cout << tUE->targetService->serviceType << " " << tUE->req.serviceType << endl;
                assert(tUE->targetService->serviceType == tUE->req.serviceType);
            }
            target_UEs.clear();
        }

    } while (req_sent);


    // calculate SP profits
    int num_cloud = 0;
    for (ue& UE : this->UEs) {
        if (UE.targetBs == nullptr) {
            UE.targetBs = &(this->remote_cloud);
            UE.targetService = &(this->remote_cloud.services[UE.req.serviceType]);
            num_cloud++;
        }
        double ue_profit = 0.0;
        ue_profit += UE.mysp->prices[UE.req.serviceType] * UE.req.resourceRequested;
        double compution_price = UE.targetBs->services[UE.req.serviceType].getPrice(UE);
        double bs_cost = compution_price * UE.req.resourceRequested;
        double other_cost = UE.req.resourceRequested * MKO;
        ue_profit -= (bs_cost + other_cost);
        // if (ue_profit < 0) {
        //     cout << UE.targetBs << " " << &this->remote_cloud << endl;
        // }
        if (UE.targetBs == &(this->remote_cloud) && ue_profit > 0.1) {
            cout << ue_profit << endl;
        }
        UE.mysp->profit += ue_profit;
    }
    cout << num_cloud << " out of " << this->UEs.size() << " are assigned to remote cloud\n";
}

void World::printSPProfit() {
    double tp = 0.0;
    for (ServiceProvider& sp : this->SPs) {
        // cout << "SP-" << sp.spID << " profit = " << sp.profit << endl;
        tp += sp.profit;
    }
    cout << "Total Profit = " << tp << endl;
}

void World::printTotalLatency() {
    double latency = 0.0;
    for (int i = 0; i < this->UEs.size(); i++) {
        ue& UE = this->UEs[i];
        double ll = UE.serviceLatency(*(UE.targetService));
        // cout << "UE-" << i << " latency = " << ll << endl;
        latency += ll;
    }
    cout << "Total Latency = " << latency << endl;
}

/*********************  other functions  *************************/

double weightedDistance(const unit& u1, const unit& u2, double sigma) {
    return sqrt(
        pow(u1.location.first - u2.location.first, 2) +
        pow(u1.location.second - u2.location.second, 2)
    ) / sigma;
}

double distance(const unit& u1, const unit& u2) {
    return sqrt(
        pow(u1.location.first - u2.location.first, 2) +
        pow(u1.location.second - u2.location.second, 2)
    );
}
