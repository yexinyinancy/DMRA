import random

SERVICE_NUM = 10
SPNUM = 10
BSNUM = 30
PRICE_SP = 10
GRIDX = 100
GRIDY = 100
BAN_MIN = 10
BAN_MAX = 20
RES_BS = 30
PRICE_BS = 10
COMPUTATIONPOWER = 10
UENUM = 30
RES_UE = 10
RES_RADIO = 10
WORKLOAD = 10

random.seed(10)
file_num = 1
for i in range(file_num):
    with open('input'+str(i)+'.txt', 'w') as f:
        f.write(str(random.randint(0,SERVICE_NUM-1)) + '\n')  
        f.write('\n')
        
        spnum = random.randint(1,SPNUM)
        f.write(str(spnum) + '\n') 
        for i in range(spnum):
            f.write(str(i) + ' ' + str(random.randint(1,PRICE_SP)) + '\n')
        f.write('\n')
        
        bsnum = random.randint(1,BSNUM)
        f.write(str(bsnum) + '\n') 
        for i in range(bsnum):
            bs_service_num= random.randint(1,SERVICE_NUM)
            f.write(str(random.randint(0,GRIDX-1)) + ' ' + str(random.randint(0,GRIDY-1)) + ' ' + str(random.randint(BAN_MIN,BAN_MAX)) + ' ' + str(bs_service_num) + ' ' + str(random.randint(0,SPNUM-1)) + '\n')
            for j in range(bs_service_num):
                f.write(str(random.randint(0,SERVICE_NUM-1)) + ' ' + str(random.randint(1,RES_BS)) + ' ' + str(random.randint(1,PRICE_BS)) + ' ' + str(random.randint(1,COMPUTATIONPOWER)) + '\n')
        f.write('\n')    
         
        uenum = random.randint(1,UENUM)
        f.write(str(uenum) + '\n') 
        for i in range(uenum):
            f.write(str(random.randint(0,GRIDX-1)) + ' ' + str(random.randint(0,GRIDY-1)) + ' ' + str(random.randint(0,SPNUM-1)) + ' ' + str(random.randint(0,SERVICE_NUM-1)) + ' ' + str(random.randint(1,WORKLOAD)) + ' ' + str(random.randint(1,RES_UE))+ ' ' + str(random.randint(1,RES_RADIO)) + '\n')
        f.write('\n')
        
    