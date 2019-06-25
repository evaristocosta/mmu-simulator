/* **************************************************************************
 * 
 * SIMULACAO DE UNIDADE DE GERENCIAMENTO DE MEMORIA (MMU)
 * 
 * Esta implementacao tem por objetivo simular as operacoes de uma MMU
 * a partir de duas politicas de substituicao de paginas: Segunda Chance
 * e LRU, com objetivo comparativo.
 * 
 * Caracteristicas do sistema:
 * - Paginacao de um nivel com TLB
 * - Tamanho TLB padrao = 8 paginas
 * - Tamanho da memoria = 64 frames 
 * - Tamanho da Tabela de Paginas = 64 paginas
 * - Numero de pagina = 20 bits
 * - Deslocamento = 12 bits
 * 
 * Estas caracteristicas podem ser alteradas na primeira execucao.
 * 
 * **************************************************************************/ 

#ifndef MMU_H
#define MMU_H

#include <stdio.h>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

struct TLB {
	int nPag[8], nFrame[8];
};

struct TabelaPagina {
	int enderecoPagina[64], enderecoFrame[64];
};

struct MemoriaLRU {
	int frames[64], contador[64];
};
struct MemoriaSC {
	int frames[64];
	bool referencia[64];
};

class estruturaDeMemoria {
public:
	// Inicializacao das tabelas e memoria
	estruturaDeMemoria();
	
	// Funcoes principais
	void simular(int,bool,string);
	bool verificaEndereco(int,int);
	
	// Atualizacao de tabelas
	void atualizaTLB(int,int);
	void atualizaTabelaPagina(int,int,int);
	
	// Politicas de substituicao
	void subsSegundaChance(int);
	void subsLRU(int);
	
	// Referencia das estruturas de memoria e tabelas
	TLB tlb;
	TabelaPagina tabelaPagina;
	MemoriaLRU memoriaLRU;
	MemoriaSC memoriaSC;
	
	// Contadores avaliativos
	int tlbHit = 0;
	int tlbMiss = 0;
	int falhaDePagina = 0;
};

#endif //MMU_H