import random

SERVICE_NUM = [10]
SPNUM = [10]
BSNUM = [30]
PRICE_SP = 10
GRIDX = 100
GRIDY = 100
BAN = 100000
RES_BS = 30
PRICE_BS = 10
COMPUTATIONPOWER = 10
UENUM = [30]
RES_UE = 10
RES_RADIO = 0
WORKLOAD = 10

def createRandomSortedList(num, start, end):
    arr = []
    tmp = random.randint(start, end)
    for x in range(num):  
        while tmp in arr:
            tmp = random.randint(start, end)  
        arr.append(tmp)
    arr.sort() 
    return arr

random.seed(10)
file_num = 1
for i in range(file_num):
    with open('input'+str(i)+'.txt', 'w') as f:
        f.write(str(SERVICE_NUM[i]) + '\n')  
        f.write('\n')
        
        f.write(str(SPNUM[i]) + '\n')
        for j in range(SPNUM[i]):
            tmp = str(j)
            price_sp = random.randint(1,PRICE_SP)
            for _ in range(SERVICE_NUM[i]):
                tmp += ' '
                tmp += str(price_sp)
            f.write(tmp + '\n')
        f.write('\n')
        
        f.write(str(BSNUM[i]) + '\n') 
        for j in range(BSNUM[i]):
            bs_service_num= random.randint(1,SERVICE_NUM[i])
            f.write(str(random.randint(0,GRIDX-1)) + ' ' + str(random.randint(0,GRIDY-1)) + ' ' + str(BAN) + ' ' + str(bs_service_num) + ' ' + str(random.randint(0,SPNUM[i]-1)) + '\n')
            sp_id = createRandomSortedList(bs_service_num, 0, SERVICE_NUM[i]-1)
            for k in range(bs_service_num):
                f.write(str(sp_id[k]) + ' ' + str(random.randint(1,RES_BS)) + ' ' + str(random.randint(1,PRICE_BS)) + ' ' + str(random.randint(1,COMPUTATIONPOWER)) + '\n')
        f.write('\n')    
         
        f.write(str(UENUM[i]) + '\n') 
        x = createRandomSortedList(UENUM[i], 0, GRIDX-1)
        for j in range(UENUM[i]):
            f.write(str(x[j]) + ' ' + str(random.randint(0,GRIDY-1)) + ' ' + str(random.randint(0,SPNUM[i]-1)) + ' ' + str(random.randint(0,SERVICE_NUM[i]-1)) + ' ' + str(random.randint(1,WORKLOAD)) + ' ' + str(random.randint(1,RES_UE))+ ' ' + str(RES_RADIO) + '\n')
        f.write('\n')
        
