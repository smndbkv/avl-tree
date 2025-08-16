#ifndef AVL_TREE_H
#define AVL_TREE_H
#include <stdio.h>
#include <string.h>
#include <memory>

enum class io_status
{
	success,
	eof,
	format,
	memory,
	open,
	create,
};

class student
{
private:
	std::unique_ptr<char[]> name;
	int value = 0;

public:
	student() = default;
	student(const student &x) = delete;
	// По умолчанию: переместить все поля класса - подходит
	student(student &&x) = default;
	~student() = default;
	student &operator=(const student &x) = delete;
	// По умолчанию: присвоить с перемещением все поля класса - подходит
	student &operator=(student &&x) = default;
	const char *get_name()
	{
		return name.get();
	}
	// bool check_condition(const char *s){
	// 	return strspn(name.get(),s)==strlen(name.get());
	// }
	void print(FILE *fp = stdout) const
	{
		fprintf(fp, "%s %d\n", name.get(), value);
	}
	void print_with_balance(FILE *fp = stdout, int balance = 0) const
	{
		fprintf(fp, "%s %d    %d\n", name.get(), value, balance);
	}
	io_status read(FILE *fp = stdin)
	{
		const int LEN = 1234;
		char n[LEN];
		int v;
		if (fscanf(fp, "%s%d", n, &v) != 2)
			return io_status::format;
		erase();
		return init(n, v);
	}
	int cmp(const student &x) const
	{
		// Используем переопределенную функцию сравнения с nullptr
		if (name == nullptr)
		{
			if (x.name != nullptr)
				return -1;
			return value - x.value;
		}
		if (x.name.get() == nullptr)
			return 1;
		int res = strcmp(name.get(), x.name.get());
		if (res)
			return res;
		return value - x.value;
	}
	int operator<(const student &x) const { return cmp(x) < 0; }
	int operator<=(const student &x) const { return cmp(x) <= 0; }
	int operator>(const student &x) const { return cmp(x) > 0; }
	int operator>=(const student &x) const { return cmp(x) >= 0; }
	int operator==(const student &x) const { return cmp(x) == 0; }
	int operator!=(const student &x) const { return cmp(x) != 0; }

private:
	io_status init(const char *n, int v)
	{
		value = v;
		if (n != nullptr)
		{
			size_t len = strlen(n);
			name = std::make_unique<char[]>(len + 1);
			if (name != nullptr)
			{
				for (size_t i = 0; i <= len; i++)
					name[i] = n[i];
			}
			else
				return io_status::memory;
		}
		return io_status::success;
	}
	void erase()
	{
		value = 0;
		name.reset();
	}
};

/////////////////////////////AVL-tree///////////////////////////////////////////////

template <class T>
class avl_tree;
template <class T>
class avl_tree_node : public T
{
private:
	avl_tree_node *left = nullptr;
	avl_tree_node *right = nullptr;
	int balance = 0;

public:
	avl_tree_node() = default;
	avl_tree_node(avl_tree_node &&x) : T((T &&)x)
	{
		left = nullptr;
		right = nullptr;
		balance = 0;
	}
	avl_tree_node &operator=(const avl_tree_node &x) = delete;
	avl_tree_node &operator=(avl_tree_node &&x)
	{
		if (this == &x)
			return this;
		(T) *this = (T &&)x;

		left = x.left;
		x.left = nullptr;

		right = x.right;
		x.right = nullptr;
	}
	friend class avl_tree<T>;
};

template <class T>
class avl_tree
{
private:
	avl_tree_node<T> *root = nullptr;

public:
	avl_tree() = default;
	~avl_tree()
	{
		delete_subtree(root);
	}
	io_status read(FILE *fp = stdin)
	{
		T x;
		while (x.read(fp) == io_status::success)
		{
			avl_tree_node<T> *curr = new avl_tree_node<T>((avl_tree_node<T> &&)x);
			if (curr == nullptr)
				return io_status::memory;
			add_node(curr);
		}
		if (!feof(fp))
			return io_status::format;
		return io_status::success;
	}
	void print(int r, FILE *fp = stdout) const
	{
		print_subtree(root, 0, r, fp);
	}
	static void print_subtree(avl_tree_node<T> *curr, int level, int r, FILE *fp = stdout)
	{
		if (curr == nullptr || level > r)
			return;
		int spaces = level * 2;
		for (int i = 0; i < spaces; i++)
			fprintf(fp, " ");

		curr->print(fp);
		// curr->print_with_balance(fp,curr->balance);
		print_subtree(curr->left, level + 1, r, fp);
		print_subtree(curr->right, level + 1, r, fp);
	}

private:
	void add_node(avl_tree_node<T> *x)
	{
		if (root == nullptr)
		{
			x->right = nullptr;
			x->left = nullptr;
			x->balance = 0;
			root = x;
		}
		else
		{
			int height = 1;
			root = add_subtree(root, x, height);
		}
	}
	avl_tree_node<T> *add_subtree(avl_tree_node<T> *curr, avl_tree_node<T> *x, int &height)
	{
		if (curr == nullptr)
		{
			curr = x;
			height = 1;
			return curr;
		}

		if (*x < *curr)
		{ // left side
			curr->left = add_subtree(curr->left, x, height);
			if (height == 0)
			{
				return curr; // значит что высота не изменилась
			}
			switch (curr->balance)
			{
			case 1:
				curr->balance = 0;
				height = 0;
				return curr;
			case 0:
				curr->balance = -1;
				height = 1;
				return curr;
			case -1:
				avl_tree_node<T> *curr_l;
				curr_l = curr->left;
				switch (curr_l->balance)
				{
				case -1:
					curr->left = curr_l->right;
					curr_l->right = curr;
					curr->balance = curr_l->balance = 0;
					height = 0;
					return curr_l;
				case 0:
					curr->left = curr_l->right;
					curr_l->right = curr;
					curr->balance = -1;
					curr_l->balance = 1;
					height = 1;
					return curr_l;
				case 1:
					avl_tree_node<T> *curr_l_r;
					curr_l_r = curr_l->right;
					curr_l->right = curr_l_r->left;
					curr_l_r->left = curr_l;
					curr->left = curr_l_r->right;
					curr_l_r->right = curr;
					switch (curr_l_r->balance)
					{
					case 0:
						curr->balance = curr_l->balance = curr_l_r->balance = 0;
						break;
					case 1:
						curr->balance = curr_l_r->balance = 0;
						curr_l->balance = -1;
						break;
					case -1:
						curr->balance = 1;
						curr_l->balance = curr_l_r->balance = 0;
						break;
					}
					height = 0;
					return curr_l_r;
				}
			}
		}

		// right side
		curr->right = add_subtree(curr->right, x, height);
		if (height == 0)
		{
			return curr;
		}

		switch (curr->balance)
		{
		case -1:
			curr->balance = 0;
			height = 0;
			return curr;
		case 0:
			curr->balance = 1;
			height = 1;
			return curr;
		case 1:
			avl_tree_node<T> *curr_r;
			curr_r = curr->right;
			switch (curr_r->balance)
			{
			case 1:
				curr->right = curr_r->left;
				curr_r->left = curr;
				curr->balance = curr_r->balance = 0;
				height = 0;
				return curr_r;
			case 0:
				curr->right = curr_r->left;
				curr_r->left = curr;
				curr->balance = -1;
				curr_r->balance = 1;
				height = 1;
				return curr_r;
			case -1:
				avl_tree_node<T> *curr_r_l;
				curr_r_l = curr_r->left;
				curr_r->left = curr_r_l->right;
				curr_r_l->right = curr_r;
				curr->right = curr_r_l->left;
				curr_r_l->left = curr;
				switch (curr_r_l->balance)
				{
				case 0:
					curr->balance = curr_r->balance = curr_r_l->balance = 0;
					break;
				case 1:
					curr->balance = -1;
					curr_r_l->balance = curr_r->balance = 0;
					break;
				case -1:
					curr->balance = curr_r_l->balance = 0;
					curr_r->balance = 1;
					break;
				}
				height = 0;
				return curr_r_l;
			}
		}
		return curr;
	}

	void delete_subtree(avl_tree_node<T> *curr)
	{
		if (curr == nullptr)
			return;
		delete_subtree(curr->left);
		delete_subtree(curr->right);
		delete curr;
	}
};

#endif
