from random import random

file = raw_input("Nome do arquivo: ")
size = input("Tamanho da instancia: ")
step = input("Quantidade de passos: ")

with open('examples/' + str(file), 'w') as inst:
    inst.write(str(size) + " " + str(step) + "\n")
    data = ""
    
    for i in range(0, int(size)):
        for j in range(0, int(size)):
            if random() <= 0.5:
                data += " "
            else:
                data += "x"
                
        data += "\n"
        
    inst.write(data)