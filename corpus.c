#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define max 20

//structure for word and word's frequency
typedef struct word{
	char wrd[max];
	int frequency;
}Word;

//structure for binary search tree
typedef struct node{
	Word* word;
	struct node* leftLeaf;
	struct node* rightLeaf;
}WordTree;

void treeInsertion(WordTree** root, Word* node);
void treePrint(WordTree* tree, FILE* outfile);

int main(int argc, char** args){

	FILE* file;
	FILE* outfile;

	int i,idx;
	char cTmp;
	char tmp[max]={0,};
	Word* nowWord;
	WordTree* wordList=NULL;

	//book file read
	file=fopen("book_1.xml","r");

	if(file==NULL){
		printf("no file exist!!");
		exit(1);
	}

	//word reading & sorting
	while(1){

		//initialize now word
		nowWord=(Word*)malloc(sizeof(Word));

		for(i=0;i<max;i++){
			tmp[i]=0;
			nowWord->wrd[i]=0;
		}

		idx=0;

		//read one word
		while(1){
			cTmp=fgetc(file);

			//eof or null
			if(feof(file)){
				printf("finish?");
				break;
			}

			if(cTmp=='\n') continue;

			//delete tag
			if(cTmp=='<'){
				idx=0;
				while(1){
					cTmp= fgetc(file);
					if(cTmp=='>')
						break;
				}
				continue;
			}

			//delete number
			if(cTmp>='0' && cTmp<='9'){
				idx=0;
				break;
			}

			//upper to lower
			if(cTmp>='A' && cTmp<='Z'){
				cTmp = cTmp-'A'+'a';
			}

			//get only alphabet
			if(cTmp<'a' || cTmp>'z'){
				idx=0;
				break;
			}

			tmp[idx]=cTmp;
			idx++;
		}

		//end of file
		if(feof(file)){
			break;
		}

		//except empty word
		if(tmp[0]==0){
			continue;
		}

		strcpy(nowWord->wrd,tmp);

		//insert word in BST
		treeInsertion(&(wordList),nowWord);
	}

	//make output file
	outfile = fopen("output.txt","w");
	treePrint(wordList,outfile);

	fclose(file);
	fclose(outfile);

	return 0;

}


void treeInsertion(WordTree** root, Word* node){
	//insertion function

	int cmp;

	if((*root)==NULL){
		(*root)=(WordTree*)malloc(sizeof(WordTree));
		(*root)->word=node;
		(*root)->word->frequency=1;
		(*root)->leftLeaf=NULL;
		(*root)->rightLeaf=NULL;
		node=NULL;
		return;
	}

	cmp=strcmp(node->wrd,(*root)->word->wrd);

	if(cmp==0){
		(*root)->word->frequency += 1;
		free(node);
	}
	else if(cmp<0){
		treeInsertion(&((*root)->leftLeaf),node);
	}
	else{
		treeInsertion(&((*root)->rightLeaf),node);
	}

}

void treePrint(WordTree* root, FILE* outfile){
	//write output file
	//using in-order traversal

	if(root==NULL)
		return;

	treePrint(root->leftLeaf, outfile);

	fprintf(outfile,"%s : %d\n",root->word->wrd,root->word->frequency);

	treePrint(root->rightLeaf, outfile);

}



