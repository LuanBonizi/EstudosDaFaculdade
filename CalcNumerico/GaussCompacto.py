# Função para ler a matriz
def input_matriz(n):
    A = [] # Declara a matriz
    for i in range(n):
        linha = []
        for j in range(n): # Lê os valores de uma linha
            linha.append(float(input(f"Digite o valor de A[{i}][{j}]: ")))
        A.append(linha) # Coloca a linha na matriz
    return A # Retorna a matriz para o local de chamada

# Função para ler o vetor
def input_vetor(n):
    # Faz a leitura do vetor para n valores
    v = [float(input(f"Digite o valor de b[{i}]: ")) for i in range(n)]
    # Retorna o vetor para o local de chamada
    return v

def print_sistema(A, b): 
    n = len(A)
    for i in range(n):
        for j in range(n):
            print(f"{A[i][j]:.2f}", end = " ")
        print(f"| {b[i]}")

# Função para realizar o pivoteamento
def pivoteamento(A, v, k):
    n = len(A) # Tamanho da matriz A
    mx = k

    for i in range(k + 1, n):
        if abs(A[i][k]) > abs(A[mx][k]):
            mx = i

    # Realiza a troca dos valores caso mx > k
    if mx > k:
        v[k], v[mx] = v[mx], v[k]
        A[k], A[mx] = A[mx], A[k]

# Realiza o método de fatoração LU
def fatoracao_lu(A):
    n = len(A) # Tamanho da matriz
    v = list(range(n)) # Inicializa o vetor
    ut = 1 # Inicializa o tal

    # Laços paras as k etapas
    for k in range(n):
        # Faz o pivoteamento
        pivoteamento(A, v, k)

        # Realiza a eliminação dos valores
        for i in range(k + 1, n):
            mult = A[i][k] / A[k][k]
            for j in range(k + 1, n):
                A[i][j] -= mult * A[k][j]
            A[i][k] = mult # Guarda o valor do multiplicador

    return A, v, ut # Retorna a matriz, vetor de permutações e o tal

# Resolve o sistema Ly = b
def resolve_Ly_b(L, y, b, v):
    # Tamanho de L
    n = len(L) 
    # Descobre o primeiro elemento
    y[0] = b[v[0]] 
    # Faz a substituição dos valores
    for i in range(1, n): 
        soma = sum(y[j] * L[i][j] for j in range(i))
        y[i] = b[v[i]] - soma
    # Retorna o resultado
    return y

# Resolve o sistema Ux = y
def resolve_Ux_y(U, x, y):
    # Tamanho de U
    n = len(U) 
    # Descobre o primeiro elemento
    x[n - 1] = y[n - 1] / U[n - 1][n - 1] 
    # Faz a substituição dos valores
    for i in range(n - 2, -1, -1): 
        soma = sum(x[j] * U[i][j] for j in range(i + 1, n))
        x[i] = (y[i] - soma) / U[i][i]
    # Retorna o resultado
    return x

# Função principal do programa
def main():
    # Entrada dos valores
    n = int(input("Digite a dimensão n da matriz A: "))
    A = input_matriz(n)
    b = input_vetor(n)

    # Imprime o sistema
    print("======== Sistema Linear ========")
    print_sistema(A, b)

    # Realiza o método
    A, v, ut = fatoracao_lu(A)

    # Resolve o sistema  Ly = b
    y = [0.0] * n
    y = resolve_Ly_b(A, y, b, v)

    # Resolve o sistema Ux = y
    x = [0.0] * n
    x = resolve_Ux_y(A, x, y)

    # Imprime o sistema fatorado
    print("======== Sistema Fatorado ========")
    print_sistema(A, b)

    # Imprime os valores obtidos
    print("======== Valores Obtidos ========")
    for i in range(n):
        print(f"x[{i}] = {x[i]:.2f}")

if __name__ == "__main__":
    main()