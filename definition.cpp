#include "definition.h"
service::service(int res, int resRem): res(res),resRem(resRem){
}
ue::ue(int ueCom, int band, int row, int col): ueCom(ueCom), band(band){
	set = true;
	location = make_pair(row,col);
}
bs::bs(int bsCom, int totalBan, int remBan, int row, int col): bsCom(bsCom), totalBan(totalBan),remBan(remBan){
	set = true;
	location = make_pair(row,col);
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
