/* MARIN Cristian-Emil GRUPA 314CB */

#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>

#define string(inf) (((TCuvant*)(inf))->cuv)
#define nap(inf) (((TCuvant*)(inf))->nr)
#define Cuvant(a) (((TCuvant*)((a)->info))->cuv)
#define nrap(a) (((TCuvant*)((a)->info))->nr)

#define NRL 26
#define MAXC 256

typedef struct celg
{ struct celg *urm;   /* adresa urmatoarei celule */
  void* info;           /* adresa informatie */
} TCelg, *TLG, **ALG; /* tipurile Celula, Lista si Adresa_Lista */

typedef struct { 
	char *cuv;		/* adresa cuvant*/
	int nr; 		/* numarul de aparitii ale cuvantului*/
} TCuvant;

void Destroy(ALG);  /* Distruge celule si elemente */
size_t LengthList(ALG);   /* Intoarce lungimea listei*/
void PrintList(TLG, void (*afiEl)(void*, ALG a,size_t contor), size_t contor); /* Afiseaza lista utilizand functia de afisare afiEL*/
void PrintWord(void *p, ALG a, size_t contor); /*Functie de afisare a cuvantului din lista*/
int CompCuv(void *a, void *b);
/* Functie de comparatie intre doua elemente. Intoarce <0 daca elementul a trebuie sa se afle inaintea lui b, 0 daca sunt egale
	si >0 daca elementul b trebuie sa se afle inaintea lui a*/
int Insert(ALG,void*);
/* Functie de inserare ordonata (utilizand functia CompCuv) a unui element in lista
	Intoarce 1 daca s-a inserat elementul, 2 daca exista deja in lista si doar se incrementeaza numarul de aparitii ale elementului
	si -1 daca nu reuseste alocarea dinamica de memorie*/
int Remove(ALG,void*);
/* Functie de eliminare a unui element din lista. Intoarce 0 daca elementul nu este gasit in lista, 1 daca elementul se elimina
	si 2 daca elementul se gaseste de mai multe ori in lista si doar se decrementeaza numarul de aparitii ale elementului.*/


int main(int argc, char* argv[])
{	
	TLG Dict[NRL]={0};
	char line[MAXC]={0},*word=NULL;
	TCuvant *x=NULL;
	int ch=0,ok=1,r=0,op=0,nr=0;
	size_t i=0;
	FILE *f;
	f = fopen(argv[1],"rt");
	if(!f)	return -2;
	for(i=0;i<NRL;++i)
	{	/*aloc spatiu pentru fiecare lista din vectorul Dict si initializez campurile cuv si nr cu NULL si respectiv, 0*/
		Dict[i]=(TLG)malloc(sizeof(TCelg));
		if(!Dict[i])
			return -1;
		Dict[i]->info=(TCuvant*)malloc(sizeof(TCuvant));
		if(!Dict[i]->info)
			return -1;
		Cuvant(Dict[i])=NULL;
		nrap(Dict[i])=0;
	}
	
	while(fgets(line, MAXC - 1, f))
	{	
		line[strlen(line)-1]='\0';
		word = strtok(line," ,.-;?!:'{}[]()@\"/+=_`~#$%^&*|<>");  
		while(word != NULL)
		{	ok = 1;
			for(i=0;i<strlen(word);i++)
				if(!isalpha(word[i]))
				{	ok = 0;
					break;
				}
			if(word!=NULL && strlen(word)>=3 && ok)
			{	++nr;
				x = (TCuvant*)malloc(sizeof(TCuvant));
				if(!x) return -1;
				x->cuv = (char*)malloc(strlen(word)+1);
				if(!x->cuv) return -1;
				strcpy(x->cuv,word);
				ch = tolower(word[0])-'a';
				Insert(&Dict[ch],x->cuv);
			}
			word = strtok(NULL," ,.-;?!:'{}[]()@\"/+=_`~#$%^&*|<>");
		}
	}
	for(i=0;i<NRL;++i)
	{	
		if(Dict[i]&&Cuvant(Dict[i]))
		{	
			printf("%c: ", i+65);
			PrintList(Dict[i],PrintWord,0);
		}
	}
	printf("\nS-au citit %i (de) cuvinte.\n",nr);
	
	for(;;)
	{	fflush(stdin);
		printf("\nIntroduceti optiunea dorita: 0 - afisare dictionar, 1 - adaugare, 2 - eliminare\n\n");
		
		if(scanf("%i",&op)==1)
		{
			if(op == 0)
				for(i=0;i<NRL;i++)
				{	
					if(Dict[i]&&Cuvant(Dict[i]))
					{	
						printf("%c: ", i+65);
						PrintList(Dict[i],PrintWord,0);
					}
				}
			else
				if(op == 1)
				{	
					ok = 1;
					printf("\nIntroduceti cuvantul pe care doriti sa il inserati in dictionar:\n\n");
					scanf("%s",line);
					for(i=0;i<strlen(line);++i)
						if(!isalpha(line[i]))
						{	
							printf("\nCuvant invalid. Introduceti un cuvant valid!\n\n");
							ok = 0;
							break;
						}
					if(ok)
					{
						if(strlen(line)>=3)
						{
							x = (TCuvant*)malloc(sizeof(TCuvant));
							if(!x) return -1;
							x->cuv = (char*)malloc(strlen(line)+1);
							if(!x->cuv) return -1;
							strcpy(x->cuv,line);
							ch = tolower(line[0])-'a';
							r = Insert(&Dict[ch],x->cuv);
							if(r==1)
							{
								printf("\nCuvantul '%s' a fost adaugat!\n\n",line);
								continue;
							}
							if(r==2)
							{
								printf("\nNr. de aparitii ale cuvantului '%s' a fost incrementat!\n\n",line);
								continue;
							}
						}
						else
							printf("\nCuvant invalid. Introduceti un cuvant valid!\n\n");
					}
				}
				else
					if(op == 2)
					{	
						ok = 1;
						printf("\nIntroduceti cuvantul pe care doriti sa il eliminati din dictionar:\n\n");
						scanf("%s",line);
						for(i=0;i<strlen(line);++i)
							if(!isalpha(line[i]))
							{	
								printf("\nCuvant invalid. Introduceti un cuvant valid!\n\n");
								ok = 0;
								break;
							}
							if(ok)
							{	
								ch = tolower(line[0])-'a';
								r=Remove(&Dict[ch],line);
								if(r==1)
								{
									printf("\nCuvantul '%s' a fost eliminat!\n\n",line);
									continue;
								}
								if(r==2)
								{
									printf("\nNr. de aparitii ale cuvantului '%s' a fost decrementat!\n\n",line);
									continue;
								}
								if(r==0)
								{
									printf("\nCuvantul '%s' nu a fost gasit in dictionar!\n\n",line);
									continue;
								}
							}
					
					}
					else
					{	printf("\nOptiune invalida! Doriti sa continuati sau sa iesiti din program? [D/]\n\n");
						if(toupper(getch())!= 'D') 
						{	
							printf("\nLa revedere!\n\n");
							break;
						}
					}
		}
		else
		{	printf("\nOptiune invalida! Doriti sa continuati sau sa iesiti din program? [D/]\n\n");
			if(toupper(getch())!= 'D') 
			{	
				printf("\nLa revedere!\n\n");
				break;
			}
		}
	}
	
	for(i=0;i<NRL;++i) /* Eliberez spatiul de memorie ocupat de vectorul de liste Dict*/
		Destroy(&Dict[i]);
	
	fclose(f);
	getch();
	return 0;
}

int Insert(ALG list,void* word)
{	/* Functie de inserare ordonata (utilizand functia CompCuv) a unui element in lista
	Intoarce 1 daca s-a inserat elementul, 2 daca exista deja in lista si doar se incrementeaza numarul de aparitii ale elementului
	si -1 daca nu reuseste alocarea dinamica de memorie*/
	TLG list2=*list,aux=NULL,aux2=NULL;
	if(!Cuvant(*list))	/*daca lista este goala insereaza element in lista*/
	{	
		Cuvant(*list)=word;
		nrap(*list)=1;
		(*list)->urm=NULL;
		return 1;
	}
	for(;list2&&(aux=list2);list2=list2->urm)	/*parcurge lista*/
		if(!strcmp(Cuvant(list2),word))	/*daca elementul exista deja in lista incrementeaza numarul de aparitii*/
		{	
			++nrap(list2);
			return 2;
		}
	list2=*list,aux=NULL;
	for(;list2&&(CompCuv(Cuvant(list2),word)<0)&&(aux=list2);list2=list2->urm);
	/*parcurge lista cat timp elementele sunt mai mici decat word
	Daca a gasit element mai mare decat word, inseareaza word inaintea elementului*/
	aux2=(TCelg*)malloc(sizeof(TCelg));
	if(!aux2)
		return -1;
	aux2->info=(TCuvant*)malloc(sizeof(TCuvant));
	if(!aux2->info)
		return -1;
	Cuvant(aux2)=word;
	nrap(aux2)=1;
	aux2->urm=list2;
	if(aux)
		aux->urm=aux2;
	else
		*list=aux2;
	return 1;
}

int Remove(ALG list, void* word)
{	/*Functie de eliminare a unui element din lista. Intoarce 0 daca elementul nu este gasit in lista, 1 daca elementul se elimina
	si 2 daca elementul se gaseste de mai multe ori in lista si doar se decrementeaza numarul de aparitii ale elementului.*/
	TLG list2=*list,aux=NULL;
	
	if(!Cuvant(*list)) /* Daca lista este goala intoarce 0*/
		return 0;
	for(;list2;list2=list2->urm)
	{	
		if(!strcmp(Cuvant(list2),word)) /* Daca s-a gasit elementul cautat in lista*/
		{	
			if(--nrap(list2)) /* Daca elementul se afla de mai multe ori in lista se decrementeaza numarul de aparitii*/
				return 2;
			break; /* Daca elementul avea numarul de aparitii egal cu 1 se va elimina din lista*/
		}
		aux=list2;
	}

	if(list2)
	{	if(aux) /* Daca lista are mai multe elemente diferite*/
			aux->urm=list2->urm;
		else /* Daca lista contine doar un element cu nr. de aparitii egal cu 1*/
			*list=list2->urm;
		free(list2->info);	/* Elibereaza zona de memorie ocupata de elementul care a fost eliminat din lista*/
		free(list2);
		return 1;
	}	
	return 0;
}

int CompCuv(void *a, void *b)
{	/* Functie de comparatie intre doua elemente. Intoarce <0 daca elementul a trebuie sa se afle inaintea lui b, 0 daca sunt egale
	si >0 daca elementul b trebuie sa se afle inaintea lui a*/
	int r = 0;
	r = *(char*)a - *(char*)b; /* Daca unul din cuvinte incepe cu litera mare, cuvantul mai mic este cel care incepe cu litera mica*/
	if(r != 0)
		return -r;
	return strcmp(a, b);
}

void PrintWord(void *p, ALG a, size_t contor) /* Functie de afisare a cuvantului din lista*/
{ 	contor++;
	if(contor == (LengthList(a)))	/* Daca s-a ajuns la final nu mai afisez ',' dupa element*/ 
		printf("%s (%i)", string(p),nap(p));
	else
		printf("%s (%i), ", string(p),nap(p));
}

void PrintList(TLG a, void (*afiEl)(void*,ALG a,size_t contor), size_t contor) /* Afiseaza lista utilizand functia de afisare afiEL*/
{	for (; a != NULL; a = a->urm)
		afiEl(a->info, &a, contor);
	putchar('\n');
}

size_t LengthList(ALG a)	/* Intoarce lungimea listei*/
{	size_t lg = 0;
    TLG p = *a;
    for (; p != NULL; p = p->urm)
		lg++;
	return lg;
}

void Destroy(ALG a)  /* Distruge celule si elemente*/
{	TLG aux;
	while (*a != NULL)     
	{	
		aux = *a;            
		*a = aux->urm;       
		free(aux->info);     
		free(aux);           
    }
}
