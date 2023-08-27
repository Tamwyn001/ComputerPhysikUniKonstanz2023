#import numpy as np

#### USED TO COMPUTED AT WICH (TIME, ALPHA) THE FTCS ALGO IS THE BEST, spoiler: at no points #####
data_nicolson = []
data_FCTS = []


#opens nicolson datas
with open('C:\\Users\\Tamwyn\\Documents\\Physik\\ComputerPhysikUniKonstanz2023\\Blatt_6\\Results\\Nicolson_Precision.dat', 'r') as f1:
    d = f1.readlines()
    #print(d)
    for i in d:
        k = i.rstrip().split(" ")
        data_nicolson.append(k[:])
#opens FTCS datas
with open('C:\\Users\\Tamwyn\\Documents\\Physik\\ComputerPhysikUniKonstanz2023\\Blatt_6\\Results\\FTCS_Precision.dat', 'r') as f2:
    d = f2.readlines()
    for i in d:
        k = i.rstrip().split(" ")
        data_FCTS.append(k[:])

print("l(FTCS) = ", len(data_FCTS), "l(Nicolson) = ", len(data_nicolson))
to_write : str = ''
for i in range(len(data_FCTS)):
    
    #exlucde undifined datas, line breaks, ...
    if (data_FCTS[i] != (['']) and (data_nicolson[i] != [''])):
        if((float(data_FCTS[i][2] != ('-nan(ind)' or 'nan(ind)')))):

            #a better value is a value wich absolute value smaller than the other is. The difference to the theorie is smaller.
            if abs(float(data_FCTS[i][2]))<abs(float(data_nicolson[i][2])):
                to_write += str(data_FCTS[i][0])+" "+str(data_FCTS[i][1])+ "\n"

#output in a file and close
f_out = open('C:\\Users\\Tamwyn\\Documents\\Physik\\ComputerPhysikUniKonstanz2023\\Blatt_6\\Results\\Best_Precision.dat', 'w')
f_out.write(to_write)
f_out.close()

