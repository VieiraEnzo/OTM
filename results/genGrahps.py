import matplotlib.pyplot as plt
import numpy as np

import matplotlib.pyplot as plt
import numpy as np

# Definindo os algoritmos
algorithms = ['Metaheurística 1', 'Metaheurística 2', 'Metaheurística 3']

# Definindo os dados das melhores soluções para diferentes instâncias
# Cada sublista representa os resultados das três metaheurísticas para uma instância específica
best_solutions_per_instance = [
    [2257, 2230, 2270],  # Instância 1
    [2265, 2245, 2260],  # Instância 2
    [2280, 2250, 2290],  # Instância 3
]

# Função para criar gráficos de barras múltiplos
def create_multiple_bar_charts(data, title_prefix, ylabel, algorithms):
    num_instances = len(data)
    x = np.arange(len(algorithms))  # posição das barras
    bar_width = 0.2  # largura das barras

    fig, axs = plt.subplots(1, num_instances, figsize=(15, 5), sharey=True)

    for i in range(num_instances):
        axs[i].bar(x, data[i], bar_width, color=['blue', 'green', 'red'])
        axs[i].set_xticks(x)
        axs[i].set_xticklabels(algorithms)
        axs[i].set_title(f'{title_prefix} Instância {i+1}')
        axs[i].set_ylabel(ylabel)

    plt.tight_layout()
    plt.show()

# Criar gráficos de barras múltiplos
create_multiple_bar_charts(best_solutions_per_instance, 'Melhor Solução', 'Melhor Solução', algorithms)
