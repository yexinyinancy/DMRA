#include "definition.h"

service::service(int res, int resRem, double price) {
    this->res = res;
    this->resRem = resRem;
    this->price = price;
}


double service::getPrice(const ue& u) {
    double iota = 1.0;
    if (u.mysp != this->mybs->mysp) {
        iota = IOTA_PRICE;
    }
    double p = this->price * (iota + weightedDistance(u, *mybs));
    return p;
}

ue::ue(ServiceProvider* sp, int band, int row, int col) {
    this->mysp = sp;
    this->band = band;
    set = true;
    location = make_pair(row,col);
}

// bs::bs(int bsCom, int totalBan, int remBan, int row, int col): bsCom(bsCom), totalBan(totalBan), remBan(remBan) {
//     set = true;
//     location = make_pair(row,col);
// }

bs::bs(ServiceProvider* sp, int totalBan, int row, int col) {
    this->mysp = sp;
    this->totalBan = totalBan;
    this->remBan = totalBan;
    this->location = make_pair(row, col);
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

double weightedDistance(const unit& u1, const unit& u2, double sigma) {
    return sqrt(
        pow(u1.location.first - u2.location.first, 2) +
        pow(u1.location.second - u2.location.second, 2)
    );
}

double ue2bsPreference(const ue& UE, const bs& BS) {
    service &s = BS.services[UE.req.serviceType];
    double preference = s.getPrice(UE);
    preference += RHO / (s.resRem + BS.remBan);
    return preference;
}

// TODO: modify this
double bs2uePreference(const bs& BS, const ue& UE) {
    return UE.req.radioNeeded + UE.req.resourceRequested;
}