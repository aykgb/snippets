#include <iostream>
#include <vector>

using namespace std;

struct ListNode {
    int val;
    ListNode* next;
    ListNode(int v): val(v) {}
};

class Solution {
public:
    static vector<int> printListFromTailToHead(ListNode* head)
    {
        ListNode* H = head;
        head = head->next;
        H->next = NULL;
        while(head != NULL) {
            ListNode* tmp = H;
            H = head;
            head = head->next;
            H->next = tmp;
        }

        vector<int> V;
        ListNode* tmp = H;
        while(tmp != NULL) {
            V.push_back(tmp->val);
            tmp = tmp->next;
        }

        return V;
    }
};

int main()
{
    int len = 10000;
    ListNode* head = new ListNode(len);
    ListNode* tmp = head;
    while(--len) {
        tmp->next = new ListNode(len);
        tmp = tmp->next;
    }
    tmp->next = NULL;

    vector<int> V = Solution::printListFromTailToHead(head);

    for(auto v : V) {
        std::cout << v << " ";
    }
    std::cout << std::endl;

    return 0;
}
