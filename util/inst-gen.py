from random import random

file = input("Nome do arquivo: ")
size = input("Tamanho da instancia: ")
step = input("Quantidade de passos: ")

with open('examples/' + str(file), 'w') as inst:
    inst.write(str(size) + " " + str(step) + "\n")
    
    for i in range(0, int(size)):
        line = ""
        
        for j in range(0, int(size)):
            if random() <= 0.5:
                line += " "
            else:
                line += "x"
                
        inst.write(line + "\n")