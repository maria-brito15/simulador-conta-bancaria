#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <sstream>

#define HIST_MAX 30

using namespace std;

struct Transacao {
    string data;
    string tipo;
    double valor;
};

string dataAtual() {
    time_t agora = time(0);
    tm* ltm = localtime(&agora);

    ostringstream oss;
    oss << setfill('0') << setw(2) << ltm -> tm_mday << "/"
        << setw(2) << ltm -> tm_mon + 1 << "/"
        << setw(4) << 1900 + ltm -> tm_year;

    return oss.str();
}

class contaBancaria {
private:
    string nome;
    double saldo;
    int id;
    Transacao historico[HIST_MAX];
    int totalTransacoes;

public:
    contaBancaria() {
        saldo = 0.0;
        totalTransacoes = 0;
    }

    int identificador() {
        int numero = 0;
        
        for (int i = 0; i < 10; ++i) {
            int digito = rand() % 10;
            
            if (i == 0 && digito == 0)
                digito = 1;
            numero = numero * 10 + digito;
        }
        
        return abs(numero);
    }

    void criarConta() {
        cout << "Nome da Conta: ";
        getline(cin >> ws, nome);
        
        id = identificador();
        cout << "Conta Criada com ID: " << id << endl;
    }

    void depositar() {
        double deposito;
        bool invalido = false;
        
        do {
            cout << "Valor do Deposito ($): ";
            cin >> deposito;
            
            if (deposito <= 0) {
                cout << "Valor Inválido." << endl;
                invalido = true;
            } else {
                invalido = false;
                
                saldo += deposito;
                adicionarHistorico("Depósito", deposito);
                
                cout << "Depósito de R$ " << deposito << " Efetuado com Sucesso!" << endl;
            }
        } while (invalido);
    }

    void sacar() {
        double saque;
        bool invalido = false;
        
        do {
            cout << "\nValor do Saque ($): ";
            cin >> saque;
            
            if (saque <= 0 || saque > saldo) {
                cout << "Valor Inválido." << endl;
                invalido = true;
            } else {
                invalido = false;
                
                saldo -= saque;
                adicionarHistorico("Saque", saque);
                
                cout << "Saque de R$ " << saque << " Efetuado com Sucesso!" << endl;
                break;
            }
        } while (invalido);
    }

    void mostrarSaldo() {
        cout << fixed << setprecision(2);
        cout << "\nSaldo Atual ($): " << saldo << endl;
    }

    void mostrarDados() {
        cout << "\nInformações da Conta: " << endl;
        cout << "ID: " << id << endl;
        cout << "Nome: " << nome << endl;
        
        cout << fixed << setprecision(2) << "Saldo ($): " << saldo << endl;
        cout << "Total de Transações: " << totalTransacoes << endl;
    }

    void adicionarHistorico(const string& tipo, double valor) {
        if (totalTransacoes < HIST_MAX) {
            historico[totalTransacoes].data = dataAtual();
            historico[totalTransacoes].tipo = tipo;
            historico[totalTransacoes].valor = valor;
            totalTransacoes++;
        } else {
            cout << "\nLimite de Histórico Atingido!" << endl;
        }
    }

    void mostrarHistorico() {
        if (totalTransacoes == 0) {
            cout << "\nSem Histórico!" << endl;
            return;
        }

        cout << "\nHistórico de Transações:" << endl;
        for (int i = 0; i < totalTransacoes; i++) {
            cout << historico[i].data << " - " 
            << historico[i].tipo << ": R$ " << fixed << setprecision(2) << historico[i].valor << endl;
        }
    }

    void fazerTransacao() {
        double valor;
        int idRecep;
        
        cout << "\nTransação:" << endl;
        
        cout << "Valor ($): ";
        cin >> valor;
        
        cout << "ID do Receptor: ";
        cin >> idRecep;

        if (valor <= 0 || valor > saldo) {
            cout << "Valor inválido para transação." << endl;
            return;
        }

        saldo -= valor;
        adicionarHistorico("Transação", valor);
        cout << "Transferência de R$ " << valor << " Realizada para ID " << idRecep << "." << endl;
    }
    
    string getNome() {
        return nome;
    }
};

void mostrarMenu() {
    cout << "\n--- MENU ---" << endl;
    cout << "1. Depositar" << endl;
    cout << "2. Sacar" << endl;
    cout << "3. Mostrar Saldo" << endl;
    cout << "4. Mostrar Dados da Conta" << endl;
    cout << "5. Mostrar Histórico" << endl;
    cout << "6. Fazer Transação" << endl;
    cout << "7. Sair" << endl;
    cout << "Escolha uma Opção: ";
}

int main() {
    srand(time(0));
    
    contaBancaria cliente;
    int opcao;

    cout << "--- Simulador de Conta Bancária ---" << endl;
    cliente.criarConta();

    do {
        mostrarMenu();
        cin >> opcao;

        switch(opcao) {
            case 1: 
                cliente.depositar(); 
                break;
            case 2: 
                cliente.sacar(); 
                break;
            case 3: 
                cliente.mostrarSaldo(); 
                break;
            case 4: 
                cliente.mostrarDados(); 
                break;
            case 5: 
                cliente.mostrarHistorico(); 
                break;
            case 6: 
                cliente.fazerTransacao(); 
                break;
            case 7: 
                cout << "\nSaindo do Programa..." << endl; 
                cout << "Obrigado por Utilizar, " << cliente.getNome() << "!" << endl; 
                break;
            default: 
                cout << "Opção Inválida!" << endl; 
                break;
        }
    } while(opcao != 7);

    return 0;
}
