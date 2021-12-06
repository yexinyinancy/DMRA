#include "definition.h"
#include <ctime>
#include <cmath>
#include <vector>
#include <cstdlib>



double dist(const unit unit_1, const unit unit_2) {
    return sqrt(pow((unit_1.location.first - unit_2.location.first), 2) +
                pow((unit_1.location.second - unit_2.location.second), 2));
}

double price(ue UE, bs BS) {    //calculate the price of the bs to the ue
    return (UE.ueCom == BS.bsCom) ? (CON_B_PRICE + dist(UE, BS) * CON_B_PRICE) : (DIFF_PRICE * CON_B_PRICE +
                                                                                  dist(UE, BS) * CON_B_PRICE);
}

double v_value(const ue &UE, const bs &BS) {    //calculate the v_value of the bs to ue
    int resRem = 0;
    for (int i = 0; i < TASK; ++i) {
        if (UE.reqService[i].res != 0) {
            resRem += BS.totalService[i].resRem;
        }
    }
    double remBan = BS.remBan;
    //cout<<BS.bsCom<<" "<<resRem<<" "<<remBan<<" "<<dist(UE,BS)<<endl;
    return price(UE, BS) + RHO / (resRem + remBan);
}

void printGrid(unit **grid) {    //use to check the map of the city, just for fun
    for (int i = 0; i < LENGTH; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            if (grid[i][j].set) {
                if (grid[i][j].isBs) {
                    cout << "B";
                } else {
                    cout << "U";
                }
            } else {
                cout << ".";
            }
        }
        cout << endl;
    }
}

void swap(bs *bs_1, bs *bs_2) {
    bs bs_tmp;
    bs_tmp = *bs_2;
    bs_2 = bs_1;
    *bs_1 = bs_tmp;
}

void swapUE(ue *ue_1, ue *ue_2) {
    ue ue_tmp;
    ue_tmp = *ue_2;
    ue_2 = ue_1;
    *ue_1 = ue_tmp;
}

void ueReq(ue *ueList,
           bs *bsList) {    //for each ue in the list, generate the target bs and add the ue to the request list of the bs
    for (int i = 0; i < UENUM; ++i) {
        //cout<<ueList[i].fu<<endl;
        while (!ueList[i].clear && ueList[i].coverBs.size() != 0) {
            double v_ui = 9999;  //the value to judge which bs should be requested
            for (int j = 0; j < ueList[i].coverBs.size(); ++j) {
                if (!ueList[i].coverBs[j].serUe[i]) {
                    double v_tmp = v_value(ueList[i], ueList[i].coverBs[j]);
//                    cout << v_tmp << " " << ueList[i].coverBs[j].bsCom << " " << ueList[i].coverBs[j].totalBan << endl;
                    if (v_tmp < v_ui) {
                        ueList[i].targetBs = &ueList[i].coverBs[j];
                        v_ui = v_tmp;
                    }
                }
            }
            if (ueList[i].targetBs) {
                //cout<<ueList[i].targetBs->bsCom<<endl;
                bool flag = ueList[i].targetBs->totalBan >= ueList[i].band;    //judge if the band is available
                for (int j = 0; j < TASK; ++j) {
                    flag &= ueList[i].targetBs->totalService[j].resRem >=
                            ueList[i].reqService[j].res;    //judge if every task service is available
                }
                if (flag) {
                    bsList[ueList[i].targetBs->bsCom].ueList[bsList[ueList[i].targetBs->bsCom].reqUe++] = ueList[i];    //put the ue to the corresponding reqList of the BS
                    break;
                } else {
                    cout << "The change happens" << endl;
                    swap(ueList[i].targetBs, &ueList[i].coverBs[ueList[i].coverBs.size() - 1]);
                    ueList[i].coverBs.pop_back();
                }
            }
        }
    }
}

void
init(unit **grid, ue *ueList, bs *bsList) {    //initialize the grid and ue bs list with the random or selected value
    int tmp = 100;
    for (int i = 0; i < LENGTH; ++i) {
        grid[i] = new unit[WIDTH];  //create the grid with length and width
        for (int j = 0; j < WIDTH; ++j) grid[i][j].set = false;
    }
    for (int i = 0; i < BSNUM; ++i) {
//        srand(time(nullptr) + 2 * i);
        srand(2 * i + tmp);
        int row = rand() % LENGTH;
//        srand(time(nullptr) - 2 * i);
        srand(-2 * i + tmp);
        int col = rand() % WIDTH; //randomly choose the location of the unit
        if (!grid[row][col].set) {
//            srand(time(nullptr) - 2 * i * i);
            int band = 10;   //randomly set the band of the BS
            bs newbs(i, band, band, row, col);
            ue *ueList = new ue[UENUM];
            newbs.ueList = ueList;
            service *bsService = new service[TASK];
            newbs.ServiceNum = 1;
            for (int j = 0; j < TASK; ++j) {
                bsService[j].resRem = 10;
                bsService[j].res = 10;   //set the resource of each service
            }
            newbs.totalService = bsService;
            newbs.reqUe = 0;
            bsList[i] = newbs;
            grid[row][col].set = true;
            grid[row][col].isBs = true;
        } else {
            i--;
        }
    }
    for (int i = 0; i < UENUM; ++i) {
//        srand(time(nullptr) + i);
        srand(i);
        int row = rand() % LENGTH;
//        srand(time(nullptr) - i);
        srand(i + 1);
        int col = rand() % WIDTH;
        if (!grid[row][col].set) {
//            srand(time(nullptr) + i * i);
            int band = 1; //rand()%BANDUP;
            ue newue(i / BSNUM, band, row, col);
            newue.index = i;
            service *ueService = new service[TASK];
            for (int j = 0; j < TASK; ++j) {
                ueService[j].res = 1;   //can be set or random, here I set the it to be 1 for each service
            }
            for (int j = 0; j < BSNUM; ++j) {
                if (dist(newue, bsList[j]) <= COVERDIS) {
                    newue.coverBs.push_back(bsList[j]);
                    //cout<<bsList[j].location.first<<" "<<bsList[j].location.second<<endl; //check the list of the bs in the range
                }
            }
            newue.reqService = ueService;
            ueList[i] = newue;
            grid[row][col].set = true;
            grid[row][col].isBs = false;
        } else {
            i--;
        }
    }
}
//need a price decision scheme to decide the profit
//for the service request of a single ue, if we can only do part of the service
/*bsReq(ue* ueList, bs* bsList){
    for(int i=0;i<BSNUM;++i){
        int pro[]        
    }
}*/

double cost_return(const ue &UE, const bs &BS) {    //calculate the v_value of the bs to ue
    return UE.reqService->res * price(UE, BS);  //need to plus radio revenue and minus cost
}

// Input: values, weights, number of distinct items, knapsack capacity
vector<ue> knapsack(double products_count, double capacity_, ue object_list[UENUM], vector<vector<double>> &res,
                    const bs &BS, int &max_m) {
    vector<ue> tmp;
    max_m = 0;
    //初始化第一行，仅考虑容量为C的背包放第0个物品的情况
    for (int i = 0; i <= capacity_; i++) {
        double value = object_list[0].reqService[0].res * price(object_list[0], BS);
        res[0][i] = object_list[0].reqService[0].res <= i ? value : 0;
    }
    for (int i = 1; i < products_count; i++) {  //i个物品
        for (int j = 0; j <= capacity_; j++) {  //背包容量为j
            if (object_list[i].reqService[0].res > j) {  //当前背包的容量 j 放不下第 i 件商品时
                res[i][j] = res[i - 1][j];
            } else {
                double value = object_list[i].reqService[0].res * price(object_list[i], BS);
                if (res[i - 1][j - object_list[i].reqService[0].res] + value > res[i - 1][j]) {
                    res[i][j] = res[i - 1][j - object_list[i].reqService[0].res] + value;
                } else {
                    res[i][j] = res[i - 1][j];
                }
            }
        }
    }
    // print result matrix for debugging
//    for (int i = 0; i < products_count; i++) {
//        for (int j = 0; j <= capacity; j++) {
//            cout << res[i][j] << "  ";
//        }
//        cout << endl;
//    }
    // get final objects list in the bag
    int j = capacity;
    for (int i = products_count - 1; i > 0; i--) {
        if (res[i][j] != res[i - 1][j]) {
            tmp.push_back(object_list[i]);
            max_m += object_list[i].reqService[0].res * price(object_list[i], BS);
            j = j - object_list[i].reqService[0].res;
        }
    }
    if (res[0][j] != 0) {
        tmp.push_back(object_list[0]);
        max_m += object_list[0].reqService[0].res * price(object_list[0], BS);
    }
    return tmp;
}

void beReq(ue *ueList, bs *bsList, int a[UENUM][BSNUM]) {
    int result_m = 0;
    for (int i = 0; i < BSNUM; ++i) {
        if (bsList[i].reqUe != 0) {
            for (int o = 0; o < bsList[i].reqUe; o++) {
                cout << "bsList[" << i << "].ueList[" << o << "].index: " << bsList[i].ueList[o].index << endl;
            }
            vector<vector<double>> res(bsList[i].reqUe, vector<double>(capacity + 1));
            vector<ue> service = knapsack(bsList[i].reqUe, capacity, bsList[i].ueList, res, bsList[i], result_m);
            for (int j = 0; j < service.size(); j++) {
                service[j].reqService[0].res = 0;
                ueList[service[j].index].clear = true;
                ueList[service[j].index].reqService[0].res = 0;
                a[service[j].index][bsList[i].bsCom] = 1;
            }
        }
    }
}

void beReq_ori(ue *ueList, bs *bsList, int a[UENUM][BSNUM]) {
    for (int i = 0; i < BSNUM; ++i) {
        for (int o = 0; o < bsList[i].reqUe; o++) {
            cout << "bsList[" << i << "].ueList[" << o << "].index: " << bsList[i].ueList[o].index << endl;
        }
        bsList[i].tmpList.clear();
        for (int j = 0;
             j < bsList[i].ServiceNum; ++j) {    //put the ue to the corresponding group according to their company
            vector<ue> U1;
            vector<ue> U2;
            for (int k = 0; k < bsList[i].reqUe; ++k) {
                if (bsList[i].bsCom == bsList[i].ueList[k].ueCom) {
                    U1.push_back(bsList[i].ueList[k]);
                } else {
                    U2.push_back(bsList[i].ueList[k]);
                }
            }
            while (!U1.empty() || !U2.empty()) {
                int min = 9999;
                int ind = 0;
                ue temp;
                if (!U1.empty()) {
                    for (int k = 0; k < U1.size(); k++) {
                        if (U1[k].coverBs.size() < min) {
                            ind = k;
                            min = U1[k].coverBs.size();
                            temp = U1[k];
                        }
                    }
                    bsList[i].tmpList.push_back(temp);
                    swapUE(&U1[ind], &U1[U1.size() - 1]);
                    U1.pop_back();
                } else {
                    for (int k = 0; k < U2.size(); k++) {
                        if (U2[k].coverBs.size() < min) {
                            ind = k;
                            min = U2[k].coverBs.size();
                            temp = U2[k];
                        }
                    }
                    bsList[i].tmpList.push_back(temp);
                    swapUE(&U2[ind], &U2[U2.size() - 1]);
                    U2.pop_back();
                }
            }
            bool flag = true;
            int sumRes;
            int sumBand;
            while (flag) {
                sumRes = 0;
                sumBand = 0;
                for (int k = 0; k < bsList[i].tmpList.size(); ++k) {
                    sumRes += bsList[i].tmpList[k].reqService[j].res;
                    sumBand += bsList[i].tmpList[k].band;
                    a[bsList[i].tmpList[k].index][i] = 1;
                }
                if (sumBand <= bsList[i].remBan && sumRes <= bsList[i].totalService[j].resRem) {
                    for (int k = 0; k < bsList[i].tmpList.size(); ++k) {
                        bsList[i].serUe[bsList[i].tmpList[k].index] = 1;
                        ueList[bsList[i].tmpList[k].index].reqService[j].res = 0;
                        ueList[bsList[i].tmpList[k].index].clear = true;
                    }
                    flag = false;
                } else {
                    bsList[i].tmpList.pop_back();
                }
            }
            bsList[i].remBan -= sumBand;
            bsList[i].totalService[j].resRem -= sumRes;
        }
    }
}

int main() {
    // Method 1:
    cout << "---------Method 1----------" << endl;
    unit **grid = new unit *[LENGTH];
    ue *ueList = new ue[UENUM];
    bs *bsList = new bs[BSNUM];    //create the ue and bs list
    int a[UENUM][BSNUM] = {0};
    init(grid, ueList, bsList);
    bool flag = false;
    while (!flag) {
        cout << "The new round begin" << endl;
        ueReq(ueList, bsList);
        //for (int i = 0; i < BSNUM; ++i) {
        // cout << bsList[i].reqUe << endl;
        //}
        //bag problem
        //a[i][j]=1 means that BS_i serves UE_j
        beReq_ori(ueList, bsList, a);
        flag = ueList[0].clear;
        for (int i = 1; i < UENUM; ++i) {
            flag &= ueList[i].clear;
        }
    }
    int total = 0;
    for (int j = 0; j < BSNUM; j++) {
        int total_price = 0;
        for (int i = 0; i < UENUM; i++) {
            cout << a[i][j] << "  ";
            if (a[i][j] != 0) {
                total_price += price(ueList[i], bsList[j]); //calculate total profit
            }
        }
        cout << "The total profit of the " << j << " bs is " << total_price << endl;
        total += total_price;
    }
    cout << "Total profit " << total << endl;
    cout << endl;

// Method 2:
    cout << "---------Method 2----------" << endl;
    unit **grid2 = new unit *[LENGTH];
    ue *ueList2 = new ue[UENUM];
    bs *bsList2 = new bs[BSNUM];    //create the ue and bs list
    int a2[UENUM][BSNUM] = {0};
    init(grid2, ueList2, bsList2);
    ueReq(ueList2, bsList2);
    beReq(ueList2, bsList2, a2);
    total = 0;
    for (int j = 0; j < BSNUM; j++) {
        int total_price = 0;
        for (int i = 0; i < UENUM; i++) {
            cout << a2[i][j] << "  ";
            if (a2[i][j] != 0) {
                total_price += price(ueList2[i], bsList2[j]); //calculate total profit
            }
        }
        cout << "The total profit of the " << j << " bs is " << total_price << endl;
        total += total_price;
    }
    cout << "Total profit " << total << endl;

    //printGrid(grid);
}

