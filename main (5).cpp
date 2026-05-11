#ifndef ASTRONAUTA_H
#define ASTRONAUTA_H

#include <string>
#include <vector>
#include <iostream>

using namespace std;

class Astronauta {
public:
    string cpf;
    string nome;
    int idade;
    bool vivo;
    bool disponivel;
    vector<int> voosParticipados;

    Astronauta(string _cpf, int _idade, string _nome) {
        cpf = _cpf;
        idade = _idade;
        nome = _nome;
        vivo = true;
        disponivel = true;
    }

    void exibir() {
        cout << "CPF: " << cpf << " | Nome: " << nome << " | Idade: " << idade << endl;
    }
};

#endif


#ifndef VOO_H
#define VOO_H

#include <string>
#include <vector>

using namespace std;

enum EstadoVoo { PLANEJADO, EM_CURSO, SUCESSO, EXPLODIDO };

class Voo {
public:
    int codigo;
    vector<string> cpfsPassageiros;
    EstadoVoo estado;

    Voo(int _codigo) {
        codigo = _codigo;
        estado = PLANEJADO;
    }
};

#endif


#include <iostream>
#include <vector>
#include <string>
#include <algorithm>


using namespace std;

vector<Astronauta> listaAstronautas;
vector<Voo> listaVoos;

Astronauta* buscarAstronauta(string cpf) {
    for (auto &a : listaAstronautas) {
        if (a.cpf == cpf) return &a;
    }
    return nullptr;
}

Voo* buscarVoo(int codigo) {
    for (auto &v : listaVoos) {
        if (v.codigo == codigo) return &v;
    }
    return nullptr;
}

int main() {
    string comando;

    while (cin >> comando && comando != "FIM") {
        if (comando == "CADASTRAR_ASTRONAUTA") {
            string cpf, nome;
            int idade;
            cin >> cpf >> idade;
            getline(cin >> ws, nome);

            if (buscarAstronauta(cpf)) {
                cout << "Erro: CPF ja cadastrado." << endl;
            } else {
                listaAstronautas.push_back(Astronauta(cpf, idade, nome));
                cout << "Astronauta " << nome << " cadastrado." << endl;
            }
        } 
        
        else if (comando == "CADASTRAR_VOO") {
            int codigo;
            cin >> codigo;
            if (buscarVoo(codigo)) {
                cout << "Erro: Codigo de voo ja existe." << endl;
            } else {
                listaVoos.push_back(Voo(codigo));
                cout << "Voo " << codigo << " planejado." << endl;
            }
        }

        else if (comando == "ADICIONAR_ASTRONAUTA") {
            string cpf;
            int codigo;
            cin >> cpf >> codigo;
            Astronauta* a = buscarAstronauta(cpf);
            Voo* v = buscarVoo(codigo);

            if (a && v && v->estado == PLANEJADO && a->vivo) {
                if (find(v->cpfsPassageiros.begin(), v->cpfsPassageiros.end(), cpf) == v->cpfsPassageiros.end()) {
                    v->cpfsPassageiros.push_back(cpf);
                    cout << "Astronauta " << a->nome << " adicionado ao voo " << codigo << "." << endl;
                } else {
                    cout << "Erro: Astronauta ja esta no voo." << endl;
                }
            } else {
                cout << "Erro: Nao foi possivel adicionar (verifique se estao vivos ou se o voo esta planejado)." << endl;
            }
        }

        else if (comando == "LANCAR_VOO") {
            int codigo;
            cin >> codigo;
            Voo* v = buscarVoo(codigo);

            if (v && v->estado == PLANEJADO && !v->cpfsPassageiros.empty()) {
                bool todosProntos = true;
                for (string cpf : v->cpfsPassageiros) {
                    Astronauta* a = buscarAstronauta(cpf);
                    if (!a->vivo || !a->disponivel) {
                        todosProntos = false;
                        break;
                    }
                }

                if (todosProntos) {
                    v->estado = EM_CURSO;
                    for (string cpf : v->cpfsPassageiros) {
                        Astronauta* a = buscarAstronauta(cpf);
                        a->disponivel = false;
                        a->voosParticipados.push_back(v->codigo);
                    }
                    cout << "Voo " << codigo << " lancado com sucesso!" << endl;
                } else {
                    cout << "Erro: Nem todos os astronautas estao vivos ou disponiveis." << endl;
                }
            } else {
                cout << "Erro: Voo vazio ou nao planejado." << endl;
            }
        }

        else if (comando == "EXPLODIR_VOO") {
            int codigo;
            cin >> codigo;
            Voo* v = buscarVoo(codigo);
            if (v && v->estado == EM_CURSO) {
                v->estado = EXPLODIDO;
                for (string cpf : v->cpfsPassageiros) {
                    Astronauta* a = buscarAstronauta(cpf);
                    a->vivo = false;
                    a->disponivel = false;
                }
                cout << "FATALIDADE: O voo " << codigo << " explodiu." << endl;
            } else {
                cout << "Erro: Apenas voos em curso podem explodir." << endl;
            }
        }

        else if (comando == "FINALIZAR_VOO") {
            int codigo;
            cin >> codigo;
            Voo* v = buscarVoo(codigo);
            if (v && v->estado == EM_CURSO) {
                v->estado = SUCESSO;
                for (string cpf : v->cpfsPassageiros) {
                    Astronauta* a = buscarAstronauta(cpf);
                    a->disponivel = true;
                }
                cout << "Sucesso: O voo " << codigo << " retornou a salvo." << endl;
            } else {
                cout << "Erro: Voo nao esta em curso." << endl;
            }
        }

        else if (comando == "LISTAR_VOOS") {
            string nomesEstados[] = {"Planejados", "Em curso", "Finalizados com Sucesso", "Finalizados com Explosao"};
            for (int i = 0; i < 4; i++) {
                cout << "--- " << nomesEstados[i] << " ---" << endl;
                for (auto &v : listaVoos) {
                    if (v.estado == i) {
                        cout << "Voo: " << v.codigo << endl;
                        for (string cpf : v.cpfsPassageiros) {
                            Astronauta* a = buscarAstronauta(cpf);
                            cout << "  - [CPF: " << a->cpf << "] " << a->nome << endl;
                        }
                    }
                }
            }
        }

        else if (comando == "LISTAR_MORTOS") {
            cout << "--- Astronautas Falecidos ---" << endl;
            for (auto &a : listaAstronautas) {
                if (!a.vivo) {
                    cout << "CPF: " << a.cpf << " | Nome: " << a.nome << " | Voos: ";
                    for (int cod : a.voosParticipados) cout << cod << " ";
                    cout << endl;
                }
            }
        }
    }
    return 0;
}