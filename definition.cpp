#include "definition.h"
#include <cassert>

/*********************  constants  *************************/

const int WIDTH = 100;
const int LENGTH = 100;
const int TASK = 1;
const int UENUM = 20;
const int BSNUM = 10;

// 10 types of services are provided
const int NUM_SERVICE_TYPES = 10;

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


/*********************  service methods  *************************/

// service::service(int res, int resRem, double price) {
//     this->res = res;
//     this->resRem = resRem;
//     this->price = price;
// }


double service::getPrice(const ue& u) const {
    double iota = 1.0;
    if (u.mysp != this->mybs->mysp) {
        iota = IOTA_PRICE;
    }
    double p = this->price * (iota + weightedDistance(u, *mybs));
    return p;
}

/*void bs::operator = (const bs &BS){
    this->bsCom = BS.bsCom;
    this->totalBan = BS.totalBan;
    this->remBan = BS.remBan;
    this->totalService = BS.totalService;
    this->reqUe = BS.reqUe;
    this->ueList = BS.ueList;
    this->serUe = BS.serUe;
}
*/

/*********************  ue methods  *************************/

// ue::ue(ServiceProvider* sp, int band, int row, int col) {
//     this->mysp = sp;
//     this->band = band;
//     set = true;
//     location = make_pair(row,col);
// }

double ue::bsPreference(const bs& BS) {
    auto service_it = BS.services.find(this->req.serviceType);
    if (service_it == BS.services.end()) {
        return 999999999.0;
    }
    const service& s = service_it->second;
    double p = s.getPrice(*this) + RHO / (s.resRem + BS.remBan);
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

/*********************  bs methods  *************************/

// bs::bs(ServiceProvider* sp, int totalBan, int row, int col) {
//     this->mysp = sp;
//     this->totalBan = totalBan;
//     this->remBan = totalBan;
//     this->location = make_pair(row, col);
// }

double bs::uePreference(const ue& UE) {
    return UE.req.radioNeeded + UE.req.resourceRequested;
}

/*********************  World methods  *************************/

World::World() {
    int service_num;
    cin >> service_num;

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
    this->result.clear();
    for (ue& UE : this->UEs) {
        this->result[&UE] = nullptr;
    }
    bool req_sent = false;

    do {
        req_sent = false;
        for (ue& UE : this->UEs) {
            while (!UE.coverBSs.empty() && result[&UE] == nullptr) {
                bs* target_bs = UE.findBestBS();
                assert(target_bs != nullptr);
                if (
                    target_bs->services[UE.req.serviceType].resRem >= UE.req.resourceRequested &&
                    target_bs->remBan >= UE.req.radioNeeded
                ) {
                    // send service request to target bs
                    target_bs->services[UE.req.serviceType].candidates.push_back(&UE);
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
                this->result[tUE] = &BS;
            }
            target_UEs.clear();
        }

    } while (req_sent);

    // calculate SP profits
    for (ue& UE : this->UEs) {
        UE.mysp->profit += UE.mysp->prices[UE.req.serviceType];
        double bs_cost = REMOTE_CLOUD_PRICE;
        if (this->result[&UE] != nullptr) {
            bs_cost =
                this->result[&UE]->services[UE.req.serviceType].getPrice(UE) *
                UE.req.resourceRequested;
        }
        UE.mysp->profit -= bs_cost;
        double other_cost = UE.req.resourceRequested * MKO;
        UE.mysp->profit -= other_cost;
    }
}

/*********************  other functions  *************************/

double weightedDistance(const unit& u1, const unit& u2, double sigma) {
    return sqrt(
        pow(u1.location.first - u2.location.first, 2) +
        pow(u1.location.second - u2.location.second, 2)
    );
}

double distance(const unit& u1, const unit& u2) {
    return sqrt(
        pow(u1.location.first - u2.location.first, 2) +
        pow(u1.location.second - u2.location.second, 2)
    );
}
