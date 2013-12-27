#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <stack>

using namespace std;
class HuffmanNode
{
private:
	virtual void test()
	{
		//cout << "from base" << endl;
	}

	int weight;
	bool is_leaf;
	HuffmanNode * l_child;
	HuffmanNode * r_child;
	HuffmanNode * next;
	HuffmanNode * father;
	char huffman_trace;
	vector<HuffmanNode*> regist_vec;

public:
	HuffmanNode(void)
		: weight(0) , is_leaf(false) , l_child(NULL) , r_child(NULL) , next(NULL) , father(NULL) , huffman_trace(0)
	{}

	HuffmanNode(const HuffmanNode & hfm_node)
	{
		this->set_all(hfm_node);
	}

	virtual ~HuffmanNode()
	{
		//cout << "base deleted" << endl;
	}

	int get_weight() const {return this->weight;}
	bool get_is_leaf() const {return this->is_leaf;}
	HuffmanNode * get_l_child() const {return this->l_child;}
	HuffmanNode * get_r_child() const {return this->r_child;}
	HuffmanNode * get_next() const {return this->next;}
	HuffmanNode * get_father() const{return this->father;}
	char get_trace() const {return this->huffman_trace;}

	HuffmanNode * set_weight(int _weight){this->weight = _weight; return this;}
	HuffmanNode * set_is_leaf(bool _is_leaf){this->is_leaf = _is_leaf; return this;}
	HuffmanNode * set_l_child(HuffmanNode * _l_child){this->l_child = _l_child; return this;}
	HuffmanNode * set_r_child(HuffmanNode * _r_child){this->r_child = _r_child; return this;}
	HuffmanNode * set_next(HuffmanNode * _next){this->next = _next; return this;}
	HuffmanNode * set_father(HuffmanNode * _father){this->father = _father; return this;}
	HuffmanNode * set_trace(char _trace){this->huffman_trace = _trace ; return this;}

	virtual void print()
	{
		cout << this->get_trace() << "-" << this->get_weight() << "-"  << this->get_is_leaf();
	}

protected:
	HuffmanNode * set_all(const HuffmanNode & hfm_node)
	{
		this->set_is_leaf(hfm_node.get_is_leaf());
		this->set_l_child(hfm_node.get_l_child());
		this->set_r_child(hfm_node.get_r_child());
		this->set_weight(hfm_node.get_weight());
	}

};
class HuffmanLeafNode : public HuffmanNode
{
private:
	char charactor;
	string huffman_code;

public:
	HuffmanLeafNode()
	{
		this->set_is_leaf(true);
	}

	HuffmanLeafNode(char _charactor , int _weight)
		: charactor(_charactor)
	{
		HuffmanLeafNode();
		this->set_weight(_weight);
	}

	HuffmanLeafNode(const HuffmanLeafNode & hfm_leafNode)
	{
		this->set_all(hfm_leafNode);
		this->set_charactor(hfm_leafNode.get_charactor());
		this->set_huffman_code(hfm_leafNode.get_huffman_code());
	}

	virtual ~HuffmanLeafNode()
	{
		//cout << "derived deleted" << endl;
	}

	char get_charactor() const {return this->charactor;}
	const string & get_huffman_code() const {return this->huffman_code;}
	string & get_huffman_code(){return this->huffman_code;}

	HuffmanLeafNode * set_charactor(char _charactor){this->charactor = _charactor;return this;}
	HuffmanLeafNode * set_huffman_code(const string & _huffman_code){this->huffman_code = _huffman_code;return this;}

	virtual void print()
	{
		cout << this->get_trace() << "-" << this->get_charactor() << "-" << this->get_weight() << "-"  << this->get_is_leaf() << "-"  << this->get_huffman_code() ;
	}
};


template<typename T>void print_vector(T & vec);

void fill_leaf_input(vector<HuffmanLeafNode*> & leaf_input , vector<HuffmanNode*> &hfm_vec);
HuffmanNode * generate_huff_node(vector<HuffmanNode *> &hfm_vec , HuffmanNode * hfm_node = NULL)
{
	if(NULL == hfm_node)
		hfm_node = new HuffmanNode();
	hfm_vec.push_back(hfm_node);
	return hfm_node;
}


void print_tree(HuffmanNode * root , int lev = 0)
{

	if(NULL == root)
		return;

	for(int i = 0 ; i < lev ; ++ i)
		cout << "	";
	root->print(); cout << endl;
	print_tree(root->get_l_child() , lev + 1);
	print_tree(root->get_r_child() , lev + 1);
}

void insert_node(HuffmanNode * sorted_root , HuffmanNode * hfmNode)
{
	if(NULL == sorted_root)
	{
		cout << "bad sorted root" << endl;
		return ;
	}

	HuffmanNode * it = sorted_root;
	while(NULL != it->get_next())
	{
		if(it->get_next()->get_weight() > hfmNode->get_weight())
		{
			hfmNode->set_next(it->get_next());
			it->set_next(hfmNode);
			return;
		}
		it = it->get_next();
	}

	it->set_next(hfmNode);
}

HuffmanNode * cut_min2(HuffmanNode * sorted_root , vector<HuffmanNode *> &hfm_reg  )
{
	if(NULL == sorted_root)
	{
		cout << "bad sorted root!" << endl;
		return NULL;
	}

	HuffmanNode * it = sorted_root->get_next();
	if(NULL == it)
	{
		cout << "empty sorted root!" << endl;
		return NULL;
	}
	if(NULL == it->get_next())
	{
		cout << "sorted list has only single node!" << endl;
		return it;
	}
	
	int weight = 0;
	weight = it->get_weight() + it->get_next()->get_weight();
	HuffmanNode * temp = generate_huff_node(hfm_reg);
	temp->set_weight(weight);
	temp->set_r_child(it);
	temp->set_l_child(it->get_next());
	it->set_father(temp)->set_trace('1'); // r mean 1
	it->get_next()->set_father(temp)->set_trace('0'); // l mean 0
	sorted_root->set_next(it->get_next()->get_next());
	return temp;
}

void generate_huffman_tree(vector<HuffmanLeafNode*> & leaf_input , vector<HuffmanNode *> &hfm_reg , HuffmanNode * root)
{
	HuffmanNode *  sorted_root = root;
	for(auto it = leaf_input.begin() ; it != leaf_input.end() ; ++ it)
	{
		insert_node(sorted_root , (HuffmanNode*)*it);
	}

	for(unsigned int i = 0 ; i < leaf_input.size() - 1 ; ++ i)
	{
		insert_node(sorted_root , cut_min2(sorted_root , hfm_reg));
	}

	sorted_root->get_next()->set_trace('0');
}

void generate_huffman_code(vector<HuffmanLeafNode*> & leaf_input)
{

	for(auto it = leaf_input.begin() ; it != leaf_input.end() ; ++ it)
	{
		stack<char> trace_end;
		HuffmanNode * temp = *it;
		while(NULL != temp)
		{
			trace_end.push(temp->get_trace());
			temp = temp->get_father();
		}
		while(trace_end.size() != 0)
		{
			(*it)->get_huffman_code().push_back(trace_end.top());
			trace_end.pop();
		}
	}
}

void print_leaf_input(vector<HuffmanLeafNode *> &leaf_input)
{
	for(auto it = leaf_input.begin() ; it != leaf_input.end() ; ++ it)
	{
		HuffmanLeafNode * temp = *it;
		cout << temp->get_charactor() << "-" << temp->get_weight() << "-" << temp->get_huffman_code() << endl; 
	}
}

void huffman_encode(vector<HuffmanLeafNode *> & leaf_input)
{
	string file_name;
	string text;
	cout << "please input the file name to encode:";
	cin >> file_name;
	ifstream fin(file_name);
	fin >> text;
	fin.close();

	ofstream fout("codefile.dat");
	for(auto it_text = text.begin() ; it_text != text.end() ; ++ it_text)
	{
		for(auto it_leaf = leaf_input.begin() ; it_leaf != leaf_input.end() ; ++ it_leaf)
		{
			HuffmanLeafNode * temp = *it_leaf;
			if(temp->get_charactor() == * it_text)
				fout << temp->get_huffman_code();
		}
	}
	fout.close();
}

void huffman_decode(HuffmanNode * root)
{
	string file_name;
	string huffman_code;
	cout << "please input the file name to decode:";
	cin >> file_name;

	ifstream fin(file_name);
	fin >> huffman_code;
	fin.close();

	if(huffman_code[0] != '0')
	{
		cout << "bad huffman code" << endl;
		return;
	}

	HuffmanNode * temp = root->get_next();
	string text;
	for(auto it_hfmcode = huffman_code.begin() + 1; it_hfmcode != huffman_code.end() ; ++ it_hfmcode)
	{
		if(temp->get_is_leaf())
		{
			HuffmanLeafNode * temp_leaf = (HuffmanLeafNode*) temp;
			text.push_back(temp_leaf->get_charactor());
			temp = root->get_next();
			continue;
		}
		if(*it_hfmcode == '1' && ! temp->get_is_leaf())
			temp = temp->get_r_child();
		if(*it_hfmcode == '0' && ! temp->get_is_leaf())
			temp = temp->get_l_child();
	}

	if(temp->get_is_leaf())
	{
		HuffmanLeafNode * temp_leaf = (HuffmanLeafNode*) temp;
		text.push_back(temp_leaf->get_charactor());
	}
	

	ofstream fout("textfile.dat");
	fout << text;
	fout.close();
	cout <<  text << endl;
}

int main()
{
	vector<HuffmanLeafNode*> leaf_input;
	vector<HuffmanNode *> hfm_reg;
	HuffmanNode * root = generate_huff_node(hfm_reg);
	
	
	fill_leaf_input(leaf_input , hfm_reg);
	generate_huffman_tree(leaf_input , hfm_reg , root);
	print_tree(root->get_next());
	generate_huffman_code(leaf_input);
	print_leaf_input(leaf_input);
	
	huffman_encode(leaf_input);
	huffman_decode(root);
	
	system("PAUSE");
	return 0;
}

template<typename T>
void print_vector(T & vec)
{
	for(auto it = vec.begin() ; it != vec.end() ; ++ it)
	{
		(*it)->print();
		cout << endl;
	}
}

void fill_leaf_input(vector<HuffmanLeafNode*> & leaf_input , vector<HuffmanNode*> &hfm_vec)
{
	int charset_size = 0;
	char char_temp;
	int weight_temp;
	cout << "please enter charset size:"; cin >> charset_size;

	if(0 != charset_size)
	{

		for(int i = 0 ; i < charset_size ; ++ i)
		{
			cout << "please enter charactor and weight:";
			cin >> char_temp >> weight_temp;
			HuffmanLeafNode * hfm_leafnode = new HuffmanLeafNode();
			hfm_leafnode->set_charactor(char_temp);
			hfm_leafnode->set_weight(weight_temp);
			generate_huff_node(hfm_vec , hfm_leafnode);
			leaf_input.push_back(hfm_leafnode);
		}
	}
	else
	{
		string file_name;
		string buf;
		cout << "please input file name:" ; cin >> file_name;
		ifstream fin(file_name);
		while(getline(fin , buf))
		{
			stringstream leaf_stream(buf);
			leaf_stream >> char_temp;
			leaf_stream >> weight_temp;
			HuffmanLeafNode * hfm_leafnode = new HuffmanLeafNode();
			hfm_leafnode->set_charactor(char_temp);
			hfm_leafnode->set_weight(weight_temp);
			generate_huff_node(hfm_vec , hfm_leafnode);
			leaf_input.push_back(hfm_leafnode);
		}
		fin.close();
	}

	cout << "done" << endl;
	print_vector< vector<HuffmanLeafNode*> >(leaf_input);
}
