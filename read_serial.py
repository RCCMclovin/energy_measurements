#imports necessarios para o codigo
import serial
import io

#Inicializacao do Serial
ser = serial.Serial('COM5', 9600)
try:    
    ser.close()
except:
    pass
ser.open()

#Definicao dos arquivos de saida
path = "./logs/Arduino_Idle/"
"""
arqs= [ "05.csv","06.csv",
        "07.csv","08.csv",
        "09.csv","10.csv",
        "11.csv","12.csv",
        "13.csv","14.csv",
        "15.csv","16.csv",
        "17.csv","18.csv",
        "19.csv","20.csv"]
#"""
arqs = ["idle.csv"]
#Cabeçalho do csv
header= "Bus Voltage,Power,Current"


#Loop para os experimentos
i = 0
while i < len(arqs):
    #Iniciando a leitura e esperando iniciar os testes
    s=str(ser.readline())[2:].replace("\'","")\
              .replace("\\r","").replace("\\n","")
    print(s)
    if("Start test:" in s):
        #Teste iniciado
        s=str(ser.readline())[2:].replace("\'","")\
              .replace("\\r","").replace("\\n","")
        print(s)
        while not ("Time" in s):
            try:
                #Abrindo o Arquivo
                f = open(path+arqs[i],'r')
                f.close()
            except:
                #Arquivo não existe
                f = open(path+arqs[i],'w')
                f.write(header+'\n')
                f.close()
            finally:
                f = open(path+arqs[i],'a')
            #Escrevendo leitura
            f.write(s+'\n')
            f.close()
            s=str(ser.readline())[2:].replace("\'","")\
              .replace("\\r","").replace("\\n","")
            print(s)
            #Renovando a Leitura
        if "Time" in s:
            timefile = (arqs[i].split('.')[0])+'_time.txt'
            f = open(path+timefile,'w')
            f.write(s+'\n')
            f.close()
        i+=1
        

#Fechamento do serial
ser.close()
