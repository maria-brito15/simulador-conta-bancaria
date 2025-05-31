#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <fstream>

#define HIST_MAX 30
#define MAX_CONTAS 5

using namespace std;

// FORMATO DE TRANSAÇÃO
struct Transacao {
    string data;
    string tipo;
    double valor;
};

// DATA ATUAL
string dataAtual() {
    time_t agora = time(0);
    tm* ltm = localtime(&agora);

    ostringstream oss;
    oss << setfill('0') << setw(2) << ltm->tm_mday << "/"
        << setw(2) << ltm->tm_mon + 1 << "/"
        << setw(4) << 1900 + ltm->tm_year;
    return oss.str();
}

// CLASSE DE CONTA BANCARIA
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
        id = 0;
        nome = "";
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

    void depositar() {
        double deposito;
        bool invalido;
        
        do {
            invalido = false;
            cout << "Valor do Deposito ($): ";
            cin >> deposito;
            if (deposito <= 0) {
                cout << "Valor Inválido." << endl;
                invalido = true;
            }
        } while (invalido);

        saldo += deposito;
        adicionarHistorico("Depósito", deposito);
        cout << "Depósito de R$ " << fixed << setprecision(2) << deposito << " efetuado com sucesso!" << endl;
    }

    void sacar() {
        double saque;
        bool invalido;
        
        do {
            invalido = false;
            cout << "Valor do Saque ($): ";
            cin >> saque;
            if (saque <= 0 || saque > saldo) {
                cout << "Valor Inválido." << endl;
                invalido = true;
            }
        } while (invalido);

        saldo -= saque;
        adicionarHistorico("Saque", saque);
        cout << "Saque de R$ " << fixed << setprecision(2) << saque << " Efetuado com Sucesso!" << endl;
    }

    void mostrarSaldo() {
        cout << fixed << setprecision(2) << "Saldo Atual: R$ " << saldo << endl;
    }

    void mostrarDados() {
        cout << "\nInformações da Conta:" << endl;
        cout << "ID: " << id << endl;
        cout << "Nome: " << nome << endl;
        cout << fixed << setprecision(2) << "Saldo: R$ " << saldo << endl;
        cout << "Total de Transações: " << totalTransacoes << endl;
    }

    void adicionarHistorico(const string& tipo, double valor) {
        if (totalTransacoes < HIST_MAX) {
            historico[totalTransacoes] = {dataAtual(), tipo, valor};
            totalTransacoes++;
        } else {
            for (int i = 1; i < HIST_MAX; i++) {
                historico[i - 1] = historico[i];
            }
            
            historico[HIST_MAX - 1] = {dataAtual(), tipo, valor};
        }
    }

    void mostrarHistorico() {
        if (totalTransacoes == 0) {
            cout << "Sem Histórico de Transações." << endl;
            return;
        }

        cout << "\nHistórico de Transações:" << endl;
        for (int i = 0; i < totalTransacoes; i++) {
            cout << historico[i].data << " - "
                 << historico[i].tipo << ": R$ " << fixed << setprecision(2) << historico[i].valor << endl;
        }
    }

    void fazerTransacao(contaBancaria contas[], int totalContas) {
        double valor;
        int idRecep;

        cout << "\nTransferência:" << endl;
        cout << "Valor ($): ";
        cin >> valor;
        cout << "ID do Receptor: ";
        cin >> idRecep;

        if (valor <= 0 || valor > saldo) {
            cout << "Valor Inválido." << endl;
            return;
        }

        contaBancaria* receptor = nullptr;
        
        for (int i = 0; i < totalContas; i++) {
            if (contas[i].getID() == idRecep) {
                receptor = &contas[i];
                break;
            }
        }

        if (receptor == nullptr) {
            cout << "Conta Receptora Não Encontrada." << endl;
            return;
        }

        saldo -= valor;
        receptor -> saldo += valor;

        adicionarHistorico("Transferência Enviada", valor);
        receptor -> adicionarHistorico("Transferência Recebida", valor);

        cout << "Transferência de R$ " << fixed << setprecision(2) << valor
             << " Realizada para ID " << idRecep << "." << endl;
    }

    int getID() {
        return id;
    }

    string getNome() {
        return nome;
    }

    void criarConta() {
        cout << "Nome da Conta: ";
        getline(cin >> ws, nome);
        
        id = identificador();
        cout << "Conta Criada com ID: " << id << endl;
    }

    void salvarConta() {
        ofstream arquivo("conta_" + to_string(id) + ".txt");
        
        if (arquivo.is_open()) {
            arquivo << nome << endl;
            arquivo << saldo << endl;
            arquivo << id << endl;
            arquivo << totalTransacoes << endl;
            
            for (int i = 0; i < totalTransacoes; i++) {
                arquivo << historico[i].data << "/"
                        << historico[i].tipo << "/"
                        << historico[i].valor << endl;
            }
            
            arquivo.close();
        } else {
            cout << "Erro ao Salvar Conta!" << endl;
        }
    }

    bool carregarConta(int buscaID) {
        ifstream arquivo("conta_" + to_string(buscaID) + ".txt");
        
        if (!arquivo.is_open()) {
            return false;
        }

        getline(arquivo >> ws, nome);
        arquivo >> saldo >> id >> totalTransacoes;
        
        string linha;
        getline(arquivo, linha);

        for (int i = 0; i < totalTransacoes; i++) {
            getline(arquivo, linha);
            
            stringstream ss(linha);
            string data, tipo, valorStr;
    
            getline(ss, data, '/');
            getline(ss, tipo, '/');
            getline(ss, valorStr);
            
            historico[i] = {data, tipo, stod(valorStr)};
        }

        arquivo.close();
        return true;
    }
};

// MENU PRINCIPAL
void mostrarMenu() {
    cout << "\n--- MENU ---" << endl;
    cout << "1. Depositar" << endl;
    cout << "2. Sacar" << endl;
    cout << "3. Mostrar Saldo" << endl;
    cout << "4. Mostrar Dados da Conta" << endl;
    cout << "5. Mostrar Histórico" << endl;
    cout << "6. Fazer Transferência" << endl;
    cout << "7. Salvar & Sair" << endl;
    cout << "Escolha uma opção: ";
}

int main() {
    srand(time(0));

    contaBancaria contas[MAX_CONTAS];
    int totalContas = 0;
    contaBancaria* cliente = nullptr;
    
    bool finalizar = false;

    cout << "--- Simulador de Conta Bancária ---" << endl;

    int escolhaInicial;
    
    do {
        cout << "\nMenu Inicial:" << endl;
        cout << "1. Criar Nova Conta" << endl;
        cout << "2. Acessar Conta Existente" << endl;
        cout << "3. Sair" << endl;
        cout << "Escolha: ";
        cin >> escolhaInicial;

        if (escolhaInicial == 1) {
            if (totalContas >= MAX_CONTAS) {
                cout << "Limite Máximo de Contas Atingido." << endl;
                continue;
            }
            
            cliente = &contas[totalContas];
            cliente -> criarConta();
            cliente -> salvarConta();
            
            totalContas++;
        } else if (escolhaInicial == 2) {
            int idBusca;
            cout << "Digite o ID da Conta: ";
            cin >> idBusca;

            cliente = nullptr;
            
            for (int i = 0; i < totalContas; i++) {
                if (contas[i].carregarConta(idBusca)) {
                    cliente = &contas[i];
                    break;
                }
            }
            
            if (cliente == nullptr) {   
                cout << "Conta Não Encontrada." << endl;
                continue;
            }
            
            cout << "Conta Acessada com Sucesso!" << endl;
        } else if (escolhaInicial == 3) {
            cout << "\nSimulação Finalizada." << endl;
            finalizar = true;
        } else { 
            cout << "\nOpção Inválida." << endl;
            continue;
        }

        if (cliente != nullptr) {
            int opcao;
            
            do {
                mostrarMenu();
                cin >> opcao;

                switch (opcao) {
                    case 1:
                        cliente -> depositar();
                        break;
                    case 2:
                        cliente -> sacar();
                        break;
                    case 3:
                        cliente -> mostrarSaldo();
                        break;
                    case 4:
                        cliente -> mostrarDados();
                        break;
                    case 5:
                        cliente -> mostrarHistorico();
                        break;
                    case 6:
                        cliente -> fazerTransacao(contas, totalContas);
                        break;
                    case 7:
                        cout << "Salvando e Saindo da Conta..." << endl;
                        cliente -> salvarConta();
                        
                        cliente = nullptr;
                        break;
                    default:
                        cout << "Opção inválida." << endl;
                        break;
                }
            } while (opcao != 7);
        }
    } while (!finalizar);

    return 0;
}