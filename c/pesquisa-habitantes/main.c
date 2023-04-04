#ifdef __linux__
#define __clear_system_call__ "clear"
#define __system_call_pause__ "read -p \"Pressione enter para sair...\" saindo"

#elif _WIN32
#define __clear_system_call__ "cls"
#define __system_call_pause__ "PAUSE"
#endif

#include <stdio.h>
#include <stdlib.h>

typedef enum sexo
{
    MASCULINO = 'm',
    FEMININO = 'f',
} sexo; // Para poder escrever MASCULINO em vez de 'm' ou FEMININO em vez de 'f'

typedef struct habitante
{
    int idade;
    float salario;
    char sexo;
} habitante; // Para poder guardar informacoes dos habitantes em variaveis com tipo "habitante"

typedef struct pesquisa
{
    float media_salarial;
    int quantidade_mulheres;
    habitante habitante_mais_novo;
    habitante habitante_mais_velho;
    habitante habitante_menor_salario;

    float salario_total;
    size_t total_habitantes;
} pesquisa; // Para poder guardar informacoes da pesquisa em variaveis com tipo "pesquisa"

habitante criar_habitante()
{
    int idade;
    float salario;
    char sexo;

    printf("Digite a idade, ou um numero negativo para finalizar a pesquisa: \n> ");
    scanf("%d", &idade);
    if (idade < 0)                         // finalizar pesquisa, retornar objeto habitante com idade -1 (para saber nas outras funcoes q e para parar o programa.)
        return (habitante){-1, 0.0, '\0'}; // (habitante){idade(-1), salario(0.0), sexo(caractere nulo)}

    printf("Digite o salario: \n> ");
    while (1)
    {
        scanf("%f", &salario);
        if (salario >= 0.0)
            break;

        printf("O salario nao pode ser negativo! escreva novamente:\n> ");
    }

    printf("Digite o sexo (%c/%c): \n> ", MASCULINO, FEMININO);
    while (1)
    {
        scanf(" %c", &sexo);

        char c;                                     // limpar caracteres extras escritos.
        while ((c = getchar()) != EOF && c != '\n') // limpar caracteres extras escritos.
            ;                                       // limpar caracteres extras escritos.

        if (sexo == MASCULINO || sexo == FEMININO)
            break;

        printf("Caractere invalido, digite '%c' ou '%c'\n> ", MASCULINO, FEMININO);
    }

    return (habitante){idade, salario, sexo};
}

void atualizar_pesquisa(pesquisa *pesquisa_alvo, habitante novo_habitante)
{
    pesquisa pesquisa_atualizada = *pesquisa_alvo;

    if (novo_habitante.idade < 0)
        return; // pesquisa invalida

    pesquisa_atualizada.total_habitantes++;

    pesquisa_atualizada.salario_total += novo_habitante.salario; // atualizar salario total

    if (pesquisa_atualizada.total_habitantes == 0 || pesquisa_atualizada.habitante_mais_novo.idade > novo_habitante.idade) // atualizar hab mais novo
        pesquisa_atualizada.habitante_mais_novo = novo_habitante;

    if (pesquisa_atualizada.total_habitantes == 0 || pesquisa_atualizada.habitante_mais_velho.idade < novo_habitante.idade) // atualizar hab mais velho
        pesquisa_atualizada.habitante_mais_velho = novo_habitante;

    if (pesquisa_atualizada.total_habitantes == 0 || pesquisa_atualizada.habitante_menor_salario.salario > novo_habitante.salario) // atualizar hab menor salario
        pesquisa_atualizada.habitante_menor_salario = novo_habitante;

    if (pesquisa_atualizada.total_habitantes != 0)
        pesquisa_atualizada.media_salarial = pesquisa_atualizada.salario_total / pesquisa_atualizada.total_habitantes;

    if (novo_habitante.sexo == FEMININO) // atualizar n mulheres
        pesquisa_atualizada.quantidade_mulheres++;

    *pesquisa_alvo = pesquisa_atualizada;
}
pesquisa criar_pesquisa()
{
    pesquisa nova_pesquisa;
    // Da um valor padrao para todos atributos (É necessario para evitar bugs)
    nova_pesquisa.quantidade_mulheres = 0;
    nova_pesquisa.media_salarial = 0.0;
    nova_pesquisa.total_habitantes = 0;
    nova_pesquisa.habitante_mais_novo = (habitante){0, 0, 0};
    nova_pesquisa.habitante_mais_velho = (habitante){0, 0, 0};
    nova_pesquisa.habitante_menor_salario = (habitante){0, 0, 0};
    return nova_pesquisa;
}
pesquisa realizar_pesquisa()
{
    pesquisa nova_pesquisa = criar_pesquisa();

    while (1) // loop ate dar break;
    {
        // Criei um macro la em cima para substituir o __clear_system_call__ pelo "clear" do linux ou "cls" no windows
        system(__clear_system_call__);

        if (nova_pesquisa.total_habitantes > 0)
            printf("Total habitantes entrevistados: %ld\n", nova_pesquisa.total_habitantes); // Mostrar esse texto apenas quando tiver mais que um habitante

        habitante novo_habitante = criar_habitante();

        if (novo_habitante.idade < 0)                                   // aq vem a importancia de retornar a idade como -1 la na funcao criar_habitante
        {                                                               // dessa forma, da para eu saber q o novo_habitante nao é um valor valido
            printf("Idade negativa digitada, finalizando pesquisa.\n"); // impedindo de contar ele na pesquisa
            break;
        }

        atualizar_pesquisa(&nova_pesquisa, novo_habitante); // ADICIONAR OS VALORES DO NOVO HABITANTE A PESQUISA
    }

    return nova_pesquisa;
}

void exibir_resultados_pesquisa(pesquisa resultado_pesquisa)
{
    // Criei um macro la em cima para substituir o __clear_system_call__ pelo "clear" do linux ou "cls" no windows
    // precisei fazer isso pq to compilando o programa no linux, nem se preocupa com isso se vc so for usar windows
    system(__clear_system_call__);

    printf("-- RESULTADOS PESQUISA -- \n");
    if (resultado_pesquisa.total_habitantes <= 0)
    {
        printf("Nenhum resultado foi inserido na pesquisa...\n");
        return;
    }

    printf("A) QUANTIDADE MULHERES -> %d\n", resultado_pesquisa.quantidade_mulheres);
    printf("B) MEDIA SALARIAL -> %0.2f\n", resultado_pesquisa.media_salarial);
    printf("C.1) MENOR IDADE -> %d\n", resultado_pesquisa.habitante_mais_novo.idade);
    printf("C.2) MAIOR IDADE -> %d\n", resultado_pesquisa.habitante_mais_velho.idade);
    printf("D) IDADE E SEXO HABITANTE MENOR SALARIO -> %d(idade) %c(sexo)\n", resultado_pesquisa.habitante_menor_salario.idade, resultado_pesquisa.habitante_menor_salario.sexo);
}

int main(void)
{
    /*
    Foi feita uma pesquisa entre os habitantes de uma região. Foram coletados os dados de idade,
    sexo (M/F) e salário. Faça um programa que calcule e mostre:

    a) A média dos salários do grupo;
    b) A maior e a menor idade do grupo;
    c) A quantidade de mulheres na região;
    d) A idade e o sexo da pessoa que possui o menor salário;

    Finalize a entrada de dados ao ser digitada uma idade negativa.
    */

    pesquisa resultado_pesquisa = realizar_pesquisa();
    exibir_resultados_pesquisa(resultado_pesquisa);
    system(__system_call_pause__);

    return 0;
}