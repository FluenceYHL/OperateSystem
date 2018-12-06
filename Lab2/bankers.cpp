#include "gnuplot.hpp"
#include <algorithm>
#include <assert.h>

namespace std {
	template<typename T>
	bool operator<=(const std::vector<T>& lhs, const std::vector<T>& rhs) {
		const int l = lhs.size();
		assert(l == rhs.size());
		for(int i = 0;i < l; ++i)
			if(lhs[i] > rhs[i])
				return false;
		return true;
	}
}

namespace {
	void print(const std::vector<int>& arr) {
		for(const auto& it : arr)
			std::cout << it << " ";
		std::cout << "\n";
	}
	void print(const std::vector< std::vector<int> >& arr) {
		for(const auto& it : arr)
			print(it);
	}
	void logCall(const char* message) {
		std::cout << message << "\n";
	}
}

class banker_Algorithm final {
private:
	int process, resource;
	std::vector<int> avaliable;
	std::vector< std::vector<int> > allocation;
	std::vector< std::vector<int> > need;
	banker_Algorithm(const banker_Algorithm&) = delete;
	banker_Algorithm(banker_Algorithm&&) = delete;

	bool is_safe() {
		auto work = this->avaliable;
		std::vector<bool> finish(this->process, false);
		while(true) {
			bool flag = false;
			for(int i = 0;i < this->process; ++i) {
				if(finish[i] == false and this->need[i] <= work) {
					finish[i] = true;
					for(int j = 0;j < this->resource; ++j)
						work[j] += this->allocation[i][j];
					flag = true;
				}
			}
			if(flag == false) break;
		}
		for(int i = 0;i < this->process; ++i)
			if(finish[i] == false)
				return false;
		return true;
	}

public:
	banker_Algorithm(const std::string& fileName) {
		std::ifstream in(fileName.c_str());
		assert(in);
		YHL::ON_EXIT_SCOPE([&]{ 
			in.close(); 
		});
		in >> this->process >> this->resource;
		this->avaliable.assign(this->resource, 0);
		this->allocation.assign(this->process, std::vector<int>(this->resource, 0));
		this->need.assign(this->process, std::vector<int>(this->resource, 0));
		for(int i = 0;i < this->resource; ++i)
			in >> this->avaliable[i];
		for(int i = 0;i < this->process; ++i) 
			for(int j = 0;j < this->resource; ++j)
				in >> this->allocation[i][j];
		for(int i = 0;i < this->process; ++i)
			for(int j = 0;j < this->resource; ++j)
				in >> this->need[i][j];
	}

	void handle_request(int id, const std::vector<int> &request) {
		if(id < 0 || id >= this->process || request.size() not_eq this->resource)
			return;
		if(request <= this->need[id] and request <= this->avaliable) {
			for(int i = 0;i < this->resource; ++i) {
				this->avaliable[i] -= request[i];
				this->allocation[id][i] += request[i];
				this->need[id][i] -= request[i];
			}
			if(!this->is_safe()) {
				logCall("不安全, 取消分配的资源");
				for(int i = 0;i < this->resource; ++i) {
					this->avaliable[i] += request[i];
					this->allocation[id][i] -= request[i];
					this->need[id][i] += request[i];
				}
				return;
			}
			logCall("成功分配");
		}
	}
};

int main() {
	banker_Algorithm one("./banker(1).txt");
	one.handle_request(2, {0, 0, 0, 0});
	return 0;
}