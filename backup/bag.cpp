//#include <iostream>
//#include<vector>
//using namespace std;
//const int maxn = 1001;
//int dp[maxn][maxn][maxn] = {0};
//const int d = 10;
//const int c = 10;
//const int n = 6;
//int v[maxn] = {10, 8, 10, 10, 1, 5};
//int b[maxn] = {10, 1, 3, 4, 2, 100};
//int w[maxn] = {10, 2, 1, 4, 3, 300};
//vector<int> res;
//vector<int> dpFunc() {
//    for (int i = 1; i <= n; i++) {    //序号
//        for (int j = 1; j <= c; j++) {   //背包剩余容量
//            for (int k = 1; k <= d; k++) {   //背包剩余容积
//                if (j >= w[i] && k >= b[i]) {
//                    if (dp[i - 1][j][k] > dp[i - 1][j - w[i]][k - b[i]] + v[i]) {
//                        dp[i][j][k] = dp[i - 1][j][k];
//                    } else {
//                        dp[i][j][k] = dp[i - 1][j - w[i]][k - b[i]] + v[i];
////                        res.pop_back(dp[])
//                        res.push_back(v[i]);
//                    }
//                } else {
//                    dp[i][j][k] = dp[i - 1][j][k];
//                }
//            }
//        }
//    }
////    res.erase(unique(res.begin(), res.end()), res.end());
//    return res;
//}
//
//int main() {
//    vector<int> tmp = dpFunc();
//    for (int i : tmp) {
//        cout << i << endl;
//    }
//    return 0;
//}


#include <iostream>
#include <vector>

using namespace std;

class object {
public:
    int weight;
    int value;

    void set(int x, int y) {
        weight = x;
        value = y;

    }
};

vector<object> knapsack(int products_count, int capacity, object object_list[5], vector<vector<int>> &res) {
    vector<object> tmp;
    for (int i = 1; i <= products_count; i++) {  //i个物品
        for (int j = 1; j <= capacity; j++) {  //背包容量为j
            if (object_list[i].weight > j) {  //当前背包的容量 j 放不下第 i 件商品时
                res[i][j] = res[i - 1][j];
            } else {
                if (res[i - 1][j - object_list[i].weight] + object_list[i].value > res[i - 1][j]) {
                    res[i][j] = res[i - 1][j - object_list[i].weight] + object_list[i].value;
                    int flag = 0;
                    for (int k = 0; k < tmp.size(); k++) {
                        if (tmp[k].weight == object_list[i].weight & tmp[k].value == object_list[i].value) {
                            flag = 1;
                        }
                    }
                    if (flag == 0) {
                        tmp.push_back(object_list[i]);
                    }
                } else {
                    res[i][j] = res[i - 1][j];
                }
            }
        }
    }
    return tmp;
}

vector<object>
knapsack2(int products_count, int capacity, object object_list[5], vector<vector<int>> &res, int &max_m) {
    vector<object> tmp;
    max_m = 0;
    //初始化第一行
    //仅考虑容量为C的背包放第0个物品的情况
    for (int i = 0; i <= capacity; i++) {
        res[0][i] = object_list[0].weight <= i ? object_list[0].value : 0;
    }
    for (int i = 1; i < products_count; i++) {  //i个物品
        for (int j = 0; j <= capacity; j++) {  //背包容量为j
            if (object_list[i].weight > j) {  //当前背包的容量 j 放不下第 i 件商品时
                res[i][j] = res[i - 1][j];
            } else {
                if (res[i - 1][j - object_list[i].weight] + object_list[i].value > res[i - 1][j]) {
                    res[i][j] = res[i - 1][j - object_list[i].weight] + object_list[i].value;
                } else {
                    res[i][j] = res[i - 1][j];
                }
            }
        }
    }
    for (int i = 0; i < products_count; i++) {
        for (int j = 0; j <= capacity; j++) {
            cout << res[i][j] << "  ";
        }
        cout << endl;
    }
    // get final objects list in the bag
    int j = capacity;
    for (int i = products_count - 1; i > 0; i--) {
        if (res[i][j] != res[i - 1][j]) {
            tmp.push_back(object_list[i]);
            max_m += object_list[i].value;
            j = j - object_list[i].weight;
        }
    }
    if (res[0][j] != 0) {
        tmp.push_back(object_list[0]);
        max_m += object_list[0].value;
    }
    return tmp;
}

int main() {
    int products_count = 5, capacity = 10;
    int result_m = 0;
    object ob1{};
    object ob2{};
    object ob3{};
    object ob4{};
    object ob5{};
    ob1.set(9, 10);
    ob2.set(2, 2);
    ob3.set(1, 4);
    ob4.set(8, 200);
    ob5.set(1, 2);
    object object_list[5] = {0};
    object_list[0] = ob1;
    object_list[1] = ob2;
    object_list[2] = ob3;
    object_list[3] = ob4;
    object_list[4] = ob5;
    vector<vector<int>> res(products_count, vector<int>(capacity + 1));
    vector<object> tmp = knapsack2(products_count, capacity, object_list, res, result_m);
    for (int i = 0; i < tmp.size(); i++) {
        cout << tmp[i].value << endl;
    }
    cout << "result_m： " << result_m << endl;
    return 0;
}
