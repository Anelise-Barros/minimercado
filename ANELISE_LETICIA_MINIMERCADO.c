/**********************************************************
     
	  TRABALHO DE ALGORITMO SISTEMA DE MINIMERCADO

Nome: Anelise Neves de Barros          RGA: 2019.0743.006-7
Nome: Letícia Santos Fialho            RGA: 2019.0743.050-4
**********************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct regProduto{	
	int codigo;
	char nome[20];
	int qtd;
	int qtd_min;
	double preco_compra;
	double preco_venda;
	char situacao;
}regProduto; /*Registro de produtos*/

typedef struct regVendas{
	int registro;
	char nomeCliente[50];
	char cpf[11];
	int dia;
	int mes;
	int ano; 
	int qtd_produtos;
	int codigos_produtos[15];
} regVendas;/*Registro de vendas*/


/*Protótipo das funções*/
void menuProdutos(regProduto produto[],int *posP);
void cadastrarProduto(regProduto produto[], int *posP);
void atualizarProduto(int *posP, regProduto produto[]);
void excluirProduto (int *posP, regProduto produto[]);
void pesquisarProduto (int *posP, regProduto produto[]);

int ValidaCodigoProduto (int cod, int posP, regProduto produto[]);
int ValidaNomeProduto(char nome[]);

void registrarVendas (regVendas vendas[], int *posV, int *posP, regProduto produto[], double totalVenda[], double totalCompra[]);

int verificarCPF(char cpf[]);

void menuRelatorio(regProduto produto[],int *posP, int *posV, regVendas vendas[], double totalVenda[], double totalCompra[]);
void estoqueProduto (int *posP, regProduto produto[]);
/*void topProdutos ();*/
void lucroMes (int *posV, regVendas vendas[], double totalVenda[], double totalCompra[]);
void relatorioVendas(int *posV, regVendas vendas[]);
void relatorioAtivos (int *posP, regProduto produto[]);

void CarregarArquivo(int *posP, regProduto produto [], int *posV, regVendas vendas[]); /* AQUI PRECISA TER PONTEIRO PORQUE A VARIAVEL SERÁ MODIFICADA DENTRO DA FUNÇÃO */
void SalvarArquivo(int posP, regProduto produto [], int posV, regVendas vendas[]); /* AQUI NÃO PRECISA TER PONTEIRO PORQUE SÓ É UTILIZADO O VALOR DA VARIÁVEL, ESSE VALOR NÃO SERÁ MODIFICADO DENTRO DA FUNÇÃO*/

/****************************************************************  MENU PRINCIPAL  ******************************************************************/

int main(){
	
	regProduto produto[10];
	int posP=0;
	int op;
	
	regVendas vendas[50];
	int posV=0;
	double totalVenda[15], totalCompra[15];
	
	
	CarregarArquivo(&posP, produto, &posV, vendas);
	
	do{			
		printf("..................................................\n");
		printf("                MENU PRINCIPAL                    \n");
		printf("..................................................\n");
		printf("1 Produtos\n2 Registrar vendas\n3 Relatorios\n4 Sair\n");
		printf("..................................................\n");
		printf("\nInforme a opcao aqui: ");
		scanf("%d", &op);
	
		switch(op) {
			case 1:
				menuProdutos(produto, &posP);/*Passagem por referência*/
			break;
			case 2:
				registrarVendas (vendas, &posV, &posP, produto, totalVenda, totalCompra);/*Passagem por referência*/
				posV++;/*Atualizar registro de venda*/
			break;
			case 3:
				menuRelatorio(produto, &posP, &posV, vendas, totalVenda, totalCompra);/*Passagem por referência*/
			break;		
			case 4:
				printf("\nFINALIZANDO SISTEMA\n");
				SalvarArquivo(posP, produto, posV, vendas);/*SALVAR AS MODIFICAÇÕES NO ARQUIVO PARA FINALIZAR*/ 
			break;		
			default:
				printf("\n\t\t#Opcao invalida!#\n");
			break;
		}	
	} while (op !=4);
	return 0;
}

/****************************************************************  PRODUTOS  ******************************************************************/

void menuProdutos(regProduto produto[],int *posP){
	
	int op;
	
	do{	
		printf("..................................................\n");
		printf("                   PRODUTOS                       \n");
		printf("..................................................\n");
		printf("1 Cadastrar produto\n2 Atualizar produto\n3 Excluir produto\n4 Pesquisar por nome do produto\n0 voltar\n");
		printf("..................................................\n"); 
		printf("\nInforme a opcao aqui: ");
		scanf("%d", &op);
	
		switch(op) {
		
			case 1:
				cadastrarProduto(produto, posP);/*posP não é necessario o & aqui, pois ele já está sendo passado como ponteiro na primeira chamada (menuPrincipal)*/
			break;
			case 2:
				atualizarProduto(posP, produto);
			break;
			case 3:
				excluirProduto (posP, produto);
			break;			
			case 4:
				pesquisarProduto (posP, produto);
			break;
			case 0:
				return;
			break;	
			default:
				printf("\n\n#Opcao invalida!#\n\n");
			break;
		}
	} while (op !=0);

}
/*A FUNÇÃO cadastrarProduto É UTILIZADA PARA CADASTRAR OS PRODUTOS QUE SERÃO VENDIDOS NO MINIMERCADO*/
void cadastrarProduto(regProduto produto[], int *posP){
	
	char nome[20];
	int codigoDisponivel, nomeCorreto;
	int cod;

	if (*posP < 10){
		while(1){
			printf("\nCODIGO DO PRODUTO...........: ");
			scanf("%d", &cod);/*É necessário utilizar uma variável antes de adicionar o valor no vetor para não ocorrer de verificar um código já existente na validação*/
			
			/*Verificar se há codigo repetido*/
			codigoDisponivel = ValidaCodigoProduto (cod, *posP, produto);/*Como o codigo nem o posP serão modificados, eles nao precisam ser passados como ponteiros, podem ir como somente valores inteiros*/
			if (codigoDisponivel == 1){
				produto[*posP].codigo = cod;
				break;
			}
			else{
				printf("Codigo %d ja cadastrado, digite outro codigo\n", cod);	
			}
		}
		
		while(1){
			printf("NOME DO PRODUTO.............: ");
			fflush(stdin);
			scanf("%s", nome);
			
			/*Verificar se o nome do produto começa com dígito numérico*/
			nomeCorreto = ValidaNomeProduto (nome); /*chamar a posição*/
			if (nomeCorreto ==1){
				strcpy(produto[*posP].nome,nome );
				break;
			}
			else{
				printf("Nome %s incorreto por iniciar com digito numerico, digite outro nome\n\n", nome);	
			}
		}
				
		printf("QUANTIDADE..................: ");
		scanf("%d", &produto[*posP].qtd);
	
		printf("QUANTIDADE MINIMA...........: ");
		scanf("%d", &produto[*posP].qtd_min);
	
		printf("PRECO.......................: ");
		scanf("%lf", &produto[*posP].preco_compra);
	
		printf("PRECO DE VENDA..............: ");
		scanf("%lf", &produto[*posP].preco_venda);
	
		printf("SITUACAO A-ATIVO/I-INATIVO..: "); 
		fflush(stdin);
		scanf(" %c", &produto[*posP].situacao); /* Para ler um %c depois de varias coisas, tem que por um espaço em branco antes do %c*/

		(*posP)++;/* Tem que ter () (para poder mudar o valor) e o * (para ter um endereço de ponteiro), *posP tem o valor do tamanho do vetor*/
	}
	else{
		printf("\nLIMITE DE CADASTRO ATINGIDO\n");
	}
}

/*A FUNÇÃO atualizarProduto É UTILIZADA SE CASO O USUÁRIO QUISER MODIFICAR ALGUMA INFORMAÇÃO DO PRODUTO*/
void atualizarProduto(int *posP, regProduto produto[]){
	int cod, j, op;
	
	printf("\nDigite o codigo do produto que deseja atualizar: \n");
	scanf("%d", &cod);
	
	for(j=0; j<*posP; j++){
		if (produto[j].codigo==cod){
			if(produto[j].situacao=='a' || produto[j].situacao=='A' ){
				printf("\n \nCODIGO:%d\nNOME:%s\nQUANTIDADE:%d\nQUANTIDADE MINIMA:%d\nPRECO DE COMPRA:%.2f\nPRECO DE VENDA:%.2f\nSITUACAO:%c\n", 
						produto[j].codigo, produto[j].nome, produto[j].qtd, produto[j].qtd_min,
						produto[j].preco_compra, produto[j].preco_venda, produto[j].situacao);
				printf("..................................................\n");
				printf("             O QUE DESEJA ATUALIZAR?\n");
				printf("..................................................\n");
				printf("1 Nome do Produto \n2 Quantidade de estoque \n3 Quantidade minima de estoque \n4 Preco de compra \n5 Preco de venda \n6 Situacao \n");
				printf("\nInfome a opcao: ");
				scanf("%d", &op);
		
				switch(op){/*AO SELECIONAR A OPÇÃO JÁ VAI DIRETO PARA A MODIFICAÇÃO DO ITEM DESEJADO PELO USUÁRIO */
					case 1:
						printf("\nInforme o novo nome do produto: ");
						scanf("%s", produto[j].nome);
					break;
					case 2:
						printf("\nInforme o novo estoque do produto: ");
						scanf("%d", &produto[j].qtd);
					break;
					case 3:
						printf("\nInforme o novo estoque minimo do produto: ");
						scanf("%d", &produto[j].qtd_min);
					break;
					case 4:
						printf("\nInforme o novo preco de compra do produto: ");
						scanf("%lf", &produto[j].preco_compra);
					break;
					case 5:
						printf("\nInforme o novo preco de venda do produto: ");
						scanf("%lf", &produto[j].preco_venda);
					break;
					case 6:
						printf("\nInforme a situacao do produto: ");
						scanf(" %c", &produto[j].situacao);
					break;
					default:
						printf("\n\n#Opcao Invalida!#\n\n");
						return;
					break;
				}
				printf("\nPRODUTO ATUALIZADO COM SUCESSO\n");
				return;
			}
			else if(produto[j].situacao=='i' || produto[j].situacao=='I'){
				printf("\nO PRODUTO ESTA INATIVO E NAO PODE SER ATUALIZADO\n");
				return;
			}
			else{
				printf("\nSITUACAO INVALIDA\n");
				return;
			}
		}	
	}
	if(produto[j].codigo!=cod) {
		printf("\nCODIGO NAO ENCONTRADO\n");
		return;
	}
}

/*A FUNÇÃO excluirProduto DEIXA O PRODUTO INATIVO PARA, ATUALIZAÇÃO, PESQUISA, VENDA E RELATÓRIOS*/
void excluirProduto (int *posP, regProduto produto[]){
	int cod, j, op;
	
	printf("\n \nDigite o codigo do produto que deseja excluir: ");
	scanf("%d", &cod);
	
	for(j=0; j<*posP; j++){
		if (produto[j].codigo==cod){
			printf("\n \nCODIGO:%d\nNOME:%s\nQUANTIDADE:%d\nQUANTIDADE MINIMA:%d\nPRECO DE COMPRA%.2f\nPRECO DE VENDA:%.2f\nSITUACAO:%c\n", 
					produto[j].codigo, produto[j].nome, produto[j].qtd, produto[j].qtd_min,
					produto[j].preco_compra, produto[j].preco_venda, produto[j].situacao);/*MOSTRAR OS DADOS DO PRODUTO*/
			
			printf("..................................................\n");
			printf("\n TEM CERTEZA QUE DESEJA EXCLUIR ESSE PRODUTO?\n ");
			printf("..................................................\n");	
			printf("1 SIM\n2 NAO ");
			printf("\nInforme a opcao:");
			scanf("%d", &op);/*MENU DE CONFIRMAÇÃO*/
			
			switch(op){
				case 1:
					produto[j].situacao='I';
					printf("\nPRODUTO EXCLUIDO COM SUCESSO\n");
					return;
				break;
				case 2:
					return;
				break;
				default:
					printf("\n\n#Opcao Invalida!#\n\n");
					return;
				break;
			}		
		}
	}
	if(produto[j].codigo!=cod) {
			printf("\nCODIGO NAO ENCONTRADO\n");
			return;
	}		
}

/*A FUNÇÃO pesquisarProduto É UTILIZADA PARA PESQUISAR O PRODUTO DESEJADO PELO USIÁRIO MOSTARNDO APENAS ALGUNS DADOS */
void pesquisarProduto (int *posP, regProduto produto[]){
	char nome[20];
	int j, compara=0;
	
	printf("\nDigite o nome do produto: ");
	fflush(stdin);
	scanf("%s", nome);

	for(j=0; j<*posP; j++){
		compara= strcmp(nome, produto[j].nome);
		if (compara==0){
			if(produto[j].situacao=='a' || produto[j].situacao=='A' ){
				printf("\n \nCODIGO:%d\nNOME:%s\nQUANTIDADE:%d\nPRECO DE VENDA:%.2f\n", 
						produto[j].codigo, produto[j].nome, produto[j].qtd, produto[j].preco_venda);
				return;		
			}
			else if(produto[j].situacao=='i' || produto[j].situacao=='I'){
				printf("\nO PRODUTO ESTA INATIVO E NAO PODE SER PESQUISADO\n");
				return;
			}
			else{
				printf("\nSITUACAO INVALIDA\n");
				return;
			}
		}
	}
	if(compara!=0){
		printf("\nPRODUTO NAO ENCONTARDO\n");
		return;
	}
}

/************************************************************  VALIDAÇÃO PRODUTOS  **************************************************************/

int ValidaCodigoProduto (int cod, int posP, regProduto produto[]){
	int j;
	
	for(j=0; j<posP; j++){
		if (produto[j].codigo==cod){
			return 0;
		}
	}
	return 1;
}/*FUNÇÃO COMPARA COM OS CÓDIGOS EXISTENTES PARA QUE O MINIMERCADO NÃO TENHA CÓDIGOS IGUAIS*/

int ValidaNomeProduto(char nome[]){
	int tam=0;
	
		tam=strlen(nome);
		if(nome[0]=='0'||nome[0]=='1'||nome[0]=='2'||nome[0]=='3'||nome[0]=='4'||nome[0]=='5'||nome[0]=='6'||nome[0]=='7'||nome[0]=='8'||nome[0]=='9'||tam>20){
			return 0;
		}
	return 1;	
}/*FUNÇÃO VERIFICA SE O NOME COMEÇA COM DÍGITO NUMÉRICO OU SE A PALAVRA TEM MAIS DE 20 LETRAS*/

/******************************************************************  VENDAS ********************************************************************/
/*A FUNÇÃO registrarVendas REALIZA TODAS AS VENDAS*/
void registrarVendas (regVendas vendas[], int *posV, int *posP, regProduto produto[], double totalVenda[], double totalCompra[]){
	char op, cpf[11];
	int cod=0, j, CPFcorreto, encontraCod;
	double tvenda=0, tcompra=0;
	

	if(*posV<50){
		printf("..................................................\n");
		printf("\n                   CADASTRO\n");
		printf("..................................................\n");
		
		vendas[*posV].registro=*posV;
		printf("\nRegistrando venda:%d ", vendas[*posV].registro+1);
		printf("\nDigite o nome do cliente: ");
		fflush(stdin);
		scanf(" %s", vendas[*posV].nomeCliente);
		
		while(1){
			printf("Digite o CPF do cliente: ");
			fflush(stdin);
			scanf("%s", cpf);
			/*Verifica se o conjunto de numeros digitados possui 11 digitos para digitar o CPF corretamente*/
			CPFcorreto = verificarCPF(cpf);/*CHAMA A FUNÇÃO*/
			if(CPFcorreto==1){
				strcpy(vendas[*posV].cpf,cpf);
				break;
			}
			else{
				printf("CPF incorreto, digite novamente\n\n");
			}
		}
	
		printf("Digite a data da venda: ");
		scanf("%d/%d/%d", &vendas[*posV].dia, &vendas[*posV].mes, &vendas[*posV].ano);
	
		
		vendas[*posV].qtd_produtos=0;/*CONTADOR DA QTD DE PRODUTOS QUE SERÁ GUARDADA NA VARIÁVEL vendas[*posV].codigos_produtos[vendas[*posV].qtd_produtos] */
		printf("..................................................\n");
		printf("\n              REGISTRAR VENDAS\n ");
		printf("..................................................\n");
		printf("<+> <codigo do produto> (PARA ACRESCENTAR UM PRODUTO)\n<-> <codigo do produto> (PARA EXCLUIR UM PRODUTO)\n<0> <0> (PARA FINALIZAR A VENDA)\nSE ATINGIR 15 PRODUTOS A COMPRA FINALIZARA AUTOMATICAMENTE\n\n");
		
		while(vendas[*posV].qtd_produtos<15){
			scanf(" %c", &op);
			scanf("%d", &cod);
			switch(op){
				case '+':
					encontraCod=0;/*VAI VERIFICAR SE ENCONTRA O CÓDIGO NO VETOR*/
					for(j=0; j<*posP; j++){/*VETOR DE PRODUTO*/
						if(produto[j].codigo==cod){/*VERIFICA SE TEM O CÓDIGO NO VETOR*/
							vendas[*posV].codigos_produtos[vendas[*posV].qtd_produtos]=cod;/*ATRIBUI PARA SALVAR NO CÓDIGO NO VETOR*/
							vendas[*posV].qtd_produtos++;/*CONTADOR DA VARIÁVEL vendas[*posV].codigos_produtos[vendas[*posV].qtd_produtos]*/
							encontraCod=1;/*COMO O CÓDIGO FOI ENCONTRADO NO VETOR DE PRODUTO A VARIÁVEL RECEBEU 1*/
							if(produto[j].situacao=='a'||produto[j].situacao=='A'){/*VERIFICA SE O PRODUTO ESTÁ ATIVO*/
								if(produto[j].qtd>0){/*VERIFICA A QUANTIDADE*/
									tvenda=tvenda+produto[j].preco_venda;/*SOMA OS PRODUTOS ADICIONADOS PARA MOSTAR NO TOTAL E SER USADO NAS PRÓXIMAS FUNÇÕES*/
									tcompra=tcompra+produto[j].preco_compra;/*SOMA OS PRODUTOS ADICIONADOS PARA SER USADO NAS PRÓXIMAS FUNÇÕES*/
									produto[j].qtd--;/*DIMINUI DO ESTOQUE*/
									printf(" %d %s %.2f SUCESSO!\n", produto[j].codigo, produto[j].nome, produto[j].preco_venda);
								}
								else{
									printf("\nPRODUTO %d EM FALTA NO ESTOQUE. FALHA!\n", cod);	
								}
							}
							else if(produto[j].situacao=='i'||produto[j].situacao=='I'){/*VERIFICA SE O PRODUTO ESTÁ INATIVO*/
								printf("\nPRODUTO %d ESTA INATIVO. FALHA!\n", cod);	
							}	
						}
					}
					if(encontraCod==0){/*SE O encontraCod NÃO RECEBER 1, QUER DIZER QUE NÃO EXISTE O CÓDIGO NO ESTOQUE DO MINIMERCADO*/
						printf("PRODUTO %d NAO CADASTRADO NO ESTOQUE. FALHA!\n", cod);
					}
					break;
				case '-':
					encontraCod=0;
					for(j=0; j<*posP; j++){
						if(produto[j].codigo==cod){
							encontraCod=1;
							if(produto[j].situacao=='a'||produto[j].situacao=='A'){
								tvenda=tvenda-produto[j].preco_venda;/*SUBTRAI DO TOTAL DA VENDA*/
								tcompra=tcompra-produto[j].preco_compra;/*SUBTRAI DO TOTAL DA VENDA*/
								if(produto[j].qtd>0){/*VERIFICA SE O PROTUDO FOI TIRADO POR DESISTÊNCIA DO CLIENTE OU SE FOI POR FALTA DO PRODUTO NO ESTOQUE*/
									produto[j].qtd++;/*SE FOI TIRADO POR DESISTÊNCIA, ACRESCENTA NOVAMENTE NO ESTOQUE*/
									printf("PRODUTO %d EXCLUIDO DA COMPRA. SUCESSO!\n", cod);
								}
								else{
									printf("PRODUTO %d EXCLUIDO DA COMPRA. SUCESSO!\n", cod);
								}
							}
						}
						else if(produto[j].situacao=='i'||produto[j].situacao=='I'){
								printf("\nPRODUTO %d ESTA INATIVO. FALHA!\n", cod);	
						}		
					}	
					if(encontraCod==0){
						printf("PRODUTO %d NAO CADASTRADO NO ESTOQUE OU NA COMPRA. FALHA!\n", cod);
					}
					break;
				case '0':/*AO DIGITAR 00 ENTRA NO CASE PELO FATO DA COMPRA TER MENOS DE 15 ITENS*/
					printf("\n------------------\n");
					printf("TOTAL: R$%.2f\n", tvenda);
					totalVenda[*posV]=tvenda;/*ATRIBUI O TOTAL PARA SER UTILIZADO NO LUCRO DO MÊS*/
					totalCompra[*posV]=tcompra;/*ATRIBUI O TOTAL PARA SER UTILIZADO NO LUCRO DO MÊS*/
					return;
				break;
				default:
					printf("\nOpcao invalida!\n");
					return;
				break;	 	
			}	
		}
		printf("\n------------------\n");/*PARA COMPRA COM 15 ITENS*/
		printf("TOTAL: R$%.2f\n", tvenda);
		totalVenda[*posV]=tvenda;
		totalCompra[*posV]=tcompra;
		return;
		(*posV)++;/* Tem que ter () (para poder mudar o valor) e o * (para ter um endereço de ponteiro), *posP tem o valor do tamanho do vetor*/	
	}
	else{
		printf("\nLIMITE DE VENDAS ATINGIDO\n");	
	}
}

/****************************************************************  VERIFICAR CPF  ******************************************************************/

int verificarCPF(char cpf[]){
	int tam=0;
	
	tam=strlen(cpf);
	if(tam!=11){
		return 0;
	}
	return 1;
}/*VERIFICAR A QUANTIDADE DE DIGITOS DO CPF, SE TIVER MENOS OU MAIS QUE 11 É INVÁLIDO*/

/****************************************************************  RELATÓRIOS  ******************************************************************/

void menuRelatorio(regProduto produto[],int *posP, int *posV, regVendas vendas[], double totalVenda[], double totalCompra[]){
	int op;	
	do{
		printf("..................................................\n");
		printf("                   RELATORIOS                       \n");
		printf("..................................................\n");
		printf("1 Produtos em falta no estoque (quantidade menor ou igual a minima)\n2 Top 10 produtos mais vendidos no mes\n3 Lucro do mes\n4 Relatorio das vendas\n5 Relatorio dos produtos ativos\n0 voltar\n");
		printf("..................................................\n"); 
		printf("\nInforme a opcao aqui: ");
		scanf("%d", &op);
		
				switch(op) {
			case 1:
				estoqueProduto (posP, produto);
				/*produtos em falta no estoque*/ 
			break;
			case 2:
				/*topProdutos ();*/
				/*top 10 produtos mais vendidos no mês*/
			break;
			case 3:
				lucroMes (posV, vendas, totalVenda, totalCompra);
			break;			
			case 4:
				relatorioVendas(posV, vendas);
				/*mostra todas as vendas cadastradas*/
			break;
			case 5:
				relatorioAtivos (posP, produto);
				/*mostrar todos os produtos ativos */
			break;
			case 0:
				return;
			default:
				printf("\nOpcao invalida!\n");
			break;
		}
	} while (op !=0);
}

/*NA FUNÇÃO estoqueProduto MOSTRA OS PRODUTOS ATIVOS QUE ESTÃO COM O ESTOQUE MENOR OU IGUAL AO ESTOQUE MÍNIMO*/
void estoqueProduto (int *posP, regProduto produto[]){
	int j, qtd_maior=0;
	
	for(j=0; j<*posP; j++){
		if(produto[j].situacao=='a'||produto[j].situacao=='A'){
			if(produto[j].qtd_min>=produto[j].qtd){
				qtd_maior=1;
				printf("\n******************************************\n");	
				printf("CODIGO: %d\nNOME: %s\nPRECO DE COMPRA: %.2f\nQUANTDADE DE ESTOQUE: %d\nQUANTIDADE MINIMA DE ESTOQUE: %d\n", 
				produto[j].codigo, produto[j].nome, produto[j].preco_compra, produto[j].qtd, produto[j].qtd_min);
			}
		}
	}
	if(qtd_maior==0){
		printf("\n\nNAO HA PRODUTOS EM FALTA NO ESTOQUE\n\n");
	}
	return;		
}


/*NA FUNÇÃO topProdutos VERIFICA OS 10 PRODUTOS MAIS VENDIDOS NO MÊS*/
/*void topProdutos (){
	int mes;

	printf("\nDigite o numero do mes: \n 01 JANEIRO\n 02 FEVEREIRO\n 03 MARCO\n 04 ABRIL\n 05 MAIO\n 06 JUNHO\n 07 JULHO\n 08 AGOSTO\n 09 SETEMBRO\n 10 OUTUBRO\n 11 NOVEMBRO\n 12 DEZEMBRO\nInfome a opcao aqui:");
	scanf("%d", &mes);
}*/

/*NA FUNÇÃO lucroMes VERIFICA O LUCRO DE UM DETERMINADO MÊS*/
void lucroMes (int *posV, regVendas vendas[], double totalVenda[], double totalCompra[]){
	int mesLucro;
	double lucroMes, totalVendaMes, totalCompraMes;
	
	printf("\nDigite o numero para saber sobre o lucro do mes: \n 01 JANEIRO\n 02 FEVEREIRO\n 03 MARCO\n 04 ABRIL\n 05 MAIO\n 06 JUNHO\n 07 JULHO\n 08 AGOSTO\n 09 SETEMBRO\n 10 OUTUBRO\n 11 NOVEMBRO\n 12 DEZEMBRO\nInfome a opcao aqui:");
	scanf("%d", &mesLucro);
	
	
	for(*posV=0; *posV<50; (*posV)++){
		if(vendas[*posV].mes==mesLucro){
			totalVendaMes=totalVendaMes+totalVenda[*posV];
			totalCompraMes=totalCompraMes+totalCompra[*posV];
		}	
	}
	printf("\n******************************************\n");
	printf("\nTOTAL PRECO DE COMPRA:%.2f\nTOTAL PRECO DE VENDA:%.2f",totalCompraMes, totalVendaMes);
	lucroMes=totalVendaMes-totalCompraMes;
	printf("\nO LUCRO DO MES %d E':%.2f\n", mesLucro, lucroMes);
}

/*NA FUNÇÃO relatorioVendas MOSTRA TOTAS AS VENDAS REALIZADAS DE UMA FORMA MAIS FÁCIL PARA O USUÁRIO TER CONTROLE DAS PESSOAS QUE FREQUENTARAM O ESTABELECIMENTO SE HOUVER ALGUM ERRO*/
void relatorioVendas(int *posV, regVendas vendas[]){
	int i, qtd_venda=0;
		
	printf("\n\nRELATORIO DAS VENDAS\n");
	for(i=0; i<*posV; i++){ 
		printf("\n******************************************\n");
		printf("\nNumero da venda:%d", vendas[i].registro+1);
		printf("\nCliente:%s", vendas[i].nomeCliente);
		printf("\nCPF do cliente: %s", vendas[i].cpf);
		printf("\nA data da venda: %d/%d/%d \n\n", vendas[i].dia, vendas[i].mes, vendas[i].ano);
		qtd_venda++;	
	}
	printf("\n\nTOTAL DE VENDAS REGISTRADAS:%d\n", qtd_venda);
	
}
/*NA FUNÇÃO relatorioAtivos MOSTRA OS PRODUTOS QUE ESTÃO DISPONÍVEIS PARA SEREM VENDIDOS E O TOTAL DE TIPOS DE PRODUTOS*/
void relatorioAtivos (int *posP, regProduto produto[]){
	int j, qtd_ativos=0;
	
	printf("\n\nRELATORIO DOS PRODUTOS ATIVOS\n");
	for(j=0; j<*posP; j++){
		if(produto[j].situacao=='a'||produto[j].situacao=='A'){
			printf("\n******************************************\n");
			printf("CODIGO DO PRODUTO..............: %d\n",produto[j].codigo);
			printf("NOME DO PRODUTO................: %s\n",produto[j].nome);
			printf("QUANTIDADE DE ESTOQUE..........: %d\n", produto[j].qtd);
			printf("QUANTIDADE MINIMA DE ESTOQUE...: %d\n",produto[j].qtd_min);
			printf("PRECO..........................: %.2f\n",produto[j].preco_compra);
			printf("PRECO DE VENDA.................: %.2f\n",produto[j].preco_venda);
			qtd_ativos++;
		}
	}
	printf("\n\nTOTAL DE PRODUTOS ATIVOS:%d\n", qtd_ativos);
	return;
}

/****************************************************************  ARQUIVOS  ******************************************************************/
/*LER*/
void CarregarArquivo(int *posP, regProduto produto [], int *posV, regVendas vendas[]){
	
	FILE *arq;
	arq=fopen("produtos.csv", "r");
	
	
	if(arq!=NULL){
		while(!feof(arq)){
			fscanf(arq,"%d;%[^;];%d;%d;%lf;%lf;%c\n", &produto[*posP].codigo, produto[*posP].nome, &produto[*posP].qtd, &produto[*posP].qtd_min, &produto[*posP].preco_compra, &produto[*posP].preco_venda, &produto[*posP].situacao);
			(*posP)++;
		}
		fclose(arq);
	}

	
	/*arq=fopen("vendas.csv", "r");
	if(arq!=NULL){
		printf("oi VENDAS\n");
		while(!feof(arq)){
			printf("oi2 VENDAS\n");
			fflush(stdin);
			fscanf(arq, "%d;%[^;];%[^;];%d;%d;%d;%d;%d\n", &vendas[*posV].registro, vendas[*posV].nomeCliente, vendas[*posV].cpf, &vendas[*posV].dia, &vendas[*posV].mes, &vendas[*posV].ano, &vendas[*posV].qtd_produtos, &vendas[*posV].codigos_produtos[vendas[*posV].qtd_produtos]);
			(*posV)++;
		}
		fclose(arq);
	}*/
}
/*ESCREVER*/
void SalvarArquivo(int posP, regProduto produto [], int posV, regVendas vendas[]){
	int j;
	
	FILE *arq;
	arq=fopen("produtos.csv", "w");
	if(arq!=NULL){
		for(j=0;j<posP; j++){
			fprintf(arq,"%d;%s;%d;%d;%.2f;%.2f;%c\n", produto[j].codigo, produto[j].nome, produto[j].qtd, produto[j].qtd_min, produto[j].preco_compra, produto[j].preco_venda, produto[j].situacao);	
		}
		fclose(arq);
	}
	
	/*arq=fopen("vendas.csv", "w");
	if(arq!=NULL){
		for(j=0;j<posV; j++){
			fflush(stdin);
			fprintf(arq, "%d;%s;%s;%d;%d;%d;%d;%d\n", vendas[j].registro, vendas[j].nomeCliente, vendas[j].cpf, vendas[j].dia, vendas[j].mes, vendas[j].ano, vendas[j].qtd_produtos, vendas[j].codigos_produtos[vendas[j].qtd_produtos]);
		}
		fclose(arq);
	}*/
}
