import random
import tqdm

SERVICE_NUM = 30
SPNUM = 5
BSNUM = 25
PRICE_SP = 10
GRIDX = 1200
GRIDY = 1200
BAN = 100000
RES_BS = 30
PRICE_BS = 10
COMPUTATIONPOWER = 10
UENUM = [400,500,600,700,800,900]
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

def createRandomList(num, start, end):
    arr = []
    tmp = random.randint(start, end)
    for x in range(num):  
        while tmp in arr:
            tmp = random.randint(start, end)  
        arr.append(tmp)
    return arr
spid_bs = createRandomList(BSNUM, 0, BSNUM-1) # sp belongs to bs

bs_position = []
for i in range(5):
    for j in range(5):
        bs_position.append((i*300, j*300))
random.shuffle(bs_position)       

random.seed(10)
for i in range(len(UENUM)):
    with open('input_paper_order' + str(i) + '.txt', 'w') as f:
        f.write(str(SERVICE_NUM) + '\n')  
        f.write('\n')
        
        f.write(str(SPNUM) + '\n')
        for j in range(SPNUM):
            tmp = str(j)
            price_sp = random.randint(1,PRICE_SP)
            for _ in range(SERVICE_NUM):
                tmp += ' '
                tmp += str(price_sp)
            f.write(tmp + '\n')
        f.write('\n')
        
        f.write(str(BSNUM) + '\n') 
        spid = 0
        for j in range(BSNUM):
            f.write(str(bs_position[j][0]) + ' ' + str(bs_position[j][1]) + ' ' + str(BAN) + ' ' + str(6) + ' ' + str(spid_bs.index(j)//5) + '\n')
            sp_id = createRandomSortedList(6, 0, SERVICE_NUM-1)
            for k in range(6):
                f.write(str(sp_id[k]) + ' ' + str(random.randint(1,RES_BS)) + ' ' + str(random.randint(1,PRICE_BS)) + ' ' + str(random.randint(1,COMPUTATIONPOWER)) + '\n')
        f.write('\n')    

        f.write(str(UENUM[i]) + '\n')   
        x = createRandomSortedList(UENUM[i], 0, GRIDX-1)
        for j in range(UENUM[i]):
            f.write(str(x[j]) + ' ' + str(random.randint(0,GRIDY-1)) + ' ' + str(random.randint(0,SPNUM-1)) + ' ' + str(random.randint(0,SERVICE_NUM-1)) + ' ' + str(random.randint(1,WORKLOAD)) + ' ' + str(random.randint(1,RES_UE))+ ' ' + str(RES_RADIO) + '\n')
        f.write('\n')
        
