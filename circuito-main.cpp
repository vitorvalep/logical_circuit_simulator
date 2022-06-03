#include <iostream>
#include <string>
#include "circuito.h"

using namespace std;

void gerarTabela(Circuit& C);

int main(void)
{
  Circuit C;
  string nome;
  int opcao;

  do {
    cout << "\nPROGRAMA SIMULADOR DE CIRCUITOS DIGITAIS:\n";
    do {
      cout << "0 - Sair\n";
      cout << "1 - Digitar um circuito via teclado\n";
      cout << "2 - Salvar um circuito em arquivo\n";
      cout << "3 - Ler um circuito de arquivo\n";
      cout << "4 - Imprimir o circuito na tela\n";
      cout << "5 - Simular o circuito para todas as entrada (gerar tabela verdade)\n";
      cout << "Qual sua opcao? ";
      cin >> opcao;
    } while(opcao<0 || opcao>5);
    switch(opcao){
    case 1:
      C.digitar();
      break;
    case 2:
    case 3:
      // Antes de ler a string com o nome do arquivo, esvaziar o buffer do teclado
      cin.ignore(256,'\n');
      do {
        cout << "Arquivo: ";
        getline(cin,nome);
      } while (nome.size() < 3); // Name do arquivo >= 3 caracteres
      if (opcao==3) {
        if (!C.ler(nome))
        {
          // Erro na leitura
          cerr << "Arquivo " << nome << " invalid para leitura\n";
        }
      }
      else {
        if (!C.salvar(nome))
        {
          // Erro no salvamento
          cerr << "Arquivo " << nome << " invalid para escrita\n";
        }
      }
      break;
    case 4:
      C.imprimir();
      break;
    case 5:
      gerarTabela(C);
      break;
    default:
      break;
    }
  } while(opcao != 0);
}

void gerarTabela(Circuit& C)
{
  vector<bool3S> inputs(C.getNumInputs());
  int i,j;

  // Comeca com todas as entradas indefinidas
  for (j=0; j<(int)C.getNumInputs(); j++) inputs[j] = bool3S::UNDEF;

  do
  {
    // Simulacao
    C.simular(inputs);

    // Impressao das entradas
    cout << "ENTRADAS: ";
    for (i=0; i<(int)C.getNumInputs(); i++)
    {
      cout << C.getInput(-i-1);
      if (i<(int)C.getNumInputs()-1) cout << ' ';
      else cout <<'\t';
    }

    // Impressao das saidas
    cout << "SAIDAS: ";
    for (i=0; i<(int)C.getNumOutputs(); i++)
    {
      cout << C.getOutput(i+1);
      if (i<(int)C.getNumOutputs()-1) cout << ' ';
      else cout << '\n';
    }

    // Determina qual entrada deve ser incrementada na proxima linha
    // Incrementa a ultima possivel que nao for TRUE
    // Se a ultima for TRUE, faz essa ser UNDEF e tenta incrementar a anterior
    j = C.getNumInputs()-1;
    while (inputs[j]==bool3S::TRUE && j>=0)
    {
      inputs[j]++;
      j--;
    };
    // Incrementa a input selecionada
    if (j>=0) inputs[j]++;


  } while (j>=0);
}

