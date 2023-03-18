#include <iostream>
#include <cstdlib>
#include <queue>
#include <cmath>

using namespace std;

// 1. ZADATAK
void input_by_hand(int k, int* K) {
	int i, j;
	for (i = 0; i < k; i++) {
		int temp;
		cin >> temp;
		j = i - 1;

		while (j >= 0 && K[j] < temp) {
			K[j + 1] = K[j];
			j--;
		}
		K[j + 1] = temp;

	}
}

void input_by_range(int k, int* K) {
	int min, max;

	cout << "Min Max: ";
	cin >> min >> max;


	int i, j;
	for (i = 0; i < k; i++) {
		int random_int = min + (rand() % (max - min));
		j = i - 1;

		while (j >= 0 && K[j] < random_int) {
			K[j + 1] = K[j];
			j--;
		}
		K[j + 1] = random_int;

	}
}

void print_array(int k, int* K) {
	int i;
	for (i = 0; i < k; i++) {
		cout << K[i] << " ";
	}
	cout << endl;
}

int m_search(int k, int* K, int left, int right, int key, int m, double* num_of_steps) {
	int i;

	int* mids = new int[m + 1];

	if (key >= K[0] || key <= K[k - 1] || k == 0) return -1;

	while (left <= right) {
		mids[0] = left;
		mids[m] = right;
		for (i = 1; i < m + 1; i++) mids[i] = left + ((right - left) * i) / m;
		//print_array(m + 1, mids);

		for (i = 1; i < m + 1; i++) {
			(*num_of_steps)++;
			if (right - left <= m) {
				for (i = left; i <= right; i++) {
					(*num_of_steps)++;
					if (K[i] == key) return i;
				}

				return -1;
			}

			if (K[mids[i]] == key) {
				return mids[i];
			}
			else if (K[mids[i - 1]] == key) {
				return mids[i - 1];
			}
			else if (K[mids[i - 1]] > key && key > K[mids[i]]) {
				left = mids[i - 1];
				right = mids[i];
			}
		}

		//cout << left << " " << right << endl;
	}

	return -1;
}

int* optimized_m_search(int k, int* K, int p, int* P, int m, double* num_of_steps) {
	int i;

	int* indexes = new int[p];
	int last_not_null = 0;

	for (i = 0; i < p; i++) {
		if (i == 0) {
			indexes[i] = m_search(k, K, 0, k - 1, P[i], m, num_of_steps);
		}
		else {
			indexes[i] = m_search(k, K, last_not_null, k - 1, P[i], m, num_of_steps);

		}

		if (indexes[i] > 0) last_not_null = indexes[i];
	}

	return indexes;
}

// 2. ZADATAK

struct Node {
	int value;
	Node* duplicate;
	Node* left;
	Node* right;
};

struct Pair {
	int start;
	int end;
};

void insert_node(Node* root, Node* new_node) {
	Node* current_node = root;

	while (1) {
		if (new_node->value > current_node->value) {
			if (current_node->right == NULL) {
				current_node->right = new_node;
				break;
			}
			else {
				current_node = current_node->right;
			}
		}
		else if (new_node->value < current_node->value) {
			if (current_node->left == NULL) {
				current_node->left = new_node;
				break;
			}
			else {
				current_node = current_node->left;
			}
		}
		else {
			if (current_node->duplicate == NULL) {
				current_node->duplicate = new_node;
				break;
			}
			else {
				Node* temp = current_node;

				while (temp->duplicate != NULL) {
					temp = temp->duplicate;
				}

				temp->duplicate = new_node;
				break;
			}
		}
	}

}

void balance_indexes(int k, int* balanced_order) {
	int index = 0;
	queue<Pair> queue;

	Pair first_pair;
	first_pair.start = 0;
	first_pair.end = k - 1;

	queue.push(first_pair);

	while (!queue.empty()) {
		Pair temp = queue.front();
		queue.pop();

		if (temp.start > temp.end) continue;
		int mid = temp.start + (temp.end - temp.start) / 2;
		balanced_order[index++] = mid;

		Pair new_pair_one;
		new_pair_one.start = temp.start;
		new_pair_one.end = mid - 1;

		Pair new_pair_two;
		new_pair_two.start = mid + 1;
		new_pair_two.end = temp.end;

		queue.push(new_pair_one);
		queue.push(new_pair_two);

	}

}

Node* create_bst(int k, int* K) {
	int i;
	Node* root = NULL;
	Node* current_node = NULL;

	int* balanced_order = new int[k];
	balance_indexes(k, balanced_order);

	for (i = 0; i < k; i++) {
		Node* new_node = new Node;
		new_node->value = K[balanced_order[i]];
		new_node->duplicate = NULL;
		new_node->left = NULL;
		new_node->right = NULL;

		if (root == NULL) {
			root = new_node;
			continue;
		}

		insert_node(root, new_node);
	}

	delete[] balanced_order;
	return root;
}

Node* search_bst(Node* root, int key, double* num_of_steps) {
	Node* current_node = root;

	while (1) {
		(*num_of_steps)++;
		if (key > current_node->value) {
			if (current_node->right == NULL) {
				return NULL;
			}
			else {
				current_node = current_node->right;
			}
		}
		else if (key < current_node->value) {
			if (current_node->left == NULL) {
				return 0;
			}
			else {
				current_node = current_node->left;
			}
		}
		else {
			return current_node;
		}
	}
}

void print2D(Node* root, int space) {
	if (root == NULL)
		return;

	space += 10;

	print2D(root->right, space);

	cout << endl;
	for (int i = 10; i < space; i++)
		cout << " ";
	cout << root->value << "\n";

	print2D(root->left, space);
}

void printTree(Node* root, int k) {
	if (!root)   return;
	else {
		queue<Node*> q;
		int i, line_len = 126;
		int first_skip = line_len, in_between_skip;

		int x = log2(k + 1);

		q.push(root);
		for (i = 0; i <= x; i++) {
			int j = 1 << i, k, l;
			in_between_skip = first_skip;
			first_skip = (first_skip - 2) / 2;
			for (k = 0; k < first_skip; k++) putchar(' ');
			for (k = 0; k < j; k++) {
				Node* btn = q.front();
				q.pop();
				if (btn) {
					q.push(btn->left);
					q.push(btn->right);
				}
				else {
					q.push(NULL);
					q.push(NULL);
				}
				if (btn)  printf("%2d", btn->value);
				else       printf("  ");
				for (l = 0; l < in_between_skip; l++) putchar(' ');
			}
			cout << endl;
			cout << endl;
		}

	}
}


void delete_bst(Node* root) {
	if (root == NULL) return;

	queue<Node*> queue;
	Node* front = NULL;

	queue.push(root);

	while (!queue.empty()) {
		front = queue.front();
		queue.pop();

		if (front->left) queue.push(front->left);
		if (front->right) queue.push(front->right);
		delete front;
	}

	root = NULL;
}

int main() {

	int first_choice = -1, k;

	cout << "Unesi duzinu niza K: ";
	cin >> k;

	int* K = new int[k];

	while (first_choice != 0) {

		int second_choice = -1;

		if (first_choice == 1) {

			while (second_choice != 0) {
				if (second_choice == 1) {
					input_by_hand(k, K);
				}
				else if (second_choice == 2) {
					input_by_range(k, K);
				}
				else if (second_choice == 3) {
					int key, m;
					double num_of_steps = 0;

					cout << "Unesi kljuc za pretragu: ";
					cin >> key;

					cout << "Unesi parametar m: ";
					cin >> m;

					int index = m_search(k, K, 0, k - 1, key, m, &num_of_steps);

					if (index < 0) {
						cout << "Ne postoji takav kljuc u nizu." << endl << endl;
					}
					else {
						cout << "Trazeni element se nalazi na indexu " << index << "." << endl << endl;
					}

				}
				else if (second_choice == 4) {
					int p, m;
					double num_of_steps = 0;

					cout << "Unesi parametar m: ";
					cin >> m;

					cout << "Unesi duzinu niza kljuceva za pretragu: ";
					cin >> p;

					int* P = new int[p];
					input_by_hand(p, P);

					cout << endl << "Kljucevi po redu: ";
					print_array(p, P);

					int* indexes = optimized_m_search(k, K, p, P, m, &num_of_steps);

					cout << "Trazeni elementi se nalaze na indeksima: " << endl;
					print_array(p, indexes);
					cout << endl;

					delete[] P;
					delete[] indexes;

				}
				else if (second_choice == 5) {
					int i, j, p;

					cout << "Unesi duzinu niza kljuceva za pretragu:  ";
					cin >> p;

					int* keys = new int[p];

					input_by_hand(p, keys);

					cout << "m" << "    Broj koraka" << endl;

					double p_double = p;
					for (i = 2; i <= 6; i++) {
						double num_of_steps = 0;

						for (j = 0; j < p; j++) {
							m_search(k, K, 0, k - 1, keys[j], i, &num_of_steps);
						}

						cout << i << "    " << num_of_steps / p_double << endl;
					}
					cout << endl;

					delete[] keys;
				}
				else if (second_choice == 6) {
					int i, p;

					cout << "Unesi duzinu niza kljuceva za pretragu:  ";
					cin >> p;

					int* P = new int[p];
					input_by_hand(p, P);

					double p_double = p;
					for (i = 2; i <= 6; i++) {
						double num_of_steps = 0;

						optimized_m_search(k, K, p, P, i, &num_of_steps);

						if (i == 2) cout << "m" << "    Broj koraka" << endl;
						cout << i << "    " << num_of_steps / p_double << endl;
					}
					cout << endl;

					delete[] P;
				}
				else if (second_choice == 7) {
					print_array(k, K);
				}

				cout << endl;
				cout << "1. Unesi elemente uredjenog niza K." << endl;
				cout << "2. Unesi opseg za genersianje uredjenog niza K." << endl;
				cout << "3. Unesi vrednost parametra m i kljuc po kome se vrsi pretraga." << endl;
				cout << "4. Unesi parametre za optimizovanu pretragu." << endl;
				cout << "5. Ispisi evaluaciju performansi m-arnog pretrazivanja." << endl;
				cout << "6. Ispisi evaluaciju performansi optimizovane pretrage." << endl;
				cout << "7. Ispisi uredjen niz K." << endl;
				cout << "0. Glavni meni." << endl;
				cout << "Unesi redni broj opcije: ";
				cin >> second_choice;
			}


		}
		else if (first_choice == 2) {
			Node* root = NULL;

			while (second_choice != 0) {
				if (second_choice == 1) {
					root = create_bst(k, K);
					cout << "Stablo uspesno formirano." << endl;
				}
				else if (second_choice == 2) {
					int new_value;
					cout << "Unesi novi kljuc: ";
					cin >> new_value;

					Node* new_node = new Node;
					new_node->value = new_value;
					new_node->duplicate = NULL;
					new_node->left = NULL;
					new_node->right = NULL;

					insert_node(root, new_node);

				}
				else if (second_choice == 3) {
					int key;
					cout << "Unesi kljuc za pretragu: ";
					cin >> key;

					double num_of_steps = 0;

					Node* result = search_bst(root, key, &num_of_steps);

					if (result == NULL) {
						cout << "Ne postoji takav kljuc u stablu." << endl;
					}
					else {
						cout << "Kljuc " << key << " se nalazi na adresi " << result << "." << endl;
					}
				}
				else if (second_choice == 4) {
					cout << endl << endl;
					print2D(root, 0);
					//printTree(root, k);
					cout << endl << endl;
				}
				else if (second_choice == 5) {
					int i, p;

					cout << "Unesi duzinu niza kljuceva za pretragu:  ";
					cin >> p;

					int* P = new int[p];
					input_by_range(p, P);

					cout << "Kljucevi po redu: ";
					print_array(p, P);

					double p_double = p, num_of_steps = 0;
					for (i = 0; i < p; i++) {
						search_bst(root, P[i], &num_of_steps);
					}

					cout << "Prosecan broj koraka je za nalazenje kljuca je: " << num_of_steps / p_double << endl;

					delete[] P;
				}
				else if (second_choice == 6) {
					delete_bst(root);
					cout << "Stablo uspesno obrisano." << endl;
				}


				cout << endl;
				cout << "1. Formiraj stablo na osnovu uredjenog niza K." << endl;
				cout << "2. Dodaj novi kljuc u stablo." << endl;
				cout << "3. Pretrazi stablo na zadati kljuc." << endl;
				cout << "4. Ispisi stablo." << endl;
				cout << "5. Ispisi evaluaciju performansi pretrazivanja stabla." << endl;
				cout << "6. Obrisi stablo." << endl;
				cout << "0. Prekini program." << endl;
				cout << "Unesi redni broj opcije: ";
				cin >> second_choice;
			}

		}

		cout << endl;
		cout << "1. Pretrazivanje linearnih struktura podataka." << endl;
		cout << "2. Implementacija BST sa ponavljanjem kljuceva." << endl;
		cout << "0. Prekini program." << endl;
		cout << "Unesi redni broj opcije: ";
		cin >> first_choice;
	}

	return 0;
}