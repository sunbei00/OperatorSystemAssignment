class BakeryArgorithm {
private:
	int* g_Flag;
	int* g_Label;
public:
	BakeryArgorithm(int cnt);
	void setFlag(int i, bool flag);
	void setLabel(int i, int label);
	bool getFlag(int i);
	int getLabel(int i);
	int Max(int thread_num);
	int Compare(int i, int myId);
	void b_lock(int myId, int thread_num);
	void b_unlock(int myId);
};