import gurobipy as gp
import time
from gurobipy import GRB


def solve(valores, pesos, capacidade, conflitos):
        # Número de itens
        n = len(valores)

        # Criação do modelo
        modelo = gp.Model("Problema da Mochila com Conflitos")

        # Criação das variáveis de decisão: x[i] = 1 se o item i é selecionado, 0 caso contrário
        x = modelo.addVars(n, vtype=GRB.BINARY, name="x")

        # Definição da função objetivo: maximizar o valor total dos itens selecionados
        modelo.setObjective(gp.quicksum(valores[i] * x[i] for i in range(n)), GRB.MAXIMIZE)

        # Adicionando a restrição de capacidade: a soma dos pesos dos itens selecionados deve ser menor ou igual à capacidade da mochila
        modelo.addConstr(gp.quicksum(pesos[i] * x[i] for i in range(n)) <= capacidade, "Capacidade")

        # Adicionando as restrições de conflito: não permitir que itens em conflito sejam selecionados juntos
        for (i, j) in conflitos:
                modelo.addConstr(x[i] + x[j] <= 1, f"Conflito_{i}_{j}")

        # Otimizando o modelo
        modelo.optimize()

        # Imprimindo a solução
        if modelo.status == GRB.OPTIMAL:
                itens_selecionados = [i for i in range(n) if x[i].X > 0.5]
                valor_total = sum(valores[i] for i in itens_selecionados)
                return valor_total, itens_selecionados
        else:
                print("Nenhuma solução ótima encontrada")

def read_file(name):
    try:
        with open(name, 'r') as myfile:
            data = myfile.read().splitlines()
        
        # Leitura dos valores nI, nP, C
        nI, nP, C = map(int, data[0].split())
                
        # Leitura dos valores dos itens
        values = list(map(int, data[1].split()))
        
        # Leitura dos pesos dos itens
        weights = list(map(int, data[2].split()))
        
        # Leitura das restrições de conflitos
        conflitos = []
        for i in range(3, 3 + nP):
            a, b = map(int, data[i].split())
        conflitos.append((a,b))
        
        return values, weights, C, conflitos
    except FileNotFoundError:
        print(f"Error: Unable to open file {name}")
        return None

isinstancias = ["500", "700", "800", "1000"]

for inst in isinstancias:
        for j in range(1, 11):
                valores, pesos, capacidade, conflitos = read_file(f"dckp_instances/{inst}/dckp_{j}_sum.txt")
                begin = time.time()
                valor_total, intens_selecionados = solve(valores, pesos, capacidade, conflitos)
                end = time.time()

                with open(f"results/Optimal/{inst}/dckp_{j}_sum.txt", 'w') as myfile:
                        myfile.write(f"optimal solution: {valor_total}\n")
                        myfile.write(f"time taken to calculate: {end - begin}\n")
                        for i in intens_selecionados: 
                                myfile.write(f"{i} ")
                        myfile.write("\n")

