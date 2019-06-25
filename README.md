# Gerência de Memória: Simulador

Em Sistemas Operacionais, no tópico gerência de memória, o assunto memória virtual é importante para se compreender a técnica de uso de espaço em disco em prol da memória física, normalmente conhecida como RAM. Essa técnica diminui a limitação do espaço de memória física, uma vez que permite executar processos maiores que ela própria.

Um detalhe dessa técnica é a necessidade de implementação de políticas de tratamento de falhas, tendo em mente um sistema que usa paginação, pois, uma vez que a página requerida pelo processo não se encontra na memória principal, é preciso buscá-la na memória secundária. Diversos algoritmos conseguem determinar quais páginas devem ser substituídas, cada qual com níveis de performance diferentes dependendo das características do sistema.

Esta implementação serve para comparar a performance de dois algoritmos de substituição de páginas: Segunda Chance e Least-Recently Used (LRU) através de uma simulação.

Para realizar os testes, é necessário realizar o download ou clone do repositório, e então instalar com:
```
./configure
make
```
Após instalado, pode-se executar seguindo a seguinte sintaxe:
```
./mmu nome.trace -modo d
```
sendo os argumentos:
- nome do arquivo de trace;
- politica de substituição, podendo ser -sc (Segunda Chance) ou -lru (LRU);
- d: Detalha os enderecos lidos (opcional);

Alternativamente, o script `salva_resultado.sh` executa todos os arquivos de trace e salva os resultados em arquivos separados no diretório superior.
