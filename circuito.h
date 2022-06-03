#ifndef _CIRCUITO_H_
#define _CIRCUITO_H_

#include <iostream>
#include <string>
#include <vector>
#include "bool3S.h"

/// #################################################################################
/// ATENCAO PARA A CONVENCAO DOS NOMES E TIPOS PARA OS PARAMETROS DAS FUNCOES:
/// unsigned I: indice (de entrada de porta): de 0 a NInputs-1
/// int Id: identificador (de entrada, saida ou porta): de 1 a N ou de -1 a -N (nao 0)
///   int IdInput: de -1 a -NumEntradasCircuito
///   int IdOutput: de 1 a NumSaidasCircuito
///   int IdPort: de 1 a NumPortasCircuito
///   int IdOrig: de 1 a NumPortasCircuito ou de -1 a -NumEntradasCircuito
///               (id da origem de uma entrada de porta ou de uma saida do circuito)
/// #################################################################################

//
// A CLASSE PORT
//

#define NUM_MAX_INPUTS_PORT 4

class Port;
typedef Port *ptr_Port;

class Port {
protected:
  // O numero de entradas da porta
  unsigned Nin;
  // As ids das entradas da porta:
  // se id_in[i]>0: a i-esima entrada da porta vem da saida da porta cuja id eh o
  // valor desse elemento do array
  // se id_in[i]<0: a i-esima entrada da porta vem da entrada do circuito cuja id eh o
  // valor desse elemento do array
  // se id_in[i]==0: a i-esima entrada da porta estah indefinida
  int id_in[NUM_MAX_INPUTS_PORT];
  // O valor da saida da porta (?, F ou T)
  bool3S saida;
public:
  // Construtor (recebe como parametro o numero de entradas da porta)
  // Depois de checar o parametro, faz Nin <- NI e inicializa os elementos do
  // array id_in com valor invalido (0) e saida com UNDEF
  Port(unsigned NI=2);
  // Construtor por copia
  Port(const Port& P);
  // Destrutor virtual
  virtual ~Port();

  // Essa funcao virtual pura deve criar e retornar um ponteiro para Port que aponta para
  // uma copia de si mesma, do tipo correto.
  // Por exemplo, se for chamada com um objeto Port_AND, retorna um ponteiro que aponta para
  // uma area que contem uma Port_AND cujo valor eh uma copia de *this
  // Deve ser utilizada no construtor por copia da classe Circuito
  virtual ptr_Port clone() const = 0;

  // Funcao virtual pura que retorna a sigla correta da Port (AN, NT, OR, NX, etc.)
  // Deve ser utilizada para imprimir uma porta
  virtual std::string getName() const = 0;

  // Funcoes de testagem

  // Retorna true se um numero de entradas eh possivel para a porta, ou seja, se NI estah
  // entre 2 e NUM_MAX_INPUTS_PORT (na funcao correspondente na porta NOT, substituir por NI==1)
  virtual bool validNumInputs(unsigned NI) const;

  // Retorna true se um indice (qual entrada da porta) eh valido (entre 0 e Nin-1) para a porta
  bool validIndex(unsigned I) const;

  // Retorna true se a porta eh valida (estah com todos os dados corretos):
  // - validNumInputs eh true
  // - Todas as id de entrada da porta sao diferentes de zero.
  bool valid() const;


  // Funcoes de consulta

  // Caracteristicas da porta
  unsigned getNumInputs() const;
  bool3S getOutput() const;
  // Depois de testar o parametro (validIndex), retorna o valor de id_in[I] ou 0 se indice invalido
  int getId_in(unsigned I) const;

  // Funcoes de modificacao

  // Fixa o numero de entradas da porta
  // Depois de testar o parametro (validNumInputs), faz: Nin <- N
  void setNumInputs(unsigned N);
  // Fixa o valor logico da saida da porta (F, T ou ?)
  void setOutput(bool3S S);
  // Fixa a origem do sinal da I-esima entrada da porta como sendo Id
  // Depois de testar os parametros (validIndex, Id!=0), faz: id_in[I] <- Id
  void setId_in(unsigned I, int Id);
  // Leh uma porta do teclado. O usuario deve digitar:
  // - o numero de entradas da porta; e
  // - a id de cada uma das entradas da porta
  // Se o usuario digitar um dado invalido, o metodo deve pedir que ele digite novamente
  // Este metodo para digitar uma porta com numero variavel de entradas (entre 2 e
  // NUM_MAX_INPUTS_PORT) vale para todas as ports, exceto NOT.
  // O metodo virtual digitar tem que ser refeito para a NOT.
  // Nao precisa ser reimplementado nas demais ports
  // ATENCAO: o metodo NAO vai solicitar que o usuario digite o tipo de porta
  // (NT,AN,NA,OR,NO,XO,NX). Esse valor jah deve ter sido digitado previamente e a porta
  // criada dinamicamente (new) do tipo certo, para que seja chamado o metodo virtual
  // digitar apropriado para o tipo de porta.
  virtual void digitar();

  // Leh uma porta da stream ArqI. Deve ler:
  // - o numero de entradas da porta; e
  // - a id de cada uma das entradas da porta
  // Retorna true se tudo OK, false se houve erro
  // Este metodo nao eh virtual, pois ser feito generico de forma a servir para
  // todas as ports.
  // Basta que o metodo teste o numero de entradas com a funcao virtual validNumInputs()
  // ATENCAO: o metodo NAO vai ler do arquivo o tipo de porta
  // (NT,AN,NA,OR,NO,XO,NX). Esse valor jah deve ter sido lido previamente e a porta
  // criada dinamicamente do tipo certo, para que seja chamado o metodo virtual ler
  // apropriado para o tipo de porta.
  bool ler(std::istream& ArqI);

  // Imprime a porta na ostrem ArqO (cout ou uma stream de arquivo, tanto faz)
  // Imprime:
  // - a string com o nome da porta;
  // - o numero de entradas;
  // - um ": "; e
  // - as ids de cada uma das entradas
  // Este metodo nao eh virtual, pois pode ser feito generico de forma a servir para
  // todas as ports.
  // Basta que o metodo imprima o resultado da chamada aa funcao virtual getName() para o nome
  // Os outros dados a serem impressos sao iguais em todas as portas
  // Retorna a propria ostream O recebida como parametro de entrada, para que possa
  // ser encadeada
  std::ostream& imprimir(std::ostream& ArqO) const;

  // Simula uma porta logica
  // Recebe um array de bool3S com os valores logicos atuais das entradas da porta
  // Retorna um bool3S com o resultado (saida da porta), alem de armazenar o valor no
  // dado saida da porta
  // Se baseia nos operadores AND, OR, etc da classe bool3S
  virtual bool3S simular(const bool3S In[]) = 0;
};

// Operador << com comportamento polimorfico
// Serve para todas as ports (NO, AND, NOR, etc.)
std::ostream& operator<<(std::ostream& O, const Port& X);

//
// As outras PORTS
//

class Port_NOT: public Port {
public:
  Port_NOT();
  ptr_Port clone() const;
  std::string getName() const;

  bool validNumInputs(unsigned NI) const;

  // Leh uma porta NOT do teclado. O usuario deve digitar:
  // - a id da entrada da porta
  // (nao deve ser solicitado a digitar o numero de entradas, que eh sempre 1)
  // Se o usuario digitar um dado invalido, o metodo deve pedir que ele digite novamente
  void digitar();

  bool3S simular(const bool3S In[]);
};

class Port_AND: public Port {
public:
  Port_AND();

  ptr_Port clone() const;
  std::string getName() const;

  bool3S simular(const bool3S In[]);
};

class Port_NAND: public Port {
public:
  Port_NAND();

  ptr_Port clone() const;
  std::string getName() const;

  bool3S simular(const bool3S In[]);
};

class Port_OR: public Port {
public:
  Port_OR();

  ptr_Port clone() const;
  std::string getName() const;

  bool3S simular(const bool3S In[]);
};

class Port_NOR: public Port {
public:
  Port_NOR();

  ptr_Port clone() const;
  std::string getName() const;

  bool3S simular(const bool3S In[]);
};

class Port_XOR: public Port {
public:
  Port_XOR();

  ptr_Port clone() const;
  std::string getName() const;

  bool3S simular(const bool3S In[]);
};

class Port_NXOR: public Port {
public:
  Port_NXOR();

  ptr_Port clone() const;
  std::string getName() const;

  bool3S simular(const bool3S In[]);
};

//
// A CLASSE CIRCUIT
//

class Circuit {
private:
  // Nao precisa manter variaveis para guardar o numero de entradas, saidas e ports.
  // Essas informacoes estao armazenadas nos tamanhos (size) dos vetores correspondentes:
  // inputs, id_out e ports, respectivamente
  // Os metodos de consulta getNumInputs, getNumOutputs e getNumPorts dao acesso a essas
  // informacoes de maneira eficiente
  std::vector<bool3S> inputs;   // vetor que deve ser alocado com dimensao "Nin"
  std::vector<int> id_out;      // vetor que deve ser alocado com dimensao "Nout"
  std::vector<ptr_Port> ports;  // vetor que deve ser alocado com dimensao "Nports"
public:
  // As variaveis do tipo Circuit sao sempre criadas sem nenhum dado
  // A definicao do numero de entradas, saidas e ports eh feita ao ler do teclado ou arquivo
  // ou ao executar o metodo realloc
  Circuit();
  // Construtor por copia
  Circuit(const Circuit& C);
  // Destrutor: apenas chama a funcao clear()
  ~Circuit();
  // Limpa todo o conteudo do circuito
  // Utiliza o metodo STL clear para limpar os vetores inputs, id_out e ports
  // ATENCAO: antes de dar um clear no vetor ports, tem que liberar (delete) as areas
  // de memoria para as quais cada ponteiro desse vetor aponta.
  void clear();
  // Operador de atribuicao
  void operator=(const Circuit& C);
  // Redimensiona o circuito para passar a ter NI entradas, NO saidas e NP ports
  // Os vetores sao inicializados com valores iniciais neutros ou invalidos:
  // inputs[i] <- UNDEF
  // id_out[i] <- 0
  // ports[i] <- nullptr
  void resize(unsigned NI, unsigned NO, unsigned NP);

  // Funcoes de consulta

  // Caracteristicas do circuito
  // Retorna o tamanho (size) dos vetores correspondentes:
  // inputs, id_out e ports, respectivamente
  unsigned getNumInputs() const;
  unsigned getNumOutputs() const;
  unsigned getNumPorts() const;

  // Retorna true se IdInput eh uma id de entrada do circuito valida (entre -1 e -NInput)
  // Usada para testar parametros de funcoes e valores lidos do teclado ou arquivo
  bool validIdInput(int IdInput) const;

  // Retorna true se IdOutput eh uma id de saida do circuito valida (entre 1 e NOutput)
  // Usada para testar parametros de funcoes e valores lidos do teclado ou arquivo
  bool validIdOutput(int IdOutput) const;

  // Retorna true se IdPort eh uma id de porta do circuito valida (entre 1 e NPort)
  // Usada para testar parametros de funcoes e valores lidos do teclado ou arquivo
  bool validIdPort(int IdPort) const;

  // Retorna true se IdOrig eh uma id valida para a origem do sinal de uma entrada de porta ou
  // para a origem de uma saida do circuito (podem vir de uma entrada do circuito ou de uma porta)
  // Usada para testar parametros de funcoes e valores lidos do teclado ou arquivo
  bool validIdOrig(int IdOrig) const;

  // Retorna true se o circuito eh valido (estah com todos os dados corretos):
  // - numero de entradas, saidas e portas valido (> 0)
  // - todos as portas alocadas (ponteiros ports[i] != nullptr)
  // - todas as portas validas (usa Port::valid)
  // - todas as entradas de todas as portas com Id de origem validas (usa validIdOrig)
  // - todas as saidas com Id de origem validas (usa validIdOrig)
  // Essa funcao deve ser usada antes de salvar ou simular um circuito
  bool valid() const;

  // Caracteristicas das entradas
  // Retorna o valor logico atual da entrada cuja id eh IdInput
  // Depois de testar o parametro (validIdInput), retorna o valor de inputs[-IdInput - 1]
  // ou UNDEF se parametro invalido
  bool3S getInput(int IdInput) const;
  // Caracteristicas das saidas
  // Retorna a origem (a id) do sinal de saida cuja id eh IdOutput
  // Depois de testar o parametro (validIdOutput), retorna id_out[IdOutput-1]
  // ou 0 se parametro invalido
  int getIdOutput(int IdOutput) const;

  // Retorna o valor logico atual da saida cuja id eh IdOutput
  // Fazendo id = getIdOutput(IdOutput), entao
  // Retorna ports[ id - 1 ]->getOutput() se id > 0
  // ou inputs[ -id - 1 ] se id < 0
  // ou UNDEF se id==0 (invalido)
  bool3S getOutput(int IdOutput) const;


  // Caracteristicas das portas
  // Nome da porta: AN, NX, etc
  // Depois de testar o parametro (validIdPort) e se ports[..]!=nullptr,
  // retorna ports[IdPort-1]->getName()
  // ou "??" se parametro invalido
  std::string getNamePort(int IdPort) const;

  // Retorna o numero de entradas (1 a 4) da porta
  // Depois de testar o parametro (validIdPort) e se ports[..]!=nullptr,
  // retorna ports[IdPort-1]->getNumInputs()
  // ou 0 se parametro invalido
  unsigned getNumInputsPort(int IdPort) const;
  // Retorna a origem (a id) da I-esima entrada da porta cuja id eh IdPort
  // Depois de testar os parametros (validIdPort) e se ports[..]!=nullptr,
  // retorna ports[IdPort-1]->getId_in(I)
  // ou 0 se parametro invalido
  int getId_inPort(int IdPort, unsigned I) const;

  // Funcoes de modificacao de valores

  // Caracteristicas das ports

  // A porta cuja id eh IdPort passa a ser do tipo Tipo (NT, AN, etc.), com NIn entradas
  // Depois de varios testes (tipo, Id, num de entradas), faz:
  // 1) Libera a antiga area de memoria: delete ports[IdPort-1]
  // 2) Cria a nova porta: ports[IdPort-1] <- new ... (de acordo com tipo T)
  // 3) Fixa o numero de entrada: ports[IdPort-1]->setNumInputs(NIn)
  void setPort(int IdPort, const std::string& Tipo, unsigned NIn);

  // Altera a origem da I-esima entrada da porta cuja id eh IdPort, que passa a ser "IdOrig"
  // Depois de VARIOS testes (validIdPort, !=nullptr, validIndex, validIdOrig),
  // faz: ports[IdPort-1]->setId_in(I,Idorig)
  void setId_inPort(int IdPort, unsigned I, int IdOrig) const;

  // Caracteristicas das saidas

  // Altera a origem da saida de id "IdOut", que passa a ser "IdOrig"
  // Depois de testar os parametros (validIdOutput,validIdOrig),
  // faz: id_out[IdOut-1] <- IdOrig
  void setIdOutput(int IdOut, int IdOrig);

  // E/S de dados

  // Entrada dos dados de um circuito via teclado
  // O usuario digita o numero de entradas, saidas e portas
  // Em seguida, para cada porta o usuario digita o tipo (NT,AN,NA,OR,NO,XO,NX)
  // Apos criada dinamicamente (new) a porta do tipo correto, chama a
  // funcao digitar na porta recem-criada
  // Em seguida, o usuario digita as ids de todas as saidas
  // Se o usuario digitar um dado invalido, o metodo deve pedir que ele digite novamente
  void digitar();

  // Entrada dos dados de um circuito via arquivo
  // Leh do arquivo o cabecalho com o numero de entradas, saidas e portas
  // Em seguida, para cada porta leh e confere a id e o tipo (NT,AN,NA,OR,NO,XO,NX)
  // Apos criada dinamicamente (new) a porta do tipo correto, chama a
  // funcao ler na porta recem-criada
  // Em seguida, leh as ids de todas as saidas
  // Retorna true se deu tudo OK; false se deu erro
  bool ler(const std::string& arq);

  // Saida dos dados de um circuito (em tela ou arquivo, a mesma funcao serve para os dois)
  // Imprime os cabecalhos e os dados do circuito, caso o circuito seja valido
  std::ostream& imprimir(std::ostream& O=std::cout) const;

  // Salvar circuito em arquivo, caso o circuito seja valido
  // Abre a stream e depois chama o metodo imprimir
  // Retorna true se deu tudo OK; false se deu erro
  bool salvar(const std::string& arq) const;

  // SIMULACAO

  // Calcula a saida das ports do circuito para os valores de entrada
  // passados como parametro, caso o circuito e a dimensao da entrada sejam validos
  // A entrada eh um vetor de bool3S, com dimensao igual ao numero de entradas do circuito.
  // Retorna true se a simulacao foi OK; false caso deh erro
  bool simular(const std::vector<bool3S>& Inputs);
};

// Operador de impressao da classe Circuit
// Utiliza o metodo imprimir
inline std::ostream& operator<<(std::ostream& O, const Circuit& C) {return C.imprimir(O);}

#endif // _CIRCUITO_H_
