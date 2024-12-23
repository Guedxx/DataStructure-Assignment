# Sistema de Gerenciamento de Imóveis do Rio de Janeiro

Este projeto faz parte do **Trabalho Computacional de Estruturas de Dados e Seus Algoritmos**. Ele consiste no desenvolvimento de um sistema de gerenciamento de imóveis usando uma **Árvore B** ou **B+** em memória secundária.

## Descrição do Projeto

O sistema permitirá o gerenciamento de um catálogo de imóveis do Rio de Janeiro, suportando operações de inserção, consulta, modificação e exclusão de imóveis, com os dados armazenados de forma eficiente usando estruturas de árvore. 

Os dados do catálogo serão manipulados com base em arquivos armazenados em disco, onde:
- Cada **nó folha** da árvore B será representado por um arquivo.
- Os **nós internos** podem estar agrupados em um único arquivo.
- Somente os arquivos necessários para a operação em andamento serão carregados na memória principal.

## Parâmetros de Entrada

O programa receberá os seguintes parâmetros como entrada:

- **Grau mínimo da árvore B (t)**: Determina o número mínimo de chaves em cada nó.
- **Catálogo inicial**: Arquivo contendo os dados iniciais no seguinte formato (campos separados por `;`):
  - `id único`
  - `bairro do imóvel`
  - `tipo de imóvel`
  - `rua` (pode estar vazia)
  - `número do imóvel` (pode estar vazio)
  - `preço total`
  - `preço por metro quadrado`
  - `descrição`
  - `CEP`
  - `latitude`
  - `longitude`

O catálogo inicial será armazenado utilizando uma Árvore B ou B+ em memória secundária.

## Funcionalidades Implementadas

O sistema oferecerá suporte às seguintes operações:

1. **Retorno do imóvel à venda mais barato e mais caro**.
2. **Retorno do imóvel alugado mais barato e mais caro**.
3. **Consulta de todos os imóveis**, dado um CEP (e latitude e longitude).
4. **Consulta de todos os imóveis de uma mesma rua**.
5. **Consulta de todos os imóveis de um mesmo bairro**.
6. **Consulta de todos os imóveis dado um tipo**.
7. **Remoção de todos os imóveis**, dado um tipo (e CEP, nome de rua, latitude, longitude e bairro).
8. **Consulta de informações adicionais do imóvel**, com base em sua descrição.
9. **Consulta ou remoção de imóveis** com base em uma faixa de metragem.
10. **Consulta de informações adicionais**, como vaga, salão de festas, valor do condomínio, entre outras.
11. **Inserção de imóveis** no catálogo.
12. **Alteração de algumas informações adicionais**, como:
    - Preço total
    - Preço por metro quadrado
    - Descrição
13. **Remoção de todos os imóveis em uma determinada faixa de preço**.

## Requisitos Técnicos

- Implementação utilizando uma Árvore B ou B+ em memória secundária.
- Manipulação eficiente de arquivos, mantendo apenas os dados necessários na memória durante as operações.

### Grupo de Desenvolvimento

Este projeto foi desenvolvido em equipe como parte do curso de **Estruturas de Dados e Seus Algoritmos** na Universidade Federal Fluminese.

