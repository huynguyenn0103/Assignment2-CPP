#include "main.h"
ofstream outfile;
int stt_customer = -100000000;
int stt_huff = -100000000;
class Customer {
public:
	string name;
	int tableId;
	int result;
	int areaId;
	int stt;
	int order;
	Customer() {
		name = "";
		tableId = -1;
		result = -1;
		areaId = -1;
		stt = stt_customer++;
		order = 1;
	}

	Customer(string name, int tableId, int result, int stt, int order) {
		this->name = name;
		this->tableId = tableId;
		this->result = result;
		this->stt = stt;
		this->order = order;
	}
};
//Area 1: HashTable with maximum of size is maxsize/2
class HashTable {
public:
	vector<Customer> v;
	HashTable() {
		for (int i = 0; i < MAXSIZE / 2; i++) {
			Customer obj;
			v.push_back(obj);
		}
	}

	int size() {
		int count = 0;
		for (int i = 0; i < MAXSIZE / 2; i++) {
			if (v[i].name.compare("") != 0) count++;
		}
		return count;
	}
	int hash(int result) {
		return result % (MAXSIZE / 2);
	}

	bool insert(Customer obj) {
		int index = hash(obj.result);
		int count = 0;
		while (v[index].result != -1 && count != MAXSIZE / 2) {
			index++;
			count++;
			if (index >= MAXSIZE / 2) {
				index = 0;
			}
		}
		if (count == MAXSIZE / 2) return false;
		v[index] = obj;
		return true;
	}

	bool remove(Customer obj) {
		int index = hash(obj.result);
		int count = 0;
		while (v[index].name.compare(obj.name) != 0
			&& v[index].tableId != obj.tableId
			&& count != MAXSIZE / 2) {
			index++;
			count++;
			if (index >= MAXSIZE / 2) {
				index = 0;
			}
		}
		if (count == MAXSIZE / 2) return false;
		Customer newObj;
		v[index] = newObj;
		return true;
	}
	bool get(Customer obj) {
		int index = hash(obj.result);
		int count = 0;
		while (v[index].name.compare(obj.name) != 0
			&& v[index].tableId != obj.tableId
			&& count != MAXSIZE / 2) {
			index++;
			count++;
			if (index >= MAXSIZE / 2) {
				index = 0;
			}
		}
		if (count == MAXSIZE / 2) return false;
		return true;
	}
	void update(string name) {
		for (int i = 0; i < MAXSIZE / 2; i++) {
			if (v[i].name.compare(name) == 0) {
				v[i].order++;
			}
		}
	}
	void print() {
		for (int i = 0; i < MAXSIZE / 2; i++) {
			outfile << i << ": " << v[i].name << " - " << v[i].result << " - " << v[i].tableId << " - " << v[i].areaId << endl;
		}
	}
	void printHT() {
		for (int i = 0; i < MAXSIZE / 2; i++) {
			if (v[i].result != -1) {
				outfile << v[i].tableId << "-" << v[i].result << "-" << v[i].order << endl;
			}
		}
	}
};

// Area 2: AVL
class NodeAVL {
public:
	Customer key;
	NodeAVL* left;
	NodeAVL* right;
	int height;

	NodeAVL() {}
	NodeAVL(Customer key) {
		this->key = key;
		this->left = nullptr;
		this->right = nullptr;
		this->height = 1;
	}
};

class AVLTree {
public:
	NodeAVL* root;
	int size(NodeAVL* root) {
		if (root == nullptr) {
			return 0;
		}
		else {
			return 1 + size(root->left) + size(root->right);
		}
	};
	AVLTree() {
		root = nullptr;
	}
	int height(NodeAVL* node) {
		if (node == nullptr) {
			return 0;
		}
		return node->height;
	}
	int max(int a, int b) {
		return (a > b) ? a : b;
	}
	int bf(NodeAVL* node) {
		if (node == nullptr) return 0;
		return height(node->left) - height(node->right);
	}

	NodeAVL* rightRotate(NodeAVL* y) {
		NodeAVL* x = y->left;
		NodeAVL* T2 = x->right;

		x->right = y;
		y->left = T2;

		y->height = max(height(y->left), height(y->right)) + 1;
		x->height = max(height(x->left), height(x->right)) + 1;
		return x;
	}

	NodeAVL* leftRotate(NodeAVL* y) {
		NodeAVL* x = y->right;
		NodeAVL* T3 = x->left;
		x->left = y;
		y->right = T3;

		y->height = max(height(y->left), height(y->right)) + 1;
		x->height = max(height(x->left), height(x->right)) + 1;
		return x;
	}

	NodeAVL* insert(NodeAVL* node, Customer key) {
		if (node == nullptr) {
			return new NodeAVL(key);
		}

		if (key.result < node->key.result) {
			node->left = insert(node->left, key);
		}
		else if (key.result >= node->key.result) {
			node->right = insert(node->right, key);
		}

		node->height = 1 + max(height(node->left), height(node->right));

		//check this node is balance or not
		int balance = bf(node);

		if (balance > 1 && bf(node->left) >= 1) {//LL
			return rightRotate(node);
		}

		if (balance < -1 && bf(node->right) <= -1) {// RR
			return leftRotate(node);
		}

		if (balance > 1 && bf(node->left) <= -1)// LR
		{
			node->left = leftRotate(node->left);
			return rightRotate(node);
		}

		if (balance < -1 && bf(node->right) >= 1)//RL
		{
			node->right = rightRotate(node->right);
			return leftRotate(node);
		}

		return node; //still balance

	}

	void update(NodeAVL*& node, string name) {
		if (node == nullptr) return;
		if (node->key.name.compare(name) == 0) {
			node->key.order++;
			return;
		}
		update(node->left, name);
		update(node->right, name);

	}
	void preOrder(NodeAVL* node)
	{
		if (node != nullptr)
		{
			outfile << node->key.result << " ";
			preOrder(node->left);
			preOrder(node->right);
		}
	}

	NodeAVL* minValueNode(NodeAVL* node)
	{
		NodeAVL* current = node;
		while (current->left != nullptr)
			current = current->left;

		return current;
	}
	NodeAVL* deleteNode(NodeAVL* root, Customer key)
	{

		if (root == nullptr)
			return root;

		if (key.result < root->key.result)
			root->left = deleteNode(root->left, key);

		else if (key.result > root->key.result) {
			root->right = deleteNode(root->right, key);

		}

		else if (key.result == root->key.result && key.name.compare(root->key.name) != 0) {
			root->left = deleteNode(root->left, key);
			root->right = deleteNode(root->right, key);
		}
		//Node to be delete
		else
		{
			// node with only one child or no child
			if ((root->left == nullptr) || (root->right == nullptr))
			{
				NodeAVL* temp = root->left ? root->left : root->right;

				// No child case
				if (temp == nullptr) {
					root = nullptr;
				}
				else // One child case
					*root = *temp;
			}
			else
			{
				NodeAVL* temp = minValueNode(root->right);

				root->key = temp->key;
				root->right = deleteNode(root->right,
					temp->key);
			}
		}


		if (root == NULL)
			return root;

		root->height = 1 + max(height(root->left),
			height(root->right));

		int balance = bf(root);


		if (balance > 1 && bf(root->left) >= 0)// LL
			return rightRotate(root);

		if (balance > 1 && bf(root->left) < 0)//LR
		{
			root->left = leftRotate(root->left);
			return rightRotate(root);
		}
		if (balance < -1 && bf(root->right) <= 0)//RR
			return leftRotate(root);

		if (balance < -1 && bf(root->right) > 0)//RL
		{
			root->right = rightRotate(root->right);
			return leftRotate(root);
		}

		return root;
	}
	void printAVL() {
		if (root == nullptr) return;
		queue<NodeAVL*> q;
		q.push(root);
		while (!q.empty()) {
			NodeAVL* x = q.front();
			q.pop();
			outfile << x->key.tableId << "-" << x->key.result << "-" << x->key.order << endl;
			if (x->left != nullptr) q.push(x->left);
			if (x->right != nullptr) q.push(x->right);
		}
	}
};

// Order minheap: MinHeap Order
class MinHeap {
public:
	vector<Customer> data;
	MinHeap() {
	}
	void swap(Customer& a, Customer& b) {
		Customer tmp = a;
		a = b;
		b = tmp;
	}
	bool pop() {
		if (data.size() == 0) return false;
		data[0] = data[data.size() - 1];
		data.pop_back();
		reHeapDown(0);
	}

	void reHeapUp(unsigned int index) {
		if (index == 0 || data.size() == 0 || index >= data.size()) return;
		int parent = (index - 1) / 2;
		if (data[index].order < data[parent].order || (data[index].order == data[parent].order && data[index].stt < data[parent].stt)) {
			swap(data[index], data[parent]);
			reHeapUp(parent);
		}
	}

	void reHeapDown(unsigned int index) {
		if (data.size() == 0 || index >= data.size()) return;
		int largest = index;
		unsigned int left = 2 * index + 1;
		unsigned int right = 2 * index + 2;
		if ((left < data.size() && ((data[left].order < data[largest].order) || (data[left].order == data[largest].order && data[left].stt < data[largest].stt)))) {
			largest = left;
		}

		if (right < data.size() && ((data[right].order < data[largest].order) || (data[right].order == data[largest].order && data[right].stt < data[largest].stt))) {
			largest = right;
		}

		if (largest != index) {
			swap(data[index], data[largest]);
			reHeapDown(largest);
		}
	}
	int find(Customer item) {
		for (unsigned int i = 0; i < data.size(); i++) {
			if (data[i].name.compare(item.name) == 0 && data[i].tableId == item.tableId) {
				return i;
			}
		}
		return -1;
	}
	void add(Customer value) {
		data.push_back(value);
		reHeapUp(data.size() - 1);
	}
	bool remove(Customer item) {
		int index = find(item);
		if (index == -1) return false;
		data[index] = data[data.size() - 1];
		data.pop_back();
		//int parent = (index - 1) / 2;
		//if (data[index].order < data[parent].order) {
		//reHeapUp(index);
		//}
		//else {
		reHeapDown(index);
		//}
	}
	bool update(Customer oldItem, Customer newItem) {
		int index = find(oldItem);
		if (index == -1) return false;
		data[index] = newItem;
		//int parent = (index - 1) / 2;
		//if (data[index] < data[parent]) {
		reHeapUp(index);
		//}
		//else {
		reHeapDown(index);
		//}
	}
	bool update_style2(Customer oldItem, Customer newItem) {
		int index = find(oldItem);
		if (index == -1) return false;
		remove(oldItem);
		add(newItem);
	}
	bool isEmpty() {
		return data.empty();
	}
	int size() {
		return data.size();
	}
	void print() {
		for (unsigned int i = 0; i < data.size(); i++) {
			outfile << data[i].name << " " << data[i].order;
		}
		outfile << endl;
	}
	void printMH(int index) {
		if (index >= data.size()) return;
		outfile << data[index].tableId << "-" << data[index].order << endl;
		printMH(2 * index + 1);
		printMH(2 * index + 2);
	}
};
queue<Customer> qCustomers;	 //update when reg, cle
vector<Customer> tableID(MAXSIZE + 1, Customer());  // update when reg, cle
list<string> customerOrder;  //update when order, CLE
HashTable area1;
AVLTree area2;
MinHeap orderTree;

//Input
class MinHeapNode {
public:
	char data;
	int frequency;
	int stt;
	MinHeapNode* left;
	MinHeapNode* right;
	MinHeapNode(char data, int frequency) {
		this->left = nullptr;
		this->right = nullptr;
		this->frequency = frequency;
		this->data = data;
	}
};

struct compare {
	bool operator()(MinHeapNode* first, MinHeapNode* second) {
		if (first->frequency == second->frequency) {
			if ((first->data == '|' && second->data == '|')) return first->stt > second->stt;
			return (first->data > second->data);
		}
		return (first->frequency > second->frequency);
	}
};

void printCodesWithAdd(struct MinHeapNode* root, string add, map<char, string>& m, bool isLeft)
{
	if (root == nullptr) {
		return;
	}

	if (isLeft) add.push_back('0');
	else add.push_back('1');

	if (root->data != '|') {
		m[root->data] = add;
	}
	else {
		printCodesWithAdd(root->left, add, m, true);
		printCodesWithAdd(root->right, add, m, false);
	}
}

void HuffmanCodes(map <char, int>& m, map<char, string>& result) {
	if (m.size() == 1) {
		map<char, int>::iterator it = m.begin();
		result[it->first] = "1";
		return;
	}
	MinHeapNode* left, * right, * top;
	priority_queue<MinHeapNode*, vector<MinHeapNode*>, compare> minHeap;
	//for (int i = 0; i < size; ++i)
	//	minHeap.push(new MinHeapNode(data[i], freq[i]));

	map<char, int>::iterator it = m.begin();
	while (it != m.end()) {
		minHeap.push(new MinHeapNode(it->first, it->second));
		++it;
	}


	priority_queue<MinHeapNode*, vector<MinHeapNode*>, compare> minHeap_copy = minHeap;
	int len = minHeap.size();
	//for (int i = 0; i < minHeap.size(); i++) {
	//	outfile << minHeap_copy.top()->frequency << "-" << minHeap_copy.top()->data << " ";
	//	minHeap_copy.pop();
	//}
	//outfile << endl;
	while (minHeap.size() != 1) {
		left = minHeap.top();
		minHeap.pop();

		right = minHeap.top();
		minHeap.pop();

		top = new MinHeapNode('|', left->frequency + right->frequency);
		top->left = left;
		top->right = right;
		top->stt = stt_huff++;
		minHeap.push(top);
	}
	//printCodes(minHeap.top(), "");
	printCodesWithAdd(minHeap.top()->left, "", result, true);
	printCodesWithAdd(minHeap.top()->right, "", result, false);
}
void process_map_character(string str, map<char, int>& m) {
	for (unsigned int i = 0; i < str.size(); i++) {
		m[str[i]]++;
	}
}
void deleteQueue(string name) {
	queue<Customer> q_copy;
	unsigned int len = qCustomers.size();
	for (unsigned int i = 0; i < len; i++) {
		if (qCustomers.front().name.compare(name) != 0) {
			q_copy.push(qCustomers.front());
		}
		qCustomers.pop();
	}
	qCustomers = q_copy;
}
int changeToDecimal(string str, map<char, string> m) {
	string result = "";
	for (unsigned int i = 0; i < str.size(); i++) {
		result.append(m[str[i]]);
	}
	if (result.size() > 15) {
		result = result.substr(result.size() - 15);
	}
	size_t* a = nullptr;
	return stoi(result, a, 2);
}

void process_add_list(string x) {
	if (customerOrder.empty()) {
		customerOrder.push_back(x);
		return;
	}
	bool isExist = false;
	for (auto i = customerOrder.begin(); i != customerOrder.end(); i++) {
		if ((*i).compare(x) == 0) {
			isExist = true;
			break;
		}
	}
	if (!isExist) {
		customerOrder.push_back(x);
	}
	else {
		customerOrder.remove(x);
		customerOrder.push_back(x);
	}
}

void print_order_list(list <string> l) {
	if (!l.empty()) {
		for (auto i = l.begin(); i != l.end(); i++) {
			outfile << *i << " ";
		}
		outfile << endl;
	}
}

//Input
bool isExistedCustomer(string name) {
	for (unsigned int i = 1; i <= MAXSIZE; i++) {
		if (tableID[i].name.compare(name) == 0) return true;
	}
	return false;
}


void processReg(string name) {
	map<char, int> m;
	map<char, string> results;
	process_map_character(name, m);
	HuffmanCodes(m, results);
	int result = changeToDecimal(name, results);
	int areaId;
	int tableId = result % MAXSIZE + 1;
	// Xu li area
	if (area1.size() == MAXSIZE / 2 && area2.size(area2.root) == MAXSIZE / 2) {
		int OPT = result % 3;
		if (OPT == 0) {
			Customer neededDelete = qCustomers.front();
			Customer newCus;

			qCustomers.pop();
			tableID[neededDelete.tableId] = newCus;
			customerOrder.remove(neededDelete.name);
			if (neededDelete.areaId == 1) {
				areaId = 1;
				area1.remove(neededDelete);
			}
			else {
				areaId = 2;
				area2.root = area2.deleteNode(area2.root, neededDelete);
			}
			orderTree.remove(neededDelete);
		}
		else if (OPT == 1) {
			string neededDelete_name = customerOrder.front();
			Customer neededDelete;
			Customer newCus;
			for (int i = 1; i <= MAXSIZE; i++) {
				if (tableID[i].name.compare(neededDelete_name) == 0) {
					neededDelete = tableID[i];
					break;
				}
			}

			deleteQueue(neededDelete.name);
			tableID[neededDelete.tableId] = newCus;
			customerOrder.remove(neededDelete.name);
			if (neededDelete.areaId == 1) {
				areaId = 1;
				area1.remove(neededDelete);
			}
			else {
				areaId = 2;
				area2.root = area2.deleteNode(area2.root, neededDelete);
			}
			orderTree.remove(neededDelete);

		}
		else {
			Customer neededDelete = orderTree.data[0];
			Customer newCus;

			deleteQueue(neededDelete.name);
			tableID[neededDelete.tableId] = newCus;
			customerOrder.remove(neededDelete.name);
			if (neededDelete.areaId == 1) {
				areaId = 1;
				area1.remove(neededDelete);
			}
			else {
				areaId = 2;
				area2.root = area2.deleteNode(area2.root, neededDelete);
			}
			orderTree.remove(neededDelete);
		}
	}
	else if (area1.size() == MAXSIZE / 2) {
		areaId = 2;
	}
	else if (area2.size(area2.root) == MAXSIZE / 2) {
		areaId = 1;
	}
	else {
		if (result % 2 == 1) {
			areaId = 1;
		}
		else areaId = 2;
	}

	// Chac chan co it nhat 1 ban trong
	while (tableID[tableId].name.compare("") != 0) {
		tableId++;
		if (tableId > MAXSIZE) tableId = 1;
	}

	Customer cus;
	cus.name = name;
	cus.tableId = tableId;
	cus.result = result;
	cus.areaId = areaId;
	// Add vao 
	qCustomers.push(cus);
	tableID[tableId] = cus;
	customerOrder.push_back(cus.name);
	if (areaId == 1) area1.insert(cus);
	else area2.root = area2.insert(area2.root, cus);
	orderTree.add(cus);
}

void processOrder(string name) {
	Customer neededUpdate;
	for (unsigned int i = 1; i <= MAXSIZE; i++) {
		if (tableID[i].name.compare(name) == 0) {
			neededUpdate = tableID[i];
		};
	}

	tableID[neededUpdate.tableId].order++;// tableID
	process_add_list(name);// customerOrder

	if (neededUpdate.areaId == 1) area1.update(name);
	else if (neededUpdate.areaId == 2) area2.update(area2.root, name);

	Customer afterUpdate = neededUpdate;
	afterUpdate.order++;
	// cap nhat order tree
	orderTree.update_style2(neededUpdate, afterUpdate);
}
void cleSupport(queue<Customer>& q1, queue<Customer>& q2) {

	unsigned int len = qCustomers.size();
	for (unsigned int i = 0; i < len; i++) {
		if (qCustomers.front().areaId == 1) {
			q1.push(qCustomers.front());
		}
		else {
			q2.push(qCustomers.front());
		}
		qCustomers.pop();
	}
}
void processCLE(int tableId) {
	if (tableId > MAXSIZE) {// Duoi het khach o khu 2
		queue<Customer> q1;
		queue<Customer> q2;
		cleSupport(q1, q2);
		unsigned int len = q2.size();

		for (unsigned int i = 0; i < len; i++) {
			Customer neededDelete = q2.front();
			q2.pop();
			Customer newCus;

			//deleteQueue(neededDelete.name);
			tableID[neededDelete.tableId] = newCus;
			customerOrder.remove(neededDelete.name);
			orderTree.remove(neededDelete);
			area2.root = area2.deleteNode(area2.root, neededDelete);
		}
		qCustomers = q1;
	}
	else if (tableId < 1) {
		queue<Customer> q1;
		queue<Customer> q2;
		cleSupport(q1, q2);
		unsigned int len = q1.size();

		for (unsigned int i = 0; i < len; i++) {
			Customer neededDelete = q1.front();
			q1.pop();
			Customer newCus;

			//deleteQueue(neededDelete.name);
			tableID[neededDelete.tableId] = newCus;
			customerOrder.remove(neededDelete.name);
			orderTree.remove(neededDelete);
			area1.remove(neededDelete);
		}
		qCustomers = q2;
	}
	else if (tableId >= 1 && tableId <= MAXSIZE) {
		Customer neededDelete = tableID[tableId];
		if (neededDelete.name.compare("") == 0) return;// Khong co nguoi ngoi o ban do
		Customer newCus;

		deleteQueue(neededDelete.name);
		tableID[neededDelete.tableId] = newCus;
		customerOrder.remove(neededDelete.name);
		if (neededDelete.areaId == 1) {
			area1.remove(neededDelete);
		}
		else {
			area2.root = area2.deleteNode(area2.root, neededDelete);
		}
		orderTree.remove(neededDelete);
	}
}







bool verifyblank(string requirement) {
	size_t check = requirement.find(' ', 0);
	if (check != string::npos) {
		return false;
	}
	return true;
}
bool verifyNumber(string requirement) {
	for (unsigned int i = 0; i < requirement.size(); i++) {
		if (!isdigit(requirement[i])) return false;
	}
	return true;
}
void simulate(string filename, int testcase) {
	outfile.open("got" + to_string(testcase) + ".txt");
	string requirement = "";
	ifstream fileReader(filename);
	while (!fileReader.eof()) {
		getline(fileReader, requirement);
		if (requirement.size() <= 4) {
			continue;
		}
		size_t index = requirement.find(' ', 0);
		if (index == 3) {
			string task = requirement.substr(0, 3);
			string result = requirement.substr(4);
			if (task.compare("REG") == 0) {
				bool isSuccess = verifyblank(result);
				if (!isSuccess) continue;
				else {
					if (isExistedCustomer(result)) {// Order mon
						processOrder(result);
					}
					else {// Den an
						processReg(result);
					}
				}

			}
			else if (task.compare("CLE") == 0) {
				bool isSuccess = verifyblank(result);
				bool isNumber = verifyNumber(result);
				if (!isSuccess || !isNumber) continue;
				else processCLE(stoi(result));
			}
			else {
				continue;
			}
		}
		else if (index == string::npos) {
			if (requirement.compare("PrintHT") == 0) {
				//outfile << "HT" << endl;
				area1.printHT();
			}
			else if (requirement.compare("PrintAVL") == 0) {
				//outfile << "AVL" << endl;
				area2.printAVL();
			}
			else if (requirement.compare("PrintMH") == 0) {
				//outfile << "MH" << endl;
				orderTree.printMH(0);
			}
			else {
				continue;
			}

		}
		else {
			continue;
		}
	}

	stt_customer = -100000000;
	stt_huff = -100000000;

	queue<Customer> qCustomers_backup;
	qCustomers = qCustomers_backup;


	vector<Customer> tableID_backup(MAXSIZE + 1, Customer());
	tableID = tableID_backup;

	list<string> customerOrder_backup;
	customerOrder = customerOrder_backup;

	HashTable area1_backup;
	area1 = area1_backup;

	AVLTree area2_backup;
	area2 = area2_backup;

	MinHeap orderTree_backup;
	orderTree = orderTree_backup;

	outfile.close();
	return;
}