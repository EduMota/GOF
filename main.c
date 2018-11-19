/*****************************************************
 	*
 *	File Name: test.c
 *
 *	Description:
 *
 *	Version: 1.0
 *	Created: 19-10-2017 17:52:16
 *	Revision: none
 *	Compiler: gcc
 *
 *	Author: Eduardo Mota Nascimento, edu.mota2121@gmail.com
 *  		Mateus Henrique Silva da Costa, mhenrique977@gmail.com
 *			Murilo Portilho Acsonov, murilloacsonovv@hotmail.com
 *
 *	Organization:
 *
 ****************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>

#define WIDTH 55
#define HEIGHT 15

int startx = 1;
int starty = 1;

char *choices[] = {
			"Definir tamanho do mundo ",
			"Selecionar os seres vivos da primeira geracao",
			"Limpar todas as posicoes",
			"Iniciar simulacao",
			"Definir velocidade",
			"Determinar numero de geracoes",
			"Simular passo-a-passo",
			"Selecionar padrao",
			"Salvar geracao em arquivo",
			"Carregar geracao pre-armazenada",
			"Exit",
};
char *patterns[]={
			"Blinker",
			"Glider",
			"Ship",
			"Boom"
};

int n_choices = sizeof(choices) / sizeof(char *);
int n_patterns = sizeof(patterns)/ sizeof(char *);

void geracaoPadrao(WINDOW *win, int **mapaPonteiro, int *linha, int *coluna);					// 	padroes pre determinados
void  simulaPasso ( int **mapaPonteiro, int *linha, int *coluna, int **vetAux, int g);			//	simula passo passo
int  geracao ( WINDOW *win );																	//	recebe qtd geracao
void  contViz (int **mapaPonteiro, int *linha, int *coluna, int i, int j, int **vetorAux);      // 	conta vizinhos
void vetorAux(int **mapaPonteiro, int *linha, int *coluna);										// 	cria vetor auxiliar
int  setVel( WINDOW *win );													   					//	recebe valor de velocidade
void loadFile( WINDOW *win, int **mapaPonteiro, int *linha, int *coluna );						//	lê matriz salva em arquivo
void saveFile( WINDOW *win, int **mapaPonteiro , int *linha, int *coluna );	    				//	salva matriz em arquivo
void simula( int **mapaPonteiro, int *linha, int *coluna, int **vetorAux, int vel, int g );		//	simula varias gerações
void printMenu( WINDOW *menu_win, int highlight, char *array[] );								//	cria menu
void setVector( int **mapaPointer, int *linha, int *coluna );									//	seta padrão não existente
void delMenu( WINDOW *menu_win );																//	deleta menu
void recebeNumCampos( WINDOW *win, int *linha, int *coluna );									//	função de receber numeros
int **zerarMatriz( int *linha, int *coluna );													//	função de alocar memoria para a matriz e seta-la como zero
void mostrarMatriz( WINDOW *win, int **mapaPonteiro, int *linha, int *coluna );					//	mostrara matriz
void menu( int *linha, int *coluna, int **mapaPonteiro, int **vetorAux );						//	menu

/*** FUNCTION **************************************
 *	Name:			main
 *	Description:
 ***************************************************/
int main(){

		int linha;
		int coluna;

		int **mapaPonteiro;
		int **vetorAux;

		menu(&linha, &coluna, mapaPonteiro,vetorAux);

		return 0;
}

/*** FUNCTION **************************************
 *	Name:			menu
 *	Description:
 ***************************************************/
void menu(int *linha, int *coluna, int **mapaPonteiro, int **vetorAux){

     WINDOW *menu_win;
     int highlight = 1;
     int choice = 0;
     int c;
	 int vel;
	 int g;

     initscr();


	 menu_win = newwin(HEIGHT, WIDTH, starty, startx);
	 box( menu_win, 0, 0  );
	 keypad(menu_win,TRUE);
     refresh();

     do {

        printMenu(menu_win,highlight, choices);
        c = wgetch(menu_win);

       switch(c) {
           case KEY_UP:
             if(highlight == 1)
                 highlight = n_choices;
             else
                 --highlight;
             break;
            case KEY_DOWN:
              if(highlight == n_choices)
                  highlight = 1;
               else
                  ++highlight;
               break;
            case 10:
                choice = highlight;

                 	if(choice==1){			//Recebe QTD de linhas e colunas
						recebeNumCampos( menu_win, linha, coluna );
						mapaPonteiro = zerarMatriz ( linha, coluna );
						vetorAux = zerarMatriz(linha, coluna);
                   	}
				   	if(choice==2){			//Usuário seta posições vivas na Matriz
                      	wclear(menu_win);
						box( menu_win, 0, 0 );
						wrefresh( menu_win );
						setVector ( mapaPonteiro, linha, coluna );
                      	keypad(menu_win,TRUE);
                      	refresh();
                   	}
				   	if(choice==3){			//Zera todas a posições da Matriz
                      	wclear( menu_win );
						box( menu_win, 0, 0 );
						mapaPonteiro = zerarMatriz( linha, coluna );
						refresh();
                   	}
				   	if(choice==4){			//Inicia simulação
	                    wclear( menu_win );
						box( menu_win ,0 ,0 );
						wrefresh( menu_win );
						simula ( mapaPonteiro, linha, coluna, vetorAux, vel, g );
                      	refresh();
                   	}
				   	if(choice==5){			//Define velocidade de simulação
	                   vel = setVel( menu_win );
                   	}
				   	if(choice==6){			//Determinar número de gerações
						g = geracao( menu_win  );
                   	}
					if(choice==7){			//Simulação passo-a-passo
					    wclear( menu_win );
						box( menu_win ,0 ,0 );
						wrefresh( menu_win );
					  	simulaPasso ( mapaPonteiro, linha, coluna, vetorAux,g );
						refresh();
                   	}
					if(choice==8){			//Selecionar padrões pré-determinados
						wclear(menu_win);
						wrefresh(menu_win);
						mapaPonteiro = zerarMatriz ( linha, coluna );
						geracaoPadrao(menu_win, mapaPonteiro, linha, coluna);
                      	refresh();
                   	}
					if(choice==9){			//Salva Matriz em arquivo
   						saveFile( menu_win, mapaPonteiro, linha, coluna );
                    	refresh();
                   	}
					if ( choice ==10 ){		//Carrega Matriz salva em arquivo
						loadFile( menu_win, mapaPonteiro, linha, coluna );
						refresh();
					}
                   	if(choice==11){
                      goto sair;
                   }
               break;
            default:
                  refresh();
               break;
           }

        }while(1);

     sair:
        refresh();
        endwin();

}

/*** FUNCTION **************************************
 *	Name:	delMenu
 *	Description:
 ***************************************************/
void delMenu(WINDOW *menu_win){

	wclear(menu_win);
    wborder(menu_win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
    wrefresh(menu_win);
    delwin(menu_win);
}

/*** FUNCTION **************************************
 *	Name:	printMenu
 *	Description:
 ***************************************************/
void printMenu(WINDOW *menu_win, int highlight, char *array[]){

	int x, y, i, n_array;

	curs_set(FALSE);

	x = 2;
	y = 2;
	
	
	if ( array == choices )
		
			n_array = n_choices;

	else
			n_array = n_patterns;
		
	box(menu_win, 0, 0);

	for(i = 0; i < n_array; ++i){
			if(highlight == i + 1){
					wattron(menu_win, A_REVERSE);
					mvwprintw(menu_win, y, x, "%s", array[i]);
					wattroff(menu_win, A_REVERSE);
			}else
					mvwprintw(menu_win, y, x, "%s", array[i]);
			++y;
	}
	wrefresh(menu_win);
}

/********************* FUNCTION *********************
 *	Name:  setVel
 *	Description:
 ****************************************************/
int setVel( WINDOW *win ) {

		int vel;

		wclear( win );
		curs_set( TRUE );
		box( win, 0, 0  );
		mvwprintw( win,starty+2,startx+1,"Velocidade:__" );

		echo();
		wmove( win, starty+2, startx+12 );
		wscanw( win, "%d",&vel );
		wrefresh(win);

		return vel;

}		/* -----  end of function setVel  ----- */

/*** FUNCTION **************************************
 *	Name:	recebeNumCampos
 *	Description: Seta numero de linhas e colunas
 *	da Matriz
 ***************************************************/
void recebeNumCampos( WINDOW *win, int *linha, int *coluna ){

	//WINDOW *janela;
	curs_set(TRUE);												//seta cursor

	//janela = newwin(HEIGHT, WIDTH, starty,startx);				//cria janela

	int total;
	total = 0;

	while( total < 50 || total > 100 ){

		wclear( win );
//		wborder( win, ACS_VLINE, ACS_VLINE,ACS_HLINE,ACS_HLINE, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
		box ( win, 0 , 0 );

		/* Labels */
		mvwprintw( win, starty+1, startx+1, "Linhas.:__" );	//printa label
		mvwprintw( win, starty+2, startx+1, "Colunas.:__" );//printa label

		echo();
		wmove(win,starty+1,startx+9);						//posiciona cursor
		wscanw(win,"%d",linha);								//recebe valor

		wmove(win,starty+2,startx+10);						//posiciona cursor
		wscanw(win,"%d",coluna);							//recebe valor

		total = *linha * *coluna;

		/* Valida tamanho do mundo */
		if( total < 50 || total > 100 ){

			mvprintw(2,3,"Numero informado invalido!");
			mvprintw(3,3,"Mundo deve ter um numero maior(igual) a 50 e ");
			mvprintw(4,3,"menor(igual) a 100 ");
			refresh();
			sleep(3);

		}

		wclear(win);

	}


}

/*** FUNCTION **************************************
 *	Name:	zerarMatriz
 *	Description: Seta toda a matriz com 0
 ***************************************************/
int **zerarMatriz(int *linha, int *coluna){

	int i,k;

  	int **mapaPonteiro; 										// matriz ponteiro de 2 dimensoes

  	mapaPonteiro = (int **) malloc( *linha * sizeof(int *)); 	//aloca um vetor só de ponteiro

  	for (i = 0; i < *linha; i++){ 								//linhas do vetor ponteiro

       mapaPonteiro[i] = (int *) malloc( *coluna * sizeof(int));//aloca um vetor de int's para cada posição do vetor de ponteiro

       for (k = 0; k < *coluna; k++){ 							//andar nas colunas do vetor de inteiros
            mapaPonteiro[i][k] = 0 ; 							//define 0
       }
  }

	return mapaPonteiro; 										//retorna a matriz de zeros

}


/********************* FUNCTION *********************
 *	Name:  setVector
 *	Description: Seta um padrão não existente
 *	na matriz a partir do teclado
 ****************************************************/
void  setVector (int **mapaPonteiro, int *linha, int *coluna){

		WINDOW *janela;
		curs_set(TRUE);

		janela = newwin( *linha+2, *coluna+2, (HEIGHT/2)-(*linha/2), (WIDTH/2)-(*coluna/2) );
		keypad(janela, TRUE);


		int i,j,c,ci,cj;
		ci=1;
		cj=1;

		do {
				box ( janela, 0, 0 );


				mostrarMatriz ( janela, mapaPonteiro, linha, coluna );

         		echo();
         		wmove(janela,ci,cj);
				c = wgetch(janela);

				switch ( c ) {                            //controla o cursor, altera matriz  e sai
						case KEY_UP:

								if ( ci == 1 )
										ci = *linha ;
								else
										ci--;
								break;

						case KEY_DOWN:

								if ( ci == *linha  )
										ci = 1;

								else
										ci++;

								break;

						case KEY_LEFT:

								if ( cj == 1 )
										cj = *coluna;

								else
										cj--;

								break;

						case KEY_RIGHT:

								if ( cj == *coluna  )
										cj = 1;

								else
										cj++;

								break;

						case 10:

								if ( mapaPonteiro[ci-1][cj-1] == 1 )
										mapaPonteiro[ci-1][cj-1] = 0;

								else
										mapaPonteiro[ci-1][cj-1] = 1;

        						break;



						default:
								goto sair;

								break;
				}				/* -----  end switch  ----- */

		  		wrefresh(janela);



		}while( 1 );
	sair:
		delMenu(janela);

}		/* -----  end of function setVector  ----- */

/********************* FUNCTION *********************
 *	Name:  contViz
 *	Description: conta numero de vizinhos
 ****************************************************/

void  contViz (int **mapaPonteiro, int *linha, int *coluna, int i, int j,int **vetorAux){

		/* como a função quando for chamada percorre e altera o estado de cada
		 * campo da matriz, faz-se necessário armazenar o estado de cada campo
		 * antes de alterar a matriz, para que possa ser verificado
		 */

		int viz = 0;
		int life = 1;

		viz += vetorAux [ ( i - 1 + *linha ) % *linha ] [ ( j - 1 + *coluna ) % *coluna ];	//coluna anterior, linha anterior
		viz += vetorAux [ ( i - 1 + *linha ) % *linha ] [ j ];								//mesma coluna, linha anterior
		viz += vetorAux [ ( i - 1 + *linha ) % *linha ] [ ( j + 1 + *coluna ) % *coluna ];	//coluna posterior, linha anterior
		viz += vetorAux [ i ] [ ( j - 1 + *coluna ) % *coluna ];							//coluna anterior, mesma linha
		viz += vetorAux [ i ] [ ( j + 1 + *coluna ) % *coluna ];							//coluna posterior, mesma linha
		viz += vetorAux [ ( i + 1 + *linha ) % *linha ] [ ( j - 1 + *coluna ) % *coluna ];	//coluna anterior, linha posterior
		viz += vetorAux [ ( i + 1 + *linha ) % *linha ] [ j ];								//mesma coluna, linha posterior
		viz += vetorAux [ ( i + 1 + *linha ) % *linha ] [ ( j + 1 + *coluna ) % *coluna ]; 	//coluna posterior, linha posterior

		if ( vetorAux[i][j] == 1 ){				//se estiver vivo

				if ( viz < 2 || viz > 3 )			//e vizinhos igual a 2 ou 3

						life = 0;					//continua vivo

		}else{										//se estiver morto

				if ( viz != 3 )						// e viz = 3

						life = 0;					// nasce
		}

		mapaPonteiro[i][j] = life;

}		/* -----  end of function contViz  ----- */

/*** FUNCTION **************************************
 *	Name:	mostrarMatriz
 *	Description:
 ***************************************************/
void mostrarMatriz( WINDOW *win, int **mapaPonteiro, int *linha, int *coluna ){

		int i, k;

		for (i = 0; i < *linha; i++) 											//anda nas linhas da matriz
				for (k = 0; k < *coluna; k++){ 									//anda nas colunas da matriz

				 		if(mapaPonteiro[i][k] == 0 ) 							//verifica se está morto
					   			mvwprintw ( win, starty+i, startx+k, " ");		// printa mortas
						else
								mvwprintw ( win, starty+i, startx+k,"#");		// printa vivas
				}

}

/********************* FUNCTION *********************
 *	Name:  geracao
 *	Description: define numero de geracoes
 ****************************************************/
int  geracao ( WINDOW *win ) {

		int g;
		wclear( win );
		curs_set( TRUE );
		box( win, 0, 0  );
		mvwprintw( win,starty+2,startx+1,"Geracao:__" );

		echo();
		wmove( win, starty+2, startx+9 );
		wscanw( win, "%d",&g );
		wrefresh(win);

		return g;
}		/* -----  end of function geracao  ----- */
/********************* FUNCTION *********************
 *	Name:  simula
 *	Description:
 ****************************************************/
void  simula (int **mapaPonteiro, int *linha, int *coluna, int **vetorAux, int vel, int g){

		WINDOW *janela;
		curs_set(FALSE);

		janela = newwin( *linha+2, *coluna+2, (HEIGHT/2)-(*linha/2), (WIDTH/2)-(*coluna/2) );
		box(janela, 0, 0);

		while( g!=0 ){
			int i, k;

			for ( i = 0 ; i < *linha ; i++)
					for ( k = 0 ; k < *coluna ; k++)
							vetorAux[i][k] = mapaPonteiro[i][k];

			for ( i = 0 ; i < *linha ; i++ )
					for ( k = 0 ; k < *coluna ; k++ )
							contViz ( mapaPonteiro, linha, coluna, i, k, vetorAux);

			mostrarMatriz(janela, mapaPonteiro, linha, coluna);
			g--;

			wrefresh(janela);
			usleep(vel*100000);

		}

		delMenu(janela);
}		/* -----  end of function simula  ----- */

/********************* FUNCTION *********************
 *	Name:  simulaPasso
 *	Description: run step by step
 ****************************************************/
void  simulaPasso ( int **mapaPonteiro, int *linha, int *coluna, int **vetAux, int g) {

		WINDOW *janela;
		curs_set(FALSE);

		janela = newwin( *linha+2, *coluna+2, (HEIGHT/2)-(*linha/2) , (WIDTH/2)-(*coluna/2) );
		box(janela, 0, 0);


		int c,g1 = g;

		while( g1!=0  ){
			int i, k;

			for ( i = 0 ; i < *linha ; i++)
					for ( k = 0 ; k < *coluna ; k++)
							vetAux[i][k] = mapaPonteiro[i][k];

			for ( i = 0 ; i < *linha ; i++ )
					for ( k = 0 ; k < *coluna ; k++ )
							contViz ( mapaPonteiro, linha, coluna, i, k, vetAux);

			mostrarMatriz(janela, mapaPonteiro, linha, coluna);
			g1--;
			wrefresh(janela);
			c = wgetch( janela  );

		}
		delMenu( janela  );
}		/* -----  end of function simulaPasso  ----- */

/********************* FUNCTION *********************
 *	Name:  saveFile
 *	Description:	Salva matriz em um arquivo
 ****************************************************/
void  saveFile ( WINDOW *win, int **mapaPonteiro , int *linha, int *coluna ) {

		FILE *file;

		file = fopen ( "cache.txt", "w" );												// cria ou abre arquivo


		if ( file == NULL  ){

				mvwprintw ( win, starty+1, startx+1, "Error 01" );						// erro caso não consiga abrir ou criar arquivo

				sleep ( 2 );

		}else
				fwrite ( mapaPonteiro, sizeof ( int ), ( *linha * *coluna ), file );		// escreve Matriz no arquivo

		fclose ( file );																// fecha arquivo

}		/* -----  end of function saveFile  ----- */

/********************* FUNCTION *********************
 *	Name:  loadFile
 *	Description:
 ****************************************************/
void  loadFile ( WINDOW *win, int **mapaPonteiro, int *linha, int *coluna ) {

		FILE *file;

		file = fopen ( "cache.txt", "r" );

		if ( file == NULL ){

				mvwprintw ( win, starty+1, startx+1, "Error 02" );
				sleep ( 2 );

		}else
				fread ( mapaPonteiro, sizeof ( int ), ( *linha * *coluna ), file );

		fclose ( file );
}		/* -----  end of function loadFile  ----- */


void vetorAux(int **vetorAuxiliar, int *linha, int *coluna){


	vetorAuxiliar = (int **) malloc( *linha * sizeof(int *));

	for(int i = 0; i < *coluna;i++){
		vetorAuxiliar = (int **) malloc( *coluna * sizeof(int *));
	}
}

/********************* FUNCTION *********************
 *	Name:  Geracoes Padroes
 *	Description: Cria geracoes padroes
 ****************************************************/
void geracaoPadrao(WINDOW *win, int **mapaPonteiro, int *linha, int *coluna){

	int xmeio = (*coluna/2)-1;
	int ymeio = (*linha/2)-1;

	int highlight = 1;
	int pattern = 0;
	int c;

	zerarMatriz(linha, coluna);

	wclear(win);
	keypad(win,TRUE);

	box(win,0,0);

	do{

		//mvwprintw ( win, starty+1, startx+1, "Escolhe o padrao desejado " );
		printMenu(win, highlight, patterns);
		c = wgetch(win);

		switch(c) {
			case KEY_UP:
			  if(highlight == 1)
				  highlight = n_patterns;
			  else
				  --highlight;
			  break;
			 case KEY_DOWN:
			   if(highlight == n_patterns)
				   highlight = 1;
				else
				   ++highlight;
				break;
			 case 10:
				 pattern = highlight;

					 if(pattern==1){			//Blinker
						 	mapaPonteiro[ymeio][xmeio-1] 	= 1;
							mapaPonteiro[ymeio][xmeio] 		= 1;
							mapaPonteiro[ymeio][xmeio+1]	= 1;
					 }
					 if(pattern==2){			//Glider
						 	mapaPonteiro[ymeio-1][xmeio-1]	= 1;
							mapaPonteiro[ymeio-1][xmeio]	= 1;
							mapaPonteiro[ymeio-1][xmeio+1]	= 1;
							mapaPonteiro[ymeio][xmeio-1]	= 1;
							mapaPonteiro[ymeio+1][xmeio]	= 1;
					 }
					 if(pattern==3){			//Ship
						 	mapaPonteiro[ymeio-1][xmeio-1]	= 1;
							mapaPonteiro[ymeio][xmeio-1]	= 1;
							mapaPonteiro[ymeio-1][xmeio]	= 1;
							mapaPonteiro[ymeio+1][xmeio]	= 1;
							mapaPonteiro[ymeio][xmeio+1]	= 1;
							mapaPonteiro[ymeio+1][xmeio+1]  = 1;
					 }
					 if(pattern==4){			//Boom
						 	mapaPonteiro[ymeio][xmeio-1]	= 1;
							mapaPonteiro[ymeio-1][xmeio]	= 1;
							mapaPonteiro[ymeio][xmeio]		= 1;
							mapaPonteiro[ymeio+1][xmeio]	= 1;
							mapaPonteiro[ymeio][xmeio+1]	= 1;
					 }
				break;
			 default:
				   refresh();
				break;
			}
		}while (c!=10);

}

