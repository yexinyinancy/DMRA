import random
import tqdm

SERVICE_NUM = 10
SPNUM = 5
BSNUM = 25
PRICE_SP = 20
GRIDX = 1200
GRIDY = 1200
BAN = 100000
RES_BS_MIN = 100
RES_BS_MAX = 150
PRICE_BS = 10
COMPUTATIONPOWER = 10
# UENUM = [400,500,600,700,800,900]
UENUM = [400, 500, 600, 700, 800, 900, 1000, 1500, 2000, 3000, 4000, 5000, 10000, 20000, 50000, 100000, 200000, 300000, 500000]
RES_UE_MIN = 3
RES_UE_MAX = 5
RES_RADIO = 0
WORKLOAD = 100



def createRandomSortedList(num, start, end):
    arr = []
    tmp = random.randint(start, end)
    for x in range(num):  
        while tmp in arr:
            tmp = random.randint(start, end)  
        arr.append(tmp)
    arr.sort() 
    return arr

def createRandomList(num, start, end):
    arr = []
    tmp = random.randint(start, end)
    for x in range(num):  
        while tmp in arr:
            tmp = random.randint(start, end)  
        arr.append(tmp)
    return arr

def createRandomListDup(num, start, end):
    arr = []
    tmp = random.randint(start, end)
    for x in range(num):  
        tmp = random.randint(start, end)  
        arr.append(tmp)
    arr.sort() 
    return arr

spid_bs = createRandomList(BSNUM, 0, BSNUM-1)


for i in range(len(UENUM)):
    with open('./data2/input_paper_random_ue' + str(UENUM[i]) + '.txt', 'w') as f:
        f.write(str(SERVICE_NUM) + '\n')  
        f.write('\n')
        
        f.write(str(SPNUM) + '\n')
        for j in range(SPNUM):
            tmp = str(j)
            for _ in range(SERVICE_NUM):
                tmp += ' '
                tmp += str(PRICE_SP)
            f.write(tmp + '\n')
        f.write('\n')
        
        f.write(str(BSNUM) + '\n') 
        spid = 0
        random.seed(20)
        for j in range(BSNUM):
            f.write(str(random.randint(0,GRIDX-1)) + ' ' + str(random.randint(0,GRIDY-1)) + ' ' + str(BAN) + ' ' + str(6) + ' ' + str(spid_bs.index(j)//5) + '\n')
            sp_id = createRandomSortedList(6, 0, SERVICE_NUM-1)
            for k in range(6):
                f.write(str(sp_id[k]) + ' ' + str(random.randint(RES_BS_MIN,RES_BS_MAX)) + ' ' + str(random.randint(1,PRICE_BS)) + ' ' + str(random.randint(1,COMPUTATIONPOWER)) + '\n')
        f.write('\n')    

        f.write(str(UENUM[i]) + '\n')   
        # x = createRandomSortedList(UENUM[i], 0, GRIDX-1)
        x = createRandomListDup(UENUM[i], 0, GRIDX-1)
        for j in range(UENUM[i]):
            f.write(str(x[j]) + ' ' + str(random.randint(0,GRIDY-1)) + ' ' + str(random.randint(0,SPNUM-1)) + ' ' + str(random.randint(0,SERVICE_NUM-1)) + ' ' + str(random.randint(1,WORKLOAD)) + ' ' + str(random.randint(RES_UE_MIN,RES_UE_MAX))+ ' ' + str(RES_RADIO) + '\n')
        f.write('\n')
        
