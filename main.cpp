#include "mmu.hpp"

int main(int argc, char **argv){
	
	/* Argumentos:
	 * 1 - Arquivo de trace
	 * 2 - 1) Segunda Chance e 2) LRU
	 * 3 - Detalhamento
	 * *********************************/
	 
	 int opcao;
	 bool detalha = false;
	 
	 string erro = "Nao foi possivel executar!\n\nArgumentos necessarios:\n" 
		"1 - Nome do arquivo de trace;\n"
		"2 - -sc (Segunda Chance) ou -lru (LRU);\n"
		"Argumento opcional:\n"
		"3 - d: Detalha os enderecos lidos;\n"
		"\nExemplo:\n"
		"./mmu nome.trace 1 d";
	
	if(!(argc > 2)) {
		cout << erro << endl;
		exit(EXIT_FAILURE);
	}
	
	// nome do arquivo
	string arquivo(argv[1]);
	
	// politica de substituicao
	string politica(argv[2]);
	if(!politica.compare("-sc")) {
		opcao = 1;
	} else if(!politica.compare("-lru")) {
		opcao = 2;
	} else {
		cout << erro << endl;
		exit(EXIT_FAILURE);
	}
	
	// detalhamento
	if(argc == 4) {		
		string detal(argv[3]);
		if(!detal.compare("d"))
			detalha = true;
		else if(!detal.empty()) {
			cout << erro << endl;
			exit(EXIT_FAILURE);
		}
	}

	estruturaDeMemoria *MMU = new estruturaDeMemoria();
	MMU->simular(opcao, detalha, arquivo);
	
	return 0;
}

