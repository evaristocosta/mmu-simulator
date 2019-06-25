#include "mmu.hpp"

// facilitador para obtençao do tamanho de array 
// http://www.cplusplus.com/forum/general/33669/
#define ARRAY_SIZE(array) (sizeof((array))/sizeof((array[0])))

// Inicializaçao das estruturas
estruturaDeMemoria::estruturaDeMemoria() {
	
	// TBL
	for(size_t i = 0; i < ARRAY_SIZE(tlb.nPag); i++) {
		tlb.nPag[i] = -1;
		tlb.nFrame[i] = -1;
	}
	
	// Tabela de paginas
	for(size_t i = 0; i < ARRAY_SIZE(tabelaPagina.enderecoFrame); i++) {
		tabelaPagina.enderecoFrame[i] = -1;
		tabelaPagina.enderecoPagina[i] = -1;
	}
	
	// Memorias
	for(size_t i = 0; i < ARRAY_SIZE(memoriaLRU.frames); i++) {
		memoriaLRU.frames[i] = -1;
		memoriaLRU.contador[i] = 0;
		
		memoriaSC.frames[i] = -1;
		memoriaSC.referencia[i] = false;
	}
	
}

// Funcao para abertura do arquivo de traces e consequente simulacao da MMU
void estruturaDeMemoria::simular(int opcao, bool detalhe, string caminho) {
	ifstream arquivo;
	string itemTrace;
	double contadorLinha = 0;
	
	// Caminho do arquivo
	//string caminho = "../traces/bzip.trace"; 
	
	// Abre e testa arquivo
	arquivo.open(caminho);
	if(!arquivo) {
		cout << "Impossível abrir o arquivo." << endl;
		exit(1);
	}
	
	// Leitura de linha em linha
	while(getline(arquivo, itemTrace)) {
		contadorLinha++;
		// Pega o endereco
		string endereco = itemTrace.substr(0,8);
		// Separa e traduz endereco
		int pagina = stoul(endereco.substr(0,5), nullptr, 16);
		
		// Detalha sobre enderecos
		if(detalhe) {			
			char tipo = itemTrace[9];
			int deslocamento = stoul(endereco.substr(5,7), nullptr, 16);;
			
			cout << "Endereco: " << endereco << ", tipo " << tipo << endl;
			cout << "Pagina (0x"<< endereco.substr(0,5) <<") " << pagina << " , deslocamento (0x"<< endereco.substr(5,7) <<") " << deslocamento << endl;
		}
		
		// =========== SIMULA A MEMORIA ===========
		if(!verificaEndereco(pagina, opcao)) {
			// Nao achou, contabiliza falha
			++falhaDePagina;
			
			if(opcao == 1) {
				subsSegundaChance(pagina);
			} else {
				subsLRU(pagina);
			} 
		}
		// =========== ================ ===========
	}
	arquivo.close();
	
	// =========== MOSTRA RESULTADOS ===========
	cout << "\nCaminho: " << caminho << "\n"
			"\nTLB Miss: " << tlbMiss << "\n"
			"TLB Hit: " << tlbHit << "\n"
			"Falhas de pagina: " << falhaDePagina << "\n"
			"-----------------------------\n"
			"Taxa de falhas: " << (double)falhaDePagina/contadorLinha << endl;
	// =========== ================= ===========
		 
}

bool estruturaDeMemoria::verificaEndereco(int pagina, int opcao) {
	// Verifica TLB
	for(size_t i = 0; i < ARRAY_SIZE(tlb.nPag); i++) {
		if(pagina == tlb.nPag[i]) {
			++tlbHit;
			return true;
		}
	}
	++tlbMiss;
	
	// Verifica tabela de enderecos
	for(size_t i = 0; i < ARRAY_SIZE(tabelaPagina.enderecoPagina); i++) {
		if(pagina == tabelaPagina.enderecoPagina[i]) {
			// Altera referencia ou contador das memorias
			if(opcao == 1) {
				// Segunda Chance
				for(size_t j = 0; j < ARRAY_SIZE(memoriaSC.frames); j++) {
					if(tabelaPagina.enderecoFrame[i] == memoriaSC.frames[j]) {
						// Muda bit de referencia
						memoriaSC.referencia[j] = true;
						break;
					}
				}
			} else {
				// LRU
				for(size_t j = 0; j < ARRAY_SIZE(memoriaLRU.frames); j++) {
					if(tabelaPagina.enderecoFrame[i] == memoriaLRU.frames[j]) {
						// Zera o contador
						memoriaLRU.contador[j] = 0;
						break;
					}
				}
			}
			// Realiza atualizacao
			atualizaTLB(pagina, tabelaPagina.enderecoPagina[i]);
			return true;
		}
		// Soma contadores independente da opcao
		memoriaLRU.contador[i]++;
	}
	// Nao achou na TBL nem na Tabela
	return false;
}

// ============= ATUALIZACAO DE TABELAS =============
void estruturaDeMemoria::atualizaTLB(int pagina, int frame) {
	// Simples movimento dentro do vetor (FIFO)
	for(size_t i = 0; i < (ARRAY_SIZE(tlb.nPag) - 1); i++) {
		tlb.nPag[i+1] = tlb.nPag[i];
		tlb.nFrame[i+1] = tlb.nFrame[i];
	}
	tlb.nPag[0] = pagina;
	tlb.nFrame[0] = frame;
}

void estruturaDeMemoria::atualizaTabelaPagina(int pagina, int frameVelho, int frameNovo) {
	// Encontra na tabela o frame a ser removido
	for(size_t i = 0; i < ARRAY_SIZE(tabelaPagina.enderecoPagina); i++) {
		if(tabelaPagina.enderecoFrame[i] == frameVelho) {
			tabelaPagina.enderecoPagina[i] = pagina;
			tabelaPagina.enderecoFrame[i] = frameNovo;
			return;
		} 
	}
}
// ------------- ATUALIZACAO DE TABELAS -------------

// ============= METODOS DE SUBSTITUICAO =============
void estruturaDeMemoria::subsSegundaChance(int pagina) {
	// Forma didadica de referencia ao frame de memoria
	int frameNovo = (pagina * 2 + 1);
	
	// No caso de memoria livre
	for(int i = (ARRAY_SIZE(memoriaSC.frames) - 1); i >= 0; i--) {
		if(memoriaSC.frames[i] == -1) {
			
			memoriaSC.frames[i] = frameNovo;
			memoriaSC.referencia[i] = true;
			
			atualizaTLB(pagina, frameNovo);
			atualizaTabelaPagina(pagina, -1, frameNovo);
			
			return;
		}
	}
	
	// Substituicao
	for(int i = (ARRAY_SIZE(memoriaSC.frames) - 1); i >= 0; i--) {
		if(memoriaSC.referencia[i] == false) {
			int frameVelho = memoriaSC.frames[i];
			
			memoriaSC.frames[i] = frameNovo;
			memoriaSC.referencia[i] = true;
			
			atualizaTLB(pagina, frameNovo);
			atualizaTabelaPagina(pagina, frameVelho, frameNovo);
			
			return;
		} else
			memoriaSC.referencia[i] = false;
	}
	
	// Se chegar aqui, todos estavam true, entao altera o ultimo
	int frameVelho = memoriaSC.frames[(ARRAY_SIZE(memoriaSC.frames) - 1)];
	
	memoriaSC.frames[(ARRAY_SIZE(memoriaSC.frames) - 1)] = frameNovo;
	memoriaSC.referencia[(ARRAY_SIZE(memoriaSC.frames) - 1)] = true;
			
	atualizaTLB(pagina, frameNovo);
	atualizaTabelaPagina(pagina, frameVelho, frameNovo);
	
	return;
}


void estruturaDeMemoria::subsLRU(int pagina) {
	int frameNovo = (pagina * 2 + 1);
	
	// no caso de memoria livre
	for(int i = (ARRAY_SIZE(memoriaLRU.frames) - 1); i >= 0; i--) {
		if(memoriaLRU.frames[i] == -1) {
			memoriaLRU.frames[i] = frameNovo;
			
			atualizaTLB(pagina, frameNovo);
			atualizaTabelaPagina(pagina, -1, frameNovo);
			
			return;
		}
	}
	
	// Substituicao
	int contador = memoriaLRU.contador[0];
	int indice = 0;
	
	for(int i = (ARRAY_SIZE(memoriaLRU.frames) - 1); i >= 0; i--) {
		// Se encontrar um contador maior, entao atualiza
		if(contador < memoriaLRU.contador[i]) {
			contador = memoriaLRU.contador[i];
			indice = i;
		}
	}
	
	// Altera no maior contador que achou, e se nao achou, altera a primeira posicao
	int frameVelho = memoriaLRU.frames[indice];
	memoriaLRU.frames[indice] = frameNovo;
	memoriaLRU.contador[indice] = 0;
			
	atualizaTLB(pagina, frameNovo);
	atualizaTabelaPagina(pagina, frameVelho, frameNovo);
	
	return;
}
// ------------- METODOS DE SUBSTITUICAO -------------