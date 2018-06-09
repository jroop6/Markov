#include <cstdlib>
#include <cstdio>
#include <string>
#include <cstdlib>
#include <forward_list>
#include <ctime>
#include <map>
#include <fstream>
#include <list>
#include "Random.h"
#include "Random.cpp"
#include "Prefix.h"
#include "Prefix.cpp"
#include "Suffix.h"
#include "Suffix.cpp"
#include "StringChain.h"
#include "StringChain.cpp"


void printUsage(){
	printf("*************************************************************************************************************\n");
	printf("* Markov.exe <order> <numGen> <\"word\"/\"char\"> <filename>                                                    *\n");
	printf("*                                                                                                           *\n");
	printf("*    <order> - (int) The order of the Markov Chain                                                          *\n");
	printf("*    <numGen> - (int) The number of items that should be generated in the output                            *\n");
	printf("*    <\"word\"/\"char\"> - (c-string) specifies whether words or characters should be used for Markov chaining. *\n");
	printf("*    <filename> - (c-string) the name of the first text file to be used for generating the random text.     *\n");
	printf("*                                                                                                           *\n");
	printf("* Additional filenames may be specified after the first <filename>                                          *\n");
	printf("*************************************************************************************************************\n");
}

char MarkovOption[] = "word";

int main(int argc, char * argv[]){
	//parse arguments
	if(argc<5){
		printf("\nerror! at least 4 command-line arguments are required. Usage:\n");
		printUsage();
		return -1;
	}
	int order = strtol(argv[1],nullptr,10);
	if(order == 0){
		printf("\nerror! Invalid command-line argument for <order>. Usage:\n");
		printUsage();
		return -1;
	} 
	int numGen = strtol(argv[2],nullptr,10);
	if(numGen == 0){
		printf("\nerror! Invalid command-line argument for <numGen>. Usage:\n");
		printUsage();
		return -1;
	} 
	if(!strcmp(argv[3],"word"));
	else if (!strcmp(argv[3],"char")){
		strcpy(MarkovOption,"char");
		printf("changed option to char\n");
	} 
	else{
		printf("\nerror! Invalid command-line argument for <\"word\"/\"char\">. Usage:\n");
		printUsage();
		return -1;
	} 
	string filename;
	StringChain stringChain(order);

	for(int i=0; i<argc-4; ++i){
		filename = argv[4+i];
		printf("reading file: %s\n",filename.c_str());
		fstream fid(filename.c_str());
		stringChain.addItems(fid);
		fid.close();
	}

	Random randr;
	stringChain.generate(numGen,randr);
	stringChain.deleteMap();
}

