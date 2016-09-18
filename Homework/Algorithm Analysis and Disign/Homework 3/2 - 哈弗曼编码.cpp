#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cstdbool>

using namespace std;

struct HuffmanNode * minValue(struct HuffmanNode * freqTree);
void printTree(struct HuffmanNode * treeNode, int layer, char a[]);

struct HuffmanNode{ //在编排哈夫曼树时所用的节点
	char text; //字符内容
	int weight; //出现频率
	bool leaf; //该节点在哈夫曼树中是否是叶子节点
	struct HuffmanNode * lchild; //左孩子
	struct HuffmanNode * rchild; //右孩子
	struct HuffmanNode * next; //在链表中指向下一个节点
};

struct HuffmanCode{ //27个字符的哈夫曼编码
	char code[20]; //编码
	int length; //编码长度
};

int value = 0; //编码总长度
struct HuffmanCode code[27]; //27个字符的哈夫曼编码（包含26个小写字母和'#'号）

int main(void){
	//首先将编码源文字读出
	FILE * fileOne;
	fileOne = fopen("1.txt", "r");
	char text[1020];
	fgets(text, 1020, fileOne);
	printf("%s", text);

	//然后定义一个哈夫曼编码的链表并初始化
	struct HuffmanNode * freqTree = new HuffmanNode;
	freqTree->weight = INFINITY;
	freqTree->text = 0;
	freqTree->next = NULL;

	struct HuffmanNode * lastNode = freqTree;
	
	//接下来将所有的字符都读入链表中，每一种字符对应一个节点
	for (int i = 0; text[i] != '\n'; i++){
		while (lastNode->next != NULL){
			if (lastNode->next->text == text[i])
				break;
			lastNode = lastNode->next;
		}
		if (lastNode->next == NULL){
			lastNode->next = new HuffmanNode;
			lastNode = lastNode->next;
			lastNode->leaf = true;
			lastNode->text = text[i];
			lastNode->weight = 1;
			lastNode->lchild = NULL;
			lastNode->rchild = NULL;
			lastNode->next = NULL;
		}
		else {
			lastNode->next->weight++;
		}
		lastNode = freqTree;
	}
	while (lastNode->next != NULL){
		cout << lastNode->next->text << ' ' << lastNode->next->weight << endl;;
		lastNode = lastNode->next;
	}

	//将哈夫曼链表按照频率重新组合成哈夫曼树
	lastNode = freqTree;
	while (lastNode->next != NULL){
		//在链表里找到最小频率的节点
		struct HuffmanNode * minLastNode1 = minValue(freqTree);
		struct HuffmanNode * minValueNode1 = minLastNode1->next;
		minLastNode1->next = minLastNode1->next->next;
		//去掉最小以后找到次小频率的节点
		struct HuffmanNode * minLastNode2 = minValue(freqTree);
		struct HuffmanNode * minValueNode2 = minLastNode2->next;
		minLastNode2->next = minLastNode2->next->next;
		//设一个新节点，将以上节点并在左右孩子下面，然后将这个节点放回链表中
		struct HuffmanNode * treeNode = new struct HuffmanNode;
		treeNode->leaf = false;
		treeNode->text = 0;
		if (minValueNode1->weight >= minValueNode2->weight){
			treeNode->lchild = minValueNode1;
			treeNode->rchild = minValueNode2;
		}
		else{
			treeNode->lchild = minValueNode2;
			treeNode->rchild = minValueNode1;
		}
		treeNode->next = NULL;
		treeNode->weight = minValueNode1->weight + minValueNode2->weight;
		if (freqTree->next == NULL){
			freqTree->next = treeNode;
			break;
		}
		else{
			struct HuffmanNode * temp = freqTree;
			while (temp->next != NULL)
				temp = temp->next;
			temp->next = treeNode;
		}
	}
	cout << endl << endl;
	char a[20];
	//将编码输出
	printTree(freqTree->next, 0, a);

	//将编码后的结果输出
	cout << endl << value << endl;
	cout << endl << endl;
	for (int i = 0; text[i] != '\0'; i++){
		if (text[i] == '#'){
			cout << code[0].code;
		}
		else{
			cout << code[text[i] - 96].code;
		}
	}

	fclose(fileOne);
	system("PAUSE");
	return 0;
}

struct HuffmanNode * minValue(struct HuffmanNode * freqTree){ //在链表中找到最小值
	struct HuffmanNode * lastNode = freqTree;
	struct HuffmanNode * minLastNode = freqTree;
	while (lastNode->next != NULL){
		if (lastNode->next->weight <= minLastNode->next->weight){//!!
			minLastNode = lastNode;
		}
		lastNode = lastNode->next;
	}
	return minLastNode;
}

void printTree(struct HuffmanNode * treeNode, int layer, char a[]){ //递归对哈夫曼树中的节点进行编码并输出
	if (treeNode->leaf == true){
		cout << treeNode->text << ' ' << treeNode->weight << endl;
		value += treeNode->weight*layer;
		if (treeNode->text == '#'){
			strcpy(code[0].code, a);
			code[0].length = layer;
			cout << code[0].code << ' ' << code[0].length << endl;
		}
		else{
			strcpy(code[treeNode->text - 96].code, a);
			code[treeNode->text - 96].length = layer;
			cout << code[treeNode->text - 96].code << ' ' << code[treeNode->text - 96].length << endl;
		}
	}
	else{
		if (treeNode->lchild){
			a[layer] = '0';
			a[layer + 1] = '\0';
			printTree(treeNode->lchild, layer + 1, a);
		}
		if (treeNode->rchild){
			a[layer] = '1';
			a[layer + 1] = '\0';
			printTree(treeNode->rchild, layer + 1, a);
		}
	}
}
